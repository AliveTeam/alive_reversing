#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"
#include "MusicController.hpp"

namespace AO {

enum class TriggeredBy : s16
{
    eTimer_0 = 0,
    eTouching_1 = 1,
    eSwitchID_2 = 2,
    eUnknown_3 = 3,
};

enum class MusicTriggerMusicType : s16
{
    eDrumAmbience_0 = 0,
    eDeathDrumShort_1 = 1,
    eSecretAreaLong_2 = 2,
    eSoftChase_3 = 3,
    eIntenseChase_4 = 4,
    eChime_5 = 5,
    eSecretAreaShort_6 = 6
};

struct Path_MusicTrigger final : public Path_TLV
{
    MusicTriggerMusicType field_18_music_type;
    TriggeredBy field_1A_triggered_by;
    s16 field_1C_switch_id;
    s16 field_1E_music_delay;
};
ALIVE_ASSERT_SIZEOF(Path_MusicTrigger, 0x20);

class MusicTrigger final : public BaseGameObject
{
public:
    MusicTrigger(MusicTriggerMusicType type, TriggeredBy triggeredBy, s32 switchId, s32 delay);
    MusicTrigger(Path_MusicTrigger* pTlv, s32 tlvInfo);

    void Init(MusicTriggerMusicType type, TriggeredBy triggeredBy, u16 switchId, s16 delay);

    ~MusicTrigger();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_10_tlvInfo = 0;
    /* enum Flags_14
    {
        eBit1_Unknown = 0x1,
        eBit2_Unknown = 0x2,
        eBit3_Unknown = 0x4,
    };*/
    s16 field_14_flags = 0;
    s16 field_16 = 0;
    s32 field_18_counter = 0;
    MusicController::MusicTypes field_1C_music_type = MusicController::MusicTypes::eType0;
    u16 field_1E_switch_id = 0;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x20);

} // namespace AO
