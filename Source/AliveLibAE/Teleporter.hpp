#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Electrocute;

struct Relive_Path_Teleporter_Data
{
    s16 mTeleporterId;
    s16 mOtherTeleporterId;
    s16 mDestCamera;
    s16 mDestPath;
    EReliveLevelIds mDestLevel;
    s16 mSwitchId;
    relive::reliveScale mScale;
    relive::reliveScreenChangeEffects mWipeEffect;
    s16 mMovieId;
    s16 mElectricX;
    s16 mElectricY;
};

class Teleporter final : public BaseGameObject
{
public:
    Teleporter(relive::Path_Teleporter* pTlv, const Guid& tlvId);
    ~Teleporter();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    static Electrocute* Create_ElectrocuteEffect();
    void SpawnRingSparks(Relive_Path_Teleporter_Data* pTlvData);

    Guid mTlvId;
    s32 mSwitchState = 0;

    enum class TeleporterState : s8
    {
        eWaitForSwitchOn_0 = 0,
        eIntoTeleporter_1 = 1,
        eTeleporting_2 = 2,
        eOutOfTeleporter_4 = 4,
    };
    TeleporterState mState = TeleporterState::eWaitForSwitchOn_0;
    bool mDestroySelf = false;
    Relive_Path_Teleporter_Data mTlvData = {};
    Guid mElectrocuteId;
    bool mEffectsCreated = false;
};
