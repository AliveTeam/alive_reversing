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
static s32 sFmvAudioSampleOffset = 0;
static bool sNoAudioOrAudioError = false;

namespace
{
    struct MkvVideoFrame final
    {
        uint64_t mPtsNs = 0;
        std::vector<u8> mPixels;
    };

    struct MkvAudioChunk final
    {
        uint64_t mPtsNs = 0;
        std::vector<u8> mBuffer;
    };

    static constexpr size_t kMaxBufferedVideoFrames = 30;
    static constexpr size_t kMaxBufferedAudioBytes = 2u * 1024u * 1024u;

    template<class T, size_t Capacity>
    class AVQueue final
    {
    public:
        bool TryPush(T value)
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

        std::deque<MkvVideoFrame>& VideoFrames()
        {
            return mVideoFrames;
        }

        const std::deque<MkvVideoFrame>& VideoFrames() const
        {
            return mVideoFrames;
        }

        std::deque<MkvAudioChunk>& AudioChunks()
        {
            return mAudioChunks;
        }

        const std::deque<MkvAudioChunk>& AudioChunks() const
        {
            return mAudioChunks;
        }

        bool ParseMore(size_t maxVideoFrames = kMaxBufferedVideoFrames, size_t maxAudioBytes = kMaxBufferedAudioBytes)
        {
            if (!mSegment || mParsingComplete)
            {
                return !mVideoFrames.empty();
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
                    LOG_INFO("FMV parser: reached EOS cluster, queue=%zu, parsingComplete", mVideoFrames.size());
                    mCurrentCluster = nullptr;
                    mCurrentBlockEntry = nullptr;
                    mParsingComplete = true;
                    return false;
                }

                LOG_INFO("FMV parser: cluster advance to %p", pNextCluster);
                mCurrentCluster = pNextCluster;
                mCurrentBlockEntry = nullptr;
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
                    else
                    {
                        const mkvparser::BlockEntry* pNextBlockEntry = nullptr;
                        const long status = mCurrentCluster->GetNext(mCurrentBlockEntry, pNextBlockEntry);
                        if (status != 0 || pNextBlockEntry == nullptr || pNextBlockEntry->EOS())
                        {
                            break;
                        }
                        mCurrentBlockEntry = pNextBlockEntry;
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

                    const u64 ptsNs = static_cast<u64>(pBlock->GetTime(mCurrentCluster));
                    for (int frameIdx = 0; frameIdx < pBlock->GetFrameCount(); ++frameIdx)
                    {
                        const mkvparser::Block::Frame& frame = pBlock->GetFrame(frameIdx);
                        std::vector<u8> payload(static_cast<size_t>(frame.len));
                        if (frame.Read(mReader, payload.data()) != 0)
                        {
                            Cleanup();
                            return false;
                        }

                        if (pBlock->GetTrackNumber() == mVideoTrackNumber)
                        {
                            if (mVideoFrames.size() >= maxVideoFrames)
                            {
                                // Keep the parse cursor at the current block so playback resumes from the exact
                                // same position after the consumer drains the queue instead of replaying stale data.
                                return !mVideoFrames.empty();
                            }

                            MkvVideoFrame videoFrame;
                            videoFrame.mPtsNs = ptsNs;
                            if (!DecodeAv1Frame(payload, videoFrame.mPixels))
                            {
                                Cleanup();
                                return false;
                            }
                            mVideoFrames.push_back(videoFrame);
                        }
                        else if (mAudioTrack != nullptr && pBlock->GetTrackNumber() == mAudioTrackNumber)
                        {
                            if (mAudioBytesBuffered + payload.size() > maxAudioBytes)
                            {
                                // Preserve the cursor at the current block until audio space is freed by the consumer.
                                return !mVideoFrames.empty();
                            }

                            MkvAudioChunk audioChunk;
                            audioChunk.mPtsNs = ptsNs;
                            audioChunk.mBuffer = payload;
                            mAudioBytesBuffered += audioChunk.mBuffer.size();
                            mAudioChunks.push_back(audioChunk);
                        }
                    }
                }

                if (!advanceToNextCluster())
                {
                    return !mVideoFrames.empty();
                }
            }

            mParsingComplete = true;
            return !mVideoFrames.empty();
        }

        bool VideoFramesReady() const
        {
            return !mVideoFrames.empty();
        }

        bool AudioChunksReady() const
        {
            return !mAudioChunks.empty();
        }

        bool ParsingComplete() const
        {
            return mParsingComplete;
        }

        void PopVideoFrame()
        {
            if (!mVideoFrames.empty())
            {
                mVideoFrames.pop_front();
            }
        }

        void PopAudioChunk()
        {
            if (!mAudioChunks.empty())
            {
                mAudioBytesBuffered -= mAudioChunks.front().mBuffer.size();
                mAudioChunks.pop_front();
            }
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

            if (!ParseMore(kMaxBufferedVideoFrames, kMaxBufferedAudioBytes))
            {
                return false;
            }

            return !mVideoFrames.empty();
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
            mParsingComplete = false;
            mVideoFrames.clear();
            mAudioChunks.clear();
            mAudioBytesBuffered = 0;
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
        std::atomic_bool mParsingComplete = false;
        size_t mAudioBytesBuffered = 0;
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

    using MkvVideoQueue = AVQueue<MkvVideoFrame, kMaxBufferedVideoFrames + 1u>;
    using MkvAudioQueue = AVQueue<MkvAudioChunk, 128u>;

    class MkvDemuxDecodeThread final
    {
    public:
        MkvDemuxDecodeThread(WebmMoviePlayer& movie, MkvVideoQueue& videoQueue, MkvAudioQueue& audioQueue)
            : mMovie(movie)
            , mVideoQueue(videoQueue)
            , mAudioQueue(audioQueue)
            , mThread(&MkvDemuxDecodeThread::Run, this)
        {
        }

        ~MkvDemuxDecodeThread()
        {
            if (mThread.joinable())
            {
                mThread.join();
            }
        }

        MkvDemuxDecodeThread(const MkvDemuxDecodeThread&) = delete;
        MkvDemuxDecodeThread& operator=(const MkvDemuxDecodeThread&) = delete;

    private:
        void Run()
        {
            for (;;)
            {
                bool movedData = false;

                while (!mMovie.VideoFrames().empty())
                {
                    MkvVideoFrame frame = std::move(mMovie.VideoFrames().front());
                    mMovie.PopVideoFrame();
                    movedData = true;
                    mVideoQueue.TryPush(std::move(frame));
                }

                while (!mMovie.AudioChunks().empty())
                {
                    MkvAudioChunk chunk = std::move(mMovie.AudioChunks().front());
                    mMovie.PopAudioChunk();
                    movedData = true;
                    mAudioQueue.TryPush(std::move(chunk));
                }

                if (!mMovie.ParsingComplete())
                {
                    mMovie.ParseMore(kMaxBufferedVideoFrames, kMaxBufferedAudioBytes);
                    movedData = !mMovie.VideoFrames().empty() || !mMovie.AudioChunks().empty();
                }

                if (mMovie.ParsingComplete()
                    && mMovie.VideoFrames().empty()
                    && mMovie.AudioChunks().empty()
                    && mVideoQueue.Empty()
                    && mAudioQueue.Empty())
                {
                    return;
                }

                if (!movedData)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }

        WebmMoviePlayer& mMovie;
        MkvVideoQueue& mVideoQueue;
        MkvAudioQueue& mAudioQueue;
        std::thread mThread;
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

    auto& videoFrames = movie.VideoFrames();
    auto& audioChunks = movie.AudioChunks();
    const bool hasAudio = !audioChunks.empty();

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
        const u32 blockAlign = (bitDepth / 8u) * channels;
        const u32 audioBytes = static_cast<u32>(std::accumulate(audioChunks.begin(), audioChunks.end(), 0ull,
            [](u64 total, const MkvAudioChunk& chunk)
            {
                return total + chunk.mBuffer.size();
            }));
        const u32 audioSamples = audioBytes / std::max<u32>(1u, blockAlign);
        const s32 soundFlags = channels > 1 ? (bitDepth == 16 ? 6 : 4) : (bitDepth == 16 ? 2 : 0);

        if (GetSoundAPI().mSND_New(&sFmvSoundEntry, static_cast<s32>(std::max<u64>(audioSamples + 4096u, 4096u)), sampleRate, bitDepth, soundFlags) < 0)
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

    if (hasAudio && !sNoAudioOrAudioError)
    {
        const u32 blockAlign = (movie.AudioBitsPerSample() / 8u) * movie.AudioChannels();
        u32 audioSampleOffset = 0;
        for (size_t i = 0; i < audioChunks.size() && i < 6; ++i)
        {
            const auto& chunk = audioChunks[i];
            if (!chunk.mBuffer.empty())
            {
                const u32 chunkSampleCount = static_cast<u32>(chunk.mBuffer.size() / std::max<u32>(1u, blockAlign));
                if (GetSoundAPI().mSND_LoadSamples(&sFmvSoundEntry, audioSampleOffset, const_cast<u8*>(chunk.mBuffer.data()), chunkSampleCount) < 0)
                {
                    sNoAudioOrAudioError = true;
                    break;
                }
                audioSampleOffset += chunkSampleCount;
            }
        }

        if (!sNoAudioOrAudioError)
        {
            sFmvAudioSampleOffset = (audioSampleOffset + 1) & ~1u;
            if (FAILED(SND_PlayEx(&sFmvSoundEntry, 116, 116, 1.0, 0, 1, 100)))
            {
                sNoAudioOrAudioError = true;
            }
        }
    }

    MkvVideoQueue videoQueue;
    MkvAudioQueue audioQueue;
    auto demuxThread = std::make_unique<MkvDemuxDecodeThread>(movie, videoQueue, audioQueue);

    const s32 movieStartTimeStamp = SYS_GetTicks();
    std::deque<MkvAudioChunk> pendingAudioChunks;
    u32 audioWriteOffset = sFmvAudioSampleOffset;
    const u32 blockAlign = (movie.AudioBitsPerSample() / 8u) * movie.AudioChannels();
    const u32 audioBufferSize = static_cast<u32>(std::max<u64>(1ull, std::accumulate(audioChunks.begin(), audioChunks.end(), 0ull,
        [](u64 total, const MkvAudioChunk& chunk)
        {
            return total + chunk.mBuffer.size();
        }) / std::max<u32>(1u, blockAlign)));

    while (!videoQueue.Empty() || !movie.ParsingComplete())
    {
        MkvVideoFrame frame;
        if (!videoQueue.TryPop(frame))
        {
            if (movie.ParsingComplete())
            {
                break;
            }
            SYS_EventsPump();
            PSX_VSync(VSyncMode::UncappedFps);
            continue;
        }

        if (AreMovieSkippingInputsHeld())
        {
            break;
        }

        LOG_INFO("FMV playback: render frame pts=%llu queued=%zu", static_cast<unsigned long long>(frame.mPtsNs), videoQueue.Size());
        std::memcpy(fmvFrame.mData.mPixels->data(), frame.mPixels.data(), frame.mPixels.size());

        if (!sNoAudioOrAudioError && hasAudio)
        {
            MkvAudioChunk audioChunk;
            while (audioQueue.TryPop(audioChunk))
            {
                pendingAudioChunks.push_back(std::move(audioChunk));
            }

            while (!pendingAudioChunks.empty())
            {
                const auto& pendingChunk = pendingAudioChunks.front();
                const u32 bytesToWrite = static_cast<u32>(pendingChunk.mBuffer.size());
                if (bytesToWrite == 0)
                {
                    pendingAudioChunks.pop_front();
                    continue;
                }

                const u32 samplesToWrite = bytesToWrite / std::max<u32>(1u, blockAlign);
                const u64 targetMs = frame.mPtsNs / 1000000ULL;
                const u64 currentMs = static_cast<u64>(SYS_GetTicks() - movieStartTimeStamp);
                if (currentMs < targetMs)
                {
                    break;
                }

                if (GetSoundAPI().mSND_LoadSamples(&sFmvSoundEntry, audioWriteOffset, const_cast<u8*>(pendingChunk.mBuffer.data()), samplesToWrite) < 0)
                {
                    sNoAudioOrAudioError = true;
                    break;
                }

                audioWriteOffset = (audioWriteOffset + samplesToWrite) % std::max<u32>(audioBufferSize, 1u);
                pendingAudioChunks.pop_front();
            }
        }

        Input_IsVKPressed_4EDD40(VK_ESCAPE);
        Input_IsVKPressed_4EDD40(VK_RETURN);

        polyFT4.mCam->mUniqueId = UniqueResId{};
        Render_DDV_Frame(&polyFT4);

        if (!videoQueue.Empty())
        {
            MkvVideoFrame nextFrame;
            if (!videoQueue.TryPeek(nextFrame))
            {
                continue;
            }
            const u64 nextFrameMs = nextFrame.mPtsNs / 1000000ULL;
            const u64 thisFrameMs = frame.mPtsNs / 1000000ULL;
            const s64 waitAhead = static_cast<s64>(nextFrameMs - thisFrameMs);
            const u64 targetTimestamp = movieStartTimeStamp + thisFrameMs;
            while (static_cast<s64>(SYS_GetTicks()) < static_cast<s64>(targetTimestamp + waitAhead))
            {
                SYS_EventsPump();
                PSX_VSync(VSyncMode::UncappedFps);
            }

        }

        SYS_EventsPump();
        PSX_VSync(VSyncMode::UncappedFps);

        if (movie.ParsingComplete() && videoQueue.Empty())
        {
            break;
        }
    }

    demuxThread.reset();

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

s16 sMovie_Kill_SEQs_563A88 = 1;
s32 gMovieRefCount = 0;

void Movie::VScreenChanged()
{
    // Null sub 0x4E02A0
}

void Movie::Init()
{
    SetSurviveDeathReset(true);
    SetUpdateDuringCamSwap(true);

    SetType(ReliveTypes::eMovie);

    ++gMovieRefCount;

    sMovie_Kill_SEQs_563A88 = 1;
}

Movie::Movie(const char_type* pName)
    : BaseGameObject(true, 0)
    , mName(pName)
{
    mName = "PHLEGINF.DDV.webm";
   // Init();
}

void Movie::VUpdate()
{
    SetDead(true);

    /*
    if (GetGameAutoPlayer().IsPlaying() || GetGameAutoPlayer().IsRecording())
    {
        SetDead(true);
    }
    else
    {
        if (sMovie_Kill_SEQs_563A88)
        {
            SND_StopAll();
        }

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
    */
}

void Movie::DeInit()
{
    PSX_VSync(VSyncMode::LimitTo30Fps);

    --gMovieRefCount;

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
