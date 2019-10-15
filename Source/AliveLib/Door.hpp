#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_Door;

class Door : public BaseAnimatedWithPhysicsGameObject
{
public:
    enum eStates
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
        eOpening_2 = 2,
        eClosing_3 = 3,
    };

    EXPORT Door* ctor_41E250(Path_Door* pTlvData, int tlvInfo);
    EXPORT BOOL vIsOpen_41EB00();
    EXPORT void vOpen_41EB20();
    EXPORT void vClose_41EB50();
    EXPORT void vSetOpen_41EBA0();
    EXPORT void vSetClosed_41EBC0();
    EXPORT void PlaySound_41EA90();
    EXPORT void dtor_41EA00();

    virtual void VScreenChanged() override;

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

private:

    EXPORT void vScreenChange_41F080();
    EXPORT Door* vdtor_41E9D0(signed int flags);
    EXPORT void vUpdate_41EBE0();


protected:
    int field_F4_tlvInfo;
    enum DoorTypes
    {
        eType_0 = 0,
        eType_1 = 1,
        eType_2 = 2,
        eType_3 = 3,
    };
    __int16 field_F8_door_type;
public:
    __int16 field_FA_door_number;
protected:
    __int16 field_FC_current_state;
    __int16 field_FE_start_state;

    __int16 field_100_switch_id;
    __int16 field_102_hub_ids[8];
};
ALIVE_ASSERT_SIZEOF(Door, 0x114);

struct Path_TrainDoor : public Path_TLV
{
    int field_10_flipX;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrainDoor, 0x14);

class TrainDoor : public Door
{
public:
    EXPORT TrainDoor* ctor_4DD090(Path_TrainDoor* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

private:
    EXPORT TrainDoor* vdtor_4DD1D0(signed int flags);

    EXPORT void dtor_4DD200();

    EXPORT void vUpdate_4DD2A0();


};
ALIVE_ASSERT_SIZEOF(TrainDoor, 0x114);
