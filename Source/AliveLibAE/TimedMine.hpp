#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "../relive_lib/Animation.hpp"

struct Path_TimedMine final : public Path_TLV
{
    s16 field_10_switch_id;
    s16 field_12_state;
    Scale_short field_14_scale;
    s16 mTicksUntilExplosion;
    s32 mDisabledResources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TimedMine, 0x1C);

enum TimedMineFlags
{
    eStickToLiftPoint = 0x1,
};

class TimedMine final : public BaseAliveGameObject
{
public:
    TimedMine(Path_TimedMine* pPath, TlvItemInfoUnion tlv);
    ~TimedMine();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;

private:
    void InitTickAnimation();
    void StickToLiftPoint();

public:
    u16 mSlappedMine = 0;
    u16 mTicksUntilExplosion = 0;
    u32 mTlvInfo = 0;
    u32 mExplosionTimer = 0;
    Animation mTickAnim = {};
    u32 mOldGnFrame = 0;
    u32 mSingleTickTimer = 0;
    BitField32<TimedMineFlags> mTimedMineFlags = {};
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1C8);
