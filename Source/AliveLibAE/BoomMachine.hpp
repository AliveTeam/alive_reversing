#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_BoomMachine : public Path_TLV
{
    Scale_short field_10_scale;
    XDirection_short field_12_nozzle_side;
    s16 field_14_disabled_resources;
    s16 field_16_number_of_grenades;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x18);

enum class BoomMachineStates : s16
{
    eInactive_0,
    eAlreadyUsed_1,
    eDropGrenadeAnimation_2,
    eDropGrenade_3
};

class BoomMachine : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BoomMachine* ctor_445B30(Path_BoomMachine* pTlv, s32 tlvInfo);


    virtual BaseGameObject* VDestructor(s32 flags) override;

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

    EXPORT BoomMachine* vdtor_445E10(s32 flags);

private:
    s32 field_F4_tlvInfo;
    s32 field_F8_nozzle_id;
    s32 field_FC_bIsButtonOn;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);
