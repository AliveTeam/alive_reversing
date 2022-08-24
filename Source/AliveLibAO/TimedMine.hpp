#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_TimedMine final : public Path_TLV
{
    s16 mSwitchId;
    s16 mState;
    Scale_short mScale;
    s16 mTicksUntilExplosion;
    s16 mDisabledResources;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimedMine, 0x24);

enum TimedMineFlags
{
    eStickToLiftPoint = 0x1
};

class TimedMine final : public BaseAliveGameObject
{
public:
    TimedMine(relive::Path_TimedMine* pTlv, s32 tlvInfo);
    ~TimedMine();
    
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
    s32 mTlvInfo = 0;
    s32 mExplosionTimer = 0;
    Animation mTickAnim = {};
    s32 mOldGnFrame = 0;
    s32 mSingleTickTimer = 0;
    BitField16<TimedMineFlags> mTimedMineFlags = {};
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1BC);

} // namespace AO
