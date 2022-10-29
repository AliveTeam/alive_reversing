#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

namespace AO {

enum TimedMineFlags
{
    eStickToLiftPoint = 0x1
};

class TimedMine final : public BaseAliveGameObject
{
public:
    TimedMine(relive::Path_TimedMine* pTlv, const Guid& tlvId);
    ~TimedMine();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;

private:
    void StickToLiftPoint();
    void InitTickAnimation();
public:
    u16 mSlappedMine = 0;
    u16 mTicksUntilExplosion = 0;
    Guid mTlvInfo;
    u32 mExplosionTimer = 0;
    Animation mTickAnim = {};
    u32 mOldGnFrame = 0;
    u32 mSingleTickTimer = 0;
    BitField32<TimedMineFlags> mTimedMineFlags = {};
};

} // namespace AO
