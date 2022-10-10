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

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;

    void StickToLiftPoint();
    void InitTickAnimation();
public:
    s16 mSlappedMine = 0;
    u16 mTicksUntilExplosion = 0;
    Guid mTlvInfo;
    s32 mExplosionTimer = 0;
    Animation mTickAnim = {};
    s32 mOldGnFrame = 0;
    s32 mSingleTickTimer = 0;
    BitField16<TimedMineFlags> mTimedMineFlags = {};
};

} // namespace AO
