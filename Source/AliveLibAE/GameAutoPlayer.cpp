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

void Recorder::SaveRecord(Pads& data)
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

Pads Player::ReadCurrent()
{
    return mLastData;
}

bool Player::ToNextRecord()
{
    if (::fread(&mLastData, sizeof(Pads), 1, mFile.GetFile()) == sizeof(Pads))
    {
        return true;
    }
    return false;
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

void GameAutoPlayer::LoopStart()
{
    mStartedGameLoop = true;
    ReadCurrentRecord();
}

bool GameAutoPlayer::LoopEnd()
{
    mStartedGameLoop = false;

    if (mJustExitedPauseLoop)
    {
        // When entering the pause loop the next record is started at that point
        // so don't end it twice or we'll get 1 record too many.
        mJustExitedPauseLoop = false;
        return false;
    }

    return SaveNextRecord();
}

void GameAutoPlayer::PauseLoopStart()
{
    if (mStartedGameLoop)
    {
        LoopEnd();
        mStartedGameLoop = false;
    }
    ReadCurrentRecord();
}

void GameAutoPlayer::PauseMenuLoopEnd()
{
    SaveNextRecord();
    mJustExitedPauseLoop = true;
}

void GameAutoPlayer::ReadCurrentRecord()
{
    if (mMode == Mode::Play)
    {
        mCurFrameData = mPlayer.ReadCurrent();
    }
}

bool GameAutoPlayer::SaveNextRecord()
{
    if (mMode == Mode::Play)
    {
        return mPlayer.ToNextRecord();
    }
    else if (mMode == Mode::Record)
    {
        mRecorder.SaveRecord(mCurFrameData);
    }
    return false;
}

u32 GameAutoPlayer::GetInput(u32 padIdx)
{
    if (mMode == Mode::Record)
    {
        mCurFrameData.mPads[padIdx] = Input_Read_Pad_4FA9C0(padIdx);
    }
    return mCurFrameData.mPads[padIdx];
}

GameAutoPlayer gGameAutoPlayer;
