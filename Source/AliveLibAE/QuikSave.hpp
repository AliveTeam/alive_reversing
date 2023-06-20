#pragma once

#include "../relive_lib/Function.hpp"
#include "MainMenu.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/FatalError.hpp"
#include "Abe.hpp"

class SerializedObjectData;


enum class LevelIds : s16;

struct Quicksave_WorldInfo final
{
    s32 mGnFrame;
    LevelIds mLevel;
    s16 mPath;
    s16 mCam;
    s16 mSaveFileId;
    s16 mControlledCharX;
    s16 mControlledCharY;
    s16 mControlledCharScale;
    s16 mRescuedMudokons;
    s16 mKilledMudokons;
    s8 field_16_muds_in_area;
    s8 field_17_last_saved_killed_muds_per_path;
    s8 field_18_saved_killed_muds_per_zulag[20];
    s8 field_2C_current_zulag_number;
    s8 mTotalMeterBars;
    s16 field_30_bDrawMeterCountDown;
    s16 mVisitedBonewerkz;
    s16 mVisitedBarracks;
    s16 mVisitedFeecoEnder;
    s32 mGasTimer;
    s16 mAbeInvincible;
    s16 field_3E_padding;
};

class SerializedObjectData final
{
public:
    SerializedObjectData()
    {
        // OG allowed for 6820 bytes of data, we allow for as
        // big as whatever we can allocate
        mBuffer.reserve(1024 * 8);
    }

    template<typename T>
    void Write(const T& item)
    {
        static_assert(std::is_trivially_copyable_v<T>, "item must be trivially_copyable");
        const auto posToWrite = mBuffer.size();

        // Allocate space for item
        mBuffer.resize(mBuffer.size() + sizeof(T));

        // Bitwise copy it into the buffer (safe cos its POD)
        ::memcpy(mBuffer.data() + posToWrite, &item, sizeof(T));
    }

    template<typename T>
    const T* ReadTmpPtr() const
    {
        const T* tmp = PeekPtr<T>();
        mBufferReadPos += sizeof(T);
        return tmp;
    }

    template <typename T>
    const T* PeekTmpPtr() const
    {
        return PeekPtr<T>();
    }

    void WriteU8(u8 v)
    {
        WriteBasicType(v);
    }

    void WriteU32(u32 v)
    {
        WriteBasicType(v);
    }

    [[nodiscard]] u32 PeekU32() const
    {
        return *PeekPtr<u32>();
    }

    [[nodiscard]] u32 ReadU32() const
    {
        return ReadBasicType<u32>();
    }

    [[nodiscard]] u8 ReadU8() const
    {
        return ReadBasicType<u8>();
    }

    void ReadRewind() const
    {
        mBufferReadPos = 0;
    }

    bool CanRead() const
    {
        return mBufferReadPos < mBuffer.size();
    }

    void WriteRewind()
    {
        mBufferReadPos = 0;
        mBuffer.clear();
    }

private:
    void ReadCheck(u32 readSize) const
    {
        if (mBufferReadPos + readSize > mBuffer.size())
        {
            ALIVE_FATAL("Attempted to read %d bytes from offset %d but total length is %d", readSize, mBufferReadPos, mBuffer.size());
        }
    }

    template<typename T>
    void WriteBasicType(T value)
    {
        const auto writePos = mBuffer.size();
        mBuffer.resize(mBuffer.size() + sizeof(T));
        *reinterpret_cast<T*>(mBuffer.data() + writePos) = value;
    }

    template <typename T>
    [[nodiscard]] const T* PeekPtr() const
    {
        ReadCheck(sizeof(T));
        return reinterpret_cast<const T*>(mBuffer.data() + mBufferReadPos);
    }

    template <typename T>
    [[nodiscard]] T ReadBasicType() const
    {
        const T* v = PeekPtr<T>();
        mBufferReadPos += sizeof(T);
        return *v;
    }

    mutable u32 mBufferReadPos = 0;
    std::vector<u8> mBuffer;
};

struct Quicksave final
{
    Quicksave_WorldInfo mWorldInfo;
    Quicksave_WorldInfo mRestartPathWorldInfo;
    AbeSaveState mRestartPathAbeState;
    SwitchStates mRestartPathSwitchStates;
    SwitchStates mSwitchStates;
    SerializedObjectData mObjectsStateData;
    SerializedObjectData mObjectBlyData;

};

struct SaveFileRec final
{
    char_type mFileName[32];
    u32 mLastWriteTimeStamp;
};
ALIVE_ASSERT_SIZEOF(SaveFileRec, 0x24);

extern SaveFileRec gSaveFileRecords[128];
extern Quicksave gActiveQuicksaveData;
extern s32 gSavedGameToLoadIdx;
extern s32 gTotalSaveFilesCount;
extern u16 sQuickSave_saved_switchResetters_count_BB234C;

void Quicksave_LoadActive();
void DoQuicksave();
void Quicksave_ReadWorldInfo(const Quicksave_WorldInfo* pInfo);
void Quicksave_SaveWorldInfo(Quicksave_WorldInfo* pInfo);
void Quicksave_FindSaves();
void QuikSave_RestoreBlyData(Quicksave& pSaveData);
void Quicksave_SaveSwitchResetterStates();
void Quicksave_RestoreSwitchResetterStates();
