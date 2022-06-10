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
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

    if (sFlintLockFireData_4BAC70[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_24_bFire)
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
    mBaseGameObjectTypeId = ReliveTypes::eFlintLockFire;

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

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
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);

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
    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_E8_tlvInfo = tlvInfo;
    field_E6_switch_id = pTlv->field_1A_switch_id;

    Layer layer = Layer::eLayer_0;
    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        layer = Layer::eLayer_BeforeShadow_Half_6;
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
        layer = Layer::eLayer_BeforeShadow_25;
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = layer;
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
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(activatingRec.mFrameTableOffset, nullptr);
        mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Count() - 1);
        mBaseAnimatedWithPhysicsGameObject_Anim.VDecode();
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
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
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    switch (field_E4_state)
    {
        case States::eDisabled_0:
            if (SwitchStates_Get(field_E6_switch_id))
            {
                field_E4_state = States::eActivating_1;
                const AnimRecord& activatingRec = AO::AnimRec(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(
                    activatingRec.mFrameTableOffset,
                    0);
            }
            break;

        case States::eActivating_1:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (mBaseAnimatedWithPhysicsGameObject_Anim.field_92_current_frame == 6)
                {
                    SFX_Play_Mono(SoundEffect::FlintLock_68, 0, 0);
                    SFX_Play_Mono(SoundEffect::PostFlint_70, 0, 0);
                }
            }

            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
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

                relive_new MusicTrigger(MusicTriggerMusicType::eSecretAreaShort_6, TriggeredBy::eTouching_1, 0, 15);
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
        const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        field_F0_anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.field_14_scale = (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(1.33));
            field_220_anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;
        }

        s16 r = mBaseAnimatedWithPhysicsGameObject_Red;
        s16 g = mBaseAnimatedWithPhysicsGameObject_Green;
        s16 b = mBaseAnimatedWithPhysicsGameObject_Blue;

        const PSX_RECT bRect = VGetBoundingRect();

        if (mApplyShadows & 1)
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos),
                FP_GetExponent(FP_FromInteger((bRect.y + bRect.h) / 2)),
                mBaseAnimatedWithPhysicsGameObject_Scale,
                &r,
                &g,
                &b);
        }


        mBaseAnimatedWithPhysicsGameObject_Anim.mRed = static_cast<u8>(r);
        mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = static_cast<u8>(g);
        mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = static_cast<u8>(b);

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
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(pScreenManager->mCamXOff)) - pScreenManager->mCamPos->field_0_x),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + (FP_FromInteger(pScreenManager->mCamYOff + mBaseAnimatedWithPhysicsGameObject_YOffset - 28)) - pScreenManager->mCamPos->field_4_y),
                ppOt,
                0,
                0);

            field_188_anim.VRender(
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + (FP_FromInteger(pScreenManager->mCamXOff - 3)) - pScreenManager->mCamPos->field_0_x),
                FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + (FP_FromInteger(pScreenManager->mCamYOff + mBaseAnimatedWithPhysicsGameObject_YOffset - 28)) - pScreenManager->mCamPos->field_4_y),
                ppOt,
                0,
                0);
        }

        mBaseAnimatedWithPhysicsGameObject_Anim.VRender(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->field_0_x),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos + (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_YOffset + pScreenManager->mCamYOff)) - pScreenManager->mCamPos->field_4_y),
            ppOt,
            0,
            0);

        field_F0_anim.VRender(
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos
                           + FP_FromInteger(pScreenManager->mCamXOff)
                           - pScreenManager->mCamPos->field_0_x),
            FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos
                           + (FP_FromInteger(mBaseAnimatedWithPhysicsGameObject_YOffset + pScreenManager->mCamYOff))
                           - pScreenManager->mCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&frameRect);
        pScreenManager->InvalidateRect(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager->mIdx);

        field_F0_anim.Get_Frame_Rect(&frameRect);
        pScreenManager->InvalidateRect(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager->mIdx);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.Get_Frame_Rect(&frameRect);
            pScreenManager->InvalidateRect(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager->mIdx);

            field_220_anim.Get_Frame_Rect(&frameRect);
            pScreenManager->InvalidateRect(
                frameRect.x,
                frameRect.y,
                frameRect.w,
                frameRect.h,
                pScreenManager->mIdx);
        }
    }
}

} // namespace AO
