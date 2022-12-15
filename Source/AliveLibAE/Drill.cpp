#include "stdafx.h"
#include "Drill.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Shadow.hpp"
#include "Sfx.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "../relive_lib/Events.hpp"
#include "Spark.hpp"
#include "Blood.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "FixedPoint.hpp"
#include "Math.hpp"

const TintEntry kDrillTints_551548[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 137u, 137u, 137u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};


void Drill::LoadAnimations()
{
    const AnimId drillAnimIds[] =
    {
        AnimId::Drill_Horizontal_Off,
        AnimId::Drill_Horizontal_On,
        AnimId::Drill_Vertical_Off,
        AnimId::Drill_Vertical_On
    };

    for (auto& animId : drillAnimIds)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    }
}

Drill::Drill(relive::Path_Drill* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDrill);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::Drill_Vertical_Off));

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_0);

    SetTint(kDrillTints_551548, gMap.mCurrentLevel);
    relive::Path_Drill tlvData = *pTlv;

    mToggleStartState_StartOn = false;
    mSpeedChange = false;
    mToggle = false;

    if (tlvData.mStartStateOn == relive::reliveChoice::eYes)
    {
        mStartOff = false;
    }
    else
    {
        mStartOff = true;
    }

    mDrillSwitchId = tlvData.mSwitchId;

    if (SwitchStates_Get(mDrillSwitchId) && mStartOff)
    {
        mToggleStartState_StartOn = true;
    }

    if (tlvData.mScale == relive::reliveScale::eFull)
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_24);
        SetScale(Scale::Fg);
    }
    else
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_RopeWebDrillMeatSaw_Half_5);
        SetScale(Scale::Bg);
    }

    mDrillDirection = tlvData.mDrillDirection;
    if (tlvData.mStartPositionBottom == relive::reliveChoice::eYes)
    {
        mStartPosIsBottom = true;
    }
    else
    {
        mStartPosIsBottom = false;
    }

    if (mToggleStartState_StartOn)
    {
        if (mStartPosIsBottom)
        {
            mState = DrillStates::State_2_GoingUp;
        }
        else
        {
            mState = DrillStates::State_1_Going_Down;
        }

        const CameraPos direction = gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos);
        mAudioChannelsMask = SFX_Play_Camera(relive::SoundEffects::DrillMovement, 25, direction);
    }

    switch (mDrillDirection)
    {
        case relive::Path_Drill::DrillDirection::eDown:
            mAdjustedXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
            mXPos = mAdjustedXPos;
            mAdjustedYPos = FP_FromInteger(pTlv->mBottomRightY);

            if (mToggleStartState_StartOn)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Vertical_On));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Vertical_Off));
            }

            mDrillDistance = pTlv->mBottomRightY - pTlv->mTopLeftY;
            if (mStartPosIsBottom)
            {
                mXYOff = FP_FromInteger(0);
            }
            else
            {
                mXYOff = FP_FromInteger(mDrillDistance);
            }
            mYPos = mAdjustedYPos - mXYOff;
            break;

        case relive::Path_Drill::DrillDirection::eRight:
            mAdjustedXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
            mAdjustedYPos = FP_FromInteger(pTlv->mBottomRightY);
            mYPos = mAdjustedYPos;

            if (mToggleStartState_StartOn)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_On));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_Off));
            }

            mDrillDistance = pTlv->mBottomRightX - pTlv->mTopLeftX;
            if (mStartPosIsBottom)
            {
                mXYOff = FP_FromInteger(0);
            }
            else
            {
                mXYOff = FP_FromInteger(mDrillDistance);
            }
            mXPos = mXYOff + mAdjustedXPos;
            break;

        case relive::Path_Drill::DrillDirection::eLeft:
            GetAnimation().SetFlipX(true);

            mAdjustedXPos = FP_FromInteger(pTlv->mBottomRightX - 12);
            mAdjustedYPos = FP_FromInteger(pTlv->mBottomRightY);
            mYPos = mAdjustedYPos;

            if (mToggleStartState_StartOn)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_On));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_Off));
            }

            mDrillDistance = pTlv->mBottomRightX - pTlv->mTopLeftX;
            if (mStartPosIsBottom)
            {
                mXYOff = FP_FromInteger(0);
            }
            else
            {
                mXYOff = FP_FromInteger(mDrillDistance);
            }
            mXPos = mAdjustedXPos - mXYOff;
            break;
    }

    mMaxOffTime = tlvData.mOnMaxPauseTime;
    mMinOffTime = tlvData.mOnMinPauseTime;
    switch (tlvData.mDrillBehavior)
    {
        case relive::Path_Drill::DrillBehavior::eToggle:
            mUseId = true;
            mToggle = false;
            break;

        case relive::Path_Drill::DrillBehavior::eUse:
            mUseId = true;
            mToggle = true;
            break;

        default:
            mUseId = false;
            mToggle = false;
            break;
    }

    mInitialSpeed = FP_FromInteger(tlvData.mOnSpeed);
    if (tlvData.mOnSpeed == 250) // juicy OWI hack to allow 0.2 speed by setting the value 250 in the tlv data
    {
        mInitialSpeed = FP_FromDouble(0.2);
    }

    mCurrentSpeed = mInitialSpeed;
    mOffSpeed = FP_FromInteger(tlvData.mOffSpeed);
    if (tlvData.mOffSpeed == 250) // juicy OWI hack to allow 0.2 speed by setting the value 250 in the tlv data
    {
        mOffSpeed = FP_FromDouble(0.2);
    }

    field_100_min_off_time_speed_change = tlvData.mOffMinPauseTime;
    field_102_max_off_time_speed_change = tlvData.mOffMaxPauseTime;
    mOffTimer = 0;
    mState = DrillStates::State_0_Restart_Cycle;
    mTlvInfo = tlvId;
    mAudioChannelsMask = 0;

    CreateShadow();
}

s32 Drill::CreateFromSaveState(const u8* pData)
{
    const DrillSaveState* pState = reinterpret_cast<const DrillSaveState*>(pData);
    auto pTlv = static_cast<relive::Path_Drill*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mDrillTlvId));
    auto pDrill = relive_new Drill(pTlv, pState->mDrillTlvId);

    if (pState->mState != DrillStates::State_0_Restart_Cycle)
    {
        switch (pDrill->mDrillDirection)
        {
            case relive::Path_Drill::DrillDirection::eDown:
            {
                pDrill->GetAnimation().Set_Animation_Data(pDrill->GetAnimRes(AnimId::Drill_Vertical_On));
                break;
            }
            case relive::Path_Drill::DrillDirection::eRight:
            case relive::Path_Drill::DrillDirection::eLeft:
            {
                pDrill->GetAnimation().Set_Animation_Data(pDrill->GetAnimRes(AnimId::Drill_Horizontal_On));
                break;
            }
        }
    }

    pDrill->mOffTimer = pState->mOffTimer;
    pDrill->mState = pState->mState;
    pDrill->mXYOff = FP_FromInteger(pState->mXYOff);
    return sizeof(DrillSaveState);
}

void Drill::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    const CameraPos soundDirection = gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos);

    switch (mState)
    {
        case DrillStates::State_0_Restart_Cycle:
            if (Expired(mOffTimer) || mToggle)
            {
                if (!mUseId || (!!SwitchStates_Get(mDrillSwitchId) == mStartOff))
                {
                    mState = DrillStates::State_1_Going_Down;

                    switch (mDrillDirection)
                    {
                        case relive::Path_Drill::DrillDirection::eDown:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Vertical_On));
                            break;
                        }
                        case relive::Path_Drill::DrillDirection::eRight:
                        case relive::Path_Drill::DrillDirection::eLeft:
                        {
                            GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_On));
                            break;
                        }
                    }

                    mSpeedChange = false;
                    mCurrentSpeed = mInitialSpeed;
                    mAudioChannelsMask = SFX_Play_Camera(relive::SoundEffects::DrillMovement, 25, soundDirection);
                    return;
                }
            }

            if (mUseId && !mToggle && FP_GetExponent(mOffSpeed) > 0 && Expired(mOffTimer))
            {
                mState = DrillStates::State_1_Going_Down;

                switch (mDrillDirection)
                {
                    case relive::Path_Drill::DrillDirection::eDown:
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Vertical_On));
                        break;
                    }

                    case relive::Path_Drill::DrillDirection::eRight:
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_On));
                        break;
                    }

                    case relive::Path_Drill::DrillDirection::eLeft:
                    {
                        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_On));
                        GetAnimation().SetFlipX(true);
                        break;
                    }
                }

                mSpeedChange = true;
                mCurrentSpeed = mOffSpeed;
                mAudioChannelsMask = SFX_Play_Camera(relive::SoundEffects::DrillMovement, 25, soundDirection);
            }
            break;

        case DrillStates::State_1_Going_Down:
            if (!mAudioChannelsMask)
            {
                mAudioChannelsMask = SFX_Play_Camera(relive::SoundEffects::DrillMovement, 25, soundDirection);
            }

            DamageTouchingObjects();

            mXYOff -= mCurrentSpeed;
            if (mXYOff <= FP_FromInteger(0))
            {
                mState = DrillStates::State_2_GoingUp;
                SFX_Play_Camera(relive::SoundEffects::DrillCollision, 50, soundDirection, FP_FromInteger(1));
            }
            EmitSparks();
            break;

        case DrillStates::State_2_GoingUp:
            if (!mAudioChannelsMask)
            {
                mAudioChannelsMask = SFX_Play_Camera(relive::SoundEffects::DrillMovement, 25, soundDirection);
            }

            DamageTouchingObjects();

            mXYOff = mCurrentSpeed + mXYOff;
            if (mXYOff >= FP_FromInteger(mDrillDistance))
            {
                if (mAudioChannelsMask)
                {
                    SND_Stop_Channels_Mask(mAudioChannelsMask);
                    mAudioChannelsMask = 0;
                }

                mState = DrillStates::State_0_Restart_Cycle;
                SFX_Play_Camera(relive::SoundEffects::DrillCollision, 50, soundDirection);

                s16 max_off = 0;
                s16 min_off = 0;
                if (mSpeedChange)
                {
                    max_off = field_102_max_off_time_speed_change;
                    min_off = field_100_min_off_time_speed_change;
                }
                else
                {
                    max_off = mMaxOffTime;
                    min_off = mMinOffTime;
                }

                mOffTimer = MakeTimer(Math_RandomRange(min_off, max_off));

                if (mDrillDirection == relive::Path_Drill::DrillDirection::eDown)
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Vertical_Off));
                }
                else
                {
                    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Drill_Horizontal_Off));
                }

                if (mToggle)
                {
                    SwitchStates_Set(mDrillSwitchId, !mStartOff);
                }
            }

            EmitSparks();
            break;
    }
}

Drill::~Drill()
{
    if (mAudioChannelsMask)
    {
        SND_Stop_Channels_Mask(mAudioChannelsMask);
        mAudioChannelsMask = 0;
    }

    if (mUseId && !!SwitchStates_Get(mDrillSwitchId) != mStartOff)
    {
        Path::TLV_Reset(mTlvInfo, 1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(mTlvInfo, 0, 0, 0);
    }
}

void Drill::VScreenChanged()
{
    if (mState != DrillStates::State_0_Restart_Cycle)
    {
        if (mStartPosIsBottom)
        {
            mXYOff = FP_FromInteger(0);
        }
        else
        {
            mXYOff = FP_FromInteger(mDrillDistance);
        }
    }

    // Stop that sound
    if (mAudioChannelsMask)
    {
        SND_Stop_Channels_Mask(mAudioChannelsMask);
        mAudioChannelsMask = 0;
    }

    // Seems the real function does this, but then always kill object at the end!
    // So it appears drills where supposed to survive for up to 1 screen away.

    /*
    // Map changed
    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId || gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }

    // More than 1 screen away on X?
    if (FP_Abs(sControlledCharacter->mXPos - mXPos) > FP_FromInteger(375))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }

    // More than 1 screen away on Y?
    if (FP_Abs(sControlledCharacter->mYPos - mYPos) > FP_FromInteger(260))
    {
        field_6_flags.Set(BaseGameObject::eDead);
        return;
    }
    */

    SetDead(true);
}

void Drill::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        if (mDrillDirection == relive::Path_Drill::DrillDirection::eDown)
        {
            mYPos = mAdjustedYPos - mXYOff;
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
        else if (mDrillDirection == relive::Path_Drill::DrillDirection::eRight)
        {
            mXPos = mAdjustedXPos + mXYOff;
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
        else if (mDrillDirection == relive::Path_Drill::DrillDirection::eLeft)
        {
            mXPos = mAdjustedXPos - mXYOff;
            BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
        }
    }
}

void Drill::VStopAudio()
{
    if (mAudioChannelsMask)
    {
        SND_Stop_Channels_Mask(mAudioChannelsMask);
        mAudioChannelsMask = 0;
    }
}

s32 Drill::VGetSaveState(u8* pSaveBuffer)
{
    DrillSaveState* pState = reinterpret_cast<DrillSaveState*>(pSaveBuffer);
    pState->mType = ReliveTypes::eDrill;
    pState->mDrillTlvId = mTlvInfo;
    pState->mOffTimer = mOffTimer;
    pState->mState = mState;
    pState->mXYOff = FP_GetExponent(mXYOff);
    return sizeof(DrillSaveState);
}

void Drill::EmitSparks()
{
    if (gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
    {
        s32 speed = 0;
        if (mState == DrillStates::State_1_Going_Down)
        {
            speed = -FP_GetExponent(mCurrentSpeed - FP_FromInteger(2));
        }
        else if (mState == DrillStates::State_2_GoingUp)
        {
            speed = FP_GetExponent(mCurrentSpeed);
        }

        // 1 in 6 chance of sparks
        if (Math_RandomRange(0, 6) == 0)
        {
            if (mDrillDirection == relive::Path_Drill::DrillDirection::eRight)
            {
                relive_new Spark(mXPos - (GetSpriteScale() * FP_FromInteger(17)) + FP_FromInteger(speed),
                                             mYPos - (GetSpriteScale() * FP_FromInteger(12)),
                                             GetSpriteScale(),
                                             6u,
                                             50,
                                             205,
                                             SparkType::eSmallChantParticle_0);

                relive_new Spark(mXPos + (GetSpriteScale() * FP_FromInteger(17)) + FP_FromInteger(speed),
                                             mYPos - (GetSpriteScale() * FP_FromInteger(12)),
                                             GetSpriteScale(),
                                             6u,
                                             50,
                                             205,
                                             SparkType::eSmallChantParticle_0);
            }
            else if (mDrillDirection == relive::Path_Drill::DrillDirection::eLeft)
            {
                relive_new Spark(mXPos + (GetSpriteScale() * FP_FromInteger(17)) - FP_FromInteger(speed),
                                             mYPos - (GetSpriteScale() * FP_FromInteger(12)),
                                             GetSpriteScale(),
                                             6u,
                                             50,
                                             205,
                                             SparkType::eSmallChantParticle_0);

                relive_new Spark(mXPos - (GetSpriteScale() * FP_FromInteger(17)) - FP_FromInteger(speed),
                                             mYPos - (GetSpriteScale() * FP_FromInteger(12)),
                                             GetSpriteScale(),
                                             6u,
                                             50,
                                             205,
                                             SparkType::eSmallChantParticle_0);
            }
            else if (mDrillDirection == relive::Path_Drill::DrillDirection::eDown)
            {
                relive_new Spark(mXPos,
                                             mYPos - (GetSpriteScale() * FP_FromInteger(22)) - FP_FromInteger(speed),
                                             GetSpriteScale(),
                                             6u,
                                             50,
                                             205,
                                             SparkType::eSmallChantParticle_0);

                relive_new Spark(mXPos,
                                             mYPos + (GetSpriteScale() * FP_FromInteger(4)) - FP_FromInteger(speed),
                                             GetSpriteScale(),
                                             6u,
                                             50,
                                             205,
                                             SparkType::eSmallChantParticle_0);
            }
        }
    }
}

s16 Drill::DamageTouchingObjects()
{
    PSX_RECT drillRect = VGetBoundingRect();

    if (mDrillDirection == relive::Path_Drill::DrillDirection::eDown)
    {
        drillRect.y += 16;
    }

    IBaseAliveGameObject* pFound = nullptr;
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            return 0;
        }

        if (pObj->GetIsBaseAliveGameObject() || pObj->Type() == ReliveTypes::eRockSpawner)
        {
            if (pObj->GetDrawable())
            {
                if (pObj->Type() != ReliveTypes::eMeat && pObj->Type() != ReliveTypes::eEvilFart && (pObj->Type() != ReliveTypes::eAbe || pObj->mCurrentMotion != eAbeMotions::Motion_68_ToOffScreenHoist_454B80))
                {
                    const PSX_RECT objRect = pObj->VGetBoundingRect();

                    if (RectsOverlap(drillRect, objRect) && pObj->GetScale() == GetScale() && pObj->mHealth > FP_FromInteger(0))
                    {
                        if (pObj->mXPos + FP_FromInteger(3) >= FP_FromInteger(drillRect.x) && pObj->mXPos - FP_FromInteger(3) <= FP_FromInteger(drillRect.w))
                        {
                            pFound = pObj;
                            break;
                        }
                    }
                }
            }
        }

        pObj = nullptr;
    }

    if (!pFound)
    {
        return 0;
    }

    if (!pFound->VTakeDamage(this))
    {
        return 1;
    }

    relive_new Blood(pFound->mXPos,
                                FP_FromInteger(drillRect.h - 10),
                                FP_FromInteger(-5),
                                FP_FromInteger(5),
                                GetSpriteScale(),
                                50);


    relive_new Blood(pFound->mXPos,
                                 FP_FromInteger(drillRect.h - 10),
                                 FP_FromInteger(0),
                                 FP_FromInteger(5),
                                 GetSpriteScale(),
                                 50);

    relive_new Blood(pFound->mXPos,
                                 FP_FromInteger(drillRect.h - 10),
                                 FP_FromInteger(5),
                                 FP_FromInteger(5),
                                 GetSpriteScale(),
                                 50);

    SFX_Play_Pitch(relive::SoundEffects::DrillCollision, 127, -500);
    SfxPlayMono(relive::SoundEffects::KillEffect, 127);
    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 127, -700);

    return 1;
}
