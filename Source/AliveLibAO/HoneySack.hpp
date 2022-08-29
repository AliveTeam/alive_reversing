#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_HoneySack final : public Path_TLV
{
    s16 mChaseTime;
    Scale_short mScale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_HoneySack, 0x1C);

class BeeSwarm;

class HoneySack final : public BaseAnimatedWithPhysicsGameObject
{
public:
    HoneySack(relive::Path_HoneySack* pTlv, const Guid& tlvId);
    ~HoneySack();

    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    Guid mTlvInfo;
    enum class State : s16
    {
        eDripHoney_0 = 0,
        eSetFallAnimation_1 = 1,
        eFallOnGround_2 = 2,
        eUpdateHoneySackOnGround_3 = 3
    };
    State mState = State::eDripHoney_0;
    s16 mHitGround = 0;
    s32 mTimer = 0;
    BeeSwarm* mBeeSwarm = nullptr;
    FP mDripTargetX = {};
    FP mDripTargetY = {};
    FP mObjectYPos = {};
    u16 mChaseTime = 0;
};
ALIVE_ASSERT_SIZEOF(HoneySack, 0x104);


} // namespace AO
