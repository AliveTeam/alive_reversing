#include "stdafx.h"
#include "GlukkonSwitch.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Sound/Midi.hpp"
#include "Glukkon.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "GameSpeak.hpp"

GlukkonSwitch* GlukkonSwitch::ctor_444E60(Path_GlukkonSwitch* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    field_100_last_event_idx = -1;
    field_114 = 0;
    field_116 = -1;

    SetVTable(this, 0x545534);
    field_4_typeId = Types::eHelpPhone_68;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6027);
    Animation_Init_424E10(1512, 70, 19u, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_F4_tlvInfo = tlvInfo;
    field_20_animation.field_C_render_layer = 22;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
    }
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
    }

    field_FA_ok_id = pTlv->field_12_ok_id;
    field_FC_fail_id = pTlv->field_14_fail_id;
    field_118_top_left = pTlv->field_8_top_left;
    field_11C_bottom_right = pTlv->field_C_bottom_right;
    
    field_B8_xpos = FP_FromInteger(pTlv->field_16_xpos);
    field_BC_ypos = FP_FromInteger(pTlv->field_18_ypos);

    PSX_Point pos = {};
    gMap_5C3030.Get_Abe_Spawn_Pos_4806D0(&pos);
    if (field_B8_xpos > FP_FromInteger(0))
    {
        field_B8_xpos -= FP_FromInteger(pos.field_0_x);
    }
    else
    {
        field_B8_xpos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    }

    if (field_BC_ypos > FP_FromInteger(0))
    {
        field_BC_ypos -= FP_FromInteger(pos.field_2_y);
    }
    else
    {
        field_BC_ypos = FP_FromInteger((pTlv->field_8_top_left.field_2_y + pTlv->field_C_bottom_right.field_2_y) / 2);
    }

    if (pTlv->field_1_unknown)
    {
        field_F8_state = pTlv->field_1_unknown - 1;
    }
    else
    {
        field_F8_state = 0;
    }

    if (field_F8_state != 1)
    {
        field_120_timer = sGnFrame_5C1B84 + 10;
    }

    return this;
}

BaseGameObject* GlukkonSwitch::VDestructor(signed int flags)
{
    return vdtor_4450C0(flags);
}

void GlukkonSwitch::VUpdate()
{
    vUpdate_445200();
}

void GlukkonSwitch::VScreenChanged()
{
    vScreenChange_4456D0();
}

void GlukkonSwitch::dtor_4450F0()
{
    SetVTable(this, 0x545534);
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

GlukkonSwitch* GlukkonSwitch::vdtor_4450C0(signed int flags)
{
    dtor_4450F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void GlukkonSwitch::vScreenChange_4456D0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

__int16 GlukkonSwitch::PlayerNearMe_445180()
{
    const short playerXPos = FP_GetExponent(sControlledCharacter_5C1B8C->field_B8_xpos);
    const short playerYPos = FP_GetExponent(sControlledCharacter_5C1B8C->field_BC_ypos);

    if ((playerXPos >= field_118_top_left.field_0_x && playerXPos <= field_11C_bottom_right.field_0_x) &&
        (playerYPos >= field_118_top_left.field_2_y && playerYPos <= field_11C_bottom_right.field_2_y))
    {
        return 1;
    }

    return 0;
}

void GlukkonSwitch::vUpdate_445200()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const int lastEventIdx = pEventSystem_5BC11C->field_28_last_event_index;
    GameSpeakEvents lastEventIdx2 = GameSpeakEvents::eNone_m1;
    if (field_100_last_event_idx == lastEventIdx)
    {
        if (pEventSystem_5BC11C->field_20_last_event == GameSpeakEvents::eNone_m1)
        {
            lastEventIdx2 = GameSpeakEvents::eNone_m1;
        }
        else
        {
            lastEventIdx2 = GameSpeakEvents::eSameAsLast_m2;
        }
    }
    else
    {
        field_100_last_event_idx = lastEventIdx;
        lastEventIdx2 = pEventSystem_5BC11C->field_20_last_event;
    }

    switch (field_F8_state)
    {
    case 0:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_120_timer)
        {
            return;
        }

        if (PlayerNearMe_445180())
        {
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_F8_state = 2;
        }
        else
        {
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        }
        return;

    case 1:
        if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer)
        {
            SND_SEQ_Play_4CAB10(SeqId::SaveTriggerMusic_31, 1, 127, 127);
        }
        else if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer && !PlayerNearMe_445180())
        {
            field_F8_state = 0;
        }
        return;

    case 2:
        Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Hey_0, 127, -200, 0);
        field_20_animation.Set_Animation_Data_409C80(1528, nullptr);
        field_F8_state = 3;
        field_120_timer = sGnFrame_5C1B84 + 150;
        return;

    case 3:
        if (!PlayerNearMe_445180())
        {
            field_F8_state = 0;
            field_120_timer = sGnFrame_5C1B84 - 1;
            return;
        }

        if (lastEventIdx2 == GameSpeakEvents::eNone_m1 || lastEventIdx2 == GameSpeakEvents::eSameAsLast_m2)
        {
            if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
            {
                field_F8_state = 0;
            }
        }
        else
        {
            if (lastEventIdx2 == GameSpeakEvents::Glukkon_Hey_36)
            {
                field_F8_state = 4;
                field_120_timer = sGnFrame_5C1B84 + 30;
            }
            else
            {
                if (lastEventIdx2 < GameSpeakEvents::Glukkon_Hey_36)
                {
                    field_F8_state = 8;
                    field_120_timer = sGnFrame_5C1B84 + 30;
                }

                if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
                {
                    field_F8_state = 0;
                }
            }
        }
        return;

    case 4:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_120_timer)
        {
            return;
        }
        Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::What_11, 127, -200, 0);
        field_20_animation.Set_Animation_Data_409C80(1528, 0);
        field_F8_state = 5;
        field_120_timer = sGnFrame_5C1B84 + 60;
        return;

    case 5:
        if (PlayerNearMe_445180())
        {
            if (lastEventIdx2 == GameSpeakEvents::eNone_m1 || lastEventIdx2 == GameSpeakEvents::eSameAsLast_m2)
            {
                if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
                {
                    field_F8_state = 7;
                    field_120_timer = sGnFrame_5C1B84 + 15;
                }
            }
            else if (lastEventIdx2 == GameSpeakEvents::Glukkon_DoIt_37)
            {
                field_F8_state = 6;
                field_120_timer = sGnFrame_5C1B84 + 30;
            }
            else if (lastEventIdx2 < GameSpeakEvents::Glukkon_Hey_36)
            {
                field_F8_state = 8;
                field_120_timer = sGnFrame_5C1B84 + 30;
            }
            else
            {
                field_F8_state = 7;
                field_120_timer = sGnFrame_5C1B84 + 15;
            }
        }
        else
        {
            field_F8_state = 0;
            field_120_timer = sGnFrame_5C1B84 - 1;
        }
        return;

    case 6:
        if (static_cast<int>(sGnFrame_5C1B84) != field_120_timer)
        {
            return;
        }
        SFX_Play_46FBA0(0x58u, 127, -700); //Bleh!
        Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Laugh_7, 127, -200, 0);
        field_20_animation.Set_Animation_Data_409C80(1528, nullptr);
        SwitchStates_Do_Operation_465F00(field_FA_ok_id, SwitchOp::eToggle_2);
        field_F8_state = 1;
        field_120_timer = sGnFrame_5C1B84 + 15;
        return;

    case 7:
        if (static_cast<int>(sGnFrame_5C1B84) != field_120_timer)
        {
            return;
        }
        Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Heh_5, 127, -200, 0);
        field_20_animation.Set_Animation_Data_409C80(1528, nullptr);
        field_F8_state = 0;
        field_120_timer = sGnFrame_5C1B84 + 90;
        return;

    case 8:
        if (static_cast<int>(sGnFrame_5C1B84) != field_120_timer)
        {
            return;
        }
        Glukkon::PlaySound_GameSpeak_444AF0(GlukkonSpeak::Heh_5, 127, -200, 0);
        field_20_animation.Set_Animation_Data_409C80(1528, nullptr);
        SwitchStates_Do_Operation_465F00(field_FC_fail_id, SwitchOp::eSetTrue_0);
        field_F8_state = 0;
        field_120_timer = sGnFrame_5C1B84 + 90;
        return;

    default:
        return;
    }
}
