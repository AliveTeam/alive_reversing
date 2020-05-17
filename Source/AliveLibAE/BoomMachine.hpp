#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_BoomMachine : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_nozzel_side;
    __int16 field_14_disabled_resources;
    __int16 field_16_number_of_grenades;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BoomMachine, 0x18);

class BoomMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BoomMachine* ctor_445B30(Path_BoomMachine* pTlv, int tlvInfo);


    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BOOL Vsub_445DF0();

    virtual void Vsub_445F00();
private:
    EXPORT void vUpdate_445F50();

    EXPORT void vScreenChange_446020();

    EXPORT BOOL vsub_445DF0();

    EXPORT void vsub_445F00();

    EXPORT void dtor_445E40();

    EXPORT BoomMachine* vdtor_445E10(signed int flags);

private:
    int field_F4_tlvInfo;
    int field_F8_nozzel_id;
    int field_FC_state;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);
