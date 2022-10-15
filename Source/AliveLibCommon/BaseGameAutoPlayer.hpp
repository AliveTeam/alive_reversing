#pragma once

#include <vector>
#include <type_traits>
#include "Sys_common.hpp"

enum RecordTypes : u32
{
    FrameCounter = 0xcafebabe,
    ObjectCounter = 0xdeadbeef,
    ObjectStates = 0x123456,
    AliveObjectStates = 0x77777,
    Rng = 0x696969,
    SysTicks = 0x19981998,
    SyncPoint = 0xf00df00d,
    InputType = 0x101010,
    Event = 0x445511,
    Buffer = 0x99911144,
};

enum SyncPoints : u32
{
    PumpEventsStart = 3,
    MainLoopStart = 4,
    ObjectsUpdateStart = 5,
    ObjectsUpdateEnd = 6,
    AnimateAll = 7,
    DrawAllStart = 8,
    DrawAllEnd = 9,
    RenderStart = 10,
    RenderEnd = 11,
    IncrementFrame = 12,
    MainLoopExit = 13,
    RenderOT = 14,
    PumpEventsEnd = 17,
};

struct RecordedEvent final
{
    u32 mType;
    u32 mData;
};

class [[nodiscard]] AutoFILE final
{
public:
    AutoFILE(const AutoFILE&) = delete;
    AutoFILE& operator=(const AutoFILE&) const = delete;
    AutoFILE() = default;

    bool Open(const char* pFileName, const char* pMode, bool autoFlushFile)
    {
        Close();
        mFile = ::fopen(pFileName, pMode);
        if (strchr(pMode, 'w'))
        {
            mIsWriter = true;
        }
        mAutoFlushFile = autoFlushFile;
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

    template <typename TypeToWrite>
    bool Write(const TypeToWrite& value)
    {
        static_assert(std::is_pod<TypeToWrite>::value, "TypeToWrite must be pod");
        const bool ret = ::fwrite(&value, sizeof(TypeToWrite), 1, mFile) == 1;
        Flush();
        return ret;
    }

    template <typename TypeToWrite>
    bool Write(const std::vector<TypeToWrite>& value)
    {
        static_assert(std::is_pod<TypeToWrite>::value, "TypeToWrite must be pod");
        const bool ret = ::fwrite(value.data(), sizeof(TypeToWrite), value.size(), mFile) == value.size();
        Flush();
        return ret;
    }

    template <typename TypeToRead>
    bool Read(TypeToRead& value)
    {
        static_assert(std::is_pod<TypeToRead>::value, "TypeToRead must be pod");
        return ::fread(&value, sizeof(TypeToRead), 1, mFile) == 1;
    }

    template <typename TypeToRead>
    bool Read(std::vector<TypeToRead>& value)
    {
        static_assert(std::is_pod<TypeToRead>::value, "TypeToRead must be pod");
        return ::fread(value.data(), sizeof(TypeToRead), value.size(), mFile) == value.size();
    }

    u32 PeekU32();

    u32 ReadU32() const;

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
            if (mIsWriter)
            {
                ::fflush(mFile);
            }
            ::fclose(mFile);
        }
    }

private:
    void Flush()
    {
        if (mAutoFlushFile)
        {
            if (::fflush(mFile) != 0)
            {
                ALIVE_FATAL("fflush failed");
            }
        }
    }

    FILE* mFile = nullptr; 
    bool mIsWriter = false;
    bool mAutoFlushFile = false;
};

struct Pads final
{
    u32 mPads[2];
};

class BaseRecorder
{
public:
    BaseRecorder() = default;
    virtual ~BaseRecorder() = default;
    void Init(const char* pFileName, bool autoFlushFile);
    void SaveInput(const Pads& data);
    void SaveRng(s32 rng);

    void SaveTicks(u32 ticks);
    void SaveSyncPoint(u32 syncPointId);
    void SaveEvent(const RecordedEvent& event);

    virtual void SaveObjectStates() = 0;

    void SaveBuffer(const std::vector<u8>& buffer);

protected:
    AutoFILE mFile;
};

class [[nodiscard]] BasePlayer
{
public:
    BasePlayer() = default;
    virtual ~BasePlayer() = default;
    void Init(const char* pFileName);
    Pads ReadInput();
    s32 ReadRng();
    u32 ReadTicks();
    u32 ReadSyncPoint();
    RecordTypes PeekNextType();
    RecordedEvent ReadEvent();
    virtual bool ValidateObjectStates() = 0;

    std::vector<u8> ReadBuffer();

protected:
    template <typename TypeToValidate>
    static void SkipValidField(AutoFILE& file)
    {
        TypeToValidate tmpValue = {};
        file.Read(tmpValue);
    }

    template <typename TypeToValidate>
    static bool ValidField(AutoFILE& file, const TypeToValidate& expectedValue, const char* name)
    {
        TypeToValidate tmpValue = {};
        file.Read(tmpValue);
        if (tmpValue != expectedValue)
        {
            LOG_ERROR("Field " << name << " de-synced");
            return true;
        }
        return false;
    }

    void ValidateNextTypeIs(RecordTypes type);

    AutoFILE mFile;
};

class [[nodiscard]] BaseGameAutoPlayer
{
protected:
    BaseGameAutoPlayer(BaseRecorder& recorder, BasePlayer& player)
        : mRecorder(recorder)
        , mPlayer(player)
    {

    }

    virtual u32 ReadInput(u32 padIdx) = 0;

public:
    void ParseCommandLine(const char* pCmdLine);

    RecordTypes PeekNextType();

    void RecordEvent(const RecordedEvent& event);
    RecordedEvent GetEvent();

    u32 GetInput(u32 padIdx);

    void ValidateObjectStates();

    bool IsRecording() const
    {
        return mMode == Mode::Record;
    }

    bool IsPlaying() const
    {
        return mMode == Mode::Play;
    }

    bool NoFpsLimitPlayBack() const
    {
        return mNoFpsLimit;
    }

    s32 Rng(s32 rng);

    u32 SysGetTicks();

    void SyncPoint(u32 syncPointId);
    void DisableRecorder();
    void EnableRecorder();

    std::vector<u8> RestoreFileBuffer(const std::vector<u8>& buffer);

private:

    enum class Mode
    {
        None,
        Play,
        Record,
        Done
    };
    Mode mMode = Mode::None;
    bool mDisabled = false;

    BaseRecorder& mRecorder;
    BasePlayer& mPlayer;
    bool mNoFpsLimit = false;
    bool mIgnoreDesyncs = false;
};

// Implemented in the top level binaries so AE and AO shared code return the same object rather 
// than the per AE/AO derived type in the AE shared functions.
BaseGameAutoPlayer& GetGameAutoPlayer();
