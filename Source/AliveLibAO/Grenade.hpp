#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Throwable.hpp"

namespace AO {

class Grenade final : public BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s16 numGrenades);
    ~Grenade();

    virtual s16 VCanThrow() override;
    virtual s16 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void AddToPlatform();
    s16 BlowUpAfterCountdown();
    s16 OnCollision_BounceOff(BaseGameObject* pHit);
    s16 InTheAir();
    
public:
    enum class States : s16
    {
        eFallingToBeCollected_0 = 0,
        eWaitToBeCollected_1 = 1,
        eDoesNothing_2 = 2,
        eCountingDown_3 = 3,
        eFalling_4 = 4,
        eHitGround_5 = 5,
        eWaitForExplodeEnd_6 = 6,
        eExploded_7 = 7
    };
    States field_110_state = States::eFallingToBeCollected_0;
    s16 field_112_explode_timer = 0;
    PathLine* field_114_pCollisionLine = nullptr;
    s16 field_118 = 0;
    BaseGameObject* field_11C = nullptr;
    FP field_120_xpos = {};
    FP field_124_ypos = {};
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x128);

extern s16 gInfiniteGrenades_5076EC;

} // namespace AO
