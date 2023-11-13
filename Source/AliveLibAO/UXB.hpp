#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_UXB;
}

namespace AO {

enum class UXBState : u16
{
    eDelay = 0,
    eActive = 1,
    eExploding = 2,
    eDeactivated = 3
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

private:
    void InitBlinkAnim();
    s16 IsColliding();

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

} // namespace AO
