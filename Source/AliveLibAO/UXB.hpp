#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

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

    void InitBlinkAnim();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s16 IsColliding();

    UXBState mCurrentState = UXBState::eDelay;
    UXBState mStartingState = UXBState::eDelay;
    Guid mTlvInfo;
    s32 mNextStateTimer = 0;
    Animation mFlashAnim;
    s16 mPatternLength = 0;
    s16 mPatternIndex = 0;
    s16 mPattern = 0;
    s16 mRedBlinkCount = 0;
    u16 mIsRed = 0;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1C0);

} // namespace AO
