#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

namespace AO {

struct Path_RollingBall : public Path_TLV
{
    Scale_short field_18_scale;
    XDirection_short field_1A_roll_direction;
    s16 field_1C_release;
    u16 field_1E_speed;
    u16 field_20_acceleration;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_RollingBall, 0x24);

class RollingBallShaker;
class PathLine;

class RollingBall : public BaseAliveGameObject
{
public:
    EXPORT RollingBall* ctor_4578C0(Path_RollingBall* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_458230();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT RollingBall* Vdtor_458490(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_457AF0();

    EXPORT void Accelerate_458410();

    EXPORT void CrushThingsInTheWay_458310();

    int field_10C_tlvInfo;
    u16 field_110_release_switch_id;
    enum class States : s16
    {
        eInactive_0,
        eStartRolling_1,
        eRolling_2,
        eFallingAndHittingWall_3,
        eCrushedBees_4
    };
    States field_112_state;
    RollingBallShaker* field_114_pRollingBallShaker;
    FP field_118_speed;
    FP field_11C_acceleration;
    PathLine* field_120_pCollisionLine;
    int field_124_padding;
    int field_128_padding;
};
ALIVE_ASSERT_SIZEOF(RollingBall, 0x12C);


}

