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

TorturedMudokon::TorturedMudokon(Path_TorturedMudokon* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eTorturedMud_141);
    field_230_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Tortured_Mudokon);
    field_224_ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (field_224_ppRes)
    {
        field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
        field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_224_ppRes, 1, 1);
        field_20_animation.SetFrame(Math_RandomRange(0, field_20_animation.Get_Frame_Count() - 1));
        field_23A_kill_switch_id = pTlv->field_10_kill_switch_id;
        field_23C_release_switch_id = pTlv->field_12_release_switch_id;
        field_23E_state = TorturedMudokonState::eBeingTortured_0;
        SetupTearsAnimation(&field_F4_tears_animation);
        SetupZapAnimation(&field_18C_zap_animation);
        field_240_pain_sound_pitch = Math_RandomRange(800, 1000);
        field_234_flash_colour_timer = sGnFrame_5C1B84 + 100;
        field_238_flash_colour_counter = 0;
    }
    else
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void TorturedMudokon::SetupTearsAnimation(Animation* pAnim)
{
    const AnimRecord& rec = AnimRec(AnimId::Tortured_Mudokon_Tears);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (pAnim->Init(rec.mFrameTableOffset, gObjList_animations_5C1A24, this, rec.mMaxW, rec.mMaxH, ppRes, 1, 0, 0))
    {
        pAnim->mRenderLayer = field_20_animation.mRenderLayer;
        pAnim->field_14_scale = field_CC_sprite_scale;
        pAnim->mRed = 128;
        pAnim->mGreen = 128;
        pAnim->mBlue = 128;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void TorturedMudokon::SetupZapAnimation(Animation* pAnim)
{
    const AnimRecord& rec = AnimRec(AnimId::Electric_Wall);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (pAnim->Init(rec.mFrameTableOffset, gObjList_animations_5C1A24, this, rec.mMaxW, rec.mMaxH, ppRes, 1, 0, 0))
    {
        // TODO: clean this up
        const s32 layerM1 = static_cast<s32>(field_20_animation.mRenderLayer) - 1;
        pAnim->mRenderLayer = static_cast<Layer>(layerM1);
        pAnim->field_14_scale = field_CC_sprite_scale;
        pAnim->mRed = 128;
        pAnim->mGreen = 128;
        pAnim->mBlue = 128;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eListAddFailed_Bit1);
    }
}

void TorturedMudokon::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void TorturedMudokon::VRender(PrimHeader** ppOt)
{
    field_F4_tears_animation.VRender(
        FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
        FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
        ppOt,
        0,
        0);
    if (field_F4_tears_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        field_18C_zap_animation.VRender(
            FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x),
            FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y),
            ppOt,
            0,
            0);
    }

    PSX_RECT rect = {};
    field_F4_tears_animation.Get_Frame_Rect(&rect);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_5BB5F4->field_3A_idx);

    field_18C_zap_animation.Get_Frame_Rect(&rect);
    pScreenManager_5BB5F4->InvalidateRect_40EC90(
        rect.x,
        rect.y,
        rect.w,
        rect.h,
        pScreenManager_5BB5F4->field_3A_idx);

    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

TorturedMudokon::~TorturedMudokon()
{
    if (field_23E_state != TorturedMudokonState::eReleased_2)
    {
        Path::TLV_Reset(field_230_tlvInfo, -1, 0, 0);
    }

    field_F4_tears_animation.VCleanUp();
    field_18C_zap_animation.VCleanUp();
}

void TorturedMudokon::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_234_flash_colour_timer == static_cast<s32>(sGnFrame_5C1B84))
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
            if (SwitchStates_Get(field_23A_kill_switch_id))
            {
                field_23E_state = TorturedMudokonState::eKilled_1;
                const AnimRecord& animRec = AnimRec(AnimId::Tortured_Mudokon_Zap);
                field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
            }
            break;

        case TorturedMudokonState::eKilled_1:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;

        case TorturedMudokonState::eReleased_2:
            return;

        default:
            break;
    }

    if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (field_20_animation.mFrameChangeCounter == field_20_animation.field_10_frame_delay)
        {
            field_18C_zap_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            if (!Math_RandomRange(0, 8))
            {
                Mudokon_SFX(MudSounds::eNoSad_22, 100, Math_RandomRange(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
            }
        }
    }

    if (field_20_animation.field_92_current_frame == 6)
    {
        if (Math_RandomRange(0, 2))
        {
            field_20_animation.field_92_current_frame = 0;
        }
    }

    u8 rgbBase = 0;
    switch (field_238_flash_colour_counter)
    {
        case 0:
            rgbBase = static_cast<u8>((field_234_flash_colour_timer & 0xFF) - sGnFrame_5C1B84);
            break;

        case 1:
            rgbBase = 0;
            break;

        case 2:
            rgbBase = static_cast<u8>(sGnFrame_5C1B84 - (field_234_flash_colour_timer & 0xFF) + 100);
            break;

        case 3:
            rgbBase = 100;
            break;
    }

    if (field_20_animation.field_92_current_frame == 6 && field_20_animation.mFrameChangeCounter == field_20_animation.field_10_frame_delay)
    {
        ae_new<Flash>(Layer::eLayer_Above_FG1_39, rgbBase + 50, rgbBase + 50, rgbBase + 110, 1, TPageAbr::eBlend_1, 1);
        field_18C_zap_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);
        SFX_Play_Mono(SoundEffect::ElectricZap_39, 70);
        const s16 sndRnd = Math_RandomRange(0, 3) - 1;
        if (sndRnd)
        {
            if (sndRnd == 1)
            {
                Mudokon_SFX(MudSounds::eHurt1_16, 127, Math_RandomRange(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
            }
        }
        else
        {
            Mudokon_SFX(MudSounds::eHurt2_9, 127, Math_RandomRange(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
        }
    }

    if (field_20_animation.field_92_current_frame >= 7 && !Math_RandomRange(0, 10))
    {
        ae_new<Flash>(Layer::eLayer_Above_FG1_39, rgbBase + 10, rgbBase + 10, rgbBase + 50, 1, TPageAbr::eBlend_1, 1);
    }

    if (SwitchStates_Get(field_23C_release_switch_id))
    {
        field_23E_state = TorturedMudokonState::eReleased_2;
        const AnimRecord& animRec = AnimRec(AnimId::Tortured_Mudokon_Released);
        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_F4_tears_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_18C_zap_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_230_tlvInfo);
        if (pTlv)
        {
            pTlv->field_1_tlv_state = 1;
        }
    }
}
