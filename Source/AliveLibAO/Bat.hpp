#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

class PathLine;

namespace AO {

struct Path_Bat final : public Path_TLV
{
    s16 mTimeBeforeMoving;
    s16 mSpeed;
    Scale_short mScale;
    s16 mAttackDuration;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Bat, 0x20);


class Bat final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Bat(relive::Path_Bat* pTlv, s32 tlvInfo);
    ~Bat();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void FlyTo(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed);

    PathLine* mBatLine = nullptr;
    FP mBatSpeed = {};
    s32 mTimeBeforeMoving = 0;
    s32 mTlvInfo = 0;
    enum class BatStates : s16
    {
        eSetTimer_0 = 0,
        eInit_1 = 1,
        eStartMoving_2 = 2,
        eFlying_3 = 3,
        eAttackTarget_4 = 4,
        eFlyAwayAndDie_5 = 5,
    };
    BatStates mBatState = BatStates::eSetTimer_0;
    s16 mAttackDuration = 0;
    s32 mTimer = 0;
    s32 mAttackDurationTimer = 0;
    FP mBatVelX = {};
    FP mEnemyXPos = {};
    FP mEnemyYPos = {};
    BaseAliveGameObject* mAttackTarget = nullptr;
};
ALIVE_ASSERT_SIZEOF(Bat, 0x110);

} // namespace AO
