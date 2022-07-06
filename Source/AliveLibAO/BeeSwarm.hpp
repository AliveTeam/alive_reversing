#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/Animation.hpp"
#include "Map.hpp"
#include "../relive_lib/AnimationUnknown.hpp"

class PathLine;

namespace AO {

struct BeeSwarmParticle final
{
    FP field_0_xpos;
    FP field_4_ypos;
    s8 field_8_angle;
    s8 field_9_angle_speed;
    s32 field_C_timer;
    AnimationUnknown field_10_anim;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmParticle, 0x80);


struct BeeSwarmParticles final
{
    BeeSwarmParticle bees[25];
};

class BeeSwarm final : public BaseAnimatedWithPhysicsGameObject
{
public:
    BeeSwarm(FP xpos, FP ypos, FP speed, s32 numBees, s32 chaseTicks);
    ~BeeSwarm();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void FollowLine(PathLine* pLine, FP target_x, FP target_y, FP speed);
    void Chase(BaseAliveGameObject* pChaseTarget);
    void ToFlyAwayAndDie();

    BeeSwarmParticles field_E4_bees = {};
    s16 field_D64_num_bees = 0;
    s16 field_D66_bee_count = 0;
    FP field_D68_xpos = {};
    FP field_D6C_ypos = {};
    FP field_D70_chase_target_x = {};
    FP field_D74_chase_target_y = {};
    FP field_D78_speed = {};
    FP field_D7C_pos_offset = {};

    enum class BeeSwarmStates : s16
    {
        eIdle_0 = 0,
        eAttackChase_1 = 1,
        eFollowPathLines_2 = 2,
        eFlyAwayAndDie_3 = 3,
    };
    BeeSwarmStates field_D80_state = BeeSwarmStates::eIdle_0;

    s32 field_D84_chaseTicks = 0;

    // TODO: Might be a rect
    FP field_D88_rect_x = {};
    FP field_D8C_rect_y = {};
    FP field_D90_rect_w = {};
    FP field_D94_rect_h = {};

    BaseAliveGameObject* field_D98_pChaseTarget = nullptr;
    s32 field_D9C_alive_timer = 0;
    s32 field_DA0_do_damage_or_pain_sound_timer = 0;
    s32 field_DA4_update_chase_timer = 0;
    PathLine* field_DA8_pLine = nullptr;
    FP field_DAC_line_follow_speed = {};
};
ALIVE_ASSERT_SIZEOF(BeeSwarm, 0xDB0);

ALIVE_VAR_EXTERN(s16, gBeeInstanceCount_5076B0);
ALIVE_VAR_EXTERN(s16, gBeesNearAbe_5076AC);

} // namespace AO
