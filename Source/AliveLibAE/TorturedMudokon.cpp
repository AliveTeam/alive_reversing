#include "stdafx.h"
#include "TorturedMudokon.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Flash.hpp"
#include "Sfx.hpp"
#include "Function.hpp"
#include "Abe.hpp"

TorturedMudokon* TorturedMudokon::ctor_47BC60(Path_TorturedMudokon* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x546378);
    SetVTable(&field_18C_zap_animation, 0x544290);
    SetVTable(&field_F4_tears_animation, 0x544290);

    field_4_typeId = Types::eTorturedMud_141;
    field_230_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Tortured_Modukon);
    field_224_ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    if (field_224_ppRes)
    {
        field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
        field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
        Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_224_ppRes, 1, 1);
        field_20_animation.SetFrame_409D50(Math_RandomRange_496AB0(0, field_20_animation.Get_Frame_Count_40AC70() - 1));
        field_23A_speed_id = pTlv->field_10_speed_id;
        field_23C_release_id = pTlv->field_12_release_id;
        field_23E_state = TorturedMudokonState::eBeingTortured_0;
        SetupTearsAnimation_47BE60(&field_F4_tears_animation);
        SetupZapAnimation_47BEF0(&field_18C_zap_animation);
        field_240_pain_sound_pitch = Math_RandomRange_496AB0(800, 1000);
        field_234_flash_colour_timer = sGnFrame_5C1B84 + 100;
        field_238_flash_colour_counter = 0;
    }
    else
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return this;
}

void TorturedMudokon::VRender(PrimHeader** ppOt)
{
    vRender_47C460(ppOt);
}

void TorturedMudokon::VScreenChanged()
{
    vScreenChanged_47C440();
}

void TorturedMudokon::VUpdate()
{
    vUpdate_47BF80();
}

BaseGameObject* TorturedMudokon::VDestructor(signed int flags)
{
    return vdtor_47BE30(flags);
}

void TorturedMudokon::SetupTearsAnimation_47BE60(Animation* pAnim)
{
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kTorturedMudTearsResID);
    if (pAnim->Init_40A030(500, gObjList_animations_5C1A24, this, 17, 19, ppRes, 1, 0, 0))
    {
        pAnim->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnim->field_14_scale = field_CC_sprite_scale;
        pAnim->field_8_r = 128;
        pAnim->field_9_g = 128;
        pAnim->field_A_b = 128;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void TorturedMudokon::SetupZapAnimation_47BEF0(Animation* pAnim)
{
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kElecwallResID);
    if (pAnim->Init_40A030(15384, gObjList_animations_5C1A24, this, 50, 80, ppRes, 1, 0, 0))
    {
        // TODO: clean this up
        const int layerM1 = static_cast<int>(field_20_animation.field_C_render_layer) - 1;
        pAnim->field_C_render_layer = static_cast<Layer>(layerM1);
        pAnim->field_14_scale = field_CC_sprite_scale;
        pAnim->field_8_r = 128;
        pAnim->field_9_g = 128;
        pAnim->field_A_b = 128;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void TorturedMudokon::vScreenChanged_47C440()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void TorturedMudokon::vRender_47C460(PrimHeader** ppOt)
{
   field_F4_tears_animation.vRender_40B820(
        FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
        FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
            ppOt,
            0,
            0);
    if (field_F4_tears_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        field_18C_zap_animation.vRender_40B820(
            FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
            FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
            ppOt,
            0,
            0);
    }

    PSX_RECT rect = {};
    field_F4_tears_animation.Get_Frame_Rect_409E10(&rect);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_5BB5F4->field_3A_idx);

    field_18C_zap_animation.Get_Frame_Rect_409E10(&rect);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_5BB5F4->field_3A_idx);

    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

void TorturedMudokon::dtor_47C380()
{
    SetVTable(this, 0x546378);
    if (field_23E_state != TorturedMudokonState::eReleased_2)
    {
        Path::TLV_Reset_4DB8E0(field_230_tlvInfo, -1, 0, 0);
    }

    field_F4_tears_animation.vCleanUp_40C630();
    field_18C_zap_animation.vCleanUp_40C630();

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

TorturedMudokon* TorturedMudokon::vdtor_47BE30(signed int flags)
{
    dtor_47C380();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void TorturedMudokon::vUpdate_47BF80()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    if (field_234_flash_colour_timer == static_cast<int>(sGnFrame_5C1B84))
    {
        field_238_flash_colour_counter++;
        field_234_flash_colour_timer = sGnFrame_5C1B84 + 100;
        if (field_238_flash_colour_counter == 4)
        {
            field_238_flash_colour_counter = 0;
        }
    }

    switch (field_23E_state)
    {
        case TorturedMudokonState::eBeingTortured_0:
            if (SwitchStates_Get_466020(field_23A_speed_id))
            {
                field_23E_state = TorturedMudokonState::eKilled_1;
                const AnimRecord& animRec = AnimRec(AnimId::Tortured_Modukon_Zap);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            break;

        case TorturedMudokonState::eKilled_1:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            return;

        case TorturedMudokonState::eReleased_2:
            return;

        default:
            break;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_20_animation.field_E_frame_change_counter == field_20_animation.field_10_frame_delay)
        {
            field_18C_zap_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
            if (!Math_RandomRange_496AB0(0, 8))
            {
                Mudokon_SFX_457EC0(MudSounds::eNoSad_22, 100, Math_RandomRange_496AB0(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
            }
        }
    }

    if (field_20_animation.field_92_current_frame == 6)
    {
        if (Math_RandomRange_496AB0(0, 2))
        {
            field_20_animation.field_92_current_frame = 0;
        }
    }

    BYTE rgbBase = 0;
    switch (field_238_flash_colour_counter)
    {
        case 0:
            rgbBase = static_cast<BYTE>((field_234_flash_colour_timer & 0xFF) - sGnFrame_5C1B84);
            break;

        case 1:
            rgbBase = 0;
            break;

        case 2:
            rgbBase = static_cast<BYTE>(sGnFrame_5C1B84 - (field_234_flash_colour_timer & 0xFF) + 100);
            break;

        case 3:
            rgbBase = 100;
            break;
    }

    if (field_20_animation.field_92_current_frame == 6 && field_20_animation.field_E_frame_change_counter == field_20_animation.field_10_frame_delay)
    {
        auto pFlash = ae_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_428570(Layer::eLayer_39, rgbBase + 50, rgbBase + 50, rgbBase + 110, 1, TPageAbr::eBlend_1, 1);
        }
        field_18C_zap_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        SFX_Play_46FA90(SoundEffect::ElectricZap_39, 70);
        const short sndRnd = Math_RandomRange_496AB0(0, 3) - 1;
        if (sndRnd)
        {
            if (sndRnd == 1)
            {
                Mudokon_SFX_457EC0(MudSounds::eHurt1_16, 127, Math_RandomRange_496AB0(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
            }
        }
        else
        {
            Mudokon_SFX_457EC0(MudSounds::eHurt2_9, 127, Math_RandomRange_496AB0(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
        }
    }

    if (field_20_animation.field_92_current_frame >= 7 && !Math_RandomRange_496AB0(0, 10))
    {
        auto pFlash = ae_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_428570(Layer::eLayer_39, rgbBase + 10, rgbBase + 10, rgbBase + 50, 1, TPageAbr::eBlend_1, 1);
        }
    }

    if (SwitchStates_Get_466020(field_23C_release_id))
    {
        field_23E_state = TorturedMudokonState::eReleased_2;
        const AnimRecord& animRec = AnimRec(AnimId::Tortured_Modukon_Released);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        field_F4_tears_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_18C_zap_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_230_tlvInfo);
        if (pTlv)
        {
            pTlv->field_1_unknown = 1;
        }
    }
}
