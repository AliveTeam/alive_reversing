#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../relive_lib/AnimationUnknown.hpp"

class PathLine;
class ::BaseAliveGameObject;

namespace AO {

struct BeeSwarmParticle final
{
    FP mXPos;
    FP mYPos;
    s8 field_8_angle;
    s8 field_9_angle_speed;
    s32 mTimer;
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
    BeeSwarm(FP xpos, FP ypos, FP speed, s32 numBees, s32 totalChaseTime);
    ~BeeSwarm();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(OrderingTable& ot) override;

    void FollowLine(PathLine* pLine, FP target_x, FP target_y, FP speed);
    void Chase(::BaseAliveGameObject* pChaseTarget);
    void ToFlyAwayAndDie();

    BeeSwarmParticles field_E4_bees = {};
    s16 mMaxBeesCount = 0;
    s16 mCurrentBeesCount = 0;
    FP field_D68_xpos = {};
    FP field_D6C_ypos = {};
    FP mChaseTargetX = {};
    FP mChaseTargetY = {};
    FP field_D78_speed = {};
    FP field_D7C_pos_offset = {};

    enum class BeeSwarmStates : s16
    {
        eIdle_0 = 0,
        eAttackChase_1 = 1,
        eFollowPathLines_2 = 2,
        eFlyAwayAndDie_3 = 3,
    };
    BeeSwarmStates mSwarmState = BeeSwarmStates::eIdle_0;

    s32 mTotalChaseTime = 0;

    // TODO: Might be a rect
    FP mRectX = {};
    FP mRectY = {};
    FP mRectW = {};
    FP mRectH = {};

    Guid mChaseTarget = {};
    s32 mAliveTimer = 0;
    s32 field_DA0_do_damage_or_pain_sound_timer = 0;
    s32 field_DA4_update_chase_timer = 0;
    PathLine* mLine = nullptr;
    FP mLineFollowSpeed = {};
};

extern s16 gBeeInstanceCount;
extern s16 gBeesNearAbe;

} // namespace AO
