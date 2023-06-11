#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

struct MinesAlarmSaveState final
{
    ReliveTypes mType;
    s32 mExplosionTimer;
};

class MinesAlarm final : public BaseGameObject
{
public:
    static void Create(s32 timer);
    static void CreateFromSaveState(SerializedObjectData& pBuffer);
    MinesAlarm(s32 timer);
    ~MinesAlarm();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;
};

extern s32 gExplosionTimer;
