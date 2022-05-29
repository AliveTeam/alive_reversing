#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Throwable.hpp"

namespace AO {

class Grenade final : public BaseThrowable
{
public:
    Grenade(FP xpos, FP ypos, s16 numGrenades);
    ~Grenade();

    EXPORT void AddToPlatform_41F7C0();
    EXPORT s16 BlowUpAfterCountdown_41EDD0();
    EXPORT s16 OnCollision_BounceOff_41F650(BaseGameObject* pHit);
    EXPORT s16 InTheAir_41EF10();

    virtual s16 VCanThrow() override;
    virtual s16 VIsFalling() override;
    virtual void VTimeToExplodeRandom() override;
    virtual void VThrow(FP velX, FP velY) override;
    virtual void VOnTrapDoorOpen() override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    

private:
    EXPORT void VScreenChanged_41F720();
    EXPORT void VUpdate_41F240();
    EXPORT void VOnTrapDoorOpen_41F920();
    EXPORT void VThrow_41ED90(FP velX, FP velY);
    EXPORT void VTimeToExplodeRandom_41F9B0();
    EXPORT s16 VIsFalling_453ED0();
    EXPORT s16 VCanThrow_453EC0();

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
    States field_110_state;
    s16 field_112_explode_timer;
    PathLine* field_114_pCollisionLine;
    s16 field_118;
    s16 field_11A_padding;
    BaseGameObject* field_11C;
    FP field_120_xpos;
    FP field_124_ypos;
};
ALIVE_ASSERT_SIZEOF(Grenade, 0x128);

ALIVE_VAR_EXTERN(s16, gInfiniteGrenades_5076EC);

} // namespace AO
