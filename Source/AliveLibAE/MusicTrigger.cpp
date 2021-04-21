#include "stdafx.h"
#include "MusicTrigger.hpp"
#include "Function.hpp"
#include "MusicController.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Game.hpp"

EXPORT BaseGameObject* MusicTrigger::ctor_47FE40(Path_MusicTrigger* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5463DC);

    Init_47FFB0(pTlv->field_10_music_type, pTlv->field_12_triggered_by, pTlv->field_14_music_delay);
    field_2C_tl = pTlv->field_8_top_left;
    field_30_br = pTlv->field_C_bottom_right;
    field_20_tlvInfo = tlvInfo;
    return this;
}

EXPORT MusicTrigger* MusicTrigger::ctor_47FF10(MusicTriggerMusicType musicType, TriggeredBy triggeredBy, int /*not_used*/, __int16 musicDelay)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x5463DC);
    Init_47FFB0(musicType, triggeredBy, musicDelay);
    field_2C_tl = {};
    field_30_br = {};
    field_20_tlvInfo = -1;
    return this;
}

EXPORT void MusicTrigger::Init_47FFB0(MusicTriggerMusicType musicType, TriggeredBy triggeredBy, __int16 musicDelay)
{
    field_24_flags.Clear(Flags_24::e24_Bit1_TriggeredByTouching);
    field_24_flags.Clear(Flags_24::e24_Bit2_TriggeredByTimer);
    field_24_flags.Clear(Flags_24::e24_Bit3_SetMusicToNoneOnDtor);

    field_4_typeId = AETypes::eMusicTrigger_94;
    field_28_counter = 0;

    switch (musicType)
    {
    case MusicTriggerMusicType::eUnknown_0:
        field_26_music_type = MusicController::MusicTypes::eType3;
        field_28_counter = 400;
        break;
    case MusicTriggerMusicType::eDeathShort_1:
        field_26_music_type = MusicController::MusicTypes::eDeathShort_10;
        field_28_counter = 30;
        break;
    case MusicTriggerMusicType::eSecretAreaLong_2:
        field_26_music_type = MusicController::MusicTypes::eSecretAreaLong_13;
        field_28_counter = 30;
        break;
    case MusicTriggerMusicType::eChase_3:
        field_26_music_type = MusicController::MusicTypes::eChase_8;
        field_24_flags.Set(Flags_24::e24_Bit3_SetMusicToNoneOnDtor);
        break;
    case MusicTriggerMusicType::eSlogChase_4:
        field_26_music_type = MusicController::MusicTypes::eSlogChase_7;
        field_24_flags.Set(Flags_24::e24_Bit3_SetMusicToNoneOnDtor);
        break;
    case MusicTriggerMusicType::eChime_5:
        field_26_music_type = MusicController::MusicTypes::eChime_2;
        field_28_counter = musicDelay;
        break;
    case MusicTriggerMusicType::eSecretAreaShort_6:
        field_26_music_type = MusicController::MusicTypes::eSecretAreaShort_12;
        field_28_counter = 30;
        break;
    default:
        break;
    }

    if (triggeredBy == TriggeredBy::eTimer_0)
    {
        field_1C_update_delay = musicDelay; // OG bug? field_1C_update_delay should've been field_28_counter?
    }
    else if (triggeredBy == TriggeredBy::eTouching_1)
    {
        field_24_flags.Set(Flags_24::e24_Bit1_TriggeredByTouching);
    }
}

EXPORT BaseGameObject* MusicTrigger::vdtor_47FEE0(signed int flags)
{
    dtor_4800C0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

EXPORT void MusicTrigger::dtor_4800C0()
{
    SetVTable(this, 0x5463DC);
    if (field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor))
    {
        MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, this, 0, 0);
    }
    BaseGameObject_dtor_4DBEC0();
}

EXPORT void MusicTrigger::vScreenChange_4802A0()
{
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

EXPORT void MusicTrigger::vUpdate_480140()
{
    if (Event_Get_422C00(kEventHeroDying))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_24_flags.Get(Flags_24::e24_Bit1_TriggeredByTouching))
    {
        FP xpos = sControlledCharacter_5C1B8C->field_B8_xpos;
        FP ypos = sControlledCharacter_5C1B8C->field_BC_ypos;

        if (xpos >= FP_FromInteger(field_2C_tl.field_0_x) && xpos <= FP_FromInteger(field_30_br.field_0_x) &&
           (ypos >= FP_FromInteger(field_2C_tl.field_2_y) && ypos <= FP_FromInteger(field_30_br.field_2_y)))
        {
            field_24_flags.Clear(Flags_24::e24_Bit1_TriggeredByTouching);
            MusicController::PlayMusic_47FD60(field_26_music_type, this, field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor), 1);
            field_24_flags.Set(Flags_24::e24_Bit2_TriggeredByTimer);
            if (field_28_counter >= 0)
            {
                field_28_counter += sGnFrame_5C1B84;
            }
        }
    }
    else if (field_24_flags.Get(Flags_24::e24_Bit2_TriggeredByTimer))
    {
        if (field_28_counter < 0 || static_cast<int>(sGnFrame_5C1B84) < field_28_counter)
        {
            MusicController::PlayMusic_47FD60(field_26_music_type, this, field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor), 0);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    else
    {
        MusicController::PlayMusic_47FD60(field_26_music_type, this, field_24_flags.Get(Flags_24::e24_Bit3_SetMusicToNoneOnDtor), 1);
        field_24_flags.Set(Flags_24::e24_Bit2_TriggeredByTimer);
        field_28_counter += sGnFrame_5C1B84;
    }
}

BaseGameObject* MusicTrigger::VDestructor(signed int flags)
{
    return vdtor_47FEE0(flags);
}

void MusicTrigger::VUpdate()
{
    vUpdate_480140();
}

void MusicTrigger::VScreenChanged()
{
    vScreenChange_4802A0();
}
