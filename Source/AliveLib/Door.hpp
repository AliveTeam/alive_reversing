#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

struct Path_Door;

class Door : public BaseAnimatedWithPhysicsGameObject
{
public:
    enum eStates
    {
        eOpen = 0,
        eClosed = 1,
        eOpening = 2,
        eClosing = 3,
    };

    EXPORT Door* ctor_41E250(Path_Door* pTlvData, int tlvInfo);
    EXPORT BOOL vIsOpen_41EB00();
    EXPORT void vOpen_41EB20();
    EXPORT void vClose_41EB50();
    EXPORT void vSetOpen_41EBA0();
    EXPORT void vSetClosed_41EBC0();
    EXPORT void vScreenChange_41F080();
    EXPORT Door* vdtor_41E9D0(signed int flags);
    EXPORT void PlaySound_41EA90();
    EXPORT void dtor_41EA00();
    EXPORT void vUpdate_41EBE0();

    virtual void VUpdate() override
    {
        vUpdate_41EBE0();
    }

private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;

    int field_F4_tlvInfo;
    enum DoorTypes
    {
        eType_0 = 0,
        eType_1 = 1,
        eType_2 = 2,
        eType_3= 3,
    };
    __int16 field_F8_door_type;
public:
    __int16 field_FA_door_number;
private:
    __int16 field_FC_current_state;
    __int16 field_FE_start_state;

    __int16 field_100_switch_id;
    __int16 field_102_hub_ids[8];
};
ALIVE_ASSERT_SIZEOF(Door, 0x114);
