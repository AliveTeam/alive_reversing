#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

class Electrocute;

class Teleporter final : public BaseGameObject
{
public:
    Teleporter(Path_Teleporter* pTlv, u32 tlvInfo);
    ~Teleporter();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    static Electrocute* Create_ElectrocuteEffect();
    void SpawnRingSparks(Path_Teleporter_Data* pTlvData);

    s32 field_20_tlvInfo = 0;
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
    Path_Teleporter_Data field_34_mTlvData;
    Path_Teleporter* field_4C_pTlv;
    s32 field_50_objId = 0;
    s16 field_54_effect_created = 0;
};
ALIVE_ASSERT_SIZEOF(Teleporter, 0x58);
