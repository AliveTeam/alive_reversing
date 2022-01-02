#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicTrigger.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "MusicController.hpp"
#include "Game.hpp"

namespace AO {

MusicTrigger* MusicTrigger::ctor_443A60(MusicTriggerMusicType type, TriggeredBy triggeredBy, s32 id, s16 delay)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBBC0);
    Init_443AD0(type, triggeredBy, static_cast<s16>(id), delay);
    field_10_tlvInfo = -1;
    return this;
}

MusicTrigger* MusicTrigger::ctor_4439F0(Path_MusicTrigger* pTlv, s32 tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBBC0);
    Init_443AD0(pTlv->field_18_music_type, pTlv->field_1A_triggered_by, pTlv->field_1C_id, pTlv->field_1E_music_delay);
    field_10_tlvInfo = tlvInfo;
    return this;
}

void MusicTrigger::Init_443AD0(MusicTriggerMusicType type, TriggeredBy triggeredBy, u16 id, s16 delay)
{
    field_14_flags &= ~7u;
    field_4_typeId = Types::eNone_0;

    switch (type)
    {
        case MusicTriggerMusicType::eDrumAmbience_0:
            field_1C_music_type = MusicController::MusicTypes::eDrumAmbience_3;
            field_18_counter = 400;
            break;

        case MusicTriggerMusicType::eDeathDrumShort_1:
            field_1C_music_type = MusicController::MusicTypes::eDeathDrumShort_13;
            field_18_counter = 30;
            break;

        case MusicTriggerMusicType::eSecretAreaLong_2:
            field_1C_music_type = MusicController::MusicTypes::eSecretAreaLong_16;
            field_18_counter = 30;
            break;

        case MusicTriggerMusicType::eSoftChase_3:
            field_1C_music_type = MusicController::MusicTypes::eSlogChase_5;
            field_14_flags |= 4;
            break;

        case MusicTriggerMusicType::eIntenseChase_4:
            field_1C_music_type = MusicController::MusicTypes::eIntenseChase_8;
            field_14_flags |= 4;
            break;

        case MusicTriggerMusicType::eChime_5:
            field_1C_music_type = MusicController::MusicTypes::eChime_2;
            field_18_counter = delay;
            break;

        case MusicTriggerMusicType::eSecretAreaShort_6:
            field_1C_music_type = MusicController::MusicTypes::eSecretAreaShort_15;
            field_18_counter = 30;
            break;

        default:
            break;
    }

    switch (triggeredBy)
    {
        case TriggeredBy::eTimer_0:
            field_8_update_delay = 0;
            break;

        case TriggeredBy::eTouching_1:
            field_8_update_delay = delay;
            break;

        case TriggeredBy::eSwitchID_2: // removed in AE
            field_14_flags |= 1u;
            field_1E_id = id;
            field_8_update_delay = 0;
            field_18_counter = delay;
            if (id > 1)
            {
                if (SwitchStates_Get(id))
                {
                    field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
            }
            break;

        case TriggeredBy::eUnknown_3: // removed in AE
            field_14_flags |= 1u;
            field_1E_id = id;
            field_8_update_delay = 0;
            field_18_counter = -1;
            break;

        default:
            return;
    }
}

BaseGameObject* MusicTrigger::dtor_443C20()
{
    SetVTable(this, 0x4BBBC0);
    if (field_14_flags & 4)
    {
        MusicController::PlayMusic_443810(MusicController::MusicTypes::eType0, this, 0, 0);
    }
    return dtor_487DF0();
}


void MusicTrigger::VScreenChanged()
{
    VScreenChanged_443DD0();
}

void MusicTrigger::VScreenChanged_443DD0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

BaseGameObject* MusicTrigger::VDestructor(s32 flags)
{
    return Vdtor_443DF0(flags);
}

MusicTrigger* MusicTrigger::Vdtor_443DF0(s32 flags)
{
    dtor_443C20();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void MusicTrigger::VUpdate()
{
    VUpdate_443C90();
}

void MusicTrigger::VUpdate_443C90()
{
    if (Event_Get_417250(kEventHeroDying_3))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        if (field_10_tlvInfo >= 0)
        {
            gMap_507BA8.TLV_Reset_446870(field_10_tlvInfo, -1, 0, 0);
        }
    }

    if (field_14_flags & 1)
    {
        if (SwitchStates_Get(field_1E_id))
        {
            field_14_flags &= ~1u;

            MusicController::PlayMusic_443810(
                field_1C_music_type,
                this,
                ((u8) field_14_flags >> 2) & 1,
                1);
            field_14_flags |= 2u;

            if (field_18_counter >= 0)
            {
                field_18_counter += gnFrameCount_507670;
            }
        }
    }
    else
    {
        if (!(field_14_flags & 2))
        {
            MusicController::PlayMusic_443810(field_1C_music_type, this, (field_14_flags >> 2) & 1, 1);
            field_14_flags |= 2u;
            field_18_counter += gnFrameCount_507670;
            return;
        }

        if (field_18_counter < 0)
        {
            if (!SwitchStates_Get(field_1E_id))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }

            if (field_18_counter < 0)
            {
                MusicController::PlayMusic_443810(field_1C_music_type, this, (field_14_flags >> 2) & 1, 0);
                return;
            }
        }

        if (static_cast<s32>(gnFrameCount_507670) < field_18_counter)
        {
            MusicController::PlayMusic_443810(field_1C_music_type, this, (field_14_flags >> 2) & 1, 0);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

} // namespace AO
