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
    LevelIds mNextLevel;
    s16 mNextPath;
    s16 mNextCamera;
    Scale_short mScale;
    s16 mDoorId;
    s16 mSwitchId;
    s16 mTargetDoorNumber;
    DoorTypes mDoorType;
    DoorStates mStartState;
    s16 mHub1;
    s16 mHub2;
    s16 mHub3;
    s16 mHub4;
    s16 mHub5;
    s16 mHub6;
    s16 mHub7;
    s16 mHub8;
    ScreenChangeEffects mWipeEffect;
    s16 mMovieId;
    s16 mDoorOffsetX;
    s16 mDoorOffsetY;
    s16 field_3A_wipe_x_org;
    s16 field_3C_wipe_y_org;
    XDirection_short mExitDirection;
    Choice_short mCloseOnExit;
    Choice_short mClearThrowables;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Door, 0x44);

class Door : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Door();
    Door(relive::Path_Door* pTlvData, const Guid& tlvId);
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
    Guid field_F4_tlvInfo;
    relive::Path_Door::DoorTypes field_F8_door_type = relive::Path_Door::DoorTypes::eBasicDoor;

public:
    s16 field_FA_door_number = 0;

protected:
    DoorStates field_FC_current_state = DoorStates::eOpen_0;
    relive::Path_Door::DoorStates field_FE_start_state = relive::Path_Door::DoorStates::eOpen;

    s16 field_100_switch_id = 0;
    s16 field_102_hub_ids[8] = {};
};
ALIVE_ASSERT_SIZEOF(Door, 0x114);

struct Path_TrainDoor final : public Path_TLV
{
    XDirection_int mDirection;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrainDoor, 0x14);

class TrainDoor final : public Door
{
public:
    TrainDoor(relive::Path_TrainDoor* pTlv, const Guid& tlvId);
    ~TrainDoor();

    virtual void VUpdate() override;
};
ALIVE_ASSERT_SIZEOF(TrainDoor, 0x114);
