#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Throwable.hpp"

namespace AO {

struct Path_RockSack final : public Path_TLV
{
    s16 field_18_side;
    s16 field_1A_x_vel;
    s16 field_1C_y_vel;
    s16 field_1E_scale;
    s16 field_20_num_rocks;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RockSack, 0x24);

class RockSack final : public BaseAliveGameObject
{
public:
    EXPORT RockSack* ctor_4573F0(Path_RockSack* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_457580();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4575F0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT RockSack* Vdtor_4578A0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_457890();

    s32 field_10C_tlvInfo;
    s16 field_110;
    s16 field_112_num_rocks;
    s16 field_114_can_play_wobble_sound;
    s16 field_116_force_wobble_sound;
    FP field_118_x_vel;
    FP field_11C_y_vel;
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x120);

class Rock final : public BaseThrowable
{
public:
    EXPORT Rock* ctor_456960(FP xpos, FP ypos, s16 count);

    EXPORT BaseGameObject* dtor_456A90();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Rock* Vdtor_4573D0(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_456EC0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_457310();

    virtual void VThrow(FP velX, FP velY) override;

    EXPORT void VThrow_456B20(FP velX, FP velY);

    virtual s16 VCanThrow() override;

    EXPORT s16 VCanThrow_4573C0();

    EXPORT void InTheAir_456B60();

    EXPORT s16 OnCollision_457240(BaseAnimatedWithPhysicsGameObject* pObj);

    virtual s16 VIsFalling() override;

    virtual void VTimeToExplodeRandom() override;

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
    s32 field_124;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x128);


} // namespace AO
