#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

class Bells;

class ChimeLock final : public BaseAliveGameObject
{
public:
    ChimeLock(relive::Path_ChimeLock* pTlv, const Guid& tlvId);
    ~ChimeLock();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VPossessed() override;
    virtual void VUnPosses() override;

    s16 DoNote(s16 note);
    void SetBallTarget(FP ballTargetX, FP ballTargetY, s16 timer, s16 xSize, s16 ySize, s16 bHitBell);
    s16 UpdateBall();
    void SetTargetBellIfSpace(s16 targetNum);

    Guid mTlvId;
    enum class ChimeLockStates : s16
    {
        eIdle_0 = 0,
        eUnused_1 = 1,
        ePossessed_2 = 2,
        eUnPossessing_3 = 3,
        eUnPossess_4 = 4,
        eUnused_5 = 5,
        eNeverRead_6 = 6
    };
    ChimeLockStates mChimeLockState = ChimeLockStates::eIdle_0;
    Bells* mLeftBell = nullptr;
    Bells* mCenterBell = nullptr;
    Bells* mRightBell = nullptr;
    u16 field_120_max_idx = 0;
    s32 field_124_code1 = 0;
    s16 field_128_idx = 0;
    s32 field_12C_timer = 0;
    bool mSongMatching = false;
    s16 mSolveSwitchId = 0;
    s16 field_134_pressed = 0;
    s16 mUnpossessionCountdown = 0;
    s16 field_138_flags = 0;
    FP mTargetX = {};
    FP mTargetY = {};
    FP mBallStartX = {};
    FP mBallStartY = {};
    FP mXPosOffset = {};
    FP mYPosOffset = {};
    s16 mXSize = 0;
    s16 mYSize = 0;
    enum class BallStates : s16
    {
        eIdle_0 = 0,
        eMovingToBell_1 = 1,
        eMovingBackToIdle_2 = 2,
        eNeverRead_3 = 3
    };
    BallStates mBallState = BallStates::eIdle_0;
    s16 field_15E_ball_angle = 0;
    s16 field_160_ball_timer = 0;
    enum class BellPositions : s16
    {
        eNone_0 = 0,
        eLeftBell_1 = 1,
        eCenterBell_2 = 2,
        eRightBell_3 = 3,
        eUnused_4 = 4
    };
    BellPositions field_164_ChimeLock_num[2] = {};
};

} // namespace AO
