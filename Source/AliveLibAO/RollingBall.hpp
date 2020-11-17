#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_RollingBall : public Path_TLV
{
    __int16 field_18_scale;
    __int16 field_1A_roll_direction;
    __int16 field_1C_release;
    unsigned __int16 field_1E_speed;
    unsigned __int16 field_20_acceleration;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RollingBall, 0x24);

class RollingBallShaker;
class PathLine;

class RollingBall : public BaseAliveGameObject
{
public:
    EXPORT RollingBall* ctor_4578C0(Path_RollingBall* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_458230();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT RollingBall* Vdtor_458490(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_457AF0();

    EXPORT void SpeedUpOrDown_458410();

    EXPORT void CrushThingsInTheWay_458310();

    int field_10C_tlvInfo;
    unsigned __int16 field_110_release_switch_id;
    __int16 field_112_state;
    RollingBallShaker* field_114_pRollingBallShaker;
    FP field_118_speed;
    FP field_11C;
    PathLine* field_120_pCollisionLine;
    int field_124;
    int field_128;
};
ALIVE_ASSERT_SIZEOF(RollingBall, 0x12C);


}

