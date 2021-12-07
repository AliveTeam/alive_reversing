#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
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

// NOTE: enum created for kPathChangeEffectToInternalScreenChangeEffect_55D55C
enum PathWipeEffects : s16
{
    ePlay1FMV_0 = 0,
    eRightToLeft_1 = 1,
    eLeftToRight_2 = 2,
    eBottomToTop_3 = 3,
    eTopToBottom_4 = 4,
    eBoxOut_5 = 5,
    eVerticalSplit_6 = 6,
    eHorizontalSplit_7 = 7,
    eUnknown_8 = 8,
    eInstantChange_9 = 9,
};

struct Path_Door final : public Path_TLV
{
    LevelIds field_10_level;
    s16 field_12_path;
    s16 field_14_camera;
    Scale_short field_16_scale;
    s16 field_18_door_number;
    s16 field_1A_id;
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
    PathWipeEffects field_32_wipe_effect;
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
    EXPORT Door* ctor_41E250(Path_Door* pTlvData, s32 tlvInfo);
    EXPORT Bool32 vIsOpen_41EB00();
    EXPORT void vOpen_41EB20();
    EXPORT void vClose_41EB50();
    EXPORT void vSetOpen_41EBA0();
    EXPORT void vSetClosed_41EBC0();
    EXPORT void PlaySound_41EA90();
    EXPORT void dtor_41EA00();

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

private:
    EXPORT void vScreenChange_41F080();
    EXPORT Door* vdtor_41E9D0(s32 flags);
    EXPORT void vUpdate_41EBE0();


protected:
    s32 field_F4_tlvInfo;
    DoorTypes field_F8_door_type;

public:
    s16 field_FA_door_number;

protected:
    DoorStates field_FC_current_state;
    DoorStates field_FE_start_state;

    s16 field_100_switch_id;
    s16 field_102_hub_ids[8];
};
ALIVE_ASSERT_SIZEOF(Door, 0x114);

struct Path_TrainDoor final : public Path_TLV
{
    s32 field_10_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrainDoor, 0x14);

class TrainDoor final : public Door
{
public:
    EXPORT TrainDoor* ctor_4DD090(Path_TrainDoor* pTlv, s32 tlvInfo);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

private:
    EXPORT TrainDoor* vdtor_4DD1D0(s32 flags);

    EXPORT void dtor_4DD200();

    EXPORT void vUpdate_4DD2A0();
};
ALIVE_ASSERT_SIZEOF(TrainDoor, 0x114);
