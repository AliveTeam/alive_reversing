#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Throwable.hpp"

namespace AO {

class Rock final : public BaseThrowable
{
public:
    Rock(FP xpos, FP ypos, s16 count);
    ~Rock();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual s16 VCanThrow() override;
    virtual s16 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;

    void InTheAir();
    s16 OnCollision(BaseAnimatedWithPhysicsGameObject* pObj);
    void BounceHorizontally( FP hitX, FP hitY );

    enum class States : s16
    {
        eNone_0 = 0,
        eFallingOutOfRockSack_1 = 1,
        eRolling_2 = 2,
        eOnGround_3 = 3,
        eBouncing_4 = 4,
        eFallingOutOfWorld_5 = 5,
    };
    States field_110_state;
    s16 field_112;
    PathLine* field_114_pLine;
    s16 field_118_vol;
    s16 field_11A;
    FP field_11C_xpos;
    FP field_120_ypos;
    s32 field_124_shimmer_timer;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x128);


} // namespace AO
