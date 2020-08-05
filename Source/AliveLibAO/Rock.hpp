#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Throwable.hpp"

START_NS_AO

struct Path_RockSack : public Path_TLV
{
    __int16 field_18_side;
    __int16 field_1A_x_vel;
    __int16 field_1C_y_vel;
    __int16 field_1E_scale;
    __int16 field_20_num_rocks;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RockSack, 0x24);

class RockSack : public BaseAliveGameObject
{
public:
    EXPORT RockSack* ctor_4573F0(Path_RockSack* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_457580();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4575F0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT RockSack* Vdtor_4578A0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_457890();

    int field_10C_tlvInfo;
    __int16 field_110;
    __int16 field_112_num_rocks;
    __int16 field_114_can_play_wobble_sound;
    __int16 field_116_force_wobble_sound;
    FP field_118_x_vel;
    FP field_11C_y_vel;
};
ALIVE_ASSERT_SIZEOF(RockSack, 0x120);

class Rock : public BaseThrowable
{
public:
    EXPORT Rock* ctor_456960(FP xpos, FP ypos, __int16 count);

    EXPORT BaseGameObject* dtor_456A90();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Rock* Vdtor_4573D0(signed int flags);


    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_457310();

    virtual void VThrow(FP velX, FP velY) override;

    EXPORT void VThrow_456B20(FP velX, FP velY);

    virtual __int16 VCanThrow() override;

    EXPORT __int16 VCanThrow_4573C0();

    __int16 field_110_state;
    __int16 field_112;
    PathLine* field_114_can_play_wobble_sound;
    __int16 field_118;
    __int16 field_11A;
    FP field_11C_xpos;
    FP field_120_ypos;
    int field_124;
};
ALIVE_ASSERT_SIZEOF(Rock, 0x128);


END_NS_AO
