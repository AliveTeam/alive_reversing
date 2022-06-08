#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

class Bells;

struct Path_ChimeLock final : public Path_TLV
{
    Scale_short field_18_scale;
    s16 field_1A_solve_switch_id;
    s16 field_1C_code1;
    s16 field_1E_code2;
    s16 field_20_password_switch_id;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_ChimeLock, 0x24);


class ChimeLock final : public BaseAliveGameObject
{
public:
    ChimeLock(Path_ChimeLock* pTlv, s32 tlvInfo);
    ~ChimeLock();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VPossessed() override;
    virtual void VUnPosses() override;

    s16 DoNote(s16 note);
    void SetBallTarget(FP ballTargetX, FP ballTargetY, s16 timer, s16 xSize, s16 ySize, s16 bHitBell);
    s16 UpdateBall();
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
    ChimeLockStates field_110_state = ChimeLockStates::eIdle_0;
    Bells* field_114_left_bell = nullptr;
    Bells* field_118_center_bell = nullptr;
    Bells* field_11C_right_bell = nullptr;
    u16 field_120_max_idx = 0;
    s32 field_124_code1 = 0;
    s16 field_128_idx = 0;
    s32 field_12C_timer = 0;
    s16 field_130_song_matching = 0;
    s16 field_132_solve_switch_id = 0;
    s16 field_134_pressed = 0;
    s16 field_136_unpossession_timer = 0;
    s16 field_138_flags = 0;
    FP field_13C_targetX = FP_FromInteger(0);
    FP field_140_targetY = FP_FromInteger(0);
    FP field_144_ball_start_x = FP_FromInteger(0);
    FP field_148_ball_start_y = FP_FromInteger(0);
    FP field_14C_increase_vely_by = FP_FromInteger(0);
    FP field_150_xpos_offset = FP_FromInteger(0);
    FP field_154_ypos_offset = FP_FromInteger(0);
    s16 field_158_xSize = 0;
    s16 field_15A_ySize = 0;
    enum class BallStates : s16
    {
        eIdle_0 = 0,
        eMovingToBell_1 = 1,
        eMovingBackToIdle_2 = 2,
        eNeverRead_3 = 3
    };
    BallStates field_15C_ball_state = BallStates::eIdle_0;
    s16 field_15E_ball_angle = 0;
    s16 field_160_ball_timer = 0;
    s16 field_162_never_set = 0;
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
ALIVE_ASSERT_SIZEOF(ChimeLock, 0x168);

} // namespace AO
