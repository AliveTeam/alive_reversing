#include "stdafx_ao.h"
#include "Function.hpp"
#include "MeatSaw.hpp"
#include "ResourceManager.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "Blood.hpp"
#include "../relive_lib/ScreenManager.hpp"

namespace AO {

MeatSaw::~MeatSaw()
{
    if (field_1A8_flags.Get(flags_1A8::eBit1_ResetOffscreen) && SwitchStates_Get(field_EE_switch_id) != field_F2_switch_value)
    {
        Path::TLV_Reset(field_100_tlvInfo, 1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_100_tlvInfo, 0, 0, 0);
    }

    field_110_anim.VCleanUp();
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
}

MeatSaw::MeatSaw(Path_MeatSaw* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eMeatSaw;

    const AnimRecord rec = AO::AnimRec(AnimId::MeatSaw_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init(AnimId::MeatSaw_Idle, ppRes);
    
    mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
    mAnim.mRenderMode = TPageAbr::eBlend_0;

    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_Half_5;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_RopeWebDrillMeatSaw_24;
        mScale = Scale::Fg;
    }

    mXPos = FP_FromInteger(pTlv->mTopLeft.x + 8);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    field_F6_switch_min_time_off = pTlv->mSwitchMinTimeOff;
    field_F8_switch_max_time_off = pTlv->mSwitchMaxTimeOff;

    field_E6_max_rise_time = pTlv->mMaxRiseTime;
    field_EE_switch_id = pTlv->mSwitchId;

    mYOffset = 0;
    field_F4 = 0;

    if (pTlv->mType == Path_MeatSaw::Type::eAutomatic_1)
    {
        field_1A8_flags.Set(flags_1A8::eBit1_ResetOffscreen);
        field_1A8_flags.Clear(flags_1A8::eBit2_SwitchIdMeatSaw);
    }
    else if (pTlv->mType == Path_MeatSaw::Type::eSwitchId_2)
    {
        field_1A8_flags.Set(flags_1A8::eBit1_ResetOffscreen);
        field_1A8_flags.Set(flags_1A8::eBit2_SwitchIdMeatSaw);
    }
    else // eAutomaticPersistOffscreen_0
    {
        field_1A8_flags.Clear(flags_1A8::eBit1_ResetOffscreen);
        field_1A8_flags.Clear(flags_1A8::eBit2_SwitchIdMeatSaw);
    }

    field_EA_speed1 = pTlv->mSpeed;
    field_E8_speed2 = pTlv->mSpeed;
    if (pTlv->mStartState == Path_MeatSaw::StartState::eOff_0)
    {
        field_F0_switch_value = SwitchStates_Get(pTlv->mSwitchId) == 0;
    }
    else
    {
        if (pTlv->mSwitchId)
        {
            field_F0_switch_value = SwitchStates_Get(pTlv->mSwitchId);
        }
        else
        {
            field_F0_switch_value = 0;
        }
    }

    if (pTlv->mSwitchId)
    {
        field_F2_switch_value = SwitchStates_Get(pTlv->mSwitchId);
    }
    else
    {
        field_F2_switch_value = 0;
    }

    field_FA_automatic_min_time_off = pTlv->mAutomaticMinTimeOff;

    field_EC_off_speed = pTlv->mOffSpeed;
    field_FC_automatic_max_time_off = pTlv->mAutomaticMaxTimeOff;

    if (pTlv->field_1_unknown)
    {
        field_F0_switch_value = field_F0_switch_value == 0;
        field_F2_switch_value = field_F2_switch_value == 0;
    }

    field_104_idle_timer = 0;
    field_E4_state = MeatSawStates::eIdle_0;
    field_10C_FrameCount = 0;
    mYPos -= FP_FromInteger(pTlv->mMaxRiseTime);
    field_100_tlvInfo = tlvInfo;

    if (pTlv->mInitialPosition != 0)
    {
        field_E4_state = MeatSawStates::eGoingUp_2;
        field_F4 = pTlv->mMaxRiseTime + pTlv->mSpeed - pTlv->mMaxRiseTime % pTlv->mSpeed;
    }

    const AnimRecord& motorRec = AO::AnimRec(AnimId::MeatSawMotor);
    u8** ppRes2 = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, motorRec.mResourceId, 1, 0);
    if (field_110_anim.Init(
            AnimId::MeatSawMotor,
            this,
            ppRes2))
    {
        field_110_anim.mRenderLayer = mAnim.mRenderLayer;
        field_110_anim.field_14_scale = mSpriteScale;

        field_110_anim.mRed = static_cast<u8>(mRGB.r);
        field_110_anim.mGreen = static_cast<u8>(mRGB.g);
        field_110_anim.mBlue = static_cast<u8>(mRGB.b);

        field_110_anim.mRenderMode = TPageAbr::eBlend_0;

        field_110_anim.mFlags.Clear(AnimFlags::eBit16_bBlending);
        field_110_anim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);

        mShadow = relive_new Shadow();
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbeblowAOResID, 1, 0);
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void MeatSaw::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || !sControlledCharacter || // Can be nullptr during the game ender
        FP_Abs(sControlledCharacter->mXPos - mXPos) > FP_FromInteger(1024))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void MeatSaw::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    GrindUpObjects_439CD0();

    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    if (!(field_10C_FrameCount % 87))
    {
        SFX_Play_Camera(SoundEffect::MeatsawOffscreen_88, 45, direction);
    }

    if (!(field_10C_FrameCount % 25))
    {
        SFX_Play_Camera(SoundEffect::MeatsawIdle_89, 45, direction);
    }

    field_10C_FrameCount++;

    switch (field_E4_state)
    {
        case MeatSawStates::eIdle_0:
            if ((field_104_idle_timer <= static_cast<s32>(sGnFrame) || (field_1A8_flags.Get(flags_1A8::eBit2_SwitchIdMeatSaw))) &&
                (!field_1A8_flags.Get(flags_1A8::eBit1_ResetOffscreen) || SwitchStates_Get(field_EE_switch_id) == field_F0_switch_value))
            {
                field_E4_state = MeatSawStates::eGoingDown_1;
                mAnim.Set_Animation_Data(AnimId::MeatSaw_Moving, nullptr);
                field_1A8_flags.Clear(flags_1A8::eBit3_AutomaticMeatSawIsDown);
                field_E8_speed2 = field_EA_speed1;
                field_108_SFX_timer = sGnFrame + 2;
            }
            else
            {
                if (field_1A8_flags.Get(flags_1A8::eBit1_ResetOffscreen))
                {
                    if (!field_1A8_flags.Get(flags_1A8::eBit2_SwitchIdMeatSaw))
                    {
                        if (field_EC_off_speed)
                        {
                            if (field_104_idle_timer <= static_cast<s32>(sGnFrame))
                            {
                                field_E4_state = MeatSawStates::eGoingDown_1;
                                mAnim.Set_Animation_Data(AnimId::MeatSaw_Moving, nullptr);
                                field_1A8_flags.Set(flags_1A8::eBit3_AutomaticMeatSawIsDown);
                                field_E8_speed2 = field_EC_off_speed;
                                field_108_SFX_timer = sGnFrame + 2;
                            }
                        }
                    }
                }
            }
            break;

        case MeatSawStates::eGoingDown_1:
            field_F4 += field_E8_speed2;

            if (!((sGnFrame - field_108_SFX_timer) % 8))
            {
                SFX_Play_Camera(SoundEffect::MeatsawDown_91, 50, direction);
            }

            if (field_F4 >= field_E6_max_rise_time)
            {
                field_E4_state = MeatSawStates::eGoingUp_2;
                field_108_SFX_timer = sGnFrame + 2;
            }
            break;

        case MeatSawStates::eGoingUp_2:
            if (!((sGnFrame - field_108_SFX_timer) % 10))
            {
                field_108_SFX_timer = sGnFrame;
                SFX_Play_Camera(SoundEffect::MeatsawUp_90, 50, direction);
            }

            field_F4 -= field_E8_speed2;

            if (field_F4 <= 0)
            {
                field_E4_state = MeatSawStates::eIdle_0;
                s16 minRnd = 0;
                s16 maxRnd = 0;
                if (field_1A8_flags.Get(flags_1A8::eBit3_AutomaticMeatSawIsDown))
                {
                    maxRnd = field_FC_automatic_max_time_off;
                    minRnd = field_FA_automatic_min_time_off;
                }
                else
                {
                    maxRnd = field_F8_switch_max_time_off;
                    minRnd = field_F6_switch_min_time_off;
                }

                field_104_idle_timer = sGnFrame + Math_RandomRange(minRnd, maxRnd);
                mAnim.Set_Animation_Data(AnimId::MeatSaw_Idle, nullptr);
                if (field_1A8_flags.Get(flags_1A8::eBit2_SwitchIdMeatSaw))
                {
                    SwitchStates_Set(field_EE_switch_id, field_F0_switch_value == 0 ? 1 : 0);
                }
            }
            break;
    }
}

void MeatSaw::GrindUpObjects_439CD0()
{
    PSX_RECT ourRect = VGetBoundingRect();

    ourRect.y += mYOffset;
    ourRect.h += mYOffset;

    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
            {
                // Can't grind meat with a meat saw, that would be grindception
                if (pObjIter->mBaseGameObjectTypeId != ReliveTypes::eMeat)
                {
                    const PSX_RECT objRect = pObjIter->VGetBoundingRect();

                    if (RectsOverlap(ourRect, objRect) && pObjIter->mSpriteScale == mSpriteScale && pObjIter->mHealth > FP_FromInteger(0))
                    {
                        if (pObjIter->mXPos >= FP_FromInteger(ourRect.x) && pObjIter->mXPos <= FP_FromInteger(ourRect.w))
                        {
                            if (!pObjIter->VTakeDamage(this))
                            {
                                return;
                            }

                            relive_new Blood(
                                pObjIter->mXPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(-5),
                                FP_FromInteger(5),
                                mSpriteScale,
                                50);

                            relive_new Blood(
                                pObjIter->mXPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(0),
                                FP_FromInteger(5),
                                mSpriteScale,
                                50);

                            relive_new Blood(
                                pObjIter->mXPos,
                                FP_FromInteger(ourRect.h - 10),
                                FP_FromInteger(5),
                                FP_FromInteger(5),
                                mSpriteScale,
                                50);

                            SfxPlayMono(SoundEffect::KillEffect_78, 127, 0);
                            SFX_Play_Pitch(SoundEffect::KillEffect_78, 127, -700, 0);
                            return;
                        }
                    }
                }
            }
        }
    }
}

void MeatSaw::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mYOffset = field_F4;
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

        field_110_anim.VRender(
            FP_GetExponent(mXPos
                           + FP_FromInteger(pScreenManager->mCamXOff)
                           - pScreenManager->mCamPos->x),
            FP_GetExponent(mYPos
                           + (FP_FromInteger(pScreenManager->mCamYOff + field_E6_max_rise_time))
                           - pScreenManager->mCamPos->y),
            ppOt,
            0,
            0);

        PSX_RECT rect = {};
        field_110_anim.Get_Frame_Rect(&rect);
        pScreenManager->InvalidateRectCurrentIdx(
            rect.x,
            rect.y,
            rect.w,
            rect.h);
    }
}

} // namespace AO
