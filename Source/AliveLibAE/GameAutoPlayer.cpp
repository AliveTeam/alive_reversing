#include "GameAutoPlayer.hpp"
#include "Input.hpp"
#include "Sys_common.hpp"

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

void Recorder::Init(const char* pFileName)
{
    LOG_INFO("Recording to " << pFileName);
    if (!mFile.Open(pFileName, "wb"))
    {
        ALIVE_FATAL("Can't open recording file for writing");
    }

    // TODO: Write a header
}

void Recorder::SaveInput(Pads& data)
{
    ::fwrite(&data, sizeof(Pads), 1, mFile.GetFile());
}

void Player::Init(const char* pFileName)
{
    LOG_INFO("Playing from " << pFileName);
    if (!mFile.Open(pFileName, "rb"))
    {
        ALIVE_FATAL("Can't open play back file for reading");
    }

    // TODO: Read + validate header
}

Pads Player::ReadInput()
{
    Pads data = {};
    if (::fread(&data, sizeof(Pads), 1, mFile.GetFile()) == sizeof(Pads))
    {
    }
    return data;
}

void GameAutoPlayer::ParseCommandLine(const char* pCmdLine)
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

u32 GameAutoPlayer::GetInput(u32 padIdx)
{
    if (mMode == Mode::Record)
    {
        mCurFrameData.mPads[padIdx] = Input_Read_Pad_4FA9C0(padIdx);
        mRecorder.SaveInput(mCurFrameData);
        return mCurFrameData.mPads[padIdx];
    }
    else
    {
        mCurFrameData = mPlayer.ReadInput();
        return mCurFrameData.mPads[padIdx];
    }
}

GameAutoPlayer gGameAutoPlayer;
