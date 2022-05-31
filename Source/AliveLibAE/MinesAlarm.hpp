#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

struct MinesAlarm_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    s32 field_4_timer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(MinesAlarm_State, 0x8);

class MinesAlarm final : public BaseGameObject
{
public:
    static void Create_4177F0(s32 timer);
    static s32 CreateFromSaveState_417740(const u8* pBuffer);
    MinesAlarm(s32 timer);
    ~MinesAlarm();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 vGetSaveState_4177C0(MinesAlarm_State* pState);
    void vScreenChanged_417A20();
    void vUpdate_417910();
};
ALIVE_ASSERT_SIZEOF(MinesAlarm, 0x20);

ALIVE_VAR_EXTERN(s32, sTimerValue_5C1BFC);
