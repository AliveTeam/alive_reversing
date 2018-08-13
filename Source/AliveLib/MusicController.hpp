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
    EXPORT static void CC sub_47FD60(__int16 a1, BaseGameObject* a2, __int16 a3, char a4);
    EXPORT static void CC sub_47FE10(__int16 a1);

    virtual void VDestructor(signed int flags) override;
    EXPORT void ctor_47EE80();
    EXPORT void vdtor_47EF20(signed int flags);
    EXPORT void dtor_47EF50();

    EXPORT void EnableMusic_47FB80(__int16 bEnable);
    
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
