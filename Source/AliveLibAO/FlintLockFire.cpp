#include "stdafx_ao.h"
#include "Function.hpp"
#include "FlintLockFire.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "MusicTrigger.hpp"
#include "Sfx.hpp"
#include "Midi.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/ShadowZone.hpp"

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


void FlintLockFire::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintLock_Gourd));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintLock_Hammers_Disabled));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::FlintLock_Hammers_Activating));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fire));
}

void FlintLockFire::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void FlintLockFire::VStopAudio()
{
    if (field_EC_fire_sound)
    {
        SND_Stop_Channels_Mask(field_EC_fire_sound);
        field_EC_fire_sound = 0;
    }
}

FlintLockFire::~FlintLockFire()
{
    Path::TLV_Reset(field_E8_tlvInfo, -1, 0, 0);
    field_F0_anim.VCleanUp();

    if (sFlintLockFireData_4BAC70[static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel))].field_24_bFire)
    {
        field_188_anim.VCleanUp();
        field_220_anim.VCleanUp();
        if (field_EC_fire_sound)
        {
            SND_Stop_Channels_Mask(field_EC_fire_sound);
        }
    }
}

FlintLockFire::FlintLockFire(relive::Path_FlintLockFire* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eFlintLockFire);

    LoadAnimations();

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    Animation_Init(GetAnimRes(sFlintLockFireData_4BAC70[cur_lvl].field_14_hammers_disabled_anim_id));
    GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);

    field_F0_anim.Init(
        GetAnimRes(sFlintLockFireData_4BAC70[cur_lvl].field_4_gourd_anim_id),
        this);

    field_F0_anim.SetRenderMode(TPageAbr::eBlend_0);
    field_F0_anim.mFlags.Clear(AnimFlags::eAnimate);
    field_F0_anim.mFlags.Set(AnimFlags::eSemiTrans);

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        field_188_anim.Init(GetAnimRes(AnimId::Fire), this);
        field_188_anim.SetRenderMode(TPageAbr::eBlend_0);
        field_188_anim.mFlags.Clear(AnimFlags::eAnimate);
        field_188_anim.mFlags.Clear(AnimFlags::eRender);
        field_188_anim.mFlags.Set(AnimFlags::eSemiTrans);

        field_220_anim.Init(GetAnimRes(AnimId::Fire), this);
        field_220_anim.SetRenderMode(TPageAbr::eBlend_0);
        field_220_anim.mFlags.Clear(AnimFlags::eAnimate);
        field_220_anim.mFlags.Clear(AnimFlags::eRender);
        field_220_anim.mFlags.Set(AnimFlags::eSemiTrans);
        field_220_anim.mFlags.Set(AnimFlags::eFlipX);
        field_220_anim.SetFrame(3u);
    }

    field_E4_state = States::eDisabled_0;
    field_EC_fire_sound = 0;
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    field_E8_tlvInfo = tlvId;
    field_E6_switch_id = pTlv->mSwitchId;

    Layer layer = Layer::eLayer_0;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        layer = Layer::eLayer_BeforeShadow_Half_6;
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        layer = Layer::eLayer_BeforeShadow_25;
        SetScale(Scale::Fg);
    }

    GetAnimation().SetRenderLayer(layer);
    field_F0_anim.SetRenderLayer(layer);

    if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
    {
        field_188_anim.SetRenderLayer(layer);
        field_220_anim.SetRenderLayer(layer);
    }

    if (SwitchStates_Get(pTlv->mSwitchId))
    {
        field_E4_state = States::eActivated_2;
        GetAnimation().Set_Animation_Data(GetAnimRes(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id));
        GetAnimation().SetFrame(GetAnimation().Get_Frame_Count() - 1);
        GetAnimation().VDecode();
        GetAnimation().mFlags.Set(AnimFlags::eSemiTrans);
        field_F0_anim.mFlags.Set(AnimFlags::eAnimate);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.mFlags.Set(AnimFlags::eAnimate);
            field_188_anim.mFlags.Set(AnimFlags::eRender);

            field_220_anim.mFlags.Set(AnimFlags::eAnimate);
            field_220_anim.mFlags.Set(AnimFlags::eRender);

            field_EC_fire_sound = SfxPlayMono(relive::SoundEffects::Fire, 0);
        }
    }
}

void FlintLockFire::VUpdate()
{
    if (EventGet(kEventDeathReset))
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
                GetAnimation().Set_Animation_Data(GetAnimRes(sFlintLockFireData_4BAC70[cur_lvl].field_18_hammers_activating_anim_id));
            }
            break;

        case States::eActivating_1:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (GetAnimation().GetCurrentFrame() == 6)
                {
                    SfxPlayMono(relive::SoundEffects::FlintLock, 0);
                    SfxPlayMono(relive::SoundEffects::PostFlint, 0);
                }
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eForwardLoopCompleted))
            {
                field_E4_state = States::eActivated_2;

                field_F0_anim.mFlags.Set(AnimFlags::eAnimate);
                if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
                {
                    field_188_anim.mFlags.Set(AnimFlags::eAnimate);
                    field_188_anim.mFlags.Set(AnimFlags::eRender);

                    field_220_anim.mFlags.Set(AnimFlags::eAnimate);
                    field_220_anim.mFlags.Set(AnimFlags::eRender);

                    field_EC_fire_sound = SfxPlayMono(relive::SoundEffects::Fire, 0);
                }

                relive_new MusicTrigger(relive::Path_MusicTrigger::MusicTriggerMusicType::eSecretAreaShort, relive::Path_MusicTrigger::TriggeredBy::eTouching, 0, 15);
            }
            break;

        case States::eActivated_2:
            if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
            {
                if (!field_EC_fire_sound)
                {
                    field_EC_fire_sound = SfxPlayMono(relive::SoundEffects::Fire, 0);
                }
            }
            break;

        default:
            break;
    }
}

void FlintLockFire::VRender(PrimHeader** ppOt)
{
    if (Is_In_Current_Camera() == CameraPos::eCamCurrent_0)
    {
        const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
        GetAnimation().SetSpriteScale(GetSpriteScale());
        field_F0_anim.SetSpriteScale(GetSpriteScale());

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.SetSpriteScale((GetSpriteScale() * FP_FromDouble(1.33)));
            field_220_anim.SetSpriteScale(GetSpriteScale());
        }

        s16 r = mRGB.r;
        s16 g = mRGB.g;
        s16 b = mRGB.b;

        const PSX_RECT bRect = VGetBoundingRect();

        if (mVisualFlags.Get(VisualFlags::eApplyShadowZoneColour))
        {
            ShadowZone::ShadowZones_Calculate_Colour(
                FP_GetExponent(mXPos),
                FP_GetExponent(FP_FromInteger((bRect.y + bRect.h) / 2)),
                GetScale(),
                &r,
                &g,
                &b);
        }


        GetAnimation().SetRGB(r, g, b);
        field_F0_anim.SetRGB(r, g, b);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.SetRGB(r, g, b);
            field_220_anim.SetRGB(r, g, b);

            field_220_anim.VRender(
                FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager->mCamXOff)) - pScreenManager->mCamPos->x),
                FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager->mCamYOff + mYOffset - 28)) - pScreenManager->mCamPos->y),
                ppOt,
                0,
                0);

            field_188_anim.VRender(
                FP_GetExponent(mXPos + (FP_FromInteger(pScreenManager->mCamXOff - 3)) - pScreenManager->mCamPos->x),
                FP_GetExponent(mYPos + (FP_FromInteger(pScreenManager->mCamYOff + mYOffset - 28)) - pScreenManager->mCamPos->y),
                ppOt,
                0,
                0);
        }

        GetAnimation().VRender(
            FP_GetExponent(mXPos + FP_FromInteger(pScreenManager->mCamXOff) - pScreenManager->mCamPos->x),
            FP_GetExponent(mYPos + (FP_FromInteger(mYOffset + pScreenManager->mCamYOff)) - pScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        field_F0_anim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(pScreenManager->mCamXOff)
                           - pScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(mYOffset + pScreenManager->mCamYOff))
                           - pScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        GetAnimation().Get_Frame_Rect(&frameRect);

        field_F0_anim.Get_Frame_Rect(&frameRect);

        if (sFlintLockFireData_4BAC70[cur_lvl].field_24_bFire)
        {
            field_188_anim.Get_Frame_Rect(&frameRect);


            field_220_anim.Get_Frame_Rect(&frameRect);

        }
    }
}

} // namespace AO
