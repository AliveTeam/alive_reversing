#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

class BaseGameObject;
class BaseAliveGameObject;

enum class SeqId : __int16;
enum class LevelIds : __int16;

class MusicController : public BaseGameObject
{
public:
    EXPORT static __int16 CC Create_4436C0();

    static void CC SetBaseTimeStamp();

    static void CC UpdateMusicTime();

    static int GetMusicTime();

    enum class MusicTypes : __int16
    {
        eTypeNull = -1,
        eType0 = 0,
        eAbeOnElum_1 = 1,
        eType2 = 2,
        eType3 = 3,
        eChase_4 = 4,
        eSlogChase_5 = 5,
        ePossessed_6 = 6,
        eType7 = 7,
        eType8 = 8,
        eType9 = 9,
        eType10 = 10,
        eType11 = 11, //death jingle?
        eType12 = 12,
        eDeathShort_13 = 13,
        eDeathLong_14 = 14,
        eSecretAreaShort_15 = 15,
        eSecretAreaLong_16 = 16,
    };

    MusicController* ctor_442930();

    EXPORT BaseGameObject* dtor_4429B0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_443450();

    virtual void VUpdate() override;

    EXPORT void VUpdate_443300();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MusicController* Vdtor_4439D0(signed int flags);

    EXPORT static void CC sub_443810(MusicTypes a1, BaseGameObject* a2, __int16 a3, __int16 a4);

    EXPORT static MusicTypes CC GetAbmientAndMusicInfo_443840(SeqId* ambientSeq, SeqId* musicSeq, WORD* ambientOrMusicDuration);

    EXPORT void UpdateVolumeState_442A10();

    static EXPORT void Shutdown_4437E0();

    static EXPORT void CC EnableMusic_443900(__int16 bEnable);

    static EXPORT int CC OnRootCounter_4437D0();

    EXPORT void PlayMusic_443460(MusicTypes a2, BaseGameObject* a3, __int16 a4, __int16 a5);

    EXPORT void UpdateMusic_442C20();

    EXPORT void UpdateAmbiance_442AC0();

    __int16 SetMusicVolumeDelayed(__int16 vol, __int16 delay);

    __int16 field_10_bEnableMusic;
    __int16 field_12_target_volume;
    __int16 field_14;
    __int16 field_16_bScreenChanged;
    LevelIds field_18_level;
    __int16 field_1A_pad;
    BaseGameObject* field_1C_pObj;
    __int16 field_20;
    __int16 field_22;
    __int16 field_24_bAmbientMusicEnabled;
    SeqId field_26_ambient_seq;
    int field_28_amibent_seq_duration;
    int field_2C_music_start_time;
    int field_30_music_timer;
    int field_34_sync_after_beats;
    SeqId field_38_music_seq;
    MusicTypes field_3A_type;
    int field_3C_music_seq_duration;
    int field_40_started_time;
    __int16 field_44_bTypeChanged;
    __int16 field_46_restart_track;
    __int16 field_48_vol_state;
    __int16 field_4A_starting_volume;
    __int16 field_4C_current_vol;
    __int16 field_4E_vol;
    int field_50_music_volume_change_time;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x54);

}

