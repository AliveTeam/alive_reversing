#pragma once

#include "../relive_lib/BaseGameObject.hpp"

struct MinesAlarmSaveState final
{
    ReliveTypes mType;
    s16 field_2_pad;
    s32 mExplosionTimer;
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

extern s32 gExplosionTimer;
