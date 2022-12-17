#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class Door final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Door(relive::Path_Door* pTlv, const Guid& tlvId);
    ~Door();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    bool vIsOpen_40E800();
    void vOpen();
    void vClose();
    void vSetOpen();
    void vSetClosed();
    void PlaySound();

    Guid mTlvInfo;
    relive::Path_Door::DoorTypes mDoorType = relive::Path_Door::DoorTypes::eBasicDoor;
    s16 mDoorId = 0;
    relive::Path_Door::DoorStates mCurrentState = relive::Path_Door::DoorStates::eOpen;
    relive::reliveChoice mDoorClosed = relive::reliveChoice::eNo;
    s16 mSwitchId = 0;
    s16 mHubIds[8] = {};
};

} // namespace AO
