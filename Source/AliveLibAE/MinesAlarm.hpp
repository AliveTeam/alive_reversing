#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

struct MinesAlarm_State
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MinesAlarm_State, 0x8);

class MinesAlarm : public BaseGameObject
{
public:
    EXPORT static void CC Create_4177F0(s32 timer);
    EXPORT static s32 CC CreateFromSaveState_417740(const u8* pBuffer);
    EXPORT BaseGameObject* ctor_417870(s32 timer);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 vGetSaveState_4177C0(MinesAlarm_State* pState);
    EXPORT void dtor_4178E0();
    EXPORT BaseGameObject* vdtor_4178B0(s32 flags);
    EXPORT void vScreenChanged_417A20();
    EXPORT void vUpdate_417910();
};
ALIVE_ASSERT_SIZEOF(MinesAlarm, 0x20);

ALIVE_VAR_EXTERN(s32, sTimerValue_5C1BFC);
