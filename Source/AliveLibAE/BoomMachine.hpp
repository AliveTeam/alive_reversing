#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_BoomMachine : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_nozzle_side;
    __int16 field_14_disabled_resources;
    __int16 field_16_number_of_grenades;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x18);

enum class BoomMachineStates : __int16
{
    eInactive_0,
    eAlreadyUsed_1,
    eDropGrenadeAnimation_2,
    eDropGrenade_3
};

class BoomMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BoomMachine* ctor_445B30(Path_BoomMachine* pTlv, int tlvInfo);


    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BOOL VIsButtonOn_445DF0();

    virtual void VHandleButton_445F00();
private:
    EXPORT void vUpdate_445F50();

    EXPORT void vScreenChange_446020();

    EXPORT BOOL vIsButtonOn_445DF0();

    EXPORT void vHandleButton_445F00();

    EXPORT void dtor_445E40();

    EXPORT BoomMachine* vdtor_445E10(signed int flags);

private:
    int field_F4_tlvInfo;
    int field_F8_nozzle_id;
    int field_FC_bIsButtonOn;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);
