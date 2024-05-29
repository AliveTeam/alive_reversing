#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/Animation.hpp"

namespace relive
{
    struct Path_TimedMine;
}

class TimedMine final : public BaseAliveGameObject
{
public:
    TimedMine(relive::Path_TimedMine* pTlv, const Guid& tlvId);
    ~TimedMine();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnAbeInteraction() override;

private:
    void StickToLiftPoint();
    void InitTickAnimation();
public:
    bool mSlappedMine = false;
    u16 mTicksUntilExplosion = 0;
    Guid mTlvInfo;
    u32 mExplosionTimer = 0;
    Animation mTickAnim = {};
    u32 mOldGnFrame = 0;
    u32 mSingleTickTimer = 0;
    bool mStickToLiftPoint = false;
};
