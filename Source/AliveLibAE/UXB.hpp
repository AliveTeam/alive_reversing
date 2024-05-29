#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "../relive_lib/Sfx.hpp"
#include "../relive_lib/SaveStateBase.hpp"

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

struct UXBSaveState final : public SaveStateBase
{
    UXBSaveState()
        : SaveStateBase(ReliveTypes::eUXB, sizeof(*this))
    { }
    Guid mTlvInfo;
    u32 mNextStateTimer;
    UXBState mCurrentState;
    UXBState mStartingState;
    u16 mDisabledResources;
    u16 mPatternIndex;
    u16 mRedBlinkCount;
    u16 mIsRed;
};

class UXB final : public BaseAliveGameObject
{
public:
    UXB(relive::Path_UXB* pTlv, const Guid& tlvId);
    ~UXB();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual void VOnAbeInteraction() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;

    static void CreateFromSaveState(SerializedObjectData&);
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

private:
    void InitBlinkAnim(Animation* pAnimation);
    void PlaySFX(relive::SoundEffects sfxIdx);
    bool IsColliding();

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
