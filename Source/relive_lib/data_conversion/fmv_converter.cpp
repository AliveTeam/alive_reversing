#include "fmv_converter.hpp"
#include "../../AliveLibAE/PathData.hpp"
#include "../FatalError.hpp"
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

#include "../Masher.hpp"
#include "rgb_conversion.hpp"
#include "file_system.hpp"


// TODO: An interface around masher + mdec reading
class IDDVReader
{
public:
    IDDVReader() = default;

    ~IDDVReader()
    {
        delete mMasher;
    }

    virtual bool Open(const char_type* ddvName)
    {
        if (mMasher)
        {
            delete mMasher;
            mMasher = nullptr;
        }

        mMasher = new Masher();
        if (!mMasher)
        {
            return false;
        }

        if (mMasher->Init(ddvName) != 0)
        {
            delete mMasher;
            mMasher = nullptr;
            return false;
        }

        return true;
    }

    u32 FrameWidth() const
    {
        return mMasher ? mMasher->field_14_video_header.field_4_width : 0;
    }

    u32 FrameHeight() const
    {
        return mMasher ? mMasher->field_14_video_header.field_8_height : 0;
    }

    u32 FrameRate() const
    {
        return mMasher ? static_cast<u32>(mMasher->field_4_ddv_header.field_8_frame_rate) : 15u;
    }

    u32 TotalVideoFrames() const
    {
        return mMasher ? static_cast<u32>(mMasher->field_4_ddv_header.field_C_number_of_frames) : 0u;
    }

    bool ReadVideoFrame(RGBA32* pPixelBuffer)
    {
        if (!mMasher || !pPixelBuffer)
        {
            return false;
        }

        if (!mMasher->ReadNextFrame())
        {
            return false;
        }

        mMasher->VideoFrameDecode(pPixelBuffer);
        return true;
    }

    s8 Mash_DecompressAudio()
    {
        if (!mHasAudio)
        {
            return 1;
        }

        u32 audioBufferStartOffset = 0;
        mAudioSampleOffset = 0;

        // Keep reading frames till we have >= number of interleaved so that we have 1 full frame
        if (mNumReadFrames < mMasher->field_2C_audio_header.field_10_num_frames_interleave)
        {
            while (Masher::ReadNextFrameToMemory_4EAC30(mMasher))
            {
                //const int bitsPerSample = (mMasher->field_2C_audio_header.field_0_audio_format & 2) ? 16 : 8;
                //const int channels = (mMasher->field_2C_audio_header.field_0_audio_format & 1) ? 2 : 1;

                void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(mMasher);

                if (pDecompressedAudioFrame)
                {
                    //ffmpeg_push_audio(reinterpret_cast<u8*>(pDecompressedAudioFrame), (bitsPerSample / 8) * mSingleAudioFrameSizeInSamples * channels);

                    /*if (GetSoundAPI().SND_LoadSamples(
                        &g_fmv_sound_entry_5CA208,
                        mAudioSampleOffset,
                        (u8*)pDecompressedAudioBuffer,
                        mSingleAudioFrameSizeInSamples))
                    {
                        mNoAudioOrAudioError = 1;
                    }*/
                }

                mAudioSampleOffset += mSingleAudioFrameSizeInSamples;
                audioBufferStartOffset = mAudioSampleOffset;
                mNumReadFrames++;

                if (mNumReadFrames >= mMasher->field_2C_audio_header.field_10_num_frames_interleave)
                {
                    break;
                }
            }
        }

        if (mNumReadFrames >= mMasher->field_2C_audio_header.field_10_num_frames_interleave)
        {
            // Update the offset to the size of the first demuxed frame
            mCurrentAudioOffset = audioBufferStartOffset;
            if (!mNoAudioOrAudioError)
            {
                // Sound entry is created and populated with 1 frame, play it
                /*if (FAILED(GetSoundAPI().SND_PlayEx(&fmv_sound_entry_5CA208, 116, 116, 1.0, 0, 1, 100)))
                {
                    mNoAudioOrAudioError = 1;
                }*/
            }
            mNumPlayedAudioFrames = 0;
            mOldBufferPlayPos = 0;
            return 1;
        }

        return 0;
    }

    bool InitDDVPlayback(const std::string& filePath, bool ffmpegExport)
    {
        mHasAudio = 0;
        mAudioSampleOffset = 0;
        mNumReadFrames = 0;
        mNoAudioOrAudioError = 0;
        mSingleAudioFrameSizeInSamples = 0;
        mCurrentAudioOffset = 0;
        mNumPlayedAudioFrames = 0;
        mOldBufferPlayPos = 0;

        //gFrameBuffer.resize(640 * 480 * 4);

        if (mMasher != nullptr)
        {
            delete mMasher;
            mMasher = nullptr;
        }

        mMasher = new Masher();

        //gMasherTexture = CreateVideoPlayerTexture();

        mMasher->Init(filePath.c_str());

        /*
        AssetFMVParams params;
        params.video.fps = 15;
        params.video.width = mMasher->field_14_video_header.field_4_width;
        params.video.height = mMasher->field_14_video_header.field_8_height;
        params.outputPath = filePath;

        params.audio.audioSampleRate = mMasher->field_2C_audio_header.field_4_samples_per_second;
        params.audio.audioChannels = (mMasher->field_2C_audio_header.field_0_audio_format & 1) ? 2 : 1;
        */

        if (ffmpegExport)
        {
            //ffmpeg_begin(params);
        }

        mHasAudio = ((u32) mMasher->field_4_ddv_header.field_4_contains >> 1) & 1;
        mSingleAudioFrameSizeInSamples = mMasher->field_2C_audio_header.field_C_single_audio_frame_size;
        // const auto fmv_sound_entry_size = mSingleAudioFrameSizeInSamples * (mMasher->field_2C_audio_header.field_10_num_frames_interleave + 6);

        mNoAudioOrAudioError = 0;
        if (mHasAudio && mMasher->field_2C_audio_header.field_0_audio_format)
        {
            // if (GetSoundAPI().SND_New(
            //     &fmv_sound_entry_5CA208,
            //     fmv_sound_entry_size,
            //     pMasher_audio_header_5CA1E0->field_4_samples_per_second,
            //     (pMasher_audio_header_5CA1E0->field_0_audio_format & 2) != 0 ? 16 : 8,
            //     (pMasher_audio_header_5CA1E0->field_0_audio_format & 1) | 6)
            //     < 0)
            //{
            //     // SND_New failed
            //     fmv_sound_entry_5CA208.field_4_pDSoundBuffer = nullptr;
            //     mNoAudioOrAudioError = 1;
            // }
        }
        else
        {
            // Source DDV has no audio
            mNoAudioOrAudioError = 1;
        }

        if (Mash_DecompressAudio() && mMasher->ReadNextFrame() && mMasher->ReadNextFrame())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool StepDDVPlayback(bool ffmpegExport)
    {
        //mMasher->VideoFrameDecode_Raw(gFrameBuffer.data());
        mMasher->VideoFrameDecode(nullptr);

        if (ffmpegExport)
        {
            //ffmpeg_push_frame(gFrameBuffer.data(), mMasher->field_14_video_header.field_4_width, mMasher->field_14_video_header.field_8_height);
        }
        else
        {
            //glBindTexture(GL_TEXTURE_2D, gMasherTexture);
            //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mMasher->field_14_video_header.field_4_width, mMasher->field_14_video_header.field_8_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, gFrameBuffer.data());
        }

        mNumReadFrames++;

        if (!mNoAudioOrAudioError)
        {
            //const int bitsPerSample = (mMasher->field_2C_audio_header.field_0_audio_format & 2) ? 16 : 8;
            //const int channels = (mMasher->field_2C_audio_header.field_0_audio_format & 1) ? 2 : 1;

            void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(mMasher);

            if (pDecompressedAudioFrame)
            {
                if (ffmpegExport)
                {
                    //ffmpeg_push_audio(reinterpret_cast<u8*>(pDecompressedAudioFrame), (bitsPerSample / 8) * mSingleAudioFrameSizeInSamples * channels);
                }

                //// Push new samples into the buffer
                // if (GetSoundAPI().SND_LoadSamples(&fmv_sound_entry_5CA208, fmv_audio_sample_offset_5CA238, (u8*)pDecompressedAudioFrame, fmv_single_audio_frame_size_in_samples_5CA240) < 0)
                //{
                //     // Reload with data fail
                //     bNoAudioOrAudioError_5CA1F4 = 1;
                // }
            }
            else
            {
                // if (GetSoundAPI().SND_Clear(&fmv_sound_entry_5CA208, fmv_audio_sample_offset_5CA238, fmv_single_audio_frame_size_in_samples_5CA240) < 0)
                //{
                //     // Reload with silence on failure or no data
                //     bNoAudioOrAudioError_5CA1F4 = 1;
                // }
            }

            mAudioSampleOffset += mSingleAudioFrameSizeInSamples;
        }
        const s32 bMoreFrames = mMasher->ReadNextFrame();

        if (!bMoreFrames)
        {
            if (ffmpegExport)
            {
                //ffmpeg_end();
            }

            return false;
        }

        return true;
    }

    void StartPlayback(const std::string& filePath, bool ffmpegExport)
    {
        mFrameTime = 0;
        mFMVHasFrames = InitDDVPlayback(filePath, ffmpegExport);
    }


    void ExportDDVThreadFunc(std::string name)
    {
        mFMVExportFilePath = name;

        int currentFrame = 0;
        StartPlayback(mFMVExportFilePath, true);
        while (StepDDVPlayback(true))
        {
            currentFrame++;
            mFMVExportProgress = (float) currentFrame / mMasher->field_4_ddv_header.field_C_number_of_frames;
            mFMVExportMessage = mFMVExportFilePath + "\nExporting frame " + std::to_string(currentFrame) + " of " + std::to_string(mMasher->field_4_ddv_header.field_C_number_of_frames);
            LOG_INFO(mFMVExportMessage.c_str());
        }

        mFMVExportProgress = 1;
        mFMVExportMessage = "Done!";
        mFMVExporting = false;
    }

private:
    bool mHasAudio = false;
    u32 mAudioSampleOffset = 0;
    s32 mNumReadFrames = 0;
    bool mNoAudioOrAudioError = false;
    u32 mSingleAudioFrameSizeInSamples = 0;
    u32 mCurrentAudioOffset = 0;
    u32 mNumPlayedAudioFrames = 0;
    u32 mOldBufferPlayPos = 0;
    Masher* mMasher = nullptr;

    std::string mFMVExportFilePath;
 
    u32 mFMVHasFrames = 0;
    float mFMVExportProgress = 0.0f;
    int mFrameTime = 0;
    std::string mFMVExportMessage;
    bool mFMVExporting = false;
};

class FmvConv final
{
public:
    explicit FmvConv(IDDVReader& ddvReader)
        : mDDVReader(ddvReader)
    {

    }

    static void FillSyntheticFrame(std::vector<RGBA32>& frameBuffer, u32 width, u32 height, u32 frameIndex)
    {
        const u32 stride = width;
        for (u32 y = 0; y < height; ++y)
        {
            for (u32 x = 0; x < width; ++x)
            {
                const u32 idx = (y * stride) + x;
                const bool checker = (((x / 16) + (y / 16) + frameIndex) % 2) == 0;
                const u8 r = static_cast<u8>((x * 255u) / std::max(1u, width - 1u));
                const u8 g = static_cast<u8>((y * 255u) / std::max(1u, height - 1u));
                const u8 b = static_cast<u8>(((x + y + frameIndex * 3u) * 255u) / std::max(1u, width + height + 1u));
                const u8 red = checker ? r : static_cast<u8>(255 - r);
                const u8 green = checker ? g : static_cast<u8>(255 - g);
                const u8 blue = checker ? b : static_cast<u8>(255 - b);

                frameBuffer[idx] = RGBA32{red, green, blue, 255};
            }
        }
    }

    static void SaveDebugFrame(const std::vector<RGBA32>& frameBuffer, u32 width, u32 height, u32 frameIndex)
    {
        std::filesystem::create_directories("fmv_debug");

        std::vector<u32> pixelData;
        pixelData.reserve(frameBuffer.size());
        for (const RGBA32& pixel : frameBuffer)
        {
            pixelData.push_back(pixel.ToU32());
        }

        PNGFile png;
        const auto pngData = png.Encode(pixelData.data(), width, height);
        const std::string path = "fmv_debug/masher_frame_" + std::to_string(frameIndex) + ".png";

        std::ofstream out(path, std::ios::binary);
        if (!out)
        {
            LOG_ERROR("Failed to open debug PNG '%s' for writing", path.c_str());
            return;
        }

        out.write(reinterpret_cast<const char*>(pngData.data()), static_cast<std::streamsize>(pngData.size()));
        LOG_INFO("Saved debug frame %u to '%s'", frameIndex, path.c_str());
    }

    void Convert(std::string fName)
    {
        TRACE_ENTRYEXIT;

        if (!mDDVReader.Open(fName.c_str()))
        {
            ALIVE_FATAL("Failed to open DDV '%s'", fName.c_str());
        }

        const u32 width = mDDVReader.FrameWidth() > 0 ? mDDVReader.FrameWidth() : 640u;
        const u32 height = mDDVReader.FrameHeight() > 0 ? mDDVReader.FrameHeight() : 240u;

        LOG_INFO("DDV dimensions: %ux%u (header reported %ux%u)", width, height,
                 mDDVReader.FrameWidth(), mDDVReader.FrameHeight());

        const u32 frameRate = mDDVReader.FrameRate();
        std::vector<RGBA32> frameBuffer(width * height);

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

        const std::string outFileName = fName + ".webm";
        FILE* outFile = fopen(outFileName.c_str(), "wb");
        if (!outFile)
        {
            ALIVE_FATAL("Failed to open output file '%s'", outFileName.c_str());
        }

        {
            mkvmuxer::MkvWriter writer(outFile);
            mkvmuxer::Segment segment;
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
                        const RGBA32& p = frameBuffer[(y * width) + x];
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
                                const RGBA32& p = frameBuffer[((sampleY + dy) * width) + (sampleX + dx)];
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

            const u32 totalFrames = mDDVReader.TotalVideoFrames();
            const bool useSyntheticPattern = false;
            const bool saveMasherFrameDebug = true;
            const auto shouldSaveDebugFrame = [&](u32 idx)
            {
                if (idx == 0 || idx == 1 || idx == 8 || idx == 32 || idx == 64)
                {
                    return true;
                }

                if (totalFrames > 0)
                {
                    const u32 quarter = std::max(1u, totalFrames / 4u);
                    const u32 half = std::max(1u, totalFrames / 2u);
                    const u32 last = totalFrames - 1u;
                    return idx == quarter || idx == half || idx == last;
                }

                return false;
            };

            u32 frame_index = 0;
            while (frame_index < (totalFrames > 0 ? totalFrames : 64u))
            {
                if (useSyntheticPattern)
                {
                    FillSyntheticFrame(frameBuffer, width, height, frame_index);
                }
                else if (!mDDVReader.ReadVideoFrame(frameBuffer.data()))
                {
                    break;
                }

                if (saveMasherFrameDebug && shouldSaveDebugFrame(frame_index))
                {
                    SaveDebugFrame(frameBuffer, width, height, frame_index);
                }

                convert_rgba_to_i420();
                const int flags = (frame_index == 0) ? AOM_EFLAG_FORCE_KF : 0;
                encode_frame(&segment, &cfg, &codec, &rawImageFrameData, static_cast<int>(frame_index), flags);

                if (totalFrames > 0)
                {
                    const double percentDone = (static_cast<double>(frame_index + 1u) / static_cast<double>(totalFrames)) * 100.0;
                    LOG_INFO("Video frame %u/%u (%.2f%%)", frame_index + 1u, totalFrames, percentDone);
                }
                else
                {
                    LOG_INFO("Video frame %u (total unknown)", frame_index + 1u);
                }

                ++frame_index;
            }

            while (encode_frame(&segment, &cfg, &codec, nullptr, -1, 0))
            {
                continue;
            }

            const bool ok = segment.Finalize();
            if (!ok)
            {
                fprintf(stderr, "webmenc> Segment::Finalize failed.\n");
            }

            fclose(outFile);
        }

        aom_img_free(&rawImageFrameData);
    }

private:

    int mkv_write_block(mkvmuxer::Segment* segment, const aom_codec_enc_cfg_t* cfg, const aom_codec_cx_pkt_t* pkt)
    {
        int64_t pts_ns = pkt->data.frame.pts * 1000000000ll * cfg->g_timebase.num / cfg->g_timebase.den;
        if (pts_ns <= mLast_pts_ns)
        {
            pts_ns = mLast_pts_ns + 1000000;
        }

        mLast_pts_ns = pts_ns;

        if (!segment->AddFrame(static_cast<uint8_t*>(pkt->data.frame.buf),
                               pkt->data.frame.sz, kVideoTrackNumber, pts_ns,
                               pkt->data.frame.flags & AOM_FRAME_IS_KEY))
        {
            fprintf(stderr, "webmenc> AddFrame failed.\n");
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
            fprintf(stderr, "webmenc> mkvmuxer Init failed.\n");
            return -1;
        }

        segment->set_mode(mkvmuxer::Segment::kFile);
        segment->OutputCues(true);

        mkvmuxer::SegmentInfo* const info = segment->GetSegmentInfo();
        if (!info)
        {
            fprintf(stderr, "webmenc> Cannot retrieve Segment Info.\n");
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
            fprintf(stderr, "webmenc> Video track creation failed.\n");
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
            fprintf(stderr, "webmenc> Unable to set AV1 config.\n");
            return -1;
        }

        ok = video_track->SetStereoMode(1); // STEREO_FORMAT_LEFT_RIGHT
        if (!ok)
        {
            fprintf(stderr, "webmenc> Unable to set stereo mode.\n");
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
                fprintf(stderr, "webmenc> Unable to allocate memory for encoder settings tag.\n");
                return -1;
            }
            ok = tag->add_simple_tag("ENCODER_SETTINGS", encoder_settings);
            if (!ok)
            {
                fprintf(stderr, "webmenc> Unable to allocate memory for encoder settings tag.\n");
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

    bool encode_frame(mkvmuxer::Segment* segment, const aom_codec_enc_cfg_t* cfg, aom_codec_ctx_t* codec, aom_image_t* img, int frame_index, int flags)
    {
        bool got_pkts = false;
        aom_codec_iter_t iter = nullptr;
        const aom_codec_cx_pkt_t* pkt = nullptr;
        const auto start = std::chrono::steady_clock::now();
        const aom_codec_err_t res = aom_codec_encode(codec, img, frame_index, 1, flags);
        const auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

        if (res != AOM_CODEC_OK)
        {
            ALIVE_FATAL("Failed to encode frame");
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

                if (mkv_write_block(segment, cfg, pkt) != 0)
                {
                    ALIVE_FATAL("Failed to write compressed frame");
                }
                LOG_INFO(keyframe ? "K" : ".");
            }
        }

        return got_pkts;
    }

private:
    const int kVideoTrackNumber = 1;
    int64_t mLast_pts_ns = 0;
    IDDVReader& mDDVReader;
};

void ConvertFMVs(const FileSystem::Path& dataDir, bool isAo)
{
    if (isAo)
    {
        return;
    }

    const std::vector<std::string> movieNames =
    {
        "prophecy.ddv",
        "INTRO.DDV",
        "DDLOGO.DDV",
        "GTILOGO.DDV"
    };

    for (const auto& movieName : movieNames)
    {
        FileSystem::Path moviePath = dataDir;
        moviePath.Append(movieName);

        IDDVReader reader;


        FmvConv fmvConv(reader);
        const std::string outPath = moviePath.GetPath() + ".webm";
        fmvConv.Convert(movieName);
    }
}
