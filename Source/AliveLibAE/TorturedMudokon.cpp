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
    SetType(ReliveTypes::eTorturedMud);
    field_230_tlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Tortured_Mudokon);
    field_224_ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    if (field_224_ppRes)
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->mTopLeft.x);
        mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->mTopLeft.y);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, field_224_ppRes, 1);
        mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(Math_RandomRange(0, mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Count() - 1));
        field_23A_kill_switch_id = pTlv->field_10_kill_switch_id;
        field_23C_release_switch_id = pTlv->field_12_release_switch_id;
        field_23E_state = TorturedMudokonState::eBeingTortured_0;
        SetupTearsAnimation(&field_F4_tears_animation);
        SetupZapAnimation(&field_18C_zap_animation);
        field_240_pain_sound_pitch = Math_RandomRange(800, 1000);
        field_234_flash_colour_timer = sGnFrame + 100;
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
    if (pAnim->Init(rec.mFrameTableOffset, gAnimations, this, rec.mMaxW, rec.mMaxH, ppRes))
    {
        pAnim->mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
        pAnim->field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
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
    if (pAnim->Init(rec.mFrameTableOffset, gAnimations, this, rec.mMaxW, rec.mMaxH, ppRes))
    {
        // TODO: clean this up
        const s32 layerM1 = static_cast<s32>(mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer) - 1;
        pAnim->mRenderLayer = static_cast<Layer>(layerM1);
        pAnim->field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
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
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos()),
        FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos()),
        ppOt,
        0,
        0);
    if (field_F4_tears_animation.mAnimFlags.Get(AnimFlags::eBit3_Render))
    {
        field_18C_zap_animation.VRender(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager->CamXPos()),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager->CamYPos()),
            ppOt,
            0,
            0);
    }

    PSX_RECT rect = {};
    field_F4_tears_animation.Get_Frame_Rect(&rect);
    pScreenManager->InvalidateRectCurrentIdx(
        rect.x,
        rect.y,
        rect.w,
        rect.h);

    field_18C_zap_animation.Get_Frame_Rect(&rect);
    pScreenManager->InvalidateRectCurrentIdx(
        rect.x,
        rect.y,
        rect.w,
        rect.h);

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

    if (field_234_flash_colour_timer == static_cast<s32>(sGnFrame))
    {
        field_238_flash_colour_counter++;
        field_234_flash_colour_timer = sGnFrame + 100;
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
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
            }
            break;

        case TorturedMudokonState::eKilled_1:
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            return;

        case TorturedMudokonState::eReleased_2:
            return;

        default:
            break;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter == mBaseAnimatedWithPhysicsGameObject_Anim.field_10_frame_delay)
        {
            field_18C_zap_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
            if (!Math_RandomRange(0, 8))
            {
                Mudokon_SFX(MudSounds::eNoSad_22, 100, Math_RandomRange(field_240_pain_sound_pitch, field_240_pain_sound_pitch + 100), 0);
            }
        }
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 6)
    {
        if (Math_RandomRange(0, 2))
        {
            mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame = 0;
        }
    }

    u8 rgbBase = 0;
    switch (field_238_flash_colour_counter)
    {
        case 0:
            rgbBase = static_cast<u8>((field_234_flash_colour_timer & 0xFF) - sGnFrame);
            break;

        case 1:
            rgbBase = 0;
            break;

        case 2:
            rgbBase = static_cast<u8>(sGnFrame - (field_234_flash_colour_timer & 0xFF) + 100);
            break;

        case 3:
            rgbBase = 100;
            break;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 6 && mBaseAnimatedWithPhysicsGameObject_Anim.mFrameChangeCounter == mBaseAnimatedWithPhysicsGameObject_Anim.field_10_frame_delay)
    {
        relive_new Flash(Layer::eLayer_Above_FG1_39, rgbBase + 50, rgbBase + 50, rgbBase + 110, 1, TPageAbr::eBlend_1, 1);
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

    if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame >= 7 && !Math_RandomRange(0, 10))
    {
        relive_new Flash(Layer::eLayer_Above_FG1_39, rgbBase + 10, rgbBase + 10, rgbBase + 50, 1, TPageAbr::eBlend_1, 1);
    }

    if (SwitchStates_Get(field_23C_release_switch_id))
    {
        field_23E_state = TorturedMudokonState::eReleased_2;
        const AnimRecord& animRec = AnimRec(AnimId::Tortured_Mudokon_Released);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_F4_tears_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_18C_zap_animation.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        Path_TLV* pTlv = sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(field_230_tlvInfo);
        if (pTlv)
        {
            pTlv->mTlvState = 1;
        }
    }
}
