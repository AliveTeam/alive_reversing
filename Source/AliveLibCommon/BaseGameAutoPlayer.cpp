#include "BaseGameAutoPlayer.hpp"
#include "Sys_common.hpp"

constexpr u32 kVersion = 0x1997 + 2;

static bool ExtractNamePairArgument(char* pOutArgument, const char* pCmdLine, const char* argumentPrefix)
{
    const char* pArg = strstr(pCmdLine, argumentPrefix);
    if (!pArg)
    {
        return false;
    }

    pArg += strlen(argumentPrefix);

    u32 len = 0;
    const char* pArgIter = pArg;
    while (*pArgIter && *pArgIter != ' ')
    {
        len++;
        pArgIter++;
    }

    if (len == 0 || len >= 254)
    {
        return false;
    }

    memcpy(pOutArgument, pArg, len);
    return true;
}

u32 AutoFILE::PeekU32()
{
    const auto oldPos = ::ftell(mFile);

    const u32 data = ReadU32();

    if (::fseek(mFile, oldPos, SEEK_SET) != 0)
    {
        ALIVE_FATAL("Seek back failed");
    }

    return data;
}

u32 AutoFILE::ReadU32() const
{
    u32 value = 0;
    if (::fread(&value, sizeof(u32), 1, mFile) != 1)
    {
        ALIVE_FATAL("Read U32 failed");
    }
    return value;
}

void BaseRecorder::Init(const char* pFileName, bool autoFlushFile)
{
    LOG_INFO("Recording to " << pFileName << " auto flush=" << autoFlushFile ? "yes" : "no");
    if (!mFile.Open(pFileName, "wb", autoFlushFile))
    {
        ALIVE_FATAL("Can't open recording file for writing");
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

void BasePlayer::Init(const char* pFileName)
{
    LOG_INFO("Playing from " << pFileName);
    if (!mFile.Open(pFileName, "rb", false))
    {
        ALIVE_FATAL("Can't open play back file for reading");
    }

    // Read + validate header
    u32 readVersion = 0;
    mFile.Read(readVersion);
    if (readVersion != kVersion)
    {
        ALIVE_FATAL("File version too old, new or corrupted data");
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

void BasePlayer::ValidateNextTypeIs(RecordTypes type)
{
    const u32 actualType = mFile.ReadU32();
    if (actualType != type)
    {
        LOG_ERROR("Expected " << static_cast<u32>(type) << " but got " << actualType);
        ALIVE_FATAL("Wrong record type");
    }
}

void BaseGameAutoPlayer::ParseCommandLine(const char* pCmdLine)
{
    char buffer[256] = {};
    if (ExtractNamePairArgument(buffer, pCmdLine, "-record="))
    {
        const bool flushFile = strstr(pCmdLine, "-flush") != nullptr;
        mRecorder.Init(buffer, flushFile);
        mMode = Mode::Record;
    }
    else if (ExtractNamePairArgument(buffer, pCmdLine, "-play="))
    {
        mPlayer.Init(buffer);
        mMode = Mode::Play;

        if (strstr(pCmdLine, "-fastest"))
        {
            mNoFpsLimit = true;
        }

        if (strstr(pCmdLine, "-ignore_desyncs"))
        {
            mIgnoreDesyncs = true;
        }
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
                LOG_ERROR("Rng de-sync! Expected " << rng << " but got " << readRng);
                ALIVE_FATAL("Rng de-sync");
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
                LOG_ERROR("Sync point de-sync! Expected " << syncPointId << " but got " << readSyncPoint);
                ALIVE_FATAL("Sync point de-sync");
            }
        }
    }
}

void BaseGameAutoPlayer::DisableRecorder()
{
    if (!mDisabled)
    {
        LOG_INFO("Auto player state paused");
        mDisabled = true;
    }
}

void BaseGameAutoPlayer::EnableRecorder()
{
    if (mDisabled)
    {
        mDisabled = false;
        LOG_INFO("Auto player state resumed");
    }
}
