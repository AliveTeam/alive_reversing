#include "stdafx.h"
#include "MusicController.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include <timeapi.h>
#include "Midi.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Map.hpp"
#include "ObjectIds.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0x5C3020, MusicController*, pMusicController_5C3020, nullptr);
ALIVE_VAR(1, 0x5C301C, DWORD, sMusicControllerBaseTimeStamp_5C301C, 0);
ALIVE_VAR(1, 0x5c3024, DWORD, sMusicTime_5C3024, 0);

struct MusicController_Record3_Sub
{
    __int16 field_0;
    char field_2_min;
    char field_3_max;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3_Sub, 4);

struct MusicController_Record3
{
    MusicController_Record3_Sub field_0[3];
    BYTE field_C;
    BYTE field_D;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record3, 0xE);

// TODO: 17 = max levels?

const MusicController_Record3 stru_55D1E0[17] =
{
    { { { 2048, 107, 108 },   { -1, 109, 109 },     { -1, 0, 0     } }, 1u, 0u },
    { { { 320, 103, 103 },    { 1280, 102, 102 },   { -1, 101, 101 } }, 1u, 0u },
    { { { 512, 8, 8 },        { 512, 7, 7 },        { -1, 6, 6     } }, 1u, 0u },
    { { { 512, 17, 17 },      { 512, 16, 16 },      { -1, 15, 15   } }, 1u, 0u },
    { { { 1024, 33, 33 },     { 512, 32, 32 },      { -1, 31, 31   } }, 1u, 0u },
    { { { 640, 49, 49 },      { 640, 48, 48 },      { -1, 47, 47   } }, 1u, 0u },
    { { { 640, 62, 62 },      { 640, 61, 61 },      { -1, 60, 60   } }, 1u, 0u },
    { { { -1, 40, 40 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { 320, 72, 72 },      { 320, 71, 71 },      { -1, 70, 70   } }, 1u, 0u },
    { { { 320, 88, 88 },      { 320, 87, 87 },      { -1, 86, 86   } }, 1u, 0u },
    { { { 512, 97, 97 },      { -1, 96, 97 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 24, 24 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 56, 56 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 66, 66 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { -1, 79, 79 },       { -1, -1, -1 },       { -1, -1, -1   } }, 1u, 0u },
    { { { 320, 103, 103 },    { 1280, 102, 102 },   { -1, 101, 101 } }, 1u, 0u },
    { { { -1, 111, 111 },     { -1, 0, 0 },         { -1, 0, 0     } }, 1u, 0u }
};


struct MusicController_Record
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record, 0x4);

const BYTE byte_55D468[17] =
{
    16u,
    20u,
    16u,
    16u,
    16u,
    20u,
    20u,
    28u,
    20u,
    20u,
    16u,
    28u,
    28u,
    28u,
    28u,
    20u,
    16u
};

const MusicController_Record stru_55D2D0[17] =
{
    { 111u, 16u, 0u, 0u },
    { 104u, 20u, 0u, 0u },
    { 9u, 20u, 0u, 0u },
    { 18u, 16u, 0u, 0u },
    { 34u, 16u, 0u, 0u },
    { 50u, 20u, 0u, 0u },
    { 63u, 20u, 0u, 0u },
    { 41u, 28u, 0u, 0u },
    { 73u, 20u, 0u, 0u },
    { 89u, 20u, 0u, 0u },
    { 98u, 20u, 0u, 0u },
    { 25u, 28u, 0u, 0u },
    { 57u, 28u, 0u, 0u },
    { 67u, 28u, 0u, 0u },
    { 80u, 28u, 0u, 0u },
    { 104u, 20u, 0u, 0u },
    { -1, 1u, 1u, 0u }
};

const MusicController_Record stru_55D314[17] =
{
    { 112, 16u, 0u, 0u },
    { 105, 20u, 0u, 0u },
    { 10, 20u, 0u, 0u },
    { 19, 16u, 0u, 0u },
    { 35, 16u, 0u, 0u },
    { 51, 20u, 0u, 0u },
    { 64, 20u, 0u, 0u },
    { 42, 28u, 0u, 0u },
    { 74, 20u, 0u, 0u },
    { 90, 20u, 0u, 0u },
    { 99, 20u, 0u, 0u },
    { 26, 28u, 0u, 0u },
    { 58, 28u, 0u, 0u },
    { 68, 28u, 0u, 0u },
    { 81, 28u, 0u, 0u },
    { 105, 20u, 0u, 0u },
    { -1, 1u, 1u, 0u }
};

const MusicController_Record stru_55D358[17] =
{
    { -1, 1u, 1u, 0u },
    { 106u, 20u, 0u, 0u },
    { 11u, 20u, 0u, 0u },
    { 20u, 16u, 0u, 0u },
    { 36u, 16u, 0u, 0u },
    { 52u, 20u, 0u, 0u },
    { 65u, 20u, 0u, 0u },
    { 43u, 28u, 0u, 0u },
    { 75u, 20u, 0u, 0u },
    { 91u, 20u, 0u, 0u },
    { 100u, 20u, 0u, 0u },
    { 27u, 28u, 0u, 0u },
    { 59u, 28u, 0u, 0u },
    { 69u, 28u, 0u, 0u },
    { 82u, 28u, 0u, 0u },
    { 106u, 20u, 0u, 0u },
    { -1, 1u, 1u, 0u }
};

const MusicController_Record stru_55D39C[17] =
{
    { -1, 1u, 1u, 0u },
    { 113u, 20u, 0u, 0u },
    { 12u, 20u, 0u, 0u },
    { 21u, 20u, 0u, 0u },
    { 37u, 20u, 0u, 0u },
    { 53u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 44u, 20u, 0u, 0u },
    { 76u, 20u, 0u, 0u },
    { 92u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 28u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 83u, 20u, 0u, 0u },
    { 113u, 20u, 0u, 0u },
    { -1, 1u, 1u, 0u }
};

const MusicController_Record stru_55D3E0[17] =
{
    { -1, 1u, 1u, 0u },
    { 114u, 20u, 0u, 0u },
    { 13u, 20u, 0u, 0u },
    { 22u, 20u, 0u, 0u },
    { 38u, 20u, 0u, 0u },
    { 54u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 45u, 20u, 0u, 0u },
    { 77u, 20u, 0u, 0u },
    { 93u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 29u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 84u, 20u, 0u, 0u },
    { 114u, 20u, 0u, 0u },
    { -1, 1u, 1u, 0u }
};

const MusicController_Record stru_55D424[17] =
{
    { -1, 1u, 1u, 0u },
    { 115u, 20u, 0u, 0u },
    { 14u, 20u, 0u, 0u },
    { 23u, 20u, 0u, 0u },
    { 39u, 20u, 0u, 0u },
    { 55u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 46u, 20u, 0u, 0u },
    { 78u, 20u, 0u, 0u },
    { 94u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 30u, 20u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { -1, 1u, 0u, 0u },
    { 85u, 20u, 0u, 0u },
    { 115u, 20u, 0u, 0u },
    { -1, 1u, 1u, 0u }
};

// TODO: 118 = max SEQs?
struct MusicController_Record2
{
    WORD field_0_idx;
    WORD field_2_duration;
};
ALIVE_ASSERT_SIZEOF(MusicController_Record2, 4);

const MusicController_Record2 stru_55D008[118] =
{
    { 27u, 320u },
    { 28u, 320u },
    { 29u, 320u },
    { 30u, 320u },
    { 31u, 320u },
    { 32u, 320u },
    { 33u, 512u },
    { 34u, 512u },
    { 35u, 256u },
    { 36u, 320u },
    { 38u, 320u },
    { 37u, 320u },
    { 39u, 160u },
    { 40u, 640u },
    { 41u, 160u },
    { 42u, 512u },
    { 43u, 512u },
    { 44u, 512u },
    { 45u, 256u },
    { 46u, 512u },
    { 47u, 256u },
    { 48u, 160u },
    { 49u, 640u },
    { 50u, 160u },
    { 51u, 448u },
    { 52u, 224u },
    { 54u, 112u },
    { 53u, 448u },
    { 55u, 160u },
    { 56u, 640u },
    { 57u, 160u },
    { 58u, 512u },
    { 59u, 512u },
    { 60u, 1024u },
    { 61u, 256u },
    { 62u, 1024u },
    { 63u, 256u },
    { 64u, 160u },
    { 65u, 640u },
    { 66u, 160u },
    { 67u, 448u },
    { 68u, 224u },
    { 70u, 112u },
    { 69u, 448u },
    { 71u, 160u },
    { 72u, 640u },
    { 73u, 160u },
    { 74u, 320u },
    { 75u, 640u },
    { 76u, 640u },
    { 77u, 320u },
    { 79u, 320u },
    { 78u, 320u },
    { 80u, 160u },
    { 81u, 640u },
    { 82u, 160u },
    { 83u, 448u },
    { 84u, 112u },
    { 86u, 448u },
    { 85u, 224u },
    { 87u, 640u },
    { 88u, 640u },
    { 89u, 640u },
    { 90u, 320u },
    { 92u, 320u },
    { 91u, 320u },
    { 93u, 448u },
    { 94u, 112u },
    { 96u, 448u },
    { 95u, 224u },
    { 97u, 320u },
    { 98u, 320u },
    { 99u, 320u },
    { 100u, 320u },
    { 102u, 320u },
    { 101u, 320u },
    { 103u, 160u },
    { 104u, 640u },
    { 105u, 160u },
    { 106u, 448u },
    { 107u, 112u },
    { 109u, 448u },
    { 108u, 224u },
    { 110u, 160u },
    { 111u, 640u },
    { 112u, 160u },
    { 113u, 320u },
    { 114u, 320u },
    { 115u, 320u },
    { 116u, 320u },
    { 118u, 320u },
    { 117u, 320u },
    { 119u, 160u },
    { 120u, 640u },
    { 121u, 160u },
    { 122u, 512u },
    { 123u, 512u },
    { 124u, 512u },
    { 125u, 320u },
    { 127u, 320u },
    { 126u, 320u },
    { 128u, 320u },
    { 129u, 1280u },
    { 130u, 320u },
    { 131u, 320u },
    { 133u, 320u },
    { 132u, 320u },
    { 134u, 256u },
    { 135u, 2048u },
    { 136u, 2048u },
    { 137u, 256u },
    { 138u, 2048u },
    { 139u, 256u },
    { 140u, 160u },
    { 141u, 640u },
    { 142u, 160u },
    { 141u, 640u },
    { 0u, 0u }
};


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

    if (field_40_flags_and_idx > 0)
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

            if (field_2C_flags_and_seq_idx > 0) 
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                field_2C_flags_and_seq_idx = -1;
            }

            if (field_40_flags_and_idx > 0)
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
                
                if (field_2C_flags_and_seq_idx  > 0)
                {
                    SND_SEQ_SetVol_4CAD20(field_2C_flags_and_seq_idx, field_50_current_vol, field_50_current_vol);
                }

                if (field_40_flags_and_idx > 0)
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

            if (field_2C_flags_and_seq_idx > 0)
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                field_2C_flags_and_seq_idx = -1;
            }

            if (field_40_flags_and_idx > 0)
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

void MusicController::sub_47F910(__int16 typeToSet, const BaseGameObject* pObj, __int16 bFlag4, char bFlag0x20)
{
    MusicController::UpdateMusicTime_47F8B0();

    if (typeToSet <= 1 || pObj)
    {
        if (!sObjectIds_5C1B70.Find_449CF0(field_28_object_id))
        {
            field_28_object_id = -1;
        }

        if (field_42_type == typeToSet)
        {
            if (typeToSet)
            {
                field_48_last_music_frame = sMusicTime_5C3024;
            }

            if (field_28_object_id != -1 && field_28_object_id == pObj->field_8_object_id)
            {
                field_58_flags = (field_58_flags & ~4) | 4 * (bFlag4 & 1);
            }

            if (!(field_58_flags & 0x20))
            {
                field_58_flags = (field_58_flags & ~0x20) | 0x20 * (bFlag0x20 & 1);
            }
            return;
        }

        if (!pObj)
        {
            if (field_58_flags & 4)
            {
                return;
            }

            if (bFlag4)
            {
                field_28_object_id = -1;
            }
        }
        else if (pObj->field_8_object_id == field_28_object_id || field_28_object_id == -1 || !(field_58_flags & 4) && (bFlag4 || typeToSet >= field_42_type))
        {
            field_28_object_id = pObj->field_8_object_id;
            field_58_flags = (field_58_flags & ~4) | 4 * (bFlag4 & 1);
        }

        field_58_flags |= 0x20u;
        field_48_last_music_frame = sMusicTime_5C3024;
        field_42_type = typeToSet;
        field_44 = 0;
        return;
    }
}

void MusicController::sub_47F260()
{
    int idx = 0;
    const MusicController_Record* pRecord = nullptr;
   
    if (field_40_flags_and_idx < 0
        || !SND_SsIsEos_DeInlined_4CACD0(field_40_flags_and_idx)
        || field_58_flags & 0x20
        && ( field_42_type == 2
            || field_42_type == 3
            || field_42_type == 10
            || field_42_type == 11
            || field_42_type == 12
            || field_42_type == 13
            || field_42_type == 7
            || field_42_type == 8))
    {
        if (field_40_flags_and_idx > 0)
        {
            SND_SEQ_Stop_4CAE60(field_40_flags_and_idx);
        }
        
        switch (field_42_type)
        {
        case 2: // Silence/base line only?
            field_58_flags &= ~0x10u;
            field_3C = 1;
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case 3: // The rupture farms screen change random ambiance?
            if (field_58_flags & 0x20)
            {
                idx = Math_RandomRange_496AB0(0, 1);
            }
            else
            {
                idx = -1;
            }
            field_58_flags &= ~0x10u;
            field_3C = 1;
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case 4: // danger near music - when slig is near
        case 5: // slog tension
        case 6: // slog tension 2?
            if (field_42_type == 4)
            {
                pRecord = &stru_55D2D0[field_24_currentLevelID];
            }
            else if (field_42_type == 5)
            {
                pRecord = &stru_55D39C[field_24_currentLevelID];
            }
            else if (field_42_type == 6)
            {
                pRecord = &stru_55D424[field_24_currentLevelID];
            }
            idx = pRecord->field_0;
            field_3C = pRecord->field_1;
            field_58_flags = field_58_flags & ~0x10 | 0x10 * (pRecord->field_2 & 1);
            SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mData[stru_55D008[pRecord->field_0].field_0_idx].field_9, 0);
            break;
        case 7: // chase music
            pRecord = &stru_55D3E0[field_24_currentLevelID];
            idx = pRecord->field_0;
            field_3C = pRecord->field_1;
            field_58_flags = field_58_flags & ~0x10 | 0x10 * (pRecord->field_2 & 1);
            SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mData[stru_55D008[idx].field_0_idx].field_9, 0);
            break;
        case 8: // slig chase?
            pRecord = &stru_55D314[field_24_currentLevelID];
            field_3C = pRecord->field_1;
            idx = pRecord->field_0;
            field_58_flags = field_58_flags & ~0x10 | 0x10 * (pRecord->field_2 & 1);
            SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mData[stru_55D008[idx].field_0_idx].field_9, 0);
            break;
        case 9: // slig possesed 
            if (field_58_flags & 0x20)
            {
                pRecord = &stru_55D358[field_24_currentLevelID];
                field_3C = pRecord->field_1;
                idx = stru_55D358[field_24_currentLevelID].field_0;
                field_58_flags = field_58_flags & ~0x10 | 0x10 * (pRecord->field_2 & 1);
                SetMusicVolumeDelayed_47FB00(sSeqData_558D50.mData[stru_55D008[idx].field_0_idx].field_9, 0);
            }
            else
            {
                field_58_flags = field_58_flags | 0x10;
                field_3C = 20;
                SetMusicVolumeDelayed_47FB00(field_20_vol, 30);
                field_58_flags |= 0x40u;
            }
            break;
        case 10: // Death jingle short
            field_3C = 1;
            idx = (field_58_flags & 0x20) != 0 ? 2 : -1;
            field_58_flags = field_58_flags & ~0x10;
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case 11: // Death jingle long
            field_3C = 1;
            idx = (field_58_flags & 0x20) != 0 ? 3 : -1;
            field_58_flags = field_58_flags & ~0x10;
            SetMusicVolumeDelayed_47FB00(field_22_vol, 0);
            break;
        case 12: // secret area short
            field_3C = 120;
            idx = (field_58_flags & 0x20) != 0 ? 4 : -1;
            field_58_flags = field_58_flags & ~0x10;
            SetMusicVolumeDelayed_47FB00(127, 0);
            break;
        case 13: // secret area long
            field_3C = 120;
            idx = (field_58_flags & 0x20) != 0 ? 5 : -1;
            field_58_flags = field_58_flags & ~0x10;
            SetMusicVolumeDelayed_47FB00(80, 0);
            break;
        default: // no change ?
            SetMusicVolumeDelayed_47FB00(field_20_vol, 30);
            field_58_flags |= 0x10u;
            field_3C = byte_55D468[field_24_currentLevelID];
            break;
        }

        if (idx > 0)
        {
            field_40_flags_and_idx = stru_55D008[idx].field_0_idx;
            field_44 = sMusicTime_5C3024 + stru_55D008[idx].field_2_duration;
            SND_SEQ_Play_4CAB10(field_40_flags_and_idx, 1, field_50_current_vol, field_50_current_vol);
        }
        else
        {
            field_40_flags_and_idx = -1;
            field_44 = 0;
        }

        field_38 = sMusicTime_5C3024;

        if (!(field_58_flags & 0x20))
        {
            return;
        }

        if (field_58_flags & 0x40)
        {
            field_58_flags = field_58_flags & ~0x40;
        }
        else
        {
            field_58_flags |= 8;
        }

        field_58_flags &= ~0x20u;
    }
}

void MusicController::sub_47F0B0()
{
    if (field_58_flags & 0x10 || (field_2C_flags_and_seq_idx < 0))
    {
        if (field_58_flags & 8)
        {
            field_30_music_time = 0;
            field_58_flags &= ~8;
            field_34_music_start_time = sMusicTime_5C3024;
        }

        int musicTime = sMusicTime_5C3024;
        if (sMusicTime_5C3024 >= this->field_30_music_time && this->field_58_flags & 0x10)
        {
            if (field_2C_flags_and_seq_idx > 0)
            {
                SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
                musicTime = sMusicTime_5C3024;
            }

            __int16 random = 0;
            if (field_42_type)
            {
                random = Math_RandomRange_496AB0(
                    stru_55D1E0[field_24_currentLevelID].field_0[0].field_2_min,
                    stru_55D1E0[field_24_currentLevelID].field_0[0].field_3_max);
            }
            else
            {
                const int calculatedTime = musicTime - field_34_music_start_time;
                int total = 0;
                bool found = false;
                for (const MusicController_Record3_Sub& rec : stru_55D1E0[field_24_currentLevelID].field_0)
                {
                    total += rec.field_0;
                    if (rec.field_0 < 0 || calculatedTime < total)
                    {
                        random = Math_RandomRange_496AB0(rec.field_2_min, rec.field_3_max);
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    field_2C_flags_and_seq_idx = -1;
                    field_30_music_time = sMusicTime_5C3024;
                    return;
                }
            }

            if (random < 0)
            {
                field_2C_flags_and_seq_idx = -1;
                field_30_music_time = sMusicTime_5C3024;
            }
            else
            {
                field_2C_flags_and_seq_idx = stru_55D008[random].field_0_idx;
                SND_SEQ_Play_4CAB10(field_2C_flags_and_seq_idx, stru_55D1E0[field_24_currentLevelID].field_C, field_50_current_vol, field_50_current_vol);
                field_30_music_time = sMusicTime_5C3024 + stru_55D008[random].field_2_duration;
            }
        }
    }
    else
    {
        SND_SEQ_Stop_4CAE60(field_2C_flags_and_seq_idx);
        field_2C_flags_and_seq_idx = -1;
    }
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
