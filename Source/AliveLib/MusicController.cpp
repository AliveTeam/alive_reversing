#include "stdafx.h"
#include "MusicController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include <timeapi.h>
#include "Midi.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5C3020, MusicController*, pMusicController_5C3020, nullptr);
ALIVE_VAR(1, 0x5C301C, DWORD, sMusicControllerBaseTimeStamp_5C301C, 0);
ALIVE_VAR(1, 0x5c3024, DWORD, sMusicTime_5C3024, 0);

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

void CC MusicController::UpdateMusicTime_47F8B0()
{
    sMusicTime_5C3024 = (3 * timeGetTime() - 3 * sMusicControllerBaseTimeStamp_5C301C) / 100;
}

void CC MusicController::Shutdown_47FD20()
{
    if (pMusicController_5C3020)
    {
        pMusicController_5C3020->field_6_flags.Set(BaseGameObject::eDead);
        pMusicController_5C3020 = nullptr;
        //nullsub_8(); // TODO: Check if PSX stub
        //sub_4FBAD0(dword_5C3028); // Some other likely strange PSX specific thing that does nothing
        //nullsub_27(); // TODO: Check if PSX stub
    }
}

__int16 MusicController::sub_47FA80(WORD* seq, WORD* seq2, DWORD* seqTime)
{
    MusicController::UpdateMusicTime_47F8B0();
    if (seq)
    {
        *seq = field_2C_flags_and_seq_idx;
    }

    if (seq2)
    {
        *seq2 = field_40_flags_and_idx;
    }

    if (seqTime)
    {
        if (!field_42_type)
        {
            *seqTime = field_30_music_time - sMusicTime_5C3024;
            return field_42_type;
        }
        *seqTime = field_44 - sMusicTime_5C3024;
    }

    return field_42_type;
}

__int16 CC MusicController::sub_47FDA0(WORD* seq1, WORD* seq2, DWORD* seqTime)
{
    if (pMusicController_5C3020)
    {
        return pMusicController_5C3020->sub_47FA80(seq1, seq2, seqTime);
    }
    return -1;
}

void MusicController::VDestructor(signed int flags)
{
    vdtor_47EF20(flags);
}

void MusicController::ctor_47EE80()
{
    BaseGameObject_ctor_4DBFA0(1, 0);
    SetVTable(this, 0x5463C0); // vTbl_MusicController_5463C0

    field_6_flags.Set(BaseGameObject::eBit08);
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

void MusicController::EnableMusic_47FB80(__int16 bEnable)
{

    MusicController::UpdateMusicTime_47F8B0();

    // If enable flag has changed
    if ((field_58_flags & 1) != bEnable)
    {
        // Flip the flag
        field_58_flags = field_58_flags ^ ((unsigned __int8)bEnable ^ (unsigned __int8)field_58_flags) & 1;

        // Is it enabled?
        if (field_58_flags & 1)
        {
            // Yes enable volume
            SetMusicVolumeDelayed_47FB00(field_20_vol, 0);

            field_44 = 0;
            field_30_music_time = 0;
            field_48_last_music_frame = sMusicTime_5C3024;

            // TODO: Figure out what this logic is testing and refactor
            int v6 = 0;
            const int type = (signed __int16)field_42_type;
            const int typeMinus4 = type - 4;
            if (!typeMinus4 || (v6 = typeMinus4 - 3) == 0 || v6 == 2)
            {
                field_58_flags |= 0x20u;
            }
        }
        else
        {
            // No so turn off volume to disable music
            SetMusicVolumeDelayed_47FB00(0, 0);
        }
        sub_47EFD0();
    }
}

void MusicController::SetMusicVolumeDelayed_47FB00(__int16 vol, __int16 delay)
{
    if (vol != field_50_current_vol)
    {
        field_4E_starting_volume = field_50_current_vol;
        field_52_target_volume = (vol <= 0) ? 0 : vol; // Clamp to 0
        field_34_music_start_time = sMusicTime_5C3024;
        field_54_music_volume_change_time = sMusicTime_5C3024 + delay;
        
        if (vol)
        {
            // vol state on?
            field_4C_state = 3;
        }
        else
        {
            // vol state off ?
            field_4C_state = 2;
        }
    }
}

void MusicController::sub_47EFD0()
{
    if (field_4C_state)
    {
        // volume off ?
        if (field_4C_state == 2)
        {
            field_4C_state = 0;
            field_50_current_vol = 0;

            if ((field_2C_flags_and_seq_idx & 0x8000u) == 0) // TODO: These checks are just != -1 ?
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                field_2C_flags_and_seq_idx = -1;
            }

            if ((field_40_flags_and_idx & 0x8000u) == 0)
            {
                SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
                field_40_flags_and_idx = -1;
            }
        }
        else
        {
            // change volume ?
            if (field_4C_state == 3)
            {
                field_50_current_vol = field_52_target_volume;
                field_4C_state = 1; // Playing ?
                
                if ((field_2C_flags_and_seq_idx & 0x8000u) == 0)
                {
                    SND_SEQ_SetVol_4CAD20(field_2C_flags_and_seq_idx, field_50_current_vol, field_50_current_vol);
                }

                if ((field_40_flags_and_idx & 0x8000u) == 0)
                {
                    SND_SEQ_SetVol_4CAD20(field_40_flags_and_idx, field_50_current_vol, field_50_current_vol);
                }
            }
        }
    }
    else
    {
        field_30_music_time = 0;
        field_34_music_start_time = sMusicTime_5C3024;
        field_48_last_music_frame = sMusicTime_5C3024;
    }
}

void MusicController::VScreenChanged()
{
    vsub_ScreenChange_47F8F0();
}

void MusicController::vsub_ScreenChange_47F8F0()
{
    field_58_flags |= 2u;
}

void MusicController::VUpdate()
{
    Update_47F730();
}

void MusicController::Update_47F730()
{
    MusicController::UpdateMusicTime_47F8B0();
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_58_flags |= 4u;         // player dead flag ?
        field_28_object_id = sActiveHero_5C1B68->field_8_object_id;
    }

    // screen changed flag ?
    if (field_58_flags & 2)
    {
        field_58_flags &= ~2; // Unset it

        if (gMap_5C3030.sCurrentLevelId_5C3030 != field_24_currentLevelID)
        {
            field_44 = 0;
            field_3C = 1;
            field_30_music_time = 0;
            field_58_flags = (field_58_flags & ~4) | 8; // TODO: Flags meaning
            field_48_last_music_frame = sMusicTime_5C3024;
            field_28_object_id = -1;

            if ((field_2C_flags_and_seq_idx & 0x8000u) == 0)
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                field_2C_flags_and_seq_idx = -1;
            }

            if ((field_40_flags_and_idx & 0x8000u) == 0)
            {
                SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
                field_40_flags_and_idx = -1;
            }
            
            field_24_currentLevelID = gMap_5C3030.sCurrentLevelId_5C3030;

            // music on flag ?
            if (field_58_flags & 1)
            {
                SetMusicVolumeDelayed_47FB00(field_20_vol, 0);
                sub_47F910(0, 0, 1, 0);
            }
        }
    }

    if (field_42_type > 1 && sMusicTime_5C3024 - field_48_last_music_frame >= 160)
    {
        sub_47F910(0, 0, 1, 0);
    }

    sub_47EFD0();

    if (field_58_flags & 1)
    {
        sub_47F260();
        sub_47F0B0();
    }
}

void MusicController::sub_47F910(__int16 /*typeToSet*/, const BaseGameObject* /*pObj*/, __int16 /*bFlag4*/, char /*bFlag0x20*/)
{
    NOT_IMPLEMENTED();
}

void MusicController::sub_47F260()
{
    // TODO: This function has crazy tables of static data
    NOT_IMPLEMENTED();
}

void MusicController::sub_47F0B0()
{
    NOT_IMPLEMENTED();
}

void CC MusicController::sub_47FD60(__int16 typeToSet, const BaseGameObject* pObj, __int16 bFlag4, char bFlag0x20)
{
    if (pMusicController_5C3020)
    {
        pMusicController_5C3020->sub_47F910(typeToSet, pObj, bFlag4, bFlag0x20);
    }
}

void CC MusicController::EnableMusic_47FE10(__int16 bOn)
{
    pMusicController_5C3020->EnableMusic_47FB80(bOn);
}
