#include "BaseGameAutoPlayer.hpp"
#include "Sys_common.hpp"

constexpr u32 kVersion = 0x1997 + 1;

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

u32 AutoFILE::ReadU32() const
{
    u32 value = 0;
    if (::fread(&value, sizeof(u32), 1, mFile) != 1)
    {
        ALIVE_FATAL("Read U32 failed");
    }
    return value;
}

void BaseRecorder::Init(const char* pFileName)
{
    LOG_INFO("Recording to " << pFileName);
    if (!mFile.Open(pFileName, "wb"))
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

void BasePlayer::Init(const char* pFileName)
{
    LOG_INFO("Playing from " << pFileName);
    if (!mFile.Open(pFileName, "rb"))
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
        mRecorder.Init(buffer);
        mMode = Mode::Record;
    }
    else if (ExtractNamePairArgument(buffer, pCmdLine, "-play="))
    {
        mPlayer.Init(buffer);
        mMode = Mode::Play;
    }
}

u32 BaseGameAutoPlayer::GetInput(u32 padIdx)
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

void BaseGameAutoPlayer::ValidateObjectStates()
{
    if (mMode == Mode::Play)
    {
        mPlayer.ValidateObjectStates();
    }
    else if (mMode == Mode::Record)
    {
        mRecorder.SaveObjectStates();
    }
}

s32 BaseGameAutoPlayer::Rng(s32 rng)
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
    return rng;
}

u32 BaseGameAutoPlayer::SysGetTicks()
{
    return SYS_GetTicks();
}
