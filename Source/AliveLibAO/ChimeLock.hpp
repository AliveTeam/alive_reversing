#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

namespace AO {

class Bells;

struct Path_ChimeLock : public Path_TLV
{
    s16 field_18_scale;
    s16 field_1A_solve_id;
    s16 field_1C_code1;
    s16 field_1E_code2;
    s16 field_20_id;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ChimeLock, 0x24);


class ChimeLock : public BaseAliveGameObject
{
public:
    EXPORT ChimeLock* ctor_40AB20(Path_ChimeLock* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_40AE60();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ChimeLock* Vdtor_40BD40(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_40BCD0();

    virtual void VPossessed() override;

    EXPORT void VPossessed_40BC40();

    virtual void VUnPosses() override;

    EXPORT void VUnPosses_40BC90();

    EXPORT s16 DoNote_40BB20(s16 note);

    EXPORT void SetBallTarget_40B7B0(FP ballTargetX, FP ballTargetY, s16 timer, s16 xSize, s16 ySize, s16 bHitBell);

    EXPORT s16 UpdateBall_40B8A0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40AEF0();

    void SetTargetBellIfSpace(s16 targetNum);

    s32 field_10C_tlvInfo;
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
    ChimeLockStates field_110_state;
    s16 field_112_padding;
    Bells* field_114_left_bell;
    Bells* field_118_center_bell;
    Bells* field_11C_right_bell;
    u16 field_120_max_idx;
    s16 field_122_padding;
    s32 field_124_code1;
    s16 field_128_idx;
    s16 field_12A_padding;
    s32 field_12C_timer;
    s16 field_130_song_matching;
    s16 field_132_solve_id;
    s16 field_134_pressed;
    s16 field_136_unpossession_timer;
    s16 field_138_flags;
    s16 field_13A_padding;
    FP field_13C_targetX;
    FP field_140_targetY;
    FP field_144_ball_start_x;
    FP field_148_ball_start_y;
    FP field_14C_increase_vely_by;
    FP field_150;
    FP field_154;
    s16 field_158_xSize;
    s16 field_15A_ySize;
    enum class BallStates : s16
    {
        eIdle_0 = 0,
        eMovingToBell_1 = 1,
        eMovingBackToIdle_2 = 2,
        eNeverRead_3 = 3
    };
    BallStates field_15C_ball_state;
    s16 field_15E_ball_angle;
    s16 field_160_ball_timer;
    s16 field_162_never_set;
    enum class BellPositions : s16
    {
        eNone_0 = 0,
        eLeftBell_1 = 1,
        eCenterBell_2 = 2,
        eRightBell_3 = 3,
        eUnused_4 = 4
    };
    BellPositions field_164_ChimeLock_num[2];
};
ALIVE_ASSERT_SIZEOF(ChimeLock, 0x168);

}

