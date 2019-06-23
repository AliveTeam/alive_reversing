#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class BoomMachine : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual BOOL Vsub_445DF0();

    virtual BaseGameObject* Vsub_445F00();
private:
    EXPORT void vUpdate_445F50();

    EXPORT void vScreenChange_446020();

    EXPORT BOOL virtual vsub_445DF0();

    EXPORT BaseGameObject* vsub_445F00();

    EXPORT void dtor_445E40();

    EXPORT BoomMachine* vdtor_445E10(signed int flags);

private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
};
ALIVE_ASSERT_SIZEOF(BoomMachine, 0x100);
