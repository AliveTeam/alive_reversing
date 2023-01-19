#include "BaseGameAutoPlayer.hpp"
#include "Sys.hpp"
#include "CommandLineParser.hpp"

constexpr u32 kVersion = 0x1997 + 2;

void BaseRecorder::Init(const char* pFileName, bool autoFlushFile)
{
    LOG_INFO("Recording to %s auto flush= %s", pFileName, (autoFlushFile ? "yes" : "no"));
    if (!mFile.Open(pFileName, "wb", autoFlushFile))
    {
        ALIVE_FATAL("Can't open recording file %s for writing", pFileName);
    }

    // Write header
    mFile.Write(kVersion);
}

void BaseRecorder::SaveInput(const Pads& data)
{
    mFile.Write(RecordTypes::InputType);
    mFile.Write(data);
}

void BaseRecorder::SaveRng(s32 rng)
{
    mFile.Write(RecordTypes::Rng);
    mFile.Write(rng);
}

void BaseRecorder::SaveTicks(u32 ticks)
{
    mFile.Write(RecordTypes::SysTicks);
    mFile.Write(ticks);
}

void BaseRecorder::SaveSyncPoint(u32 syncPointId)
{
    mFile.Write(RecordTypes::SyncPoint);
    mFile.Write(syncPointId);
}

void BaseRecorder::SaveEvent(const RecordedEvent& event)
{
    mFile.Write(RecordTypes::EventPoint);
    mFile.Write(event.mType);
    mFile.Write(event.mData);
}

void BaseRecorder::SaveBuffer(const std::vector<u8>& buffer)
{
    mFile.Write(RecordTypes::Buffer);
    const u32 len = static_cast<u32>(buffer.size());
    mFile.Write(len);
    mFile.Write(buffer);
}

void BasePlayer::Init(const char* pFileName)
{
    LOG_INFO("Playing from %s", pFileName);
    if (!mFile.Open(pFileName, "rb", false))
    {
        ALIVE_FATAL("Can't open play back file %s for reading", pFileName);
    }

    // Read + validate header
    u32 readVersion = 0;
    mFile.Read(readVersion);
    if (readVersion != kVersion)
    {
        ALIVE_FATAL("File version %d too old, new or corrupted data (should be %d)", readVersion, kVersion);
    }
}

Pads BasePlayer::ReadInput()
{
    ValidateNextTypeIs(RecordTypes::InputType);

    Pads data = {};
    mFile.Read(data);
    return data;
}

s32 BasePlayer::ReadRng()
{
    ValidateNextTypeIs(RecordTypes::Rng);

    s32 rng = 0;
    mFile.Read(rng);
    return rng;
}

u32 BasePlayer::ReadTicks()
{
    ValidateNextTypeIs(RecordTypes::SysTicks);

    u32 ticks = 0;
    mFile.Read(ticks);
    return ticks;
}

u32 BasePlayer::ReadSyncPoint()
{
    ValidateNextTypeIs(RecordTypes::SyncPoint);

    u32 syncPointId = 0;
    mFile.Read(syncPointId);
    return syncPointId;
}

RecordTypes BasePlayer::PeekNextType()
{
    return static_cast<RecordTypes>(mFile.PeekU32());
}

RecordedEvent BasePlayer::ReadEvent()
{
    ValidateNextTypeIs(RecordTypes::EventPoint);

    RecordedEvent event = {};
    event.mType = mFile.ReadU32();
    event.mData = mFile.ReadU32();
    return event;
}

std::vector<u8> BasePlayer::ReadBuffer()
{
    ValidateNextTypeIs(RecordTypes::Buffer);

    const u32 len = mFile.ReadU32();
    std::vector<u8> tmp;
    tmp.resize(len);
    mFile.Read(tmp);
    return tmp;
}

void BasePlayer::ValidateNextTypeIs(RecordTypes type)
{
    const u32 actualType = mFile.ReadU32();
    if (actualType != type)
    {
        ALIVE_FATAL("Wrong record type. Expected %d but got %d", static_cast<u32>(type), actualType);
    }
}

void BaseGameAutoPlayer::ParseCommandLine(const char* pCmdLine)
{
    char buffer[256] = {};
    CommandLineParser parser(pCmdLine);
    if (parser.ExtractNamePairArgument(buffer, "-record="))
    {
        mRecorder.Init(buffer, parser.SwitchExists("-flush"));
        mMode = Mode::Record;
    }
    else if (parser.ExtractNamePairArgument(buffer, "-play="))
    {
        mPlayer.Init(buffer);
        mMode = Mode::Play;

        mNoFpsLimit = parser.SwitchExists("-fastest");
        mIgnoreDesyncs = parser.SwitchExists("-ignore_desyncs");
    }
}

RecordTypes BaseGameAutoPlayer::PeekNextType()
{
    return mPlayer.PeekNextType();
}

void BaseGameAutoPlayer::RecordEvent(const RecordedEvent& event)
{
    if (!mDisabled && IsRecording())
    {
        mRecorder.SaveEvent(event);
    }
}

RecordedEvent BaseGameAutoPlayer::GetEvent()
{
    RecordedEvent event = {};
    if (!mDisabled && IsPlaying())
    {
        event = mPlayer.ReadEvent();
    }
    return event;
}

u32 BaseGameAutoPlayer::GetInput(u32 padIdx)
{
    if (!mDisabled)
    {
        if (mMode == Mode::Play)
        {
            Pads data = mPlayer.ReadInput();
            return data.mPads[padIdx];
        }
        else
        {
            Pads data = {};
            data.mPads[padIdx] = ReadInput(padIdx);
            if (mMode == Mode::Record)
            {
                mRecorder.SaveInput(data);
            }
            return data.mPads[padIdx];
        }
    }
    return 0;
}

void BaseGameAutoPlayer::ValidateObjectStates()
{
    if (!mDisabled)
    {
        if (mMode == Mode::Play)
        {
            if (!mPlayer.ValidateObjectStates())
            {
                if (!mIgnoreDesyncs)
                {
                    ALIVE_FATAL("Play back de-synced, see console log for details");
                }
                else
                {
                    static bool warned = false;
                    if (!warned)
                    {
                        LOG_ERROR("!!!! Play back has de-synced, attempting to carry on");
                        warned = true;
                    }
                }
            }
        }
        else if (mMode == Mode::Record)
        {
            mRecorder.SaveObjectStates();
        }
    }
}

s32 BaseGameAutoPlayer::Rng(s32 rng)
{
    if (!mDisabled)
    {
        if (IsRecording())
        {
            mRecorder.SaveRng(rng);
            return rng;
        }
        else if (IsPlaying())
        {
            const s32 readRng = mPlayer.ReadRng();
            if (readRng != rng)
            {
                ALIVE_FATAL("Rng de-sync! Expected %d but got %d", rng, readRng);
            }
            return readRng;
        }
    }
    return rng;
}

u32 BaseGameAutoPlayer::SysGetTicks()
{
    if (!mDisabled)
    {
        if (IsRecording())
        {
            const u32 ticks = SYS_GetTicks();
            mRecorder.SaveTicks(ticks);
            return ticks;
        }
        else if (IsPlaying())
        {
            const u32 readTicks = mPlayer.ReadTicks();
            return readTicks;
        }
    }
    return SYS_GetTicks();
}

std::vector<u8> BaseGameAutoPlayer::RestoreFileBuffer(const std::vector<u8>& buffer)
{
    if (!mDisabled)
    {
        if (IsRecording())
        {
            mRecorder.SaveBuffer(buffer);
        }
        else if (IsPlaying())
        {
            return mPlayer.ReadBuffer();
        }
    }
    return buffer;
}

void BaseGameAutoPlayer::SyncPoint(u32 syncPointId)
{
    if (!mDisabled)
    {
        if (IsRecording())
        {
            mRecorder.SaveSyncPoint(syncPointId);
        }
        else if (IsPlaying())
        {
            const u32 readSyncPoint = mPlayer.ReadSyncPoint();
            if (readSyncPoint != syncPointId)
            {
                ALIVE_FATAL("Sync point de-sync! Expected %d but got %d", syncPointId, readSyncPoint);
            }
        }
    }
}

void BaseGameAutoPlayer::DisableRecorder()
{
    if (!mDisabled)
    {
        //LOG_INFO("Auto player state paused");
        mDisabled = true;
    }
}

void BaseGameAutoPlayer::EnableRecorder()
{
    if (mDisabled)
    {
        mDisabled = false;
        //LOG_INFO("Auto player state resumed");
    }
}