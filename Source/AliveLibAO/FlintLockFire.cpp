#include "stdafx_ao.h"
#include "Function.hpp"
#include "FlintLockFire.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "MusicTrigger.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"
#include "ScreenManager.hpp"
#include "ShadowZone.hpp"

namespace AO {

struct FlintLockFireData final
{
    s32 field_0_gourd_resourceId;
    AnimId field_4_gourd_anim_id;
    s32 field_8_gourd_maxW;
    s32 field_C_gourd_maxH;
    s32 field_10_hammers_resourceId;
    AnimId field_14_hammers_disabled_anim_id;
    AnimId field_18_hammers_activating_anim_id;
    s32 field_1C_hammers_maxW;
    s32 field_20_hammers_maxH;
    s32 field_24_bFire;
};
ALIVE_ASSERT_SIZEOF(FlintLockFireData, 0x28);

const FlintLockFireData sFlintLockFireData_4BAC70[] = {
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {kGourdResID, AnimId::FlintLock_Gourd, 105, 84, kFlintLockResID, AnimId::FlintLock_Hammers_Disabled, AnimId::FlintLock_Hammers_Activating, 125, 59, 1},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {kGourdResID, AnimId::FlintLock_Gourd, 105, 84, kFlintLockResID, AnimId::FlintLock_Hammers_Disabled, AnimId::FlintLock_Hammers_Activating, 125, 59, 1},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {kGourdResID, AnimId::FlintLock_Gourd, 105, 84, kFlintLockResID, AnimId::FlintLock_Hammers_Disabled, AnimId::FlintLock_Hammers_Activating, 125, 59, 1},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0},
    {0, AnimId::None, 0, 0, 0, AnimId::None, AnimId::None, 0, 0, 0}};



void FlintLockFire::VUpdate()
{
    VUpdate_41AEE0();
}

void FlintLockFire::VScreenChanged_41B0B0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

void FlintLockFire::VScreenChanged()
{
    VScreenChanged_41B0B0();
}

void FlintLockFire::VStopAudio_41B0C0()
{
    if (field_EC_fire_sound)
    {
        SND_Stop_Channels_Mask_4774A0(field_EC_fire_sound);
        field_EC_fire_sound = 0;
    }
}

void FlintLockFire::VStopAudio()
{
    VStopAudio_41B0C0();
}

FlintLockFire* FlintLockFire::Vdtor_41B500(s32 flags)
{
    dtor_41AE20();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* FlintLockFire::VDestructor(s32 flags)
{
    return Vdtor_41B500(flags);
}

BaseGameObject* FlintLockFire::dtor_41AE20()
{
    SetVTable(this, 0x4BAEA0);

    gMap_507BA8.TLV_Reset_446870(field_E8_tlvInfo, -1, 0, 0);
    field_F0_anim.vCleanUp();

    if (sFlintLockFireData_4BAC70[static_cast<s32>(gMap_507BA8.field_0_current_level)].field_24_bFire)
    {
        field_188_anim.vCleanUp();
        field_220_anim.vCleanUp();
        if (field_EC_fire_sound)
        {
            SND_Stop_Channels_Mask_4774A0(field_EC_fire_sound);
        }
    }
    return dtor_417D10();
}

FlintLockFire* FlintLockFire::ctor_41AA90(Path_FlintLockFire* pTlv, s32 tlvInfo)
{
    ctor_417C10();
    field_4_typeId = Types::eFlintLockFire_34;

    SetVTable(this, 0x4BAEA0);

    SetVTable(&field_F0_anim, 0x4BA2B8);
    SetVTable(&field_188_anim, 0x4BA2B8);
    SetVTable(&field_220_anim, 0x4BA2B8);

    const s32 cur_lvl = static_cast<s32>(gMap_507BA8.field_0_current_level);

    const AnimRecord& disabledHammersRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_14_hammers_disabled_anim_id);
    const AnimRecord& gourdRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_4_gourd_anim_id);
    u8** ppHammersRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, disabledHammersRec.mResourceId, 1, 0);
    u8** ppGourdRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, gourdRec.mResourceId, 1, 0);

    Animation_Init_417FD0(
        disabledHammersRec.mFrameTableOffset,
        disabledHammersRec.mMaxW,
        disabledHammersRec.mMaxH,
        ppHammersRes,
        1);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_F0_anim.Init_402D20(
        gourdRec.mFrameTableOffset,
        gObjList_animations_505564,
        this,
        static_cast<s16>(gourdRec.mMaxW),
        static_cast<s16>(gourdRec.mMaxH),
        ppGourdRes,
        1,
        0,
        0);

    field_F0_anim.field_B_render_mode = TPageAbr::eBlend_0;
    field_F0_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
    field_F0_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        const AnimRecord& fireRec = AO::AnimRec(AnimId::Fire);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, fireRec.mResourceId, 1, 0);
        u8** ppFireRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, fireRec.mResourceId, 1, 0);
        field_188_anim.Init_402D20(
            fireRec.mFrameTableOffset,
            gObjList_animations_505564,
            this,
            fireRec.mMaxW,
            fireRec.mMaxH,
            ppFireRes,
            1,
            0,
            0);
        field_188_anim.field_B_render_mode = TPageAbr::eBlend_0;
        field_188_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_188_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_188_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_220_anim.Init_402D20(fireRec.mFrameTableOffset, gObjList_animations_505564, this, fireRec.mMaxW, fireRec.mMaxH, ppFireRes, 1, 0, 0);
        field_220_anim.field_B_render_mode = TPageAbr::eBlend_0;
        field_220_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_220_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_220_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_220_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
        field_220_anim.SetFrame_402AC0(3u);
    }

    field_E4_state = States::eDisabled_0;
    field_EC_fire_sound = 0;
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_E8_tlvInfo = tlvInfo;
    field_E6_switch_id = pTlv->field_1A_id;

    Layer layer = Layer::eLayer_0;
    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        layer = Layer::eLayer_BeforeShadow_Half_6;
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        layer = Layer::eLayer_BeforeShadow_25;
        field_C6_scale = 1;
    }

    field_10_anim.field_C_layer = layer;
    field_F0_anim.field_C_layer = layer;

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        field_188_anim.field_C_layer = layer;
        field_220_anim.field_C_layer = layer;
    }

    if (SwitchStates_Get(pTlv->field_1A_id))
    {
        field_E4_state = States::eActivated_2;
        const AnimRecord& activatingRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id);
        field_10_anim.Set_Animation_Data_402A40(activatingRec.mFrameTableOffset, nullptr);
        field_10_anim.SetFrame_402AC0(field_10_anim.Get_Frame_Count_403540() - 1);
        field_10_anim.vDecode();
        field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_F0_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_188_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_220_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
            field_220_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

            field_EC_fire_sound = SFX_Play_43AD70(SoundEffect::Fire_69, 0, 0);
        }
    }
    return this;
}

void FlintLockFire::VUpdate_41AEE0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    const s32 cur_lvl = static_cast<s32>(gMap_507BA8.field_0_current_level);

    switch (field_E4_state)
    {
        case States::eDisabled_0:
            if (SwitchStates_Get(field_E6_switch_id))
            {
                field_E4_state = States::eActivating_1;
                const AnimRecord& activatingRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id);
                field_10_anim.Set_Animation_Data_402A40(
                    activatingRec.mFrameTableOffset,
                    0);
            }
            break;

        case States::eActivating_1:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (field_10_anim.field_92_current_frame == 6)
                {
                    SFX_Play_43AD70(SoundEffect::FlintLock_68, 0, 0);
                    SFX_Play_43AD70(SoundEffect::PostFlint_70, 0, 0);
                }
            }

            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                field_E4_state = States::eActivated_2;

                field_F0_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
                {
                    field_188_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                    field_188_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

                    field_220_anim.field_4_flags.Set(AnimFlags::eBit2_Animate);
                    field_220_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

                    field_EC_fire_sound = SFX_Play_43AD70(SoundEffect::Fire_69, 0, 0);
                }

                auto pMusicTrigger = ao_new<MusicTrigger>();
                if (pMusicTrigger)
                {
                    pMusicTrigger->ctor_443A60(MusicTriggerMusicType::eSecretAreaShort_6, TriggeredBy::eTouching_1, 0, 15);
                }
            }
            break;

        case States::eActivated_2:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (!field_EC_fire_sound)
                {
                    field_EC_fire_sound = SFX_Play_43AD70(SoundEffect::Fire_69, 0, 0);
                }
            }
            break;

        default:
            break;
    }
}

void FlintLockFire::VRender(PrimHeader** ppOt)
{
    VRender_41B0F0(ppOt);
}

void FlintLockFire::VRender_41B0F0(PrimHeader** ppOt)
{
    if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
    {
        const s32 cur_lvl = static_cast<s32>(gMap_507BA8.field_0_current_level);
        field_10_anim.field_14_scale = field_BC_sprite_scale;
        field_F0_anim.field_14_scale = field_BC_sprite_scale;

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.field_14_scale = (field_BC_sprite_scale * FP_FromDouble(1.33));
            field_220_anim.field_14_scale = field_BC_sprite_scale;
        }

        s16 r = field_C0_r;
        s16 g = field_C2_g;
        s16 b = field_C4_b;

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        if (field_CC_bApplyShadows & 1)
        {
            ShadowZone::ShadowZones_Calculate_Colour_435FF0(
                FP_GetExponent(field_A8_xpos),
                FP_GetExponent(FP_FromInteger((bRect.y + bRect.h) / 2)),
                field_C6_scale,
                &r,
                &g,
                &b);
        }


        field_10_anim.field_8_r = static_cast<u8>(r);
        field_10_anim.field_9_g = static_cast<u8>(g);
        field_10_anim.field_A_b = static_cast<u8>(b);

        field_F0_anim.field_8_r = static_cast<u8>(r);
        field_F0_anim.field_9_g = static_cast<u8>(g);
        field_F0_anim.field_A_b = static_cast<u8>(b);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.field_8_r = static_cast<u8>(r);
            field_188_anim.field_9_g = static_cast<u8>(g);
            field_188_anim.field_A_b = static_cast<u8>(b);

            field_220_anim.field_8_r = static_cast<u8>(r);
            field_220_anim.field_9_g = static_cast<u8>(g);
            field_220_anim.field_A_b = static_cast<u8>(b);

            field_220_anim.vRender(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + field_C8_yOffset - 28)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                ppOt,
                0,
                0);

            field_188_anim.vRender(
                FP_GetExponent(field_A8_xpos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos - 3)) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                FP_GetExponent(field_AC_ypos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + field_C8_yOffset - 28)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                ppOt,
                0,
                0);
        }

        field_10_anim.vRender(
            FP_GetExponent(field_A8_xpos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos + (FP_FromInteger(field_C8_yOffset + pScreenManager_4FF7C8->field_16_ypos)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        field_F0_anim.vRender(
            FP_GetExponent(field_A8_xpos
                           + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos
                           + (FP_FromInteger(field_C8_yOffset + pScreenManager_4FF7C8->field_16_ypos))
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        field_10_anim.Get_Frame_Rect_402B50(&frameRect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_4FF7C8->field_2E_idx);

        field_F0_anim.Get_Frame_Rect_402B50(&frameRect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_4FF7C8->field_2E_idx);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.Get_Frame_Rect_402B50(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect_406E40(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->field_2E_idx);

            field_220_anim.Get_Frame_Rect_402B50(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect_406E40(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->field_2E_idx);
        }
    }
}

} // namespace AO
