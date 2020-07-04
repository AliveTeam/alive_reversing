#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_BeeSwarmHole : public Path_TLV
{
    enum class MovementType : __int16
    {
        eHover_0 = 0,
        eAttack_1 = 1,
        eFollowPath_2 = 2,
    };
    __int16 field_18_what_to_spawn;
    __int16 field_1A_interval;
    __int16 field_1C_id;
    MovementType field_1E_movement_type;
    __int16 field_20_size;
    __int16 field_22_chase_time;
    __int16 field_24_speed;
    __int16 field_26_scale;
};
ALIVE_ASSERT_SIZEOF(Path_BeeSwarmHole, 0x28);

class BeeSwarmHole : public BaseGameObject
{
public:
    EXPORT BeeSwarmHole* ctor_4782B0(Path_BeeSwarmHole* pTlv, int tlvInfo);

    virtual void VUpdate() override;

    EXPORT void VUpdate_478320();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_10_tlvInfo;
    PSX_RECT field_14_rect;
    int field_1C_interval_timer;
    unsigned __int16 field_20_interval;
    __int16 field_22_id;
    Path_BeeSwarmHole::MovementType field_24_movement_type;
    __int16 field_26_num_bees;
    unsigned __int16 field_28_chase_time;
    unsigned __int16 field_2A_speed;
};
ALIVE_ASSERT_SIZEOF(BeeSwarmHole, 0x2C);


END_NS_AO

