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
private:
    void ReadCheck(u32 readSize) const 
    {
        if (mBufferReadPos + readSize > mBuffer.size())
        {
            ALIVE_FATAL("Attempted to read %d bytes from offset %d but total length is %d", readSize, mBufferReadPos, mBuffer.size());
        }
    }

public:
    SerializedObjectData()
    {
        // OG allowed for 6820 bytes of data, we allow for as
        // big as whatever we can allocate
        mBuffer.reserve(8192);
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
        ReadCheck(sizeof(T));
        const T* v = reinterpret_cast<const T*>(mBuffer.data() + mBufferReadPos);
        mBufferReadPos += sizeof(T);
        return v;
    }

    template <typename T>
    const T* PeekTmpPtr() const
    {
        ReadCheck(sizeof(T));
        const T* v = reinterpret_cast<const T*>(mBuffer.data() + mBufferReadPos);
        return v;
    }

    void WriteU8(u8 v)
    {
        ReadCheck(sizeof(u8));
        const auto writePos = mBuffer.size();
        mBuffer.resize(mBuffer.size() + 1);
        *reinterpret_cast<u8*>(mBuffer.data() + writePos) = v;
    }

    void WriteU32(u32 v)
    {
        const auto writePos = mBuffer.size();
        mBuffer.resize(mBuffer.size() + 4);
        *reinterpret_cast<u32*>(mBuffer.data() + writePos) = v;
    }

    [[nodiscard]] u32 PeekU32() const
    {
        ReadCheck(sizeof(u32));
        const u32 v = *reinterpret_cast<const u32*>(mBuffer.data() + mBufferReadPos);
        return v;
    }

    [[nodiscard]] u32 ReadU32() const
    {
        ReadCheck(sizeof(u32));
        const u32 v = *reinterpret_cast<const u32*>(mBuffer.data() + mBufferReadPos);
        mBufferReadPos += 4;
        return v;
    }

    [[nodiscard]] u8 ReadU8() const
    {
        ReadCheck(sizeof(u8));
        const u8 v = *reinterpret_cast<const u8*>(mBuffer.data() + mBufferReadPos);
        mBufferReadPos += 1;
        return v;
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
