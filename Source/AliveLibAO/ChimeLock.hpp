#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

namespace AO {

class Bells;

struct Path_ChimeLock : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_solve_id;
    __int16 field_1C_code1;
    __int16 field_1E_code2;
    __int16 field_20_id;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ChimeLock, 0x24);


class ChimeLock : public BaseAliveGameObject
{
public:
    EXPORT ChimeLock* ctor_40AB20(Path_ChimeLock* pTlv, signed int tlvInfo);

    EXPORT BaseGameObject* dtor_40AE60();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ChimeLock* Vdtor_40BD40(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_40BCD0();

    virtual void VPossessed() override;

    EXPORT void VPossessed_40BC40();

    virtual void VUnPosses() override;

    EXPORT void VUnPosses_40BC90();

    EXPORT __int16 DoNote_40BB20(__int16 note);

    EXPORT void SetBallTarget_40B7B0(FP ballTargetX, FP ballTargetY, __int16 timer, __int16 xSize, __int16 ySize, __int16 bHitBell);

    EXPORT __int16 UpdateBall_40B8A0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40AEF0();

    void SetTargetBellIfSpace(__int16 targetNum);

    int field_10C_tlvInfo;
    enum class ChimeLockStates : __int16
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
    __int16 field_112_padding;
    Bells* field_114_left_bell;
    Bells* field_118_center_bell;
    Bells* field_11C_right_bell;
    unsigned __int16 field_120_max_idx;
    __int16 field_122_padding;
    int field_124_code1;
    __int16 field_128_idx;
    __int16 field_12A_padding;
    int field_12C_timer;
    __int16 field_130_song_matching;
    __int16 field_132_solve_id;
    __int16 field_134_pressed;
    __int16 field_136_unpossession_timer;
    __int16 field_138_flags;
    __int16 field_13A_padding;
    FP field_13C_targetX;
    FP field_140_targetY;
    FP field_144_ball_start_x;
    FP field_148_ball_start_y;
    FP field_14C_increase_vely_by;
    FP field_150;
    FP field_154;
    __int16 field_158_xSize;
    __int16 field_15A_ySize;
    enum class BallStates : __int16
    {
        eIdle_0 = 0,
        eMovingToBell_1 = 1,
        eMovingBackToIdle_2 = 2,
        eNeverRead_3 = 3
    };
    BallStates field_15C_ball_state;
    __int16 field_15E_ball_angle;
    __int16 field_160_ball_timer;
    __int16 field_162_never_set;
    enum class BellPositions : __int16
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

