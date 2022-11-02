#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

struct MinesAlarmSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_pad;
    s32 field_4_timer;
};

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

extern s32 sTimerValue_5C1BFC;
