#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class BaseAliveGameObject;

// NOTE: Called MusicEnv in real game (derived from AE PC demo)
class MusicController : public BaseGameObject
{
public:
    EXPORT static int CC Create_47FC40();
    EXPORT static void CC SetBaseTimeStamp_47FD00();
    EXPORT static void CC Shutdown_47FD20();
    EXPORT static void CC UpdateMusicTime_47F8B0();
    EXPORT static __int16 CC MusicController::sub_47FDA0(WORD* seq1, WORD* seq2, DWORD* seqTime);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    EXPORT MusicController* ctor_47EE80();
    EXPORT BaseGameObject* vdtor_47EF20(signed int flags);
    EXPORT void dtor_47EF50();
    EXPORT __int16 sub_47FA80(WORD *seq, WORD *seq2, DWORD *seqTime);

    virtual void VScreenChanged() override;
    EXPORT void vsub_ScreenChange_47F8F0();

    virtual void VUpdate() override;
    EXPORT void Update_47F730();


    EXPORT void EnableMusic_47FB80(__int16 bEnable);
    EXPORT void SetMusicVolumeDelayed_47FB00(__int16 vol, __int16 delay);
    EXPORT void sub_47EFD0();
    EXPORT void sub_47F910(__int16 typeToSet, const BaseGameObject* pObj, __int16 bFlag4, char bFlag0x20);

    EXPORT void sub_47F260();
    EXPORT void sub_47F0B0();

    EXPORT static void CC sub_47FD60(__int16 typeToSet, const BaseGameObject* pObj, __int16 bFlag4, char bFlag0x20);
    EXPORT static void CC EnableMusic_47FE10(__int16 bOn);

private:
    WORD field_20_vol;
    WORD field_22_vol;
    signed __int16 field_24_currentLevelID;
    __int16 field_26;
    int field_28_object_id;
    __int16 field_2C_flags_and_seq_idx;
    __int16 field_2E;
    DWORD field_30_music_time;
    DWORD field_34_music_start_time;
    DWORD field_38;
    DWORD field_3C;
    __int16 field_40_flags_and_idx;
    WORD field_42_type;
    int field_44;
    DWORD field_48_last_music_frame;
    WORD field_4C_state;
    WORD field_4E_starting_volume;
    WORD field_50_current_vol;
    WORD field_52_target_volume;
    DWORD field_54_music_volume_change_time;
    WORD field_58_flags;
    WORD field_60_pad;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x5C);


ALIVE_VAR_EXTERN(MusicController*, pMusicController_5C3020);
