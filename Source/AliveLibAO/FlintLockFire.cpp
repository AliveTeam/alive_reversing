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


void FlintLockFire::VScreenChanged()
{
    mGameObjectFlags.Set(BaseGameObject::eDead);
}

void FlintLockFire::VStopAudio()
{
    if (field_EC_fire_sound)
    {
        SND_Stop_Channels_Mask_4774A0(field_EC_fire_sound);
        field_EC_fire_sound = 0;
    }
}

FlintLockFire::~FlintLockFire()
{
    gMap.TLV_Reset(field_E8_tlvInfo, -1, 0, 0);
    field_F0_anim.VCleanUp();

    if (sFlintLockFireData_4BAC70[static_cast<s32>(gMap.mCurrentLevel)].field_24_bFire)
    {
        field_188_anim.VCleanUp();
        field_220_anim.VCleanUp();
        if (field_EC_fire_sound)
        {
            SND_Stop_Channels_Mask_4774A0(field_EC_fire_sound);
        }
    }
}

FlintLockFire::FlintLockFire(Path_FlintLockFire* pTlv, s32 tlvInfo)
{
    mTypeId = Types::eFlintLockFire_34;

    const s32 cur_lvl = static_cast<s32>(gMap.mCurrentLevel);

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
    mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    field_F0_anim.Init(
        gourdRec.mFrameTableOffset,
        gObjList_animations_505564,
        this,
        static_cast<s16>(gourdRec.mMaxW),
        static_cast<s16>(gourdRec.mMaxH),
        ppGourdRes,
        1,
        0,
        0);

    field_F0_anim.mRenderMode = TPageAbr::eBlend_0;
    field_F0_anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
    field_F0_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        const AnimRecord& fireRec = AO::AnimRec(AnimId::Fire);
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, fireRec.mResourceId, 1, 0);
        u8** ppFireRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, fireRec.mResourceId, 1, 0);
        field_188_anim.Init(
            fireRec.mFrameTableOffset,
            gObjList_animations_505564,
            this,
            fireRec.mMaxW,
            fireRec.mMaxH,
            ppFireRes,
            1,
            0,
            0);
        field_188_anim.mRenderMode = TPageAbr::eBlend_0;
        field_188_anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        field_188_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_188_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

        field_220_anim.Init(fireRec.mFrameTableOffset, gObjList_animations_505564, this, fireRec.mMaxW, fireRec.mMaxH, ppFireRes, 1, 0, 0);
        field_220_anim.mRenderMode = TPageAbr::eBlend_0;
        field_220_anim.mAnimFlags.Clear(AnimFlags::eBit2_Animate);
        field_220_anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_220_anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        field_220_anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
        field_220_anim.SetFrame(3u);
    }

    field_E4_state = States::eDisabled_0;
    field_EC_fire_sound = 0;
    mXPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mYPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_E8_tlvInfo = tlvInfo;
    field_E6_switch_id = pTlv->field_1A_switch_id;

    Layer layer = Layer::eLayer_0;
    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        layer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = 0;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        layer = Layer::eLayer_BeforeShadow_25;
        mScale = 1;
    }

    mAnim.mRenderLayer = layer;
    field_F0_anim.mRenderLayer = layer;

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        field_188_anim.mRenderLayer = layer;
        field_220_anim.mRenderLayer = layer;
    }

    if (SwitchStates_Get(pTlv->field_1A_switch_id))
    {
        field_E4_state = States::eActivated_2;
        const AnimRecord& activatingRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id);
        mAnim.Set_Animation_Data(activatingRec.mFrameTableOffset, nullptr);
        mAnim.SetFrame(mAnim.Get_Frame_Count() - 1);
        mAnim.VDecode();
        mAnim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
        field_F0_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            field_188_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

            field_220_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
            field_220_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

            field_EC_fire_sound = SFX_Play_Mono(SoundEffect::Fire_69, 0, 0);
        }
    }
}

void FlintLockFire::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const s32 cur_lvl = static_cast<s32>(gMap.mCurrentLevel);

    switch (field_E4_state)
    {
        case States::eDisabled_0:
            if (SwitchStates_Get(field_E6_switch_id))
            {
                field_E4_state = States::eActivating_1;
                const AnimRecord& activatingRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id);
                mAnim.Set_Animation_Data(
                    activatingRec.mFrameTableOffset,
                    0);
            }
            break;

        case States::eActivating_1:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (mAnim.field_92_current_frame == 6)
                {
                    SFX_Play_Mono(SoundEffect::FlintLock_68, 0, 0);
                    SFX_Play_Mono(SoundEffect::PostFlint_70, 0, 0);
                }
            }

            if (mAnim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                field_E4_state = States::eActivated_2;

                field_F0_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
                if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
                {
                    field_188_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
                    field_188_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

                    field_220_anim.mAnimFlags.Set(AnimFlags::eBit2_Animate);
                    field_220_anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

                    field_EC_fire_sound = SFX_Play_Mono(SoundEffect::Fire_69, 0, 0);
                }

                ao_new<MusicTrigger>(MusicTriggerMusicType::eSecretAreaShort_6, TriggeredBy::eTouching_1, 0, 15);
            }
            break;

        case States::eActivated_2:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (!field_EC_fire_sound)
                {
                    field_EC_fire_sound = SFX_Play_Mono(SoundEffect::Fire_69, 0, 0);
                }
            }
            break;

        default:
            break;
    }
}

void FlintLockFire::VRender(PrimHeader** ppOt)
{
    if (Is_In_Current_Camera_417CC0() == CameraPos::eCamCurrent_0)
    {
        const s32 cur_lvl = static_cast<s32>(gMap.mCurrentLevel);
        mAnim.field_14_scale = mSpriteScale;
        field_F0_anim.field_14_scale = mSpriteScale;

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.field_14_scale = (mSpriteScale * FP_FromDouble(1.33));
            field_220_anim.field_14_scale = mSpriteScale;
        }

        s16 r = mRed;
        s16 g = mGreen;
        s16 b = mBlue;

        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        if (mApplyShadows & 1)
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                FP_GetExponent(FP_FromInteger((bRect.y + bRect.h) / 2)),
                mScale,
                &r,
                &g,
                &b);
        }


        mAnim.mRed = static_cast<u8>(r);
        mAnim.mGreen = static_cast<u8>(g);
        mAnim.mBlue = static_cast<u8>(b);

        field_F0_anim.mRed = static_cast<u8>(r);
        field_F0_anim.mGreen = static_cast<u8>(g);
        field_F0_anim.mBlue = static_cast<u8>(b);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.mRed = static_cast<u8>(r);
            field_188_anim.mGreen = static_cast<u8>(g);
            field_188_anim.mBlue = static_cast<u8>(b);

            field_220_anim.mRed = static_cast<u8>(r);
            field_220_anim.mGreen = static_cast<u8>(g);
            field_220_anim.mBlue = static_cast<u8>(b);

            field_220_anim.VRender(
                FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + mYOffset - 28)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                ppOt,
                0,
                0);

            field_188_anim.VRender(
                FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos - 3)) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
                FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos + mYOffset - 28)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
                ppOt,
                0,
                0);
        }

        mAnim.VRender(
            FP_GetExponent(mXPos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(mYPos + (FP_FromInteger(mYOffset + pScreenManager_4FF7C8->field_16_ypos)) - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        field_F0_anim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos)
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(mYOffset + pScreenManager_4FF7C8->field_16_ypos))
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        mAnim.Get_Frame_Rect(&frameRect);
        pScreenManager_4FF7C8->InvalidateRect(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_4FF7C8->field_2E_idx);

        field_F0_anim.Get_Frame_Rect(&frameRect);
        pScreenManager_4FF7C8->InvalidateRect(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_4FF7C8->field_2E_idx);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.Get_Frame_Rect(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->field_2E_idx);

            field_220_anim.Get_Frame_Rect(&frameRect);
            pScreenManager_4FF7C8->InvalidateRect(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager_4FF7C8->field_2E_idx);
        }
    }
}

} // namespace AO
