#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Door : public BaseAnimatedWithPhysicsGameObject
{
public:
    Door(); // Only exists for TrainDoor ctor
    Door(relive::Path_Door* pTlv, const Guid& tlvId);
    ~Door();

    void LoadAnimations(const std::string& theme);
    bool IsOpen();
    void Open();
    void Close();
    
protected:
    void HandleFeeCoDepotSwitches();
    void SetDoorPosition(relive::Path_Door* pTlv);

    void SetOpen();
    void SetClosed();

    void PlaySound();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

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
