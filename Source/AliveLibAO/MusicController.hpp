#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

class BaseGameObject;
class BaseAliveGameObject;

enum class SeqId : s16;
enum class LevelIds : s16;

class MusicController final : public BaseGameObject
{
public:
    EXPORT static s16 CC Create_4436C0();

    static void CC SetBaseTimeStamp();

    static void CC UpdateMusicTime();

    static s32 GetMusicTime();

    enum class MusicTypes : s16
    {
        eTypeNull = -1,
        eType0 = 0,
        eAbeOnElum_1 = 1,
        eChime_2 = 2,
        eDrumAmbience_3 = 3,
        eChase_4 = 4,
        eSlogChase_5 = 5,
        ePossessed_6 = 6,
        eSlogTension = 7,
        eIntenseChase_8 = 8,
        eSlogChaseTension = 9,
        eType10 = 10, // nothing? has no references
        eType11 = 11, //death jingle?
        eType12 = 12, // some sort of chase music
        eDeathDrumShort_13 = 13,
        eDeathLong_14 = 14,
        eSecretAreaShort_15 = 15,
        eSecretAreaLong_16 = 16,
    };

    MusicController();
    ~MusicController();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_443450();

    virtual void VUpdate() override;

    EXPORT void VUpdate_443300();

    EXPORT static void CC PlayMusic_443810(MusicTypes a1, BaseGameObject* a2, s16 a3, s16 a4);
    
    static void ClearObject(BaseGameObject* pObj);

    EXPORT static MusicTypes CC GetAbmientAndMusicInfo_443840(SeqId* ambientSeq, SeqId* musicSeq, u16* ambientOrMusicDuration);

    EXPORT void UpdateVolumeState_442A10();

    static EXPORT void Shutdown_4437E0();

    static EXPORT void CC EnableMusic_443900(s16 bEnable);

    static EXPORT s32 CC OnRootCounter_4437D0();

    EXPORT void PlayMusic_443460(MusicTypes a2, BaseGameObject* a3, s16 a4, s16 a5);
    void DoClearObject(BaseGameObject* pObj);

    EXPORT void UpdateMusic_442C20();

    EXPORT void UpdateAmbiance_442AC0();

    s16 SetMusicVolumeDelayed(s16 vol, s16 delay);

    s16 field_10_bEnableMusic;
    s16 field_12_target_volume;
    s16 field_14;
    s16 field_16_bScreenChanged;
    LevelIds field_18_level;
    s16 field_1A_pad;
    BaseGameObject* field_1C_pObj;
    s16 field_20;
    s16 field_22;
    s16 field_24_bAmbientMusicEnabled;
    SeqId field_26_ambient_seq;
    s32 field_28_amibent_seq_duration;
    s32 field_2C_music_start_time;
    s32 field_30_music_timer;
    s32 field_34_sync_after_beats;
    SeqId field_38_music_seq;
    MusicTypes field_3A_type;
    s32 field_3C_music_seq_duration;
    s32 field_40_started_time;
    s16 field_44_bTypeChanged;
    s16 field_46_restart_track;
    s16 field_48_vol_state;
    s16 field_4A_starting_volume;
    s16 field_4C_current_vol;
    s16 field_4E_vol;
    s32 field_50_music_volume_change_time;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x54);

} // namespace AO
