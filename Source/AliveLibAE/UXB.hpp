#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "Sfx.hpp"

namespace relive
{
    struct Path_UXB;
}

enum class UXBState : u16
{
    eDelay = 0,
    eActive = 1,
    eExploding = 2,
    eDeactivated = 3
};

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
    UXB(relive::Path_UXB* pTlv, const Guid& tlvId);
    ~UXB();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    static s32 CreateFromSaveState(const u8*);
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    void InitBlinkAnim(Animation* pAnimation);
    void PlaySFX(relive::SoundEffects sfxIdx);
    s32 IsColliding();

private:
    UXBState mCurrentState = UXBState::eDelay;
    UXBState mStartingState = UXBState::eDelay;
    Guid mTlvInfo;
    u32 mNextStateTimer = 0;
    Animation mFlashAnim;
    u16 mPatternLength = 0;
    u16 mPatternIndex = 0;
    u16 mPattern = 0;
    u16 mRedBlinkCount = 0;
    u16 mIsRed = 0;
};
