#include "stdafx.h"
#include "MusicController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include <timeapi.h>
#include "Midi.hpp"

ALIVE_VAR(1, 0x5C3020, MusicController*, pMusicController_5C3020, nullptr);
ALIVE_VAR(1, 0x5C301C, DWORD, sMusicControllerBaseTimeStamp_5C301C, 0);

int CC MusicController::Create_47FC40()
{
    if (pMusicController_5C3020)
    {
        return 0;
    }

    pMusicController_5C3020 = alive_new<MusicController>();
    if (pMusicController_5C3020)
    {
        // NOTE: OG bug - only ctor was guarded by null check
        pMusicController_5C3020->ctor_47EE80();

        MusicController::SetBaseTimeStamp_47FD00();
        pMusicController_5C3020->EnableMusic_47FB80(FALSE);
    }
    return 1;
}

void CC MusicController::SetBaseTimeStamp_47FD00()
{
    sMusicControllerBaseTimeStamp_5C301C = timeGetTime();
}

void CC MusicController::Shutdown_47FD20()
{
    NOT_IMPLEMENTED();
}

void CC MusicController::sub_47FD60(__int16 /*a1*/, BaseGameObject* /*a2*/, __int16 /*a3*/, char /*a4*/)
{
    NOT_IMPLEMENTED();
}

void CC MusicController::sub_47FE10(__int16 /*a1*/)
{
    NOT_IMPLEMENTED();
}

void MusicController::VDestructor(signed int flags)
{
    vdtor_47EF20(flags);
}

void MusicController::ctor_47EE80()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5463C0); // vTbl_MusicController_5463C0

    field_6_flags |= BaseGameObject::eBit08;
    field_40_flags_and_idx = -1;
    field_24_currentLevelID = -1;
    field_28_object_id = -1;
    field_2C_flags_and_seq_idx = -1;
    field_42_type = 0;
    field_30_music_time = 0;

    field_58_flags &= ~4;
    field_58_flags |= 0x1B;

    field_34_music_start_time = 0;
    field_48_last_music_frame = 0;
    field_38 = 0;
    field_4C_state = 0;

    field_4E_starting_volume = 0;
    field_50_current_vol = 0;
    field_52_target_volume = 0;
    field_54_music_volume_change_time = 0;
    
    field_4_typeId = BaseGameObject::Types::eMusicController;
    
    field_20_vol = 100;
    field_22_vol = 127;
    field_3C = 1;
}

void MusicController::vdtor_47EF20(signed int flags)
{
    dtor_47EF50();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void MusicController::dtor_47EF50()
{
    SetVTable(this, 0x5463C0); // vTbl_MusicController_5463C0

    if ((field_40_flags_and_idx & 0x8000) == 0)
    {
        SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
    }

    BaseGameObject_dtor_4DBEC0();
}

void MusicController::EnableMusic_47FB80(__int16 /*bEnable*/)
{
    NOT_IMPLEMENTED();
}
