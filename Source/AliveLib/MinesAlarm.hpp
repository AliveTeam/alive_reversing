#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

struct MinesAlarm_State
{
    Types field_0_type;
    __int16 field_2_pad;
    int field_4_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MinesAlarm_State, 0x8);

class MinesAlarm : public BaseGameObject
{
public:
    EXPORT static void CC Create_4177F0(int timer);
    EXPORT static int CC CreateFromSaveState_417740(const BYTE* pBuffer);
    EXPORT BaseGameObject* ctor_417870(int timer);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
private:
    int vGetSaveState_4177C0(MinesAlarm_State* pState);
    EXPORT void dtor_4178E0();
    EXPORT BaseGameObject* vdtor_4178B0(signed int flags);
    EXPORT void vScreenChanged_417A20();
    EXPORT void vUpdate_417910();
};
ALIVE_ASSERT_SIZEOF(MinesAlarm, 0x20);
