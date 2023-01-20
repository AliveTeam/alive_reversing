#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Door : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Door();
    Door(relive::Path_Door* pTlv, const Guid& tlvId);
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
    Guid mTlvId;
    relive::Path_Door::DoorTypes mDoorType = relive::Path_Door::DoorTypes::eBasicDoor;
    relive::Path_Door::DoorStates mCurrentState = relive::Path_Door::DoorStates::eOpen;
    relive::Path_Door::DoorStates mStartState = relive::Path_Door::DoorStates::eOpen;

    s16 mSwitchId = 0;
    s16 mHubIds[8] = {};

public:
    s16 mDoorId = 0;
};

class TrainDoor final : public Door
{
public:
    TrainDoor(relive::Path_TrainDoor* pTlv, const Guid& tlvId);
    ~TrainDoor();

    void LoadAnimations();

    virtual void VUpdate() override;
};
