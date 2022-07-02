#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum DoorStates : s16
{
    eOpen_0 = 0,
    eClosed_1 = 1,
    eOpening_2 = 2,
    eClosing_3 = 3,
};

enum DoorTypes : s16
{
    eBasicDoor_0 = 0,
    eDoorPadding_1 = 1,
    eTasksDoorWithSecretMusic_2 = 2,
    eTasksDoor_3 = 3,
};

struct Path_Door final : public Path_TLV
{
    LevelIds field_10_level;
    s16 field_12_path;
    s16 field_14_camera;
    Scale_short field_16_scale;
    s16 field_18_door_number;
    s16 field_1A_switch_id;
    s16 field_1C_target_door_id;
    DoorTypes field_1E_type;
    DoorStates field_20_start_state;
    s16 field_22_hub1;
    s16 field_22_hub2;
    s16 field_22_hub3;
    s16 field_22_hub4;
    s16 field_22_hub5;
    s16 field_22_hub6;
    s16 field_22_hub7;
    s16 field_22_hub8;
    ScreenChangeEffects field_32_wipe_effect;
    s16 field_34_movie_number;
    s16 field_36_x_offset;
    s16 field_38_y_offset;
    s16 field_3A_wipe_x_org;
    s16 field_3C_wipe_y_org;
    XDirection_short field_3E_abe_direction;
    Choice_short field_40_close_on_exit;
    Choice_short field_42_clear_throwables;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Door, 0x44);

class Door : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Door();
    Door(Path_Door* pTlvData, s32 tlvInfo);
    ~Door();

    bool vIsOpen();
    void vOpen();
    void vClose();
    void vSetOpen();
    void vSetClosed();
    void PlaySound();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

protected:
    s32 field_F4_tlvInfo = 0;
    DoorTypes field_F8_door_type = DoorTypes::eBasicDoor_0;

public:
    s16 field_FA_door_number = 0;

protected:
    DoorStates field_FC_current_state = DoorStates::eOpen_0;
    DoorStates field_FE_start_state = DoorStates::eOpen_0;

    s16 field_100_switch_id = 0;
    s16 field_102_hub_ids[8] = {};
};
ALIVE_ASSERT_SIZEOF(Door, 0x114);

struct Path_TrainDoor final : public Path_TLV
{
    XDirection_int field_10_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrainDoor, 0x14);

class TrainDoor final : public Door
{
public:
    TrainDoor(Path_TrainDoor* pTlv, s32 tlvInfo);
    ~TrainDoor();

    virtual void VUpdate() override;
};
ALIVE_ASSERT_SIZEOF(TrainDoor, 0x114);
