#include "fmv_converter.hpp"
#include "../../AliveLibAE/PathData.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../FatalError.hpp"
#include "../logger.hpp"
#include "../FmvInfo.hpp"
#include "PNGFile.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <thread>

#ifdef _MSC_VER
    #pragma warning(push)
    #pragma warning(disable : 4505)
#endif
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#include "aom/third_party/libwebm/mkvmuxer/mkvmuxer.h"
#include "aom/third_party/libwebm/mkvmuxer/mkvmuxerutil.h"
#include "aom/third_party/libwebm/mkvmuxer/mkvwriter.h"

#include "aom/common/av1_config.h"

#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>

#include "../Masher.hpp"
#include "rgb_conversion.hpp"
#include "file_system.hpp"

#include "DDVAe.hpp"
#include "PsxStrDemuxer.hpp"
#include "ThreadPool.hpp"
#include "ConversionProgress.hpp"
#include "data_conversion.hpp"
#include "nlohmann/json.hpp"

#include <mutex>
#include <optional>
#include <set>
#include <unordered_map>
#include <cstdio>

// Some FMV sources (e.g. AO's raw PSX STR streams, which have no header - each frame's
// width/height comes from its own MOIR sector) don't know their true frame size or frame
// count until every frame has been decoded once. For those, the caller does a throwaway
// pass with ScanFmvSource() first to find the largest frame size and the frame count -
// also handy groundwork for real conversion progress reporting later.
struct FmvScanResult final
{
    u32 mMaxWidth = 0;
    u32 mMaxHeight = 0;
    u32 mFrameCount = 0;
};

static FmvScanResult ScanFmvSource(relive::IFmvSource& source)
{
    FmvScanResult result;
    if (!source.ReadInfo())
    {
        return result;
    }

    while (source.StepFrame())
    {
        result.mMaxWidth = std::max(result.mMaxWidth, source.FrameWidth());
        result.mMaxHeight = std::max(result.mMaxHeight, source.FrameHeight());
        ++result.mFrameCount;
    }

    return result;
}

// Tracks, per movie, whether its FMV conversion fully completed (encoded + finalized + moved
// into place - see FmvConv::Convert's temp-file handling) under the fmvVersion this manifest was
// constructed with. Backs ConvertFMVs' resume-on-relaunch behavior: FMV conversion is the one
// data category slow enough that a user is likely to actually quit mid-conversion (killed, or
// cancelled via ThreadPool::RequestCancel/Engine::Run()'s quit handling), so an interrupted run
// shouldn't have to redo movies that already finished. A version bump invalidates the whole
// manifest rather than trying to reconcile old entries against new encoding settings - simpler,
// and correct, since a version bump means everything needs redoing anyway.
class FmvConversionManifest final
{
public:
    FmvConversionManifest(FileSystem& fs, FileSystem::Path manifestPath, u32 fmvVersion)
        : mFs(fs)
        , mManifestPath(std::move(manifestPath))
        , mFmvVersion(fmvVersion)
    {
        const std::string jsonStr = mFs.LoadToString(mManifestPath);
        if (jsonStr.empty())
        {
            return;
        }

        try
        {
            const nlohmann::json j = nlohmann::json::parse(jsonStr);
            if (j.value("fmv_version", 0u) != mFmvVersion)
            {
                // A different code version's encoding settings - everything needs redoing, so
                // don't trust any of this list.
                return;
            }
            for (const auto& name : j.at("completed"))
            {
                mCompleted.insert(name.get<std::string>());
            }
        }
        catch (const nlohmann::json::exception&)
        {
            // Corrupt/unreadable manifest - treat as empty, everything gets (re)converted.
        }
    }

    [[nodiscard]] bool IsCompleted(const std::string& movieName) const
    {
        std::lock_guard<std::mutex> lock(mMutex);
        return mCompleted.count(movieName) != 0;
    }

    // Called once a movie's conversion is fully done (see ConvertFmvJob::Execute) - multiple
    // worker threads can call this around the same time, so it's mutex-guarded and re-saves the
    // whole manifest under the lock rather than trying to do a lock-free incremental update.
    void MarkCompleted(const std::string& movieName)
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mCompleted.insert(movieName);

        std::vector<std::string> names(mCompleted.begin(), mCompleted.end());
        std::sort(names.begin(), names.end());
        const nlohmann::json j = {
            {"fmv_version", mFmvVersion},
            {"completed", names},
        };
        SaveJson(j, mFs, mManifestPath);
    }

private:
    FileSystem& mFs;
    FileSystem::Path mManifestPath;
    u32 mFmvVersion = 0;
    mutable std::mutex mMutex;
    std::set<std::string> mCompleted;
};

// Encodes interleaved s16 PCM into Ogg Vorbis packets for muxing into the FMV's WebM
// container as an A_VORBIS audio track (replacing raw, much larger, A_PCM audio).
class VorbisAudioEncoder final
{
public:
    ~VorbisAudioEncoder()
    {
        if (mInitialized)
        {
            vorbis_block_clear(&mBlock);
            vorbis_dsp_clear(&mDspState);
            vorbis_comment_clear(&mComment);
            vorbis_info_clear(&mInfo);
        }
    }

    bool Init(u32 sampleRate, u32 channels, float baseQuality = 0.4f)
    {
        vorbis_info_init(&mInfo);
        if (vorbis_encode_init_vbr(&mInfo, static_cast<long>(channels), static_cast<long>(sampleRate), baseQuality) != 0)
        {
            vorbis_info_clear(&mInfo);
            return false;
        }

        vorbis_comment_init(&mComment);
        vorbis_analysis_init(&mDspState, &mInfo);
        vorbis_block_init(&mDspState, &mBlock);
        mChannels = channels;
        mInitialized = true;
        return true;
    }

    // The 3 Vorbis header packets (identification/comment/setup), concatenated using
    // Matroska's "Xiph lacing" CodecPrivate format - see the Matroska A_VORBIS spec. Must be
    // called once, right after Init() and before any EncodePcm()/Finish() calls.
    std::vector<u8> BuildCodecPrivate()
    {
        ogg_packet idHeader = {};
        ogg_packet commentHeader = {};
        ogg_packet setupHeader = {};
        vorbis_analysis_headerout(&mDspState, &mComment, &idHeader, &commentHeader, &setupHeader);

        std::vector<u8> out;
        out.push_back(2); // 2 lengths follow (the setup header's length is implied as the remainder)
        AppendXiphLacedLength(out, idHeader.bytes);
        AppendXiphLacedLength(out, commentHeader.bytes);
        out.insert(out.end(), idHeader.packet, idHeader.packet + idHeader.bytes);
        out.insert(out.end(), commentHeader.packet, commentHeader.packet + commentHeader.bytes);
        out.insert(out.end(), setupHeader.packet, setupHeader.packet + setupHeader.bytes);
        return out;
    }

    // samples is interleaved s16 PCM, sampleFrameCount frames (i.e. samples.size() == sampleFrameCount * channels).
    template <typename OnPacketFn>
    void EncodePcm(const s16* samples, size_t sampleFrameCount, const OnPacketFn& onPacket)
    {
        if (sampleFrameCount > 0)
        {
            float** buffer = vorbis_analysis_buffer(&mDspState, static_cast<int>(sampleFrameCount));
            for (size_t i = 0; i < sampleFrameCount; ++i)
            {
                for (u32 ch = 0; ch < mChannels; ++ch)
                {
                    buffer[ch][i] = static_cast<float>(samples[i * mChannels + ch]) / 32768.0f;
                }
            }
            vorbis_analysis_wrote(&mDspState, static_cast<int>(sampleFrameCount));
        }
        DrainPackets(onPacket);
    }

    // Signals end-of-stream and flushes any packets libvorbis was still holding onto.
    template <typename OnPacketFn>
    void Finish(const OnPacketFn& onPacket)
    {
        vorbis_analysis_wrote(&mDspState, 0);
        DrainPackets(onPacket);
    }

private:
    template <typename OnPacketFn>
    void DrainPackets(const OnPacketFn& onPacket)
    {
        while (vorbis_analysis_blockout(&mDspState, &mBlock) == 1)
        {
            vorbis_analysis(&mBlock, nullptr);
            vorbis_bitrate_addblock(&mBlock);

            ogg_packet packet;
            while (vorbis_bitrate_flushpacket(&mDspState, &packet))
            {
                onPacket(packet);
            }
        }
    }

    static void AppendXiphLacedLength(std::vector<u8>& out, long length)
    {
        while (length >= 255)
        {
            out.push_back(255);
            length -= 255;
        }
        out.push_back(static_cast<u8>(length));
    }

    vorbis_info mInfo = {};
    vorbis_comment mComment = {};
    vorbis_dsp_state mDspState = {};
    vorbis_block mBlock = {};
    u32 mChannels = 0;
    bool mInitialized = false;
};

class FmvConv final
{
public:
    // pScan, when given, overrides the source's own (possibly unknown/0) frame size and
    // frame count - see ScanFmvSource() above. Returns true once the movie is fully encoded and
    // its temp output file has been moved into place; false if the source couldn't be opened, or
    // tp.IsCancelRequested() was set partway through (see ThreadPool::RequestCancel/Engine::Run()'s
    // quit handling) - the caller (ConvertFmvJob) only marks a movie complete in the manifest when
    // this returns true.
    bool Convert(relive::IFmvSource& source, std::string fName, const FileSystem::Path& outDir, ThreadPool& tp, ConversionProgress& progress, const FmvScanResult* pScan = nullptr)
    {
        TRACE_ENTRYEXIT;

        mCurrentMovieName = fName;

        if (!source.ReadInfo())
        {
            LOG_WARNING("Failed to open FMV source '%s'", fName.c_str());
            // At least one retail FMV is missing for some reason
            return false;
        }

        const u32 width = (pScan && pScan->mMaxWidth > 0) ? pScan->mMaxWidth : (source.FrameWidth() > 0 ? source.FrameWidth() : 640u);
        const u32 height = (pScan && pScan->mMaxHeight > 0) ? pScan->mMaxHeight : (source.FrameHeight() > 0 ? source.FrameHeight() : 240u);

        LOG_INFO("FMV '%s' dimensions: %ux%u (header reported %ux%u)", fName.c_str(), width, height,
                 source.FrameWidth(), source.FrameHeight());

        // TODO: FIX ME - hack to 15
        const u32 frameRate = 15; //source.FrameRate() > 0 ? source.FrameRate() : 15u;
        const u32 audioSampleRate = source.AudioSampleRate() > 0 ? source.AudioSampleRate() : 44100u;
        const u32 audioChannels = source.AudioChannels() > 0 ? source.AudioChannels() : 2u;
        const u32 audioBitsPerSample = source.AudioBitsPerSample() > 0 ? source.AudioBitsPerSample() : 16u;
        std::vector<u8> frameBuffer(width * height * sizeof(u32));

        aom_codec_iface_t* encoder = &aom_codec_av1_cx_algo;
        if (!encoder)
        {
            ALIVE_FATAL("Unsupported codec.");
        }

        LOG_INFO("Encoding AV1 FMV: %ux%u @ %u fps (real-time settings)", width, height, frameRate > 0 ? frameRate : 15u);

        aom_image_t rawImageFrameData;
        if (!aom_img_alloc(&rawImageFrameData, AOM_IMG_FMT_I420, width, height, 1))
        {
            ALIVE_FATAL("Failed to allocate image.");
        }

        aom_codec_enc_cfg_t cfg = {};
        if (aom_codec_enc_config_default(encoder, &cfg, AOM_USAGE_REALTIME))
        {
            ALIVE_FATAL("Failed to get default codec config.");
        }
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = frameRate > 0 ? frameRate : 15; // fps
        cfg.g_lag_in_frames = 0;
        cfg.g_threads = std::max(1u, std::thread::hardware_concurrency() / 2u);

        // AOM_USAGE_REALTIME's default kf_mode is AOM_KF_DISABLED - we were only ever getting
        // the single forced keyframe at frame 0 below, and nothing else for the rest of the
        // movie (confirmed via ffprobe: exactly one K frame in a 530-frame file). Besides making
        // every seek in an external player replay the whole movie from the start to get there,
        // that's also more fragile than it needs to be - losing/corrupting any single P-frame
        // breaks every frame after it for the rest of the movie, instead of just until the next
        // keyframe.
        cfg.kf_mode = AOM_KF_AUTO;
        cfg.kf_min_dist = 0;
        // 1s, matching normal seek-to-the-second expectations from typical video files. The
        // usual reason to space keyframes out further (they're much bigger than a P-frame, so
        // more of them costs real bitrate) barely applies at this tiny 320x240/~15fps size - a
        // keyframe here only runs about 3.8KB more than an average P-frame (measured via
        // ffprobe), so a 1s cadence instead of 2s only adds a few % to typical FMV file sizes.
        cfg.kf_max_dist = (frameRate > 0 ? frameRate : 15) * 1;

        aom_codec_ctx_t codec = {};
        if (aom_codec_enc_init(&codec, encoder, &cfg, 0))
        {
            ALIVE_FATAL("Failed to initialize encoder");
        }

        const int speed = 8;
        if (aom_codec_control(&codec, AOME_SET_CPUUSED, speed))
        {
            ALIVE_FATAL("Failed to set cpu-used");
        }

        FileSystem::Path outPathBuilder = outDir;
        outPathBuilder.Append(relive::FmvNameWithoutExtension(fName) + ".webm");
        const std::string outFileName = outPathBuilder.GetPath();
        // Written under a temp name and only renamed to outFileName once fully finalized (see
        // below), so a conversion that gets killed or cancelled mid-write (see
        // ThreadPool::IsCancelRequested() below) never leaves a corrupt/truncated file sitting at
        // the final path - and so ConvertFMVs' "is this one already done" resume check (backed by
        // FmvConversionManifest, which only records a movie complete once this rename happens)
        // can't mistake a partial file for a finished one.
        const std::string tempFileName = outFileName + ".tmp";
        FILE* outFile = fopen(tempFileName.c_str(), "wb");
        if (!outFile)
        {
            ALIVE_FATAL("Failed to open output file '%s'", tempFileName.c_str());
        }

        bool cancelled = false;

        {
            mkvmuxer::MkvWriter writer(outFile);
            mkvmuxer::Segment segment;
            mAudioSampleRate = audioSampleRate;
            mAudioChannels = audioChannels;
            mAudioBitsPerSample = audioBitsPerSample;
            mkv_init(&writer, &segment, &cfg, &codec);

            const auto clamp_to_u8 = [](s32 value) -> u8
            {
                if (value < 0)
                {
                    return 0;
                }
                if (value > 255)
                {
                    return 255;
                }
                return static_cast<u8>(value);
            };

            const auto convert_rgba_to_i420 = [&]()
            {
                for (u32 y = 0; y < height; ++y)
                {
                    u8* yPlane = rawImageFrameData.planes[0] + (y * rawImageFrameData.stride[0]);
                    for (u32 x = 0; x < width; ++x)
                    {
                        const RGBA32& p = ((RGBA32*)frameBuffer.data())[(y * width) + x];
                        const s32 r = p.r;
                        const s32 g = p.g;
                        const s32 b = p.b;
                        const s32 yVal = (77 * r + 150 * g + 29 * b) >> 8;
                        yPlane[x] = clamp_to_u8(yVal);
                    }
                }

                const u32 chromaWidth = (width + 1) / 2;
                const u32 chromaHeight = (height + 1) / 2;
                for (u32 y = 0; y < chromaHeight; ++y)
                {
                    u8* uPlane = rawImageFrameData.planes[1] + (y * rawImageFrameData.stride[1]);
                    u8* vPlane = rawImageFrameData.planes[2] + (y * rawImageFrameData.stride[2]);
                    for (u32 x = 0; x < chromaWidth; ++x)
                    {
                        const u32 sampleX = x * 2;
                        const u32 sampleY = y * 2;
                        s32 rTotal = 0;
                        s32 gTotal = 0;
                        s32 bTotal = 0;
                        const u32 sampleCount = 4;
                        for (u32 dy = 0; dy < 2 && sampleY + dy < height; ++dy)
                        {
                            for (u32 dx = 0; dx < 2 && sampleX + dx < width; ++dx)
                            {
                                const RGBA32& p = ((RGBA32*)frameBuffer.data())[((sampleY + dy) * width) + (sampleX + dx)];
                                rTotal += p.r;
                                gTotal += p.g;
                                bTotal += p.b;
                            }
                        }

                        const s32 rAvg = rTotal / sampleCount;
                        const s32 gAvg = gTotal / sampleCount;
                        const s32 bAvg = bTotal / sampleCount;
                        const s32 uVal = (-43 * rAvg - 85 * gAvg + 128 * bAvg) >> 8;
                        const s32 vVal = (128 * rAvg - 107 * gAvg - 21 * bAvg) >> 8;
                        uPlane[x] = clamp_to_u8(uVal + 128);
                        vPlane[x] = clamp_to_u8(vVal + 128);
                    }
                }
            };

            const u32 totalFrames = (pScan && pScan->mFrameCount > 0) ? pScan->mFrameCount : source.TotalVideoFrames();
            u32 frame_index = 0;
            while (totalFrames == 0 || frame_index < totalFrames)
            {
                // Checked once per frame rather than more granularly - encoding a single frame
                // is fast enough (well under the ~1s ALIVE_FATAL warns about in encode_frame)
                // that this is still a prompt response to a quit request, without adding
                // per-frame overhead anywhere else.
                if (tp.IsCancelRequested())
                {
                    LOG_INFO("FMV conversion of '%s' cancelled at frame %u/%u", fName.c_str(), frame_index, totalFrames);
                    cancelled = true;
                    break;
                }

                if (!source.StepFrame())
                {
                    break;
                }

                const std::vector<u8>& srcPixels = source.GetPixels();
                const u32 frameW = source.FrameWidth();
                const u32 frameH = source.FrameHeight();

                if (frameW == width && frameH == height)
                {
                    frameBuffer = srcPixels;
                }
                else
                {
                    // The source changed frame size mid-stream (seen in some AO PSX STR
                    // streams). Composite into the top-left of the canvas sized to the
                    // largest frame seen (see ScanFmvSource()), clearing first so a
                    // shrinking frame doesn't leave stale pixels behind; a frame somehow
                    // bigger than the canvas gets truncated to fit it.
                    std::fill(frameBuffer.begin(), frameBuffer.end(), 0);
                    const u32 copyW = std::min(frameW, width);
                    const u32 copyH = std::min(frameH, height);
                    for (u32 y = 0; y < copyH; ++y)
                    {
                        std::memcpy(frameBuffer.data() + (static_cast<size_t>(y) * width * sizeof(u32)),
                                    srcPixels.data() + (static_cast<size_t>(y) * frameW * sizeof(u32)),
                                    static_cast<size_t>(copyW) * sizeof(u32));
                    }
                }

                const std::vector<u8> audioFrames = source.GetAudioFrames();
                if (!audioFrames.empty() && mAudioTrackNumber != 0)
                {
                    const u32 bytesPerSampleFrame = (mAudioBitsPerSample / 8u) * mAudioChannels;
                    if (bytesPerSampleFrame > 0 && mAudioBitsPerSample == 16 && (audioFrames.size() % bytesPerSampleFrame) == 0)
                    {
                        const size_t sampleFrameCount = audioFrames.size() / bytesPerSampleFrame;
                        mVorbisEncoder.EncodePcm(reinterpret_cast<const s16*>(audioFrames.data()), sampleFrameCount,
                            [&](const ogg_packet& packet)
                            {
                                // granulepos is the total PCM sample count (per channel) the
                                // encoder has actually finalized up to and including this
                                // packet. This lags behind the raw count of samples fed into
                                // EncodePcm() so far by the codec's own look-ahead/windowing
                                // delay, so it - not a running count of input samples - has to
                                // be the single clock both audio and video timestamps are
                                // derived from below; otherwise video's timestamp (fed samples)
                                // can run ahead of audio's (encoder-confirmed samples) and the
                                // muxer rejects the next audio frame as non-monotonic.
                                mLastAudioGranulePos = static_cast<u64>(packet.granulepos);
                                const u64 audioPtsNs = (mLastAudioGranulePos * 1000000000ULL) / static_cast<u64>(mAudioSampleRate);
                                if (!segment.AddFrame(packet.packet, static_cast<uint64_t>(packet.bytes), mAudioTrackNumber, audioPtsNs, true))
                                {
                                    LOG_ERROR("AddAudioFrame failed.");
                                }
                            });
                    }
                }

                // frameBuffer

                /*
                if (!ddv.ReadVideoFrame(frameBuffer.data()))
                {
                    break;
                }*/


                convert_rgba_to_i420();
                const int flags = (frame_index == 0) ? AOM_EFLAG_FORCE_KF : 0;

                // Tie video timestamps to the same real (sample-counted) audio clock rather
                // than an assumed-constant frame rate - AO's true playback rate isn't a clean
                // 15fps (see the frameRate hack above), so the two would otherwise drift apart
                // over a long enough movie until a video frame's assumed-rate timestamp landed
                // behind audio's real-rate timestamp, which the muxer rejects as non-monotonic.
                const int64_t videoPtsNs = (mAudioTrackNumber != 0)
                    ? static_cast<int64_t>((mLastAudioGranulePos * 1000000000ULL) / static_cast<u64>(mAudioSampleRate))
                    : -1;
                encode_frame(&segment, &cfg, &codec, &rawImageFrameData, static_cast<int>(frame_index), flags, videoPtsNs);

                if (totalFrames > 0)
                {
                    const double percentDone = (static_cast<double>(frame_index + 1u) / static_cast<double>(totalFrames)) * 100.0;
                    LOG_INFO("Video frame %u/%u (%.2f%%)", frame_index + 1u, totalFrames, percentDone);
                }
                else
                {
                    LOG_INFO("Video frame %u (total unknown)", frame_index + 1u);
                }

                progress.AddCompleted(ConversionCategory::Fmvs, 1);
                progress.UpdateItemProgress(fName, frame_index + 1);

                ++frame_index;
            }

            bool finalizedOk = false;
            if (!cancelled)
            {
                while (encode_frame(&segment, &cfg, &codec, nullptr, -1, 0))
                {
                    continue;
                }

                if (mAudioTrackNumber != 0)
                {
                    mVorbisEncoder.Finish(
                        [&](const ogg_packet& packet)
                        {
                            const u64 audioPtsNs = (static_cast<u64>(packet.granulepos) * 1000000000ULL) / static_cast<u64>(mAudioSampleRate);
                            if (!segment.AddFrame(packet.packet, static_cast<uint64_t>(packet.bytes), mAudioTrackNumber, audioPtsNs, true))
                            {
                                LOG_ERROR("AddAudioFrame failed.");
                            }
                        });
                }

                finalizedOk = segment.Finalize();
                if (!finalizedOk)
                {
                    LOG_ERROR("Segment::Finalize failed.");
                }
            }

            fclose(outFile);

            if (cancelled || !finalizedOk)
            {
                std::remove(tempFileName.c_str());
                aom_img_free(&rawImageFrameData);
                return false;
            }
        }

        aom_img_free(&rawImageFrameData);

        if (std::rename(tempFileName.c_str(), outFileName.c_str()) != 0)
        {
            LOG_ERROR("Failed to move converted FMV '%s' into place (from '%s' to '%s')",
                fName.c_str(), tempFileName.c_str(), outFileName.c_str());
            std::remove(tempFileName.c_str());
            return false;
        }

        return true;
    }

private:

    // videoPtsNs, when >= 0, overrides the timestamp derived from the encoder's own pts.
    // The caller uses this to tie video timestamps to the same real (sample-counted)
    // audio clock as mkv audio frames - see the call site for why.
    int mkv_write_block(mkvmuxer::Segment* segment, const aom_codec_enc_cfg_t* cfg, const aom_codec_cx_pkt_t* pkt, int64_t videoPtsNs)
    {
        int64_t pts_ns = videoPtsNs >= 0 ? videoPtsNs : (pkt->data.frame.pts * 1000000000ll * cfg->g_timebase.num / cfg->g_timebase.den);
        if (pts_ns <= mLast_pts_ns)
        {
            pts_ns = mLast_pts_ns + 1000000;
        }

        mLast_pts_ns = pts_ns;

        if (!segment->AddFrame(static_cast<uint8_t*>(pkt->data.frame.buf),
                               pkt->data.frame.sz, kVideoTrackNumber, pts_ns,
                               pkt->data.frame.flags & AOM_FRAME_IS_KEY))
        {
            LOG_ERROR("AddFrame failed.");
            return -1;
        }
        return 0;
    }

    int mkv_init(mkvmuxer::MkvWriter* writer, mkvmuxer::Segment* segment, aom_codec_enc_cfg_t* cfg, aom_codec_ctx_t* codec)
    {
        mLast_pts_ns = 0;

        bool ok = segment->Init(writer);
        if (!ok)
        {
            LOG_ERROR("mkvmuxer Init failed.");
            return -1;
        }

        segment->set_mode(mkvmuxer::Segment::kFile);
        segment->OutputCues(true);

        if (mAudioSampleRate > 0 && mAudioChannels > 0 && mAudioBitsPerSample > 0)
        {
            if (!mVorbisEncoder.Init(mAudioSampleRate, mAudioChannels))
            {
                LOG_ERROR("Vorbis encoder init failed.");
                return -1;
            }

            const uint64_t audio_track_id = segment->AddAudioTrack(static_cast<int32_t>(mAudioSampleRate), static_cast<int32_t>(mAudioChannels), kAudioTrackNumber);
            mkvmuxer::AudioTrack* const audio_track = static_cast<mkvmuxer::AudioTrack*>(segment->GetTrackByNumber(audio_track_id));
            if (!audio_track)
            {
                LOG_ERROR("Audio track creation failed.");
                return -1;
            }

            const std::vector<u8> codecPrivate = mVorbisEncoder.BuildCodecPrivate();
            if (!audio_track->SetCodecPrivate(codecPrivate.data(), codecPrivate.size()))
            {
                LOG_ERROR("Unable to set Vorbis codec private data.");
                return -1;
            }

            audio_track->set_codec_id("A_VORBIS");
            audio_track->set_bit_depth(mAudioBitsPerSample);
            mAudioTrackNumber = audio_track_id;
        }

        mkvmuxer::SegmentInfo* const info = segment->GetSegmentInfo();
        if (!info)
        {
            LOG_ERROR("Cannot retrieve Segment Info.");
            return -1;
        }

        const uint64_t kTimecodeScale = 1000000;
        info->set_timecode_scale(kTimecodeScale);
        std::string version = "aomenc";
        /*
        if (!webm_ctx->debug)
        {
            version.append(std::string(" ") + aom_codec_version_str());
        }*/

        info->set_writing_app(version.c_str());


        const uint64_t video_track_id = segment->AddVideoTrack(static_cast<int>(cfg->g_w),
                                                               static_cast<int>(cfg->g_h), kVideoTrackNumber);
        mkvmuxer::VideoTrack* const video_track = static_cast<mkvmuxer::VideoTrack*>(
            segment->GetTrackByNumber(video_track_id));

        if (!video_track)
        {
            LOG_ERROR("Video track creation failed.");
            return -1;
        }

        ok = false;
        aom_fixed_buf_t* obu_sequence_header = aom_codec_get_global_headers(codec);
        if (obu_sequence_header)
        {
            Av1Config av1_config;
            if (get_av1config_from_obu(
                    reinterpret_cast<const uint8_t*>(obu_sequence_header->buf),
                    obu_sequence_header->sz, false, &av1_config)
                == 0)
            {
                uint8_t av1_config_buffer[4] = {0};
                size_t bytes_written = 0;
                if (write_av1config(&av1_config, sizeof(av1_config_buffer),
                                    &bytes_written, av1_config_buffer)
                    == 0)
                {
                    ok = video_track->SetCodecPrivate(av1_config_buffer,
                                                      sizeof(av1_config_buffer));
                }
            }
            free(obu_sequence_header->buf);
            free(obu_sequence_header);
        }
        if (!ok)
        {
            LOG_ERROR("Unable to set AV1 config.");
            return -1;
        }

        ok = video_track->SetStereoMode(1); // STEREO_FORMAT_LEFT_RIGHT
        if (!ok)
        {
            LOG_ERROR("Unable to set stereo mode.");
            return -1;
        }

        video_track->set_codec_id("V_AV1");

        /*
        // Default to 1:1 pixel aspect ratio.
        input->pixel_aspect_ratio.numerator = 1;
        input->pixel_aspect_ratio.denominator = 1;

        if (par->numerator > 1 || par->denominator > 1)
        {
            const uint64_t display_width = static_cast<uint64_t>(((cfg->g_w * par->numerator * 1.0) / par->denominator) + .5);
            video_track->set_display_width(display_width);
            video_track->set_display_height(cfg->g_h);
        }
        */

        /*
        if (encoder_settings != nullptr)
        {
            mkvmuxer::Tag* tag = segment->AddTag();
            if (tag == nullptr)
            {
                LOG_ERROR("Unable to allocate memory for encoder settings tag.");
                return -1;
            }
            ok = tag->add_simple_tag("ENCODER_SETTINGS", encoder_settings);
            if (!ok)
            {
                LOG_ERROR("Unable to allocate memory for encoder settings tag.");
                return -1;
            }
        }*/

        /*
        if (webm_ctx->debug)
        {
            video_track->set_uid(kDebugTrackUid);
        }*/

        // webm_ctx->writer = writer.release();
        // webm_ctx->segment = segment.release();
        return 0;
    }

    bool encode_frame(mkvmuxer::Segment* segment, const aom_codec_enc_cfg_t* cfg, aom_codec_ctx_t* codec, aom_image_t* img, int frame_index, int flags, int64_t videoPtsNs = -1)
    {
        bool got_pkts = false;
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t* pkt = nullptr;
        const auto start = std::chrono::steady_clock::now();
        const aom_codec_err_t res = aom_codec_encode(codec, img, frame_index, 1, flags);
        const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

        if (res != AOM_CODEC_OK)
        {
            const char* const detail = aom_codec_error_detail(codec);
            ALIVE_FATAL("Failed to encode frame %d of FMV '%s': %s (%s)", frame_index, mCurrentMovieName.c_str(),
                        aom_codec_err_to_string(res), detail ? detail : aom_codec_error(codec));
        }

        if (elapsedMs > 1000)
        {
            LOG_INFO("Encoding frame %d took %lld ms", frame_index, static_cast<long long>(elapsedMs));
        }

        while ((pkt = aom_codec_get_cx_data(codec, &iter)) != nullptr)
        {
            got_pkts = true;

            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT)
            {
                const int keyframe = (pkt->data.frame.flags & AOM_FRAME_IS_KEY) != 0;

                if (mkv_write_block(segment, cfg, pkt, videoPtsNs) != 0)
                {
                    ALIVE_FATAL("Failed to write compressed frame %d of FMV '%s'", frame_index, mCurrentMovieName.c_str());
                }
                LOG_INFO(keyframe ? "K" : ".");
            }
        }

        return got_pkts;
    }

private:
    std::string mCurrentMovieName;
    const int kVideoTrackNumber = 1;
    const int kAudioTrackNumber = 2;
    int64_t mLast_pts_ns = 0;
    uint64_t mAudioTrackNumber = 0;
    uint64_t mLastAudioGranulePos = 0;
    uint32_t mAudioSampleRate = 0;
    uint32_t mAudioChannels = 0;
    uint32_t mAudioBitsPerSample = 0;
    VorbisAudioEncoder mVorbisEncoder;
};

class ConvertFmvJob final : public IJob
{
public:
    // scan, when set, is the AO frame-count/size dry-run result already computed upfront by
    // ConvertFMVs' scan wave (see below) - reused here instead of redecoding the whole movie a
    // second time. Always empty for AE (its DDV header already declares this, no scan needed).
    // totalFrames is this movie's own frame count (already known either way by the time
    // ConvertFMVs dispatches this job) - shown as this item's own sub-progress ("frame X/Y") in
    // the UI's in-progress list, since a single fmv can itself take long enough that just seeing
    // its name sit there looks stalled.
    ConvertFmvJob(FileSystem& fs, std::string movieName, FileSystem::Path outDir, bool isAo,
                  ThreadPool& tp, std::shared_ptr<FmvConversionManifest> manifest,
                  ConversionProgress& progress, std::optional<FmvScanResult> scan, u32 totalFrames)
        : mFs(fs)
        , mMovieName(std::move(movieName))
        , mOutDir(std::move(outDir))
        , mIsAo(isAo)
        , mThreadPool(tp)
        , mManifest(std::move(manifest))
        , mProgress(progress)
        , mScan(std::move(scan))
        , mTotalFrames(totalFrames)
    {
    }

    void Execute() override
    {
        if (mThreadPool.IsCancelRequested())
        {
            // Don't even start a fresh conversion once shutdown's underway.
            return;
        }

        LOG_INFO("ConvertFmvJob: starting '%s' (isAo=%d)", mMovieName.c_str(), mIsAo ? 1 : 0);

        mProgress.ReportItemStarted(mMovieName, mTotalFrames);

        FmvConv fmvConv;
        bool completed = false;
        if (mIsAo)
        {
            relive::PsxStrDemuxer source(mFs, mMovieName.c_str());
            completed = fmvConv.Convert(source, mMovieName, mOutDir, mThreadPool, mProgress, mScan ? &*mScan : nullptr);
        }
        else
        {
            // AE's DDV header already declares a fixed frame size and frame count up
            // front, so no pre-pass is needed here.
            relive::DDVAe source(mFs, mMovieName.c_str(), nullptr);
            completed = fmvConv.Convert(source, mMovieName, mOutDir, mThreadPool, mProgress);
        }

        if (completed)
        {
            mManifest->MarkCompleted(mMovieName);
        }

        // Cleared regardless of outcome (success/failure/cancel) so a cancelled/failed movie
        // doesn't linger in the "in progress" list forever.
        mProgress.ReportItemFinished(mMovieName);
    }

private:
    FileSystem& mFs;
    std::string mMovieName;
    FileSystem::Path mOutDir;
    bool mIsAo = false;
    ThreadPool& mThreadPool;
    std::shared_ptr<FmvConversionManifest> mManifest;
    ConversionProgress& mProgress;
    std::optional<FmvScanResult> mScan;
    u32 mTotalFrames = 0;
};

namespace
{
    // Only used for AO's upfront frame-count dry run (see ConvertFMVs) - AE doesn't need this,
    // its DDV header already declares frame count directly (ReadInfo() alone, no decode needed).
    class ScanAoFmvJob final : public IJob
    {
    public:
        ScanAoFmvJob(FileSystem& fs, std::string movieName, ThreadPool& tp,
                     std::mutex& resultsMutex, std::unordered_map<std::string, FmvScanResult>& results)
            : mFs(fs)
            , mMovieName(std::move(movieName))
            , mThreadPool(tp)
            , mResultsMutex(resultsMutex)
            , mResults(results)
        {
        }

        void Execute() override
        {
            if (mThreadPool.IsCancelRequested())
            {
                return;
            }

            relive::PsxStrDemuxer source(mFs, mMovieName.c_str());
            const FmvScanResult scan = ScanFmvSource(source);
            LOG_INFO("ScanAoFmvJob: '%s' scanned max %ux%u over %u frames", mMovieName.c_str(),
                     scan.mMaxWidth, scan.mMaxHeight, scan.mFrameCount);

            std::lock_guard<std::mutex> lock(mResultsMutex);
            mResults.emplace(mMovieName, scan);
        }

    private:
        FileSystem& mFs;
        std::string mMovieName;
        ThreadPool& mThreadPool;
        std::mutex& mResultsMutex;
        std::unordered_map<std::string, FmvScanResult>& mResults;
    };
} // namespace

void ConvertFMVs(ThreadPool& tp, FileSystem& fs, const FileSystem::Path& dataDir, bool isAo, u32 fmvVersion, ConversionProgress& progress)
{
    FileSystem::Path fmvOutDir = dataDir;
    fmvOutDir.Append("fmvs");
    fs.CreateDirectory(fmvOutDir);

    FileSystem::Path manifestPath = fmvOutDir;
    manifestPath.Append("_conversion_progress.json");
    auto manifest = std::make_shared<FmvConversionManifest>(fs, manifestPath, fmvVersion);

    // Real, per-level FMV filenames straight from each game's own reversed FmvInfo
    // tables (AliveLibAE/AliveLibAO PathData.cpp) rather than a separately hand
    // maintained list here.
    const std::vector<std::string> movieNames = isAo ? AO::Path_GetAllFmvNames() : ::Path_GetAllFmvNames();

    // Upfront per-movie frame-count dry run, so the Fmvs category total (weighted highest of all
    // categories, since fmvs take by far the longest to convert) is known before any real
    // encoding starts. Every movie gets scanned here regardless of whether the manifest already
    // has it marked complete - frame counts aren't persisted into the manifest (would need a
    // schema migration for a resume-only cost: this only re-scans already-done AO movies on a
    // launch that resumes an interrupted conversion, not on every ordinary launch, since once
    // data_version.json is stamped ConvertFmvs() is false and none of this runs again).
    std::unordered_map<std::string, FmvScanResult> aoScanResults;
    if (isAo)
    {
        std::mutex resultsMutex;
        for (const auto& movieName : movieNames)
        {
            tp.AddJob(std::make_unique<ScanAoFmvJob>(fs, movieName, tp, resultsMutex, aoScanResults));
        }

        // Wait for the whole scan wave to drain before dispatching any real conversion below -
        // real ConvertFmvJobs need aoScanResults fully populated so they can reuse it instead of
        // rescanning, and the category total needs to reflect every movie before any
        // AddCompleted() call could otherwise run ahead of it.
        while (tp.Busy())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
    }

    // Per-movie frame count/scan, computed for every movie before any AddToTotal() call below -
    // deliberately a separate pass from the skip-or-dispatch one further down. Combining the two
    // into one loop (as an earlier version of this did) let a job dispatched for an early movie
    // start completing frames on a worker thread while the loop was still only partway through
    // adding later movies' totals - the category total kept growing out from under jobs already
    // in flight, making the reported percentage swing wildly (confirmed live: 0% -> 57% -> 61% ->
    // 33% -> ...) instead of climbing monotonically.
    struct MovieInfo final
    {
        std::string mName;
        u32 mFrameCount = 0;
        std::optional<FmvScanResult> mScan;
    };
    std::vector<MovieInfo> movieInfos;
    movieInfos.reserve(movieNames.size());

    for (const auto& movieName : movieNames)
    {
        MovieInfo info;
        info.mName = movieName;
        if (isAo)
        {
            const auto it = aoScanResults.find(movieName);
            if (it != aoScanResults.end())
            {
                info.mScan = it->second;
                info.mFrameCount = it->second.mFrameCount;
            }
        }
        else
        {
            relive::DDVAe source(fs, movieName.c_str(), nullptr);
            if (source.ReadInfo())
            {
                info.mFrameCount = source.TotalVideoFrames();
            }
        }
        progress.AddToTotal(ConversionCategory::Fmvs, info.mFrameCount);
        movieInfos.push_back(std::move(info));
    }

    for (const MovieInfo& info : movieInfos)
    {
        // Resuming an interrupted conversion (killed, or cancelled because the user quit - see
        // ThreadPool::RequestCancel/Engine::Run()'s quit handling): skip movies the manifest already
        // has recorded as done under this exact fmvVersion, so relaunching doesn't have to
        // needlessly re-encode everything that already finished. The manifest only ever records
        // a movie complete once FmvConv::Convert has moved its finished temp file into place, so
        // this can't mistake a half-written file for a done one.
        FileSystem::Path outFile = fmvOutDir;
        outFile.Append(relive::FmvNameWithoutExtension(info.mName) + ".webm");
        if (manifest->IsCompleted(info.mName) && fs.FileExists(outFile.GetPath().c_str()))
        {
            LOG_INFO("ConvertFMVs: '%s' already converted, skipping", info.mName.c_str());
            // No job will run for this movie, so credit its (just-scanned) frame count as done
            // immediately rather than leaving the category total ahead of its completed count.
            progress.AddCompleted(ConversionCategory::Fmvs, info.mFrameCount);
            continue;
        }

        tp.AddJob(std::make_unique<ConvertFmvJob>(fs, info.mName, fmvOutDir, isAo, tp, manifest, progress, info.mScan, info.mFrameCount));
    }
}
