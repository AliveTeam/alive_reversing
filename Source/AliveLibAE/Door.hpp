#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Door : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Door();
    Door(relive::Path_Door* pTlvData, const Guid& tlvId);
    ~Door();

    void LoadAnimations();

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
    relive::Path_Door::DoorStates field_FC_current_state = relive::Path_Door::DoorStates::eOpen;
    relive::Path_Door::DoorStates field_FE_start_state = relive::Path_Door::DoorStates::eOpen;

    s16 field_100_switch_id = 0;
    s16 field_102_hub_ids[8] = {};
};
ALIVE_ASSERT_SIZEOF(Door, 0x114);

class TrainDoor final : public Door
{
public:
    TrainDoor(relive::Path_TrainDoor* pTlv, const Guid& tlvId);
    ~TrainDoor();

    void LoadAnimations();

    virtual void VUpdate() override;
};
ALIVE_ASSERT_SIZEOF(TrainDoor, 0x114);
