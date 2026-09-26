#include "data_conversion/guid.hpp"
#include "stdafx.h"
#include "Movie.hpp"
#include "MovieFrameSync.hpp"
#include "Function.hpp"
#include "Psx.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../AliveLibAE/Text.hpp"
#include "../AliveLibAE/MainMenu.hpp"
#include "Sound/Midi.hpp"
#include "Sys.hpp"
#include "Sound/Sound.hpp"
#include "../AliveLibAE/VGA.hpp"
#include "../AliveLibAE/GameAutoPlayer.hpp"
#include "Engine.hpp"
#include "GameObjects/ScreenManager.hpp"
#include "Renderer/IRenderer.hpp"
#include "data_conversion/rgb_conversion.hpp"
#include "data_conversion/file_system.hpp"

#include <numeric>
#include <cstring>
#include <deque>
#include <array>
#include <atomic>
#include <chrono>
#include <memory>
#include <thread>
#include <utility>
#include <algorithm>

#pragma warning(push)
#pragma warning(disable: 4505)
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/common/webmdec.h"
#include "aom/third_party/libwebm/mkvparser/mkvparser.h"
#include "aom/third_party/libwebm/mkvparser/mkvreader.h"
#pragma warning(pop)

#include <vorbis/codec.h>

// Inputs on the controller that can be used for aborting skippable movies
const u32 MOVIE_SKIPPER_GAMEPAD_INPUTS = (InputCommands::eUnPause_OrConfirm | InputCommands::eBack | InputCommands::ePause);

// Tells whether reverb was enabled before starting the FMV
static bool wasReverbEnabled = false;
static SoundEntry sFmvSoundEntry = {};
static bool sNoAudioOrAudioError = false;
static std::atomic<u32> sFmvPlaybackId = 0;

namespace
{
    struct MkvVideoFrame final
    {
        uint64_t mPtsNs = 0;
        long long mFileOffset = 0;
        std::vector<u8> mPixels;
    };

    struct MkvAudioChunk final
    {
        uint64_t mPtsNs = 0;
        long long mFileOffset = 0;
        std::vector<u8> mBuffer;
    };

    struct MkvEncodedPacket final
    {
        uint64_t mPtsNs = 0;
        long long mFileOffset = 0;
        int mTrackNumber = 0;
        std::vector<u8> mPayload;
    };

    // Decodes Ogg Vorbis packets (demuxed from an A_VORBIS WebM audio track) back to
    // interleaved s16 PCM - the decode-side counterpart of VorbisAudioEncoder in
    // fmv_converter.cpp, which produced those packets in the first place.
    class VorbisAudioDecoder final
    {
    public:
        ~VorbisAudioDecoder()
        {
            Cleanup();
        }

        bool IsReady() const
        {
            return mReady;
        }

        void Reset()
        {
            Cleanup();
        }

        // CodecPrivate for A_VORBIS is the 3 Vorbis header packets (identification, comment,
        // setup) concatenated using Matroska's "Xiph lacing" format - see the encoder side
        // (VorbisAudioEncoder::BuildCodecPrivate in fmv_converter.cpp) for the exact layout
        // this reverses.
        bool Init(const unsigned char* pCodecPrivate, size_t codecPrivateSize)
        {
            Cleanup();

            if (!pCodecPrivate || codecPrivateSize < 1)
            {
                return false;
            }

            size_t offset = 0;
            const u8 numLengths = pCodecPrivate[offset++];
            std::vector<size_t> lengths;
            for (u8 i = 0; i < numLengths; ++i)
            {
                size_t len = 0;
                while (offset < codecPrivateSize && pCodecPrivate[offset] == 255)
                {
                    len += 255;
                    ++offset;
                }
                if (offset >= codecPrivateSize)
                {
                    return false;
                }
                len += pCodecPrivate[offset++];
                lengths.push_back(len);
            }

            size_t totalHeaderBytes = 0;
            for (const size_t len : lengths)
            {
                totalHeaderBytes += len;
            }
            if (offset + totalHeaderBytes > codecPrivateSize)
            {
                return false;
            }

            vorbis_info_init(&mInfo);
            vorbis_comment_init(&mComment);

            size_t packetOffset = offset;
            for (size_t i = 0; i <= lengths.size(); ++i)
            {
                const size_t packetLen = (i < lengths.size()) ? lengths[i] : (codecPrivateSize - packetOffset);
                ogg_packet op = {};
                op.packet = const_cast<unsigned char*>(pCodecPrivate + packetOffset);
                op.bytes = static_cast<long>(packetLen);
                op.b_o_s = (i == 0) ? 1 : 0;
                op.packetno = static_cast<long>(i);
                if (vorbis_synthesis_headerin(&mInfo, &mComment, &op) < 0)
                {
                    vorbis_comment_clear(&mComment);
                    vorbis_info_clear(&mInfo);
                    return false;
                }
                packetOffset += packetLen;
            }

            if (vorbis_synthesis_init(&mDspState, &mInfo) != 0)
            {
                vorbis_comment_clear(&mComment);
                vorbis_info_clear(&mInfo);
                return false;
            }

            vorbis_block_init(&mDspState, &mBlock);
            mReady = true;
            return true;
        }

        // Appends the decoded PCM for one packet to pcmOut (does not clear it first, so the
        // caller can accumulate several packets' worth before consuming).
        bool Decode(const std::vector<u8>& payload, u32 channels, std::vector<u8>& pcmOut)
        {
            if (!mReady || payload.empty())
            {
                return false;
            }

            ogg_packet op = {};
            op.packet = const_cast<unsigned char*>(payload.data());
            op.bytes = static_cast<long>(payload.size());

            if (vorbis_synthesis(&mBlock, &op) != 0)
            {
                return false;
            }

            if (vorbis_synthesis_blockin(&mDspState, &mBlock) != 0)
            {
                return false;
            }

            float** pPcm = nullptr;
            int samples = 0;
            while ((samples = vorbis_synthesis_pcmout(&mDspState, &pPcm)) > 0)
            {
                const size_t writeOffset = pcmOut.size();
                pcmOut.resize(writeOffset + (static_cast<size_t>(samples) * channels * sizeof(s16)));
                s16* pOut = reinterpret_cast<s16*>(pcmOut.data() + writeOffset);
                for (int i = 0; i < samples; ++i)
                {
                    for (u32 ch = 0; ch < channels; ++ch)
                    {
                        float sampleValue = pPcm[ch][i];
                        sampleValue = std::max(-1.0f, std::min(1.0f, sampleValue));
                        pOut[(i * channels) + ch] = static_cast<s16>(sampleValue * 32767.0f);
                    }
                }
                vorbis_synthesis_read(&mDspState, samples);
            }

            return true;
        }

    private:
        void Cleanup()
        {
            if (mReady)
            {
                vorbis_block_clear(&mBlock);
                vorbis_dsp_clear(&mDspState);
                vorbis_comment_clear(&mComment);
                vorbis_info_clear(&mInfo);
                mReady = false;
            }
        }

        vorbis_info mInfo = {};
        vorbis_comment mComment = {};
        vorbis_dsp_state mDspState = {};
        vorbis_block mBlock = {};
        bool mReady = false;
    };

    static constexpr size_t kMaxBufferedVideoFrames = 30;

    template<class T, size_t Capacity>
    class AVQueue final
    {
    public:
        bool TryPush(T& value)
        {
            const size_t writeIndex = mWriteIndex.load(std::memory_order_relaxed);
            const size_t nextWriteIndex = (writeIndex + 1u) % Capacity;
            if (nextWriteIndex == mReadIndex.load(std::memory_order_acquire))
            {
                return false;
            }

            mItems[writeIndex] = std::move(value);
            mWriteIndex.store(nextWriteIndex, std::memory_order_release);
            return true;
        }

        bool TryPop(T& value)
        {
            const size_t readIndex = mReadIndex.load(std::memory_order_relaxed);
            if (readIndex == mWriteIndex.load(std::memory_order_acquire))
            {
                return false;
            }

            value = std::move(mItems[readIndex]);
            mReadIndex.store((readIndex + 1u) % Capacity, std::memory_order_release);
            return true;
        }

        bool TryPeek(T& value) const
        {
            const size_t readIndex = mReadIndex.load(std::memory_order_acquire);
            if (readIndex == mWriteIndex.load(std::memory_order_acquire))
            {
                return false;
            }

            value = mItems[readIndex];
            return true;
        }

        size_t Size() const
        {
            const size_t readIndex = mReadIndex.load(std::memory_order_acquire);
            const size_t writeIndex = mWriteIndex.load(std::memory_order_acquire);
            return (writeIndex + Capacity - readIndex) % Capacity;
        }

        bool Empty() const
        {
            return mReadIndex.load(std::memory_order_acquire) == mWriteIndex.load(std::memory_order_acquire);
        }

    private:
        std::array<T, Capacity> mItems = {};
        std::atomic<size_t> mReadIndex = 0;
        std::atomic<size_t> mWriteIndex = 0;
    };

    inline void ClampToRGB(s32& value)
    {
        if (value < 0)
        {
            value = 0;
        }
        else if (value > 255)
        {
            value = 255;
        }
    }

    void ConvertI420ToRGBA(const aom_image_t* pImage, std::vector<u8>& rgbaPixels)
    {
        const u32 width = pImage->d_w;
        const u32 height = pImage->d_h;
        rgbaPixels.resize(static_cast<size_t>(width) * static_cast<size_t>(height) * 4u);

        const u8* yPlane = pImage->planes[AOM_PLANE_Y];
        const u8* uPlane = pImage->planes[AOM_PLANE_U];
        const u8* vPlane = pImage->planes[AOM_PLANE_V];

        const s32 yStride = pImage->stride[AOM_PLANE_Y];
        const s32 uStride = pImage->stride[AOM_PLANE_U];
        const s32 vStride = pImage->stride[AOM_PLANE_V];

        for (u32 y = 0; y < height; ++y)
        {
            for (u32 x = 0; x < width; ++x)
            {
                const s32 yVal = yPlane[y * yStride + x];
                const s32 uVal = uPlane[(y / 2) * uStride + (x / 2)];
                const s32 vVal = vPlane[(y / 2) * vStride + (x / 2)];

                const s32 r = (298 * (yVal - 16) + 409 * (vVal - 128) + 128) >> 8;
                const s32 g = (298 * (yVal - 16) - 100 * (uVal - 128) - 208 * (vVal - 128) + 128) >> 8;
                const s32 b = (298 * (yVal - 16) + 516 * (uVal - 128) + 128) >> 8;

                s32 rr = r;
                s32 gg = g;
                s32 bb = b;
                ClampToRGB(rr);
                ClampToRGB(gg);
                ClampToRGB(bb);

                const size_t dstIndex = (static_cast<size_t>(y) * width + x) * 4u;
                rgbaPixels[dstIndex + 0] = static_cast<u8>(rr);
                rgbaPixels[dstIndex + 1] = static_cast<u8>(gg);
                rgbaPixels[dstIndex + 2] = static_cast<u8>(bb);
                rgbaPixels[dstIndex + 3] = 255;
            }
        }
    }

    // RAII guard for an aom_codec_ctx_t. aom_codec_destroy() is only valid to call on a context
    // that aom_codec_dec_init() actually succeeded on, so Init()/Reset() track that instead of
    // relying on a caller-managed "ready" flag next to a bare aom_codec_ctx_t.
    class AutoAomCodecCtx final
    {
    public:
        ~AutoAomCodecCtx()
        {
            Reset();
        }

        bool Init(aom_codec_iface_t* pIface)
        {
            Reset();
            if (aom_codec_dec_init(&mCtx, pIface, nullptr, 0) != AOM_CODEC_OK)
            {
                return false;
            }
            mInitialized = true;
            return true;
        }

        void Reset()
        {
            if (mInitialized)
            {
                aom_codec_destroy(&mCtx);
                mInitialized = false;
            }
        }

        aom_codec_ctx_t* Get()
        {
            return &mCtx;
        }

    private:
        aom_codec_ctx_t mCtx = {};
        bool mInitialized = false;
    };

    class WebmMoviePlayer final
    {
    public:
        WebmMoviePlayer() = default;
        ~WebmMoviePlayer()
        {
            Cleanup();
        }

        bool Open(const char_type* pMovieName)
        {
            Cleanup();

            if (!pMovieName || !*pMovieName)
            {
                return false;
            }

            const std::vector<std::string> candidateNames =
            {
                std::string(pMovieName),
                std::string(pMovieName) + ".webm",
                std::string(pMovieName) + ".mkv",
                std::string(pMovieName) + ".WEBM",
                std::string(pMovieName) + ".MKV",
            };

            for (const auto& candidate : candidateNames)
            {
                if (TryOpenFile(candidate))
                {
                    return true;
                }
            }

            return false;
        }

        bool DemuxNext(MkvEncodedPacket& packet)
        {
            packet = {};
            if (!mSegment || mParsingComplete)
            {
                return false;
            }

            if (!mCurrentCluster)
            {
                mCurrentCluster = mSegment->GetFirst();
                mCurrentBlockEntry = nullptr;
            }

            auto advanceToNextCluster = [&]() {
                const mkvparser::Cluster* pNextCluster = mSegment->GetNext(mCurrentCluster);
                if (pNextCluster == nullptr || pNextCluster->EOS())
                {
                    LOG_INFO("FMV parser: reached EOS cluster, parsingComplete");
                    mCurrentCluster = nullptr;
                    mCurrentBlockEntry = nullptr;
                    mCurrentFrameIndex = 0;
                    mParsingComplete = true;
                    return false;
                }

                LOG_INFO("FMV parser: cluster advance to %p", static_cast<const void*>(pNextCluster));
                mCurrentCluster = pNextCluster;
                mCurrentBlockEntry = nullptr;
                mCurrentFrameIndex = 0;
                return true;
            };

            while (mCurrentCluster && !mCurrentCluster->EOS())
            {
                for (;;)
                {
                    if (mCurrentBlockEntry == nullptr)
                    {
                        const long status = mCurrentCluster->GetFirst(mCurrentBlockEntry);
                        if (status != 0 || mCurrentBlockEntry == nullptr || mCurrentBlockEntry->EOS())
                        {
                            break;
                        }
                    }
                    else if (mCurrentBlockEntry->GetBlock() && mCurrentFrameIndex >= mCurrentBlockEntry->GetBlock()->GetFrameCount())
                    {
                        const mkvparser::BlockEntry* pNextBlockEntry = nullptr;
                        const long status = mCurrentCluster->GetNext(mCurrentBlockEntry, pNextBlockEntry);
                        if (status != 0 || pNextBlockEntry == nullptr || pNextBlockEntry->EOS())
                        {
                            break;
                        }
                        mCurrentBlockEntry = pNextBlockEntry;
                        mCurrentFrameIndex = 0;
                    }

                    if (mCurrentBlockEntry == nullptr || mCurrentBlockEntry->EOS())
                    {
                        break;
                    }

                    const mkvparser::Block* pBlock = mCurrentBlockEntry->GetBlock();
                    if (!pBlock)
                    {
                        continue;
                    }

                    const mkvparser::Block::Frame& frame = pBlock->GetFrame(mCurrentFrameIndex++);
                    std::vector<u8> payload(static_cast<size_t>(frame.len));
                    if (frame.Read(mReader.get(), payload.data()) != 0)
                    {
                        mParsingComplete = true;
                        return false;
                    }

                    const int trackNumber = static_cast<int>(pBlock->GetTrackNumber());
                    if (trackNumber != mVideoTrackNumber && trackNumber != mAudioTrackNumber)
                    {
                        continue;
                    }

                    packet.mPtsNs = static_cast<u64>(pBlock->GetTime(mCurrentCluster));
                    packet.mFileOffset = frame.pos;
                    packet.mTrackNumber = trackNumber;
                    packet.mPayload = std::move(payload);
                    return true;
                }

                if (!advanceToNextCluster())
                {
                    return false;
                }
            }

            mParsingComplete = true;
            return false;
        }

        bool ParsingComplete() const
        {
            return mParsingComplete;
        }

        int VideoTrackNumber() const
        {
            return mVideoTrackNumber;
        }

        u32 Width() const
        {
            return mWidth;
        }

        u32 Height() const
        {
            return mHeight;
        }

        bool Parse()
        {
            if (!mSegment)
            {
                return false;
            }

            const mkvparser::Tracks* pTracks = mSegment->GetTracks();
            if (!pTracks)
            {
                return false;
            }

            for (unsigned long i = 0; i < pTracks->GetTracksCount(); ++i)
            {
                const mkvparser::Track* pTrack = pTracks->GetTrackByIndex(i);
                if (!pTrack)
                {
                    continue;
                }

                if (pTrack->GetType() == mkvparser::Track::kVideo)
                {
                    mVideoTrack = static_cast<const mkvparser::VideoTrack*>(pTrack);
                    mVideoTrackNumber = static_cast<int>(pTrack->GetNumber());
                    if (mVideoTrack && mVideoTrack->GetWidth() > 0)
                    {
                        mWidth = static_cast<u32>(mVideoTrack->GetWidth());
                    }
                    if (mVideoTrack && mVideoTrack->GetHeight() > 0)
                    {
                        mHeight = static_cast<u32>(mVideoTrack->GetHeight());
                    }
                }
                else if (pTrack->GetType() == mkvparser::Track::kAudio)
                {
                    mAudioTrack = static_cast<const mkvparser::AudioTrack*>(pTrack);
                    mAudioTrackNumber = static_cast<int>(pTrack->GetNumber());
                    if (mAudioTrack)
                    {
                        mAudioSampleRate = static_cast<u32>(mAudioTrack->GetSamplingRate());
                        mAudioChannels = static_cast<u32>(mAudioTrack->GetChannels());
                        mAudioBitsPerSample = static_cast<u32>(mAudioTrack->GetBitDepth());

                        const char* pCodecId = mAudioTrack->GetCodecId();
                        if (pCodecId && std::strcmp(pCodecId, "A_VORBIS") == 0)
                        {
                            size_t codecPrivateSize = 0;
                            const unsigned char* pCodecPrivate = mAudioTrack->GetCodecPrivate(codecPrivateSize);
                            mIsVorbisAudio = mVorbisDecoder.Init(pCodecPrivate, codecPrivateSize);
                        }
                    }
                }
            }

            if (!mVideoTrack)
            {
                return false;
            }

            // A "video freezes then drops a big backlog at once" report traced back to this
            // decode call taking 20-70ms per (320x240!) frame - only ever reproduces in an
            // unoptimized debug build (a Release build keeps every frame comfortably under the
            // ~67ms/frame budget at 15fps, dropped=0), so it's not a real decode throughput
            // problem. Tried multi-threaded decode here as a workaround, confirmed it does clear
            // up the debug-build stutter, but it also produced visible tile-decode corruption
            // (vertical banding) - not worth it to paper over a debug-only slowdown, so left on
            // the single decode thread default.
            if (!mCodec.Init(aom_codec_av1_dx()))
            {
                return false;
            }

            mCurrentCluster = mSegment->GetFirst();
            mCurrentBlockEntry = nullptr;
            mParsingComplete = false;

            return true;
        }

        u32 AudioSampleRate() const
        {
            return mAudioSampleRate;
        }

        u32 AudioChannels() const
        {
            return mAudioChannels;
        }

        u32 AudioBitsPerSample() const
        {
            return mAudioBitsPerSample;
        }

        bool HasAudio() const
        {
            return mAudioTrack != nullptr;
        }

        bool DecodeVideo(const MkvEncodedPacket& packet, MkvVideoFrame& frame)
        {
            frame.mPtsNs = packet.mPtsNs;
            frame.mFileOffset = packet.mFileOffset;
            return DecodeAv1Frame(packet.mPayload, frame.mPixels);
        }

        // Vorbis packets decode to a variable number of PCM sample frames each, so pcmOut's
        // size isn't knowable up front the way a fixed-format PCM passthrough's is. Non-Vorbis
        // (legacy raw A_PCM) files are passed straight through unchanged for backwards
        // compatibility with any webm converted before Vorbis audio was added.
        bool DecodeAudio(const MkvEncodedPacket& packet, std::vector<u8>& pcmOut)
        {
            pcmOut.clear();
            if (packet.mPayload.empty())
            {
                return false;
            }

            if (!mIsVorbisAudio)
            {
                pcmOut = packet.mPayload;
                return true;
            }

            return mVorbisDecoder.Decode(packet.mPayload, mAudioChannels, pcmOut);
        }

    private:
        bool TryOpenFile(const std::string& path)
        {
            FileSystem fs;
            mMovieFile = fs.OpenFile(path.c_str(), "rb");
            if (!mMovieFile.GetFile())
            {
                return false;
            }

            mReader = std::make_unique<mkvparser::MkvReader>(mMovieFile.GetFile());

            mkvparser::EBMLHeader header;
            long long pos = 0;
            if (header.Parse(mReader.get(), pos) < 0)
            {
                Cleanup();
                return false;
            }

            mkvparser::Segment* pSegment = nullptr;
            if (mkvparser::Segment::CreateInstance(mReader.get(), pos, pSegment) != 0)
            {
                Cleanup();
                return false;
            }
            mSegment.reset(pSegment);

            if (mSegment->Load() < 0)
            {
                Cleanup();
                return false;
            }

            return true;
        }

        bool DecodeAv1Frame(const std::vector<u8>& compressedFrame, std::vector<u8>& rgbaPixels)
        {
            if (compressedFrame.empty())
            {
                return false;
            }

            aom_codec_err_t status = aom_codec_decode(mCodec.Get(), compressedFrame.data(), static_cast<unsigned int>(compressedFrame.size()), nullptr);
            if (status != AOM_CODEC_OK)
            {
                return false;
            }

            aom_codec_iter_t iter = nullptr;
            aom_image_t* pImage = nullptr;
            while ((pImage = aom_codec_get_frame(mCodec.Get(), &iter)) != nullptr)
            {
                ConvertI420ToRGBA(pImage, rgbaPixels);
                return true;
            }

            return false;
        }

        void Cleanup()
        {
            mCodec.Reset();

            mVorbisDecoder.Reset();
            mIsVorbisAudio = false;

            // Reset in this order (segment, then reader, then file) to mirror the ownership
            // chain: the segment references the reader, and the reader (constructed from an
            // already-open FILE*, see MkvReader(FILE*)) never closes the file itself.
            mSegment.reset();
            mReader.reset();
            mMovieFile.Close();

            mVideoTrack = nullptr;
            mAudioTrack = nullptr;
            mVideoTrackNumber = 0;
            mAudioTrackNumber = 0;
            mCurrentCluster = nullptr;
            mCurrentBlockEntry = nullptr;
            mCurrentFrameIndex = 0;
            mParsingComplete = false;
            mAudioSampleRate = 44100;
            mAudioChannels = 2;
            mAudioBitsPerSample = 16;
            mWidth = 640;
            mHeight = 240;
        }

        AutoFILE mMovieFile;
        std::unique_ptr<mkvparser::MkvReader> mReader;
        std::unique_ptr<mkvparser::Segment> mSegment;
        const mkvparser::VideoTrack* mVideoTrack = nullptr;
        const mkvparser::AudioTrack* mAudioTrack = nullptr;
        std::deque<MkvVideoFrame> mVideoFrames;
        std::deque<MkvAudioChunk> mAudioChunks;
        const mkvparser::Cluster* mCurrentCluster = nullptr;
        const mkvparser::BlockEntry* mCurrentBlockEntry = nullptr;
        int mCurrentFrameIndex = 0;
        std::atomic_bool mParsingComplete = false;
        AutoAomCodecCtx mCodec;
        int mVideoTrackNumber = 0;
        int mAudioTrackNumber = 0;
        u32 mWidth = 640;
        u32 mHeight = 240;
        u32 mAudioSampleRate = 44100;
        u32 mAudioChannels = 2;
        u32 mAudioBitsPerSample = 16;
        VorbisAudioDecoder mVorbisDecoder;
        bool mIsVorbisAudio = false;
    };

    using MkvPacketQueue = AVQueue<MkvEncodedPacket, 128u>;
    using MkvVideoQueue = AVQueue<MkvVideoFrame, kMaxBufferedVideoFrames + 1u>;
    using MkvAudioQueue = AVQueue<MkvAudioChunk, 128u>;

    class MkvMoviePipeline final
    {
    public:
        MkvMoviePipeline(WebmMoviePlayer& movie, MkvVideoQueue& videoQueue, MkvAudioQueue& audioQueue)
            : mMovie(movie)
            , mVideoQueue(videoQueue)
            , mAudioQueue(audioQueue)
            , mDemuxThread(&MkvMoviePipeline::DemuxRun, this)
            , mVideoThread(&MkvMoviePipeline::VideoRun, this)
            , mAudioThread(&MkvMoviePipeline::AudioRun, this)
        {
        }

        ~MkvMoviePipeline()
        {
            mStop = true;
            Join(mDemuxThread);
            Join(mVideoThread);
            Join(mAudioThread);
        }

        MkvMoviePipeline(const MkvMoviePipeline&) = delete;
        MkvMoviePipeline& operator=(const MkvMoviePipeline&) = delete;

        bool VideoComplete() const
        {
            return mVideoComplete && mVideoQueue.Empty();
        }

        bool AudioComplete() const
        {
            return mAudioComplete && mAudioQueue.Empty();
        }

    private:
        static void Join(std::thread& thread)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }

        void DemuxRun()
        {
            MkvEncodedPacket packet;
            while (!mStop && mMovie.DemuxNext(packet))
            {
                MkvPacketQueue& queue = packet.mTrackNumber == mMovie.VideoTrackNumber() ? mVideoPackets : mAudioPackets;
                while (!mStop && !queue.TryPush(packet))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                packet = {};
            }
            mDemuxComplete = true;
        }

        void VideoRun()
        {
            MkvEncodedPacket packet;
            while (!mStop && (!mDemuxComplete || !mVideoPackets.Empty()))
            {
                if (!mVideoPackets.TryPop(packet))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

                MkvVideoFrame frame;
                if (mMovie.DecodeVideo(packet, frame))
                {
                    while (!mStop && !mVideoQueue.TryPush(frame))
                    {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                }
                packet = {};
            }
            mVideoComplete = true;
        }

        void AudioRun()
        {
            MkvEncodedPacket packet;
            while (!mStop && (!mDemuxComplete || !mAudioPackets.Empty()))
            {
                if (!mAudioPackets.TryPop(packet))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    continue;
                }

                MkvAudioChunk chunk;
                chunk.mPtsNs = packet.mPtsNs;
                chunk.mFileOffset = packet.mFileOffset;
                if (!mMovie.DecodeAudio(packet, chunk.mBuffer))
                {
                    packet = {};
                    continue;
                }
                while (!mStop && !mAudioQueue.TryPush(chunk))
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                packet = {};
            }
            mAudioComplete = true;
        }

        WebmMoviePlayer& mMovie;
        MkvVideoQueue& mVideoQueue;
        MkvAudioQueue& mAudioQueue;
        MkvPacketQueue mVideoPackets;
        MkvPacketQueue mAudioPackets;
        std::atomic_bool mStop = false;
        std::atomic_bool mDemuxComplete = false;
        std::atomic_bool mVideoComplete = false;
        std::atomic_bool mAudioComplete = false;
        std::thread mDemuxThread;
        std::thread mVideoThread;
        std::thread mAudioThread;
    };

    // Real IMovieSyncClock backing DDV_Play_Impl's own playback state - see MovieFrameSync.hpp
    // for the fake used by MovieFrameSyncTests.cpp instead of this.
    class RealMovieSyncClock final : public IMovieSyncClock
    {
    public:
        RealMovieSyncClock(bool& audioStarted, u64& audioStartSample)
            : mAudioStarted(audioStarted)
            , mAudioStartSample(audioStartSample)
        {
        }

        bool AudioStarted() const override
        {
            return mAudioStarted;
        }

        u64 AudioClockMs() const override
        {
            return (SND_Get_Generated_Audio_Samples() - mAudioStartSample) * 1000 / SND_Get_Device_Sample_Rate();
        }

        bool SkipRequested() const override
        {
            return AreMovieSkippingInputsHeld();
        }

        void PumpIdle() override
        {
            SYS_EventsPump();
            PSX_VSync(VSyncMode::UncappedFps);
        }

    private:
        bool& mAudioStarted;
        u64& mAudioStartSample;
    };
}

static void Render_DDV_Frame(Poly_FT4* poly)
{
    IRenderer::GetRenderer()->Draw(*poly);
    VGA_EndFrame();
    IRenderer::GetRenderer()->StartFrame();
}

s8 DDV_Play_Impl(const char_type* pMovieName)
{
    if (!pMovieName || !*pMovieName)
    {
        return 1;
    }

    while (AreMovieSkippingInputsHeld())
    {
        SYS_EventsPump();
    }

    WebmMoviePlayer movie;
    if (!movie.Open(pMovieName) || !movie.Parse())
    {
        return 0;
    }

    const u32 playbackId = ++sFmvPlaybackId;
    u32 renderedFrameCount = 0;
    u32 droppedFrameCount = 0;
    u32 staleFrameDisplayCount = 0;
    u32 invalidDisplayedFrameCount = 0;
    bool haveLastDisplayedOffset = false;
    long long lastDisplayedOffset = 0;
    u64 lastDisplayUpdateMs = SYS_GetTicks();
    LOG_INFO("FMV playback %u: started", playbackId);

    const bool hasAudio = movie.HasAudio();

    sNoAudioOrAudioError = false;
    if (hasAudio)
    {
        #if USE_SDL3_SOUND
        wasReverbEnabled = gReverbEnabled;
        gReverbEnabled = false;
        #endif

        const u32 sampleRate = movie.AudioSampleRate();
        const u32 bitDepth = movie.AudioBitsPerSample();
        const u32 channels = movie.AudioChannels();
        const s32 soundFlags = channels > 1 ? 7 : (bitDepth == 16 ? 2 : 0);
        const u32 audioBufferSamples = std::max<u32>(sampleRate * 4u, 4096u);

        if (GetSoundAPI().mSND_New(&sFmvSoundEntry, static_cast<s32>(audioBufferSamples), sampleRate, bitDepth, soundFlags) < 0)
        {
            sFmvSoundEntry.field_4_pDSoundBuffer = nullptr;
            sNoAudioOrAudioError = true;
        }
    }
    else
    {
        sNoAudioOrAudioError = true;
    }

    CamResource fmvFrame;
    fmvFrame.mData.mWidth = movie.Width();
    fmvFrame.mData.mHeight = movie.Height();
    fmvFrame.mData.mPixels = std::make_shared<std::vector<u8>>();
    fmvFrame.mData.mPixels->resize(fmvFrame.mData.mWidth * fmvFrame.mData.mHeight * sizeof(RGBA32));

    Poly_FT4 polyFT4 = {};
    polyFT4.SetXYWH(0, 0, 640, 240);
    polyFT4.mCam = &fmvFrame;

    // Shared by both the normal (in-sync) render path and the stale-frame-while-catching-up
    // path below (see ShouldDisplayStaleFrame) - both paint real decoded pixels to the screen,
    // so both should feed the same non-increasing-screen-content diagnostic below.
    auto DisplayFrame = [&](const MkvVideoFrame& f)
    {
        std::memcpy(fmvFrame.mData.mPixels->data(), f.mPixels.data(), f.mPixels.size());

        Input_IsVKPressed_4EDD40(VK_ESCAPE);
        Input_IsVKPressed_4EDD40(VK_RETURN);

        polyFT4.mCam->mUniqueId = UniqueResId{};
        Render_DDV_Frame(&polyFT4);

        // Keep the "camera" ScreenManager draws into the ordering table every tick (behind
        // whatever Movie's own direct Render_DDV_Frame presents while a movie is actively
        // playing) in sync with the last FMV frame actually shown. CameraSwapper deliberately
        // doesn't apply its own (real, pre-chain) camera to ScreenManager until an entire
        // multi-FMV chain finishes, so without this, ScreenManager's per-tick draw is stuck
        // showing whatever camera was active before the chain even started - invisible while a
        // movie's own rendering is running, but visible as a one-tick flash of that stale camera
        // in the gap between one chained FMV finishing and the next one starting (a whole
        // movie's blocking playback runs within a single object-update pass, so that pass's own
        // ScreenManager render already happened by the time CameraSwapper gets a chance to react
        // and start the next movie in the chain - see CameraSwapper's ePlay2FMVs_9/
        // ePlay3FMVs_10).
        if (gScreenManager)
        {
            gScreenManager->DecompressCameraToVRam(fmvFrame);
        }

        // mFileOffset already comes from the demuxer strictly increasing (each video packet
        // occupies a later position in the file than the last) - the same "same or earlier
        // offset shown twice" pipeline bug a full pixel-content hash would have been trying to
        // catch here shows up for free as this offset failing to have advanced, no need to hash
        // ~300KB of every displayed frame to get that.
        if (haveLastDisplayedOffset && f.mFileOffset <= lastDisplayedOffset)
        {
            ++invalidDisplayedFrameCount;
            LOG_ERROR("FMV playback %u: non-increasing screen offset=%lld previous=%lld pts=%llu",
                playbackId, f.mFileOffset, lastDisplayedOffset, static_cast<unsigned long long>(f.mPtsNs));
        }
        haveLastDisplayedOffset = true;
        lastDisplayedOffset = f.mFileOffset;
        lastDisplayUpdateMs = SYS_GetTicks();
    };

    MkvVideoQueue videoQueue;
    MkvAudioQueue audioQueue;
    auto moviePipeline = std::make_unique<MkvMoviePipeline>(movie, videoQueue, audioQueue);

    u64 audioStartSample = 0;
    std::deque<MkvAudioChunk> pendingAudioChunks;
    const u32 blockAlign = (movie.AudioBitsPerSample() / 8u) * movie.AudioChannels();
    const u32 audioBufferSamples = std::max<u32>(movie.AudioSampleRate() * 4u, 4096u);
    u32 audioWriteOffset = 0;
    u32 audioSamplesSubmitted = 0;
    u32 audioWriteCount = 0;
    bool audioStarted = false;
    bool audioFinished = !hasAudio || sNoAudioOrAudioError;

    while (!videoQueue.Empty() || !moviePipeline->VideoComplete())
    {
        MkvAudioChunk audioChunk;
        while (audioQueue.TryPop(audioChunk))
        {
            pendingAudioChunks.push_back(std::move(audioChunk));
        }

        const u32 maxBufferedSamples = audioBufferSamples - std::min<u32>(audioBufferSamples / 4u, 1024u);
        while (!sNoAudioOrAudioError && hasAudio && !pendingAudioChunks.empty())
        {
            const u32 readOffset = audioStarted
                ? GetSoundAPI().mSND_Get_Sound_Entry_Pos(&sFmvSoundEntry)
                : 0;
            const u32 bufferedSamples = audioStarted
                ? (audioWriteOffset >= readOffset ? audioWriteOffset - readOffset : audioBufferSamples - readOffset + audioWriteOffset)
                : audioSamplesSubmitted;
            if (bufferedSamples >= maxBufferedSamples)
            {
                break;
            }

            MkvAudioChunk& pendingChunk = pendingAudioChunks.front();
            const u32 pendingSamples = static_cast<u32>(pendingChunk.mBuffer.size() / std::max<u32>(1u, blockAlign));
            if (pendingSamples == 0)
            {
                pendingAudioChunks.pop_front();
                continue;
            }

            const u32 bufferSpaceSamples = maxBufferedSamples - bufferedSamples;
            const u32 samplesUntilBufferEnd = audioBufferSamples - audioWriteOffset;
            const u32 samplesToWrite = std::min({pendingSamples, bufferSpaceSamples, samplesUntilBufferEnd});
            if (samplesToWrite == 0)
            {
                audioWriteOffset = 0;
                continue;
            }

            u64 audioHash = 1469598103934665603ULL;
            const size_t bytesToWrite = static_cast<size_t>(samplesToWrite) * blockAlign;
            for (size_t byteIndex = 0; byteIndex < bytesToWrite; ++byteIndex)
            {
                audioHash ^= pendingChunk.mBuffer[byteIndex];
                audioHash *= 1099511628211ULL;
            }

            if (GetSoundAPI().mSND_LoadSamples(&sFmvSoundEntry, audioWriteOffset, pendingChunk.mBuffer.data(), samplesToWrite) < 0)
            {
                sNoAudioOrAudioError = true;
                break;
            }
            ++audioWriteCount;
            LOG_INFO("FMV playback %u: audio write=%u sourceOffset=%lld pts=%llu writeOffset=%u readOffset=%u samples=%u hash=%llu",
                playbackId, audioWriteCount, pendingChunk.mFileOffset,
                static_cast<unsigned long long>(pendingChunk.mPtsNs), audioWriteOffset, readOffset, samplesToWrite,
                static_cast<unsigned long long>(audioHash));
            audioWriteOffset = (audioWriteOffset + samplesToWrite) % audioBufferSamples;
            audioSamplesSubmitted += samplesToWrite;
            pendingChunk.mBuffer.erase(pendingChunk.mBuffer.begin(), pendingChunk.mBuffer.begin() + samplesToWrite * blockAlign);
            if (pendingChunk.mBuffer.empty())
            {
                pendingAudioChunks.pop_front();
            }

            if (!audioStarted && (audioSamplesSubmitted >= movie.AudioSampleRate() / 5u || moviePipeline->AudioComplete()))
            {
                if (FAILED(SND_PlayEx(&sFmvSoundEntry, 116, 116, 1.0, 0, 1, 100)))
                {
                    sNoAudioOrAudioError = true;
                }
                audioStartSample = SND_Get_Generated_Audio_Samples();
                audioStarted = !sNoAudioOrAudioError;
            }
        }

        if (audioStarted && !audioFinished && moviePipeline->AudioComplete() && pendingAudioChunks.empty()
            && SND_Get_Generated_Audio_Samples() - audioStartSample >= audioSamplesSubmitted)
        {
            SND_StopAll();
            audioStarted = false;
            audioFinished = true;
            LOG_INFO("FMV playback %u: audio finished samples=%u", playbackId, audioSamplesSubmitted);
        }

        if (hasAudio && !audioStarted && !audioFinished && !sNoAudioOrAudioError)
        {
            if ((SYS_GetTicks() & 255) < 2)
            {
                LOG_INFO("FMV playback %u: waiting for audio preroll samples=%u pending=%zu", playbackId,
                    audioSamplesSubmitted, pendingAudioChunks.size());
            }
            SYS_EventsPump();
            PSX_VSync(VSyncMode::UncappedFps);
            continue;
        }

        MkvVideoFrame frame;
        if (!videoQueue.TryPop(frame))
        {
            if (moviePipeline->VideoComplete())
            {
                break;
            }
            SYS_EventsPump();
            PSX_VSync(VSyncMode::UncappedFps);
            continue;
        }

        // SND_Get_Generated_Audio_Samples() counts samples at the mixer's fixed output rate
        // (every voice, including this movie's, gets resampled to it - see
        // SDLSoundBuffer::SetFrequency) - so it must be divided by that device rate here, not
        // by the movie's own AudioSampleRate(). AE's DDV audio happens to already be 44100Hz,
        // matching the (also 44100Hz) device rate, which is why this was previously masked;
        // AO's true 18900Hz stream exposed it as frames dropping and audio going out of sync.
        LOG_INFO("FMV playback %u: dequeued offset=%lld pts=%llu clock=%llu", playbackId, frame.mFileOffset,
            static_cast<unsigned long long>(frame.mPtsNs),
            static_cast<unsigned long long>(audioStarted
                ? (SND_Get_Generated_Audio_Samples() - audioStartSample) * 1000 / SND_Get_Device_Sample_Rate()
                : 0));

        if (AreMovieSkippingInputsHeld())
        {
            break;
        }

        const u64 frameMs = frame.mPtsNs / 1000000ULL;
        RealMovieSyncClock syncClock(audioStarted, audioStartSample);
        const MovieFrameOutcome syncOutcome = ProcessMovieFrameSync(frameMs, syncClock);
        if (syncOutcome == MovieFrameOutcome::Dropped)
        {
            ++droppedFrameCount;

            // Otherwise, with a big enough backlog of stale/behind frames to drop, the screen
            // sits on whatever was last actually rendered for however long the backlog takes to
            // clear, then jumps straight to current - looks frozen, then hitches. Occasionally
            // painting one of the stale frames anyway (throttled by wall-clock time so it
            // doesn't slow down actually catching up) makes it read as fast-forwarding instead.
            const u64 nowMs = SYS_GetTicks();
            if (ShouldDisplayStaleFrame(nowMs, lastDisplayUpdateMs))
            {
                ++staleFrameDisplayCount;
                DisplayFrame(frame);
                LOG_INFO("FMV playback %u: stale frame while catching up offset=%lld pts=%llu dropped=%u queued=%zu",
                    playbackId, frame.mFileOffset, static_cast<unsigned long long>(frame.mPtsNs),
                    droppedFrameCount, videoQueue.Size());
            }
            continue;
        }
        if (syncOutcome == MovieFrameOutcome::SkippedByUserInput)
        {
            moviePipeline.reset();
            break;
        }

        LOG_INFO("FMV playback: render frame pts=%llu queued=%zu", static_cast<unsigned long long>(frame.mPtsNs), videoQueue.Size());
        ++renderedFrameCount;
        DisplayFrame(frame);
        LOG_INFO("FMV playback %u: screen frame=%u offset=%lld pts=%llu clock=%llu queued=%zu", playbackId, renderedFrameCount,
            frame.mFileOffset, static_cast<unsigned long long>(frame.mPtsNs),
            static_cast<unsigned long long>(audioStarted
                ? (SND_Get_Generated_Audio_Samples() - audioStartSample) * 1000 / SND_Get_Device_Sample_Rate()
                : 0),
            videoQueue.Size());

        SYS_EventsPump();
        PSX_VSync(VSyncMode::UncappedFps);
    }

    moviePipeline.reset();

    LOG_INFO("FMV playback %u: finished rendered=%u dropped=%u staleDisplayed=%u invalidDisplayed=%u", playbackId,
        renderedFrameCount, droppedFrameCount, staleFrameDisplayCount, invalidDisplayedFrameCount);

    if (sFmvSoundEntry.field_4_pDSoundBuffer)
    {
        SND_StopAll();
        GetSoundAPI().mSND_Free(&sFmvSoundEntry);
        sFmvSoundEntry.field_4_pDSoundBuffer = nullptr;
    }

    return 1;
}

s8 DDV_Play(const char_type* pDDVName)
{
    gMovieSoundEntry = &sFmvSoundEntry;
    const s8 ret = DDV_Play_Impl(pDDVName);
    gMovieSoundEntry = nullptr;
    return ret;
}

s32 Movie::gMovieRefCount = 0;

void Movie::VScreenChanged()
{
    // Null sub 0x4E02A0
}

void Movie::Init()
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eMovie);

    ++Movie::gMovieRefCount;
}

Movie::Movie(const char_type* pName, ResourceManagerWrapper& resMan, BaseMap& map)
    : BaseGameObject(true, 0, resMan, map)
{
    mName = resMan.FmvPath(pName);
    Init();
}

extern bool gBreakGameLoop;

void Movie::VUpdate()
{
    LOG_INFO("Movie VUpdate begin break=%d", gBreakGameLoop ? 1 : 0);
    if (gBreakGameLoop)
    {
        SetDead(true);
    }
    else if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
    {
        SetDead(true);
    }
    else
    {
        SND_StopAll();

        while (!DDV_Play(mName.c_str()))
        {
            if (gAttract)
            {
                break;
            }

            if (!Display_Full_Screen_Message_Blocking(MessageType::eSkipMovie_1, mResMan, mMap))
            {
                break;
            }
        }
    }
    DeInit();
    //gBreakGameLoop = true;
    //LOG_INFO("Movie VUpdate complete break=%d", gBreakGameLoop ? 1 : 0);
}

void Movie::DeInit()
{
    PSX_VSync(VSyncMode::LimitTo30Fps);

    --Movie::gMovieRefCount;

    #if USE_SDL3_SOUND
    gReverbEnabled = wasReverbEnabled;
    #endif

    SetDead(true);
}

bool AreMovieSkippingInputsHeld()
{
    if (Input().IsJoyStickEnabled())
    {
        return (Input_Read_Pad(sCurrentControllerIndex) & MOVIE_SKIPPER_GAMEPAD_INPUTS) != 0;
    }
    else
    {
        return Input_IsVKPressed_4EDD40(VK_ESCAPE) || Input_IsVKPressed_4EDD40(VK_RETURN);
    }
}
