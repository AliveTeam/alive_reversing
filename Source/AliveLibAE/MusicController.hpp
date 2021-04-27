#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class BaseAliveGameObject;

enum class LevelIds : s16;

// NOTE: Called MusicEnv in real game (derived from AE PC demo)
class MusicController : public BaseGameObject
{
public:
    enum Flags_58
    {
        e58_MusicEnabled_Bit1 = 0x1,
        e58_ScreenChanged_Bit2 = 0x2,
        e58_Dead_Bit3 = 0x4,
        e58_Bit4 = 0x8,
        e58_AmbientMusicEnabled_Bit5 = 0x10,
        e58_UnPause_Bit6 = 0x20,
        e58_Bit7 = 0x40,
        e58_Bit8 = 0x80,
        e58_Bit9 = 0x100,
        e58_Bit10 = 0x200,
        e58_Bit11 = 0x400,
        e58_Bit12 = 0x800,
        e58_eBit13 = 0x1000,
        e58_eBit14 = 0x2000,
        e58_eBit15 = 0x4000,
        e58_eBit16 = 0x8000,
    };

    enum class MusicTypes : s16
    {
        eTypeNull = -1,
        eNone_0 = 0,
        eType1 = 1,
        eChime_2 = 2,
        eType3 = 3,
        eTension_4 = 4,
        eSlogTension_5 = 5,
        eSlogChaseTension_6 = 6,
        eSlogChase_7 = 7,
        eChase_8 = 8,
        ePossessed_9 = 9,
        eDeathShort_10 = 10,
        eDeathLong_11 = 11, //death jingle?
        eSecretAreaShort_12 = 12,
        eSecretAreaLong_13 = 13,
    };


    EXPORT static s32 CC Create_47FC40();
    EXPORT static void CC SetBaseTimeStamp_47FD00();
    EXPORT static void CC Shutdown_47FD20();
    EXPORT static void CC UpdateMusicTime_47F8B0();
    EXPORT static MusicTypes CC GetMusicType_47FDA0(u16* seq1, u16* seq2, u32* seqTime);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    EXPORT MusicController* ctor_47EE80();
    EXPORT BaseGameObject* vdtor_47EF20(s32 flags);
    EXPORT void dtor_47EF50();
    EXPORT MusicTypes GetMusicType_47FA80(u16* seq, u16* seq2, u32* seqTime);

    virtual void VScreenChanged() override;
    EXPORT void vsub_ScreenChange_47F8F0();

    virtual void VUpdate() override;
    EXPORT void Update_47F730();


    EXPORT void EnableMusic_47FB80(s16 enable);
    EXPORT void SetMusicVolumeDelayed_47FB00(s16 vol, s16 delay);
    EXPORT void UpdateVolumeState_47EFD0();
    EXPORT void PlayMusic_47F910(MusicTypes typeToSet, const BaseGameObject* pObj, s16 bFlag4, s8 bFlag0x20);

    EXPORT void UpdateMusic_47F260();
    EXPORT void UpdateAmbiance_47F0B0();

    EXPORT static void CC PlayMusic_47FD60(MusicTypes typeToSet, const BaseGameObject* pObj, s16 bFlag4, s8 bFlag0x20);
    EXPORT static void CC EnableMusic_47FE10(s16 bOn);

private:
    u16 field_20_vol;
    u16 field_22_vol;
    LevelIds field_24_currentLevelID;
    s16 field_26_padding;
    s32 field_28_object_id;
    s16 field_2C_flags_and_seq_idx;
    s16 field_2E_padding;
    u32 field_30_music_time;
    u32 field_34_music_start_time;
    u32 field_38_unused;
    u32 field_3C_unused;
    s16 field_40_flags_and_idx;
    MusicTypes field_42_type;
    s32 field_44;
    u32 field_48_last_music_frame;
    u16 field_4C_state;
    u16 field_4E_starting_volume;
    u16 field_50_current_vol;
    u16 field_52_target_volume;
    u32 field_54_music_volume_change_time;
    BitField16<Flags_58> field_58_flags;
    u16 field_60_padding;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x5C);


ALIVE_VAR_EXTERN(MusicController*, pMusicController_5C3020);
