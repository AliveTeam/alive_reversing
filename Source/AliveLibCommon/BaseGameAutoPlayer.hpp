#pragma once

#include <vector>
#include <type_traits>

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

    template <typename TypeToWrite>
    bool Write(const TypeToWrite& value)
    {
        static_assert(std::is_pod<TypeToWrite>::value, "TypeToWrite must be pod");
        return ::fwrite(&value, sizeof(TypeToWrite), 1, mFile) == 1;
    }

    template <typename TypeToRead>
    bool Read(TypeToRead& value)
    {
        static_assert(std::is_pod<TypeToRead>::value, "TypeToRead must be pod");
        return ::fread(&value, sizeof(TypeToRead), 1, mFile) == 1;
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

class BaseRecorder
{
public:
    BaseRecorder() = default;
    virtual ~BaseRecorder() = default;
    void Init(const char* pFileName);
    void SaveInput(const Pads& data);
    virtual void SaveObjectStates() = 0;

protected:
    AutoFILE mFile;
};

class BasePlayer
{
public:
    BasePlayer() = default;
    virtual ~BasePlayer() = default;
    void Init(const char* pFileName);
    Pads ReadInput();
    virtual void ValidateObjectStates() = 0;
protected:
    template <typename TypeToValidate>
    static void ValidField(AutoFILE& file, const TypeToValidate& expectedValue, const char* name)
    {
        TypeToValidate tmpValue = {};
        file.Read(tmpValue);
        if (tmpValue != expectedValue)
        {
            LOG_ERROR("Field " << name << " de-synced");
            ALIVE_FATAL("Field value de-sync");
        }
    }
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

private:

    enum class Mode
    {
        None,
        Play,
        Record,
        Done
    };
    Mode mMode = Mode::None;

    BaseRecorder& mRecorder;
    BasePlayer& mPlayer;
};
