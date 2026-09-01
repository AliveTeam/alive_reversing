#include "data_conversion/guid.hpp"
#include "stdafx.h"
#include "Movie.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Psx.hpp"
#include "stdlib.hpp"
#include "Text.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/Sound/Sound.hpp"
#include "VGA.hpp"
#include "GameAutoPlayer.hpp"
#include "Game.hpp"
#include "../relive_lib/Renderer/IRenderer.hpp"
#include "../relive_lib/data_conversion/rgb_conversion.hpp"

#include <numeric>
#include <deque>
#include <array>
#include <atomic>
#include <chrono>
#include <thread>
#include <utility>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/common/webmdec.h"
#include "aom/third_party/libwebm/mkvparser/mkvparser.h"
#include "aom/third_party/libwebm/mkvparser/mkvreader.h"

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
        std::vector<u8> mBuffer;
    };

    struct MkvEncodedPacket final
    {
        uint64_t mPtsNs = 0;
        long long mFileOffset = 0;
        int mTrackNumber = 0;
        std::vector<u8> mPayload;
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

                LOG_INFO("FMV parser: cluster advance to %p", pNextCluster);
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
                    if (frame.Read(mReader, payload.data()) != 0)
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
                    }
                }
            }

            if (!mVideoTrack)
            {
                return false;
            }

            if (aom_codec_dec_init(&mCodec, aom_codec_av1_dx(), nullptr, 0) != AOM_CODEC_OK)
            {
                return false;
            }
            mCodecReady = true;

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

    private:
        bool TryOpenFile(const std::string& path)
        {
            mMovieFile = fopen(path.c_str(), "rb");
            if (!mMovieFile)
            {
                return false;
            }

            mReader = new mkvparser::MkvReader(mMovieFile);
            if (!mReader)
            {
                fclose(mMovieFile);
                mMovieFile = nullptr;
                return false;
            }

            mkvparser::EBMLHeader header;
            long long pos = 0;
            if (header.Parse(mReader, pos) < 0)
            {
                Cleanup();
                return false;
            }

            if (mkvparser::Segment::CreateInstance(mReader, pos, mSegment) != 0)
            {
                Cleanup();
                return false;
            }

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

            aom_codec_err_t status = aom_codec_decode(&mCodec, compressedFrame.data(), static_cast<unsigned int>(compressedFrame.size()), nullptr);
            if (status != AOM_CODEC_OK)
            {
                return false;
            }

            aom_codec_iter_t iter = nullptr;
            aom_image_t* pImage = nullptr;
            while ((pImage = aom_codec_get_frame(&mCodec, &iter)) != nullptr)
            {
                ConvertI420ToRGBA(pImage, rgbaPixels);
                return true;
            }

            return false;
        }

        void Cleanup()
        {
            if (mCodecReady)
            {
                aom_codec_destroy(&mCodec);
                mCodecReady = false;
            }

            if (mSegment)
            {
                delete mSegment;
                mSegment = nullptr;
            }

            if (mReader)
            {
                delete mReader;
                mReader = nullptr;
            }

            if (mMovieFile)
            {
                fclose(mMovieFile);
                mMovieFile = nullptr;
            }

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

        FILE* mMovieFile = nullptr;
        mkvparser::MkvReader* mReader = nullptr;
        mkvparser::Segment* mSegment = nullptr;
        const mkvparser::VideoTrack* mVideoTrack = nullptr;
        const mkvparser::AudioTrack* mAudioTrack = nullptr;
        std::deque<MkvVideoFrame> mVideoFrames;
        std::deque<MkvAudioChunk> mAudioChunks;
        const mkvparser::Cluster* mCurrentCluster = nullptr;
        const mkvparser::BlockEntry* mCurrentBlockEntry = nullptr;
        int mCurrentFrameIndex = 0;
        std::atomic_bool mParsingComplete = false;
        aom_codec_ctx_t mCodec = {};
        bool mCodecReady = false;
        int mVideoTrackNumber = 0;
        int mAudioTrackNumber = 0;
        u32 mWidth = 640;
        u32 mHeight = 240;
        u32 mAudioSampleRate = 44100;
        u32 mAudioChannels = 2;
        u32 mAudioBitsPerSample = 16;
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
                chunk.mBuffer = std::move(packet.mPayload);
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
    u32 invalidDisplayedFrameCount = 0;
    bool haveLastDisplayedOffset = false;
    long long lastDisplayedOffset = 0;
    u64 lastDisplayedPixelHash = 0;
    u32 repeatedDisplayedPixelCount = 0;
    LOG_INFO("FMV playback %u: started", playbackId);

    const bool hasAudio = movie.HasAudio();

    sNoAudioOrAudioError = false;
    if (hasAudio)
    {
        #if USE_SDL2_SOUND
        wasReverbEnabled = gReverbEnabled;
        gReverbEnabled = false;
        #endif

        const u32 sampleRate = movie.AudioSampleRate();
        const u32 bitDepth = movie.AudioBitsPerSample();
        const u32 channels = movie.AudioChannels();
        const s32 soundFlags = channels > 1 ? (bitDepth == 16 ? 6 : 4) : (bitDepth == 16 ? 2 : 0);
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

    MkvVideoQueue videoQueue;
    MkvAudioQueue audioQueue;
    auto moviePipeline = std::make_unique<MkvMoviePipeline>(movie, videoQueue, audioQueue);

    s32 audioStartTimeStamp = 0;
    std::deque<MkvAudioChunk> pendingAudioChunks;
    const u32 blockAlign = (movie.AudioBitsPerSample() / 8u) * movie.AudioChannels();
    const u32 audioBufferSamples = std::max<u32>(movie.AudioSampleRate() * 4u, 4096u);
    u32 audioWriteOffset = 0;
    u32 audioSamplesSubmitted = 0;
    bool audioStarted = false;

    while (!videoQueue.Empty() || !moviePipeline->VideoComplete())
    {
        MkvAudioChunk audioChunk;
        while (audioQueue.TryPop(audioChunk))
        {
            pendingAudioChunks.push_back(std::move(audioChunk));
        }

        const u32 playedSamples = audioStarted
            ? std::min<u32>(audioSamplesSubmitted, static_cast<u32>((SYS_GetTicks() - audioStartTimeStamp) * movie.AudioSampleRate() / 1000))
            : 0;
        while (!sNoAudioOrAudioError && hasAudio && !pendingAudioChunks.empty()
            && audioSamplesSubmitted - playedSamples < audioBufferSamples - std::min<u32>(audioBufferSamples / 4u, 1024u))
        {
            MkvAudioChunk& pendingChunk = pendingAudioChunks.front();
            const u32 samplesToWrite = static_cast<u32>(pendingChunk.mBuffer.size() / std::max<u32>(1u, blockAlign));
            if (samplesToWrite == 0)
            {
                pendingAudioChunks.pop_front();
                continue;
            }

            if (GetSoundAPI().mSND_LoadSamples(&sFmvSoundEntry, audioWriteOffset, pendingChunk.mBuffer.data(), samplesToWrite) < 0)
            {
                sNoAudioOrAudioError = true;
                break;
            }
            audioWriteOffset = (audioWriteOffset + samplesToWrite) % audioBufferSamples;
            audioSamplesSubmitted += samplesToWrite;
            pendingAudioChunks.pop_front();

            if (!audioStarted && (audioSamplesSubmitted >= movie.AudioSampleRate() / 5u || moviePipeline->AudioComplete()))
            {
                if (FAILED(SND_PlayEx(&sFmvSoundEntry, 116, 116, 1.0, 0, 1, 100)))
                {
                    sNoAudioOrAudioError = true;
                }
                audioStartTimeStamp = SYS_GetTicks();
                audioStarted = !sNoAudioOrAudioError;
            }
        }

        if (hasAudio && !audioStarted && !sNoAudioOrAudioError)
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

        LOG_INFO("FMV playback %u: dequeued offset=%lld pts=%llu clock=%llu", playbackId, frame.mFileOffset,
            static_cast<unsigned long long>(frame.mPtsNs),
            static_cast<unsigned long long>(audioStarted ? SYS_GetTicks() - audioStartTimeStamp : 0));

        if (AreMovieSkippingInputsHeld())
        {
            break;
        }

        const u64 frameMs = frame.mPtsNs / 1000000ULL;
        const u64 audioClockMs = audioStarted ? static_cast<u64>(SYS_GetTicks() - audioStartTimeStamp) : 0;
        if (audioStarted && frameMs + 100 < audioClockMs)
        {
            ++droppedFrameCount;
            continue;
        }

        while (audioStarted && frameMs > static_cast<u64>(SYS_GetTicks() - audioStartTimeStamp))
        {
            if (AreMovieSkippingInputsHeld())
            {
                moviePipeline.reset();
                break;
            }
            SYS_EventsPump();
            PSX_VSync(VSyncMode::UncappedFps);
        }
        if (!moviePipeline)
        {
            break;
        }

        LOG_INFO("FMV playback: render frame pts=%llu queued=%zu", static_cast<unsigned long long>(frame.mPtsNs), videoQueue.Size());
        std::memcpy(fmvFrame.mData.mPixels->data(), frame.mPixels.data(), frame.mPixels.size());

        Input_IsVKPressed_4EDD40(VK_ESCAPE);
        Input_IsVKPressed_4EDD40(VK_RETURN);

        polyFT4.mCam->mUniqueId = UniqueResId{};
        Render_DDV_Frame(&polyFT4);
        ++renderedFrameCount;
        u64 pixelHash = 1469598103934665603ULL;
        for (const u8 pixel : frame.mPixels)
        {
            pixelHash ^= pixel;
            pixelHash *= 1099511628211ULL;
        }
        if (renderedFrameCount > 1 && pixelHash == lastDisplayedPixelHash)
        {
            ++repeatedDisplayedPixelCount;
            LOG_ERROR("FMV playback %u: repeated screen pixels frame=%u offset=%lld hash=%llu",
                playbackId, renderedFrameCount, frame.mFileOffset,
                static_cast<unsigned long long>(pixelHash));
        }
        lastDisplayedPixelHash = pixelHash;
        if (haveLastDisplayedOffset && frame.mFileOffset <= lastDisplayedOffset)
        {
            ++invalidDisplayedFrameCount;
            LOG_ERROR("FMV playback %u: non-increasing screen frame=%u offset=%lld previous=%lld pts=%llu",
                playbackId, renderedFrameCount, frame.mFileOffset, lastDisplayedOffset,
                static_cast<unsigned long long>(frame.mPtsNs));
        }
        haveLastDisplayedOffset = true;
        lastDisplayedOffset = frame.mFileOffset;
        LOG_INFO("FMV playback %u: screen frame=%u offset=%lld pts=%llu clock=%llu hash=%llu queued=%zu", playbackId, renderedFrameCount,
            frame.mFileOffset, static_cast<unsigned long long>(frame.mPtsNs),
            static_cast<unsigned long long>(audioStarted ? SYS_GetTicks() - audioStartTimeStamp : 0),
            static_cast<unsigned long long>(pixelHash), videoQueue.Size());

        SYS_EventsPump();
        PSX_VSync(VSyncMode::UncappedFps);
    }

    moviePipeline.reset();

    LOG_INFO("FMV playback %u: finished rendered=%u dropped=%u invalidDisplayed=%u repeatedPixels=%u", playbackId,
        renderedFrameCount, droppedFrameCount, invalidDisplayedFrameCount, repeatedDisplayedPixelCount);

    if (sFmvSoundEntry.field_4_pDSoundBuffer)
    {
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

Movie::Movie(const char_type* pName)
    : BaseGameObject(true, 0)
    , mName(pName)
{
    mName = "PHLEGINF.DDV.webm";
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

        while (!DDV_Play(mName))
        {
            if (gAttract)
            {
                break;
            }

            if (!Display_Full_Screen_Message_Blocking(MessageType::eSkipMovie_1))
            {
                break;
            }
        }
    }
    DeInit();
    gBreakGameLoop = true;
    LOG_INFO("Movie VUpdate complete break=%d", gBreakGameLoop ? 1 : 0);
}

void Movie::DeInit()
{
    PSX_VSync(VSyncMode::LimitTo30Fps);

    --Movie::gMovieRefCount;

    #if USE_SDL2_SOUND
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
