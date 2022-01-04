#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "PathData.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class LevelIds : s16;

enum class DoorStates : s16
{
    eOpen_0 = 0,
    eClosed_1 = 1,
    eOpening_2 = 2,
    eClosing_3 = 3,
};

struct Path_Door final : public Path_TLV
{
    LevelIds field_18_level;
    s16 field_1A_path;
    s16 field_1C_camera;
    Scale_short field_1E_scale;
    u16 field_20_door_number;
    s16 field_22_switch_id;
    s16 field_24_target_door_number;
    DoorStates field_26_start_state;
    Choice_short field_28_door_closed;
    s16 field_2A_hub1;
    s16 field_2A_hub2;
    s16 field_2A_hub3;
    s16 field_2A_hub4;
    s16 field_2A_hub5;
    s16 field_2A_hub6;
    s16 field_2A_hub7;
    s16 field_2A_hub8;
    s16 field_3A_wipe_effect;
    s16 field_3C_movie_number;
    s16 field_3E_x_offset;
    s16 field_40_y_offset;
    s16 field_42_wipe_x_org;
    s16 field_44_wipe_y_org;
    XDirection_short field_46_abe_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Door, 0x48);

class Door final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Door* ctor_40E010(Path_Door* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_40E710();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_40EDE0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Door* Vdtor_40EDF0(s32 flags);

    EXPORT Bool32 vIsOpen_40E800();

    EXPORT void vOpen_40E810();

    EXPORT void vClose_40E830();

    EXPORT void vSetOpen_40E850();

    EXPORT void vSetClosed_40E860();

    EXPORT void PlaySound_40E780();

    virtual void VUpdate() override;

    EXPORT void VUpdate_40E870();

    s32 field_D4[4];
    s32 field_E4_tlvInfo;
    DoorStates field_E8_start_state;
    s16 field_EA_door_number;
    DoorStates field_EC_current_state;
    Choice_short field_EE_door_closed;
    s16 field_F0_switch_id;
    s16 field_F2_hubs_ids[8];
    s16 field_102_pad;
};
ALIVE_ASSERT_SIZEOF(Door, 0x104);

} // namespace AO
