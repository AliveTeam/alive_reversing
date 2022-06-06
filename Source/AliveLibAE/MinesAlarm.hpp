#pragma once

#include "../relive_lib/BaseGameObject.hpp"
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
    static void Create(s32 timer);
    static s32 CreateFromSaveState(const u8* pBuffer);
    MinesAlarm(s32 timer);
    ~MinesAlarm();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
};
ALIVE_ASSERT_SIZEOF(MinesAlarm, 0x20);

ALIVE_VAR_EXTERN(s32, sTimerValue_5C1BFC);
