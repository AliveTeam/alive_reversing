#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
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

    Guid field_20_tlvInfo;
    s16 field_24_global_y1 = 0;
    s16 field_26_global_x1 = 0;
    s16 field_28_global_y2 = 0;
    s16 field_2A_global_x2 = 0;
    s32 field_2C_switch_state = 0;

    enum class TeleporterState : s8
    {
        eWaitForSwitchOn_0 = 0,
        eIntoTeleporter_1 = 1,
        eTeleporting_2 = 2,
        eOutOfTeleporter_4 = 4,
    };
    TeleporterState field_30_state = TeleporterState::eWaitForSwitchOn_0;
    s16 field_32_bDestroySelf = 0;
    Relive_Path_Teleporter_Data field_34_mTlvData = {};
    relive::Path_Teleporter* field_4C_pTlv = nullptr;
    Guid field_50_objId;
    s16 field_54_effect_created = 0;
};
