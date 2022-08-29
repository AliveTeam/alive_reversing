#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/Animation.hpp"
#include "Sfx.hpp"

enum class UXBState : u16
{
    eDelay = 0,
    eActive = 1,
    eExploding = 2,
    eDeactivated = 3
};

struct Path_UXB final : public Path_TLV
{
    enum class StartState : s16
    {
        eOn = 0,
        eOff = 1,
    };
    s16 mPatternLength;
    s16 mPattern;
    Scale_short mScale;
    StartState mStartState;
    s32 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_UXB, 0x1c);

struct SaveState_UXB final
{
    AETypes mType;
    s16 field_2_padding;
    Guid mTlvInfo;
    u32 mNextStateTimer;
    UXBState mCurrentState;
    UXBState mStartingState;
    u16 mDisabledResources;
    u16 mPatternIndex;
    u16 mRedBlinkCount;
    u16 mIsRed;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(SaveState_UXB, 24);

enum UXB_Flags_1C8
{
    eUnused_Bit0 = 0x1,
    eIsRed_Bit1 = 0x2,
};

class UXB final : public BaseAliveGameObject
{
public:
    UXB(relive::Path_UXB* params, const Guid& tlvId);
    ~UXB();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    static s32 CreateFromSaveState(const u8*);

private:
    void InitBlinkAnim(Animation* pAnimation);
    void PlaySFX(SoundEffect sfxIdx);
    s32 IsColliding();

private:
    UXBState mCurrentState = UXBState::eDelay;
    UXBState mStartingState = UXBState::eDelay;
    u16 mDisabledResources = 0;
    Guid mTlvInfo;
    u32 mNextStateTimer = 0;
    Animation mFlashAnim = {};
    u16 mPatternLength = 0;
    u16 mPatternIndex = 0;
    u16 mPattern = 0;
    u16 mRedBlinkCount = 0;
    u16 mIsRed = 0;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1CC);
