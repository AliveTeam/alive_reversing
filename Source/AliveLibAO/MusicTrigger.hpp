#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
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
    s16 field_1C_id;
    s16 field_1E_music_delay;
};
ALIVE_ASSERT_SIZEOF(Path_MusicTrigger, 0x20);

class MusicTrigger final : public BaseGameObject
{
public:
    EXPORT MusicTrigger* ctor_443A60(MusicTriggerMusicType type, TriggeredBy triggeredBy, s32 id, s16 delay);

    EXPORT MusicTrigger* ctor_4439F0(Path_MusicTrigger* pTlv, s32 tlvInfo);

    EXPORT void Init_443AD0(MusicTriggerMusicType type, TriggeredBy triggeredBy, u16 id, s16 delay);

    EXPORT BaseGameObject* dtor_443C20();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_443DD0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT MusicTrigger* Vdtor_443DF0(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_443C90();

    s32 field_10_tlvInfo;
    /* enum Flags_14
    {
        eBit1_Unknown = 0x1,
        eBit2_Unknown = 0x2,
        eBit3_Unknown = 0x4,
    };*/
    s16 field_14_flags;
    s16 field_16;
    s32 field_18_counter;
    MusicController::MusicTypes field_1C_music_type;
    u16 field_1E_id;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x20);

} // namespace AO
