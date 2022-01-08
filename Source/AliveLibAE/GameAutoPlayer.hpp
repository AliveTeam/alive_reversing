#pragma once

#include <vector>

class [[nodiscard]] AutoFILE final
{
public:
    AutoFILE(const AutoFILE&) = delete;
    AutoFILE& operator=(const AutoFILE&) const = delete;
    AutoFILE() = default;

    bool Open(const char* pFileName, const char* pMode)
    {
        Close();
        mFile = ::fopen(pFileName, pMode);
        return mFile != nullptr;
    }

    ~AutoFILE()
    {
        Close();
    }

    FILE* GetFile()
    {
        return mFile;
    }

    long FileSize()
    {
        const long oldPos = ftell(mFile);
        fseek(mFile, 0, SEEK_END);
        const long fileSize = ftell(mFile);
        fseek(mFile, oldPos, SEEK_SET);
        return fileSize;
    }

    void Close()
    {
        if (mFile)
        {
            ::fclose(mFile);
        }
    }

private:
    FILE* mFile = nullptr;
};

struct Pads final
{
    u32 mPads[2];
};

class Recorder final
{
public:
    void Init(const char* pFileName);
    void SaveRecord(Pads& data);
private:
    AutoFILE mFile;
};

class Player final
{
public:
    void Init(const char* pFileName);

    Pads ReadCurrent();
    bool ToNextRecord();

private:
    AutoFILE mFile;
    Pads mLastData = {};
};

class [[nodiscard]] GameAutoPlayer final
{
public:
    void ParseCommandLine(const char* pCmdLine);

    void LoopStart();
    bool LoopEnd();

    void PauseLoopStart();
    void PauseMenuLoopEnd();

    u32 GetInput(u32 padIdx);

    bool IsRecording() const
    {
        return mMode == Mode::Record;
    }

    bool IsPlaying() const
    {
        return mMode == Mode::Play;
    }

private:
    void ReadCurrentRecord();
    bool SaveNextRecord();

    enum class Mode
    {
        None,
        Play,
        Record,
        Done
    };
    Mode mMode = Mode::None;

    Pads mCurFrameData = {};

    bool mStartedGameLoop = false;
    bool mJustExitedPauseLoop = false;

    Recorder mRecorder;
    Player mPlayer;
};

extern GameAutoPlayer gGameAutoPlayer;
