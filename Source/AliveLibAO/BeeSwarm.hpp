#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Animation.hpp"
#include "Map.hpp"

namespace AO {

struct BeeSwarmParticle
{
    FP field_0_xpos;
    FP field_4_ypos;
    s8 field_8_angle;
    s8 field_9_angle_speed;
    s16 field_A_pad;
    s32 field_C_timer;
    AnimationUnknown field_10_anim;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmParticle, 0x80);

class PathLine;

struct BeeSwarmParticles
{
    BeeSwarmParticle bees[25];
};

class BeeSwarm final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BeeSwarm* ctor_47FC60(FP xpos, FP ypos, FP speed, s16 numBees, s32 chaseTicks);

    EXPORT BaseGameObject* dtor_47FDF0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BeeSwarm* Vdtor_480E00(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChange_480D40();

    EXPORT void FollowLine_47FF10(PathLine* pLine, FP target_x, FP target_y, FP speed);

    EXPORT void Chase_47FEB0(BaseAliveGameObject* pChaseTarget);

    virtual void VUpdate() override;

    EXPORT void VUpdate_47FF50();

    void ToFlyAwayAndDie();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_480AC0(PrimHeader** ppOt);

    s32 field_D4[4];
    BeeSwarmParticles field_E4_bees;
    s16 field_D64_num_bees;
    s16 field_D66_bee_count;
    FP field_D68_xpos;
    FP field_D6C_ypos;
    FP field_D70_chase_target_x;
    FP field_D74_chase_target_y;
    FP field_D78_speed;
    FP field_D7C_pos_offset;

    enum class BeeSwarmStates : s16
    {
        eIdle_0 = 0,
        eAttackChase_1 = 1,
        eFollowPathLines_2 = 2,
        eFlyAwayAndDie_3 = 3,
    };
    BeeSwarmStates field_D80_state;

    s16 field_D82_pad;

    s32 field_D84_chaseTicks;

    // TODO: Might be a rect
    FP field_D88_rect_x;
    FP field_D8C_rect_y;
    FP field_D90_rect_w;
    FP field_D94_rect_h;

    BaseAliveGameObject* field_D98_pChaseTarget;
    s32 field_D9C_alive_timer;
    s32 field_DA0_do_damage_or_pain_sound_timer;
    s32 field_DA4_update_chase_timer;
    PathLine* field_DA8_pLine;
    FP field_DAC_line_follow_speed;
};
ALIVE_ASSERT_SIZEOF(BeeSwarm, 0xDB0);

ALIVE_VAR_EXTERN(s16, gBeeInstanceCount_5076B0);
ALIVE_VAR_EXTERN(s16, gBeesNearAbe_5076AC);

} // namespace AO
