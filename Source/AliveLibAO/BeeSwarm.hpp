#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Animation.hpp"
#include "Map.hpp"

START_NS_AO

struct BeeSwarmParticle
{
    int field_0_xpos;
    int field_4_ypos;
    char field_8;
    char field_9;
    __int16 field_A;
    int field_C_timer;
    AnimationUnknown field_10_anim;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmParticle, 0x80);

class PathLine;

struct BeeSwarmParticles
{
    BeeSwarmParticle bees[25];
};

class BeeSwarm : public BaseAnimatedWithPhysicsGameObject
{
public:

    EXPORT BeeSwarm* ctor_47FC60(FP xpos, FP ypos, FP speed, signed __int16 numBees, int chaseTicks);

    EXPORT BaseGameObject* dtor_47FDF0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BeeSwarm* Vdtor_480E00(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_480D40();

    EXPORT void FollowLine_47FF10(PathLine* pLine, FP target_x, FP target_y, FP speed);

    EXPORT void Chase_47FEB0(BaseAliveGameObject* pChaseTarget);

    int field_D4[4];
    BeeSwarmParticles field_E4_bees;
    __int16 field_D64_num_bees;
    __int16 field_D66_bee_count;
    FP field_D68_xpos;
    FP field_D6C_ypos;
    FP field_D70_chase_target_x;
    FP field_D74_chase_target_y;
    FP field_D78_speed;
    int field_D7C;
    __int16 field_D80_state;
    __int16 field_D82;
    int field_D84_chaseTicks;
    int field_D88;
    int field_D8C;
    FP field_D90_fp;
    FP field_D94_fp;
    BaseAliveGameObject* field_D98_pChaseTarget;
    int field_D9C_chase_timer;
    int field_DA0_timer;
    int field_DA4;
    PathLine* field_DA8_pLine;
    FP field_DAC_line_follow_speed;
};
ALIVE_ASSERT_SIZEOF(BeeSwarm, 0xDB0);

ALIVE_VAR_EXTERN(short, gBeeInstanceCount_5076B0);
ALIVE_VAR_EXTERN(short, word_5076AC);

END_NS_AO

