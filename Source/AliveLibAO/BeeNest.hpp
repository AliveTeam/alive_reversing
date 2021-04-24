#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Path_BeeNest : public Path_TLV
{
    s16 field_18_id;
    s16 field_1A_swarm_size;
    s16 field_1C_chase_time;
    s16 field_1E_speed;
    s16 field_20_disabled_resources;
    s16 field_22_num_bees;
};
ALIVE_ASSERT_SIZEOF(Path_BeeNest, 0x24);

enum class BeeNestStates : u16
{
    eWaitForTrigger_0,
    eResetIfDead_1
};

class BeeSwarm;

enum class LevelIds : s16;

class BeeNest : public BaseGameObject
{
public:
    EXPORT BeeNest* ctor_480E20(Path_BeeNest* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_4810C0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BeeNest* Vdtor_4810E0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_481040();

    virtual void VUpdate() override;

    EXPORT void VUpdate_480F30();

    FP field_10_bee_x;
    FP field_14_bee_y;
    LevelIds field_18_level;
    s16 field_1A_path;
    int field_1C_tlvInfo;
    PSX_Point field_20_top_left;
    PSX_Point field_24_bottom_right;
    u16 field_28_switch_id;
    s16 field_2A_swarm_size;
    u16 field_2C_chase_ticks;
    BeeNestStates field_2E_state;
    FP field_30_speed;
    BeeSwarm* field_34_pBeeSwarm;
};
ALIVE_ASSERT_SIZEOF(BeeNest, 0x38);


}

