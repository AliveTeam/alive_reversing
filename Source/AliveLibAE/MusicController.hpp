#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class BaseAliveGameObject;

enum class LevelIds : __int16;

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

    enum class MusicTypes : __int16
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
        eType12 = 12,
        eSecretAreaLong_13 = 13,
    };


    EXPORT static int CC Create_47FC40();
    EXPORT static void CC SetBaseTimeStamp_47FD00();
    EXPORT static void CC Shutdown_47FD20();
    EXPORT static void CC UpdateMusicTime_47F8B0();
    EXPORT static MusicTypes CC GetMusicType_47FDA0(WORD* seq1, WORD* seq2, DWORD* seqTime);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    EXPORT MusicController* ctor_47EE80();
    EXPORT BaseGameObject* vdtor_47EF20(signed int flags);
    EXPORT void dtor_47EF50();
    EXPORT MusicTypes GetMusicType_47FA80(WORD *seq, WORD *seq2, DWORD *seqTime);

    virtual void VScreenChanged() override;
    EXPORT void vsub_ScreenChange_47F8F0();

    virtual void VUpdate() override;
    EXPORT void Update_47F730();


    EXPORT void EnableMusic_47FB80(__int16 enable);
    EXPORT void SetMusicVolumeDelayed_47FB00(__int16 vol, __int16 delay);
    EXPORT void sub_47EFD0();
    EXPORT void sub_47F910(MusicTypes typeToSet, const BaseGameObject* pObj, __int16 bFlag4, char bFlag0x20);

    EXPORT void sub_47F260();
    EXPORT void sub_47F0B0();

    EXPORT static void CC sub_47FD60(MusicTypes typeToSet, const BaseGameObject* pObj, __int16 bFlag4, char bFlag0x20);
    EXPORT static void CC EnableMusic_47FE10(__int16 bOn);

private:
    WORD field_20_vol;
    WORD field_22_vol;
    LevelIds field_24_currentLevelID;
    __int16 field_26_padding;
    int field_28_object_id;
    __int16 field_2C_flags_and_seq_idx;
    __int16 field_2E_padding;
    DWORD field_30_music_time;
    DWORD field_34_music_start_time;
    DWORD field_38;
    DWORD field_3C;
    __int16 field_40_flags_and_idx;
    MusicTypes field_42_type;
    int field_44;
    DWORD field_48_last_music_frame;
    WORD field_4C_state;
    WORD field_4E_starting_volume;
    WORD field_50_current_vol;
    WORD field_52_target_volume;
    DWORD field_54_music_volume_change_time;
    BitField16<Flags_58> field_58_flags;
    WORD field_60_padding;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x5C);


ALIVE_VAR_EXTERN(MusicController*, pMusicController_5C3020);
