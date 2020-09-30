#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

START_NS_AO

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

    int field_10C_tlvInfo;
    __int16 field_110_state;
    __int16 field_112;
    Bells* field_114_bells;
    Bells* field_118_bells;
    Bells* field_11C_bells;
    unsigned __int16 field_120_max_idx;
    __int16 field_122;
    int field_124_code1;
    __int16 field_128_idx;
    __int16 field_12A;
    int field_12C;
    __int16 field_130_song_matching;
    __int16 field_132_solve_id;
    __int16 field_134;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    FP field_13C_targetX;
    FP field_140_targetY;
    FP field_144_ball_start_x;
    FP field_148_ball_start_y;
    FP field_14C;
    FP field_150;
    FP field_154;
    __int16 field_158_xSize;
    __int16 field_15A_ySize;
    __int16 field_15C_ball_state;
    __int16 field_15E_ball_angle;
    __int16 field_160_ball_timer;
    __int16 field_162;
    __int16 field_164;
    __int16 field_166;
};
ALIVE_ASSERT_SIZEOF(ChimeLock, 0x168);

END_NS_AO

