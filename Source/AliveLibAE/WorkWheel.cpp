#include "stdafx.h"
#include "WorkWheel.hpp"
#include "Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "GameEnderController.hpp"
#include "Map.hpp"
#include "Path.hpp"

void WorkWheel::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Work_Wheel_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Work_Wheel_Turning));
}

WorkWheel::WorkWheel(relive::Path_WorkWheel* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eWheel);

    LoadAnimations();

    mBaseGameObjectTlvInfo = tlvId;

    Animation_Init(GetAnimRes(AnimId::Work_Wheel_Idle));

    GetAnimation().mFlags.Set(eSemiTrans);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale != relive::reliveScale::eFull)
    {
        if (pTlv->mScale == relive::reliveScale::eHalf)
        {
            SetSpriteScale(FP_FromDouble(0.5));
            GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
            SetScale(Scale::Bg);
        }
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        SetScale(Scale::Fg);
    }

    mSwitchId = pTlv->mSwitchId;
    mActivationTime = pTlv->mActivationTime;
    mTurnOffTime = pTlv->mTurnOffTime;
    mTurningTime = 0;
    mTurnOffWhenStopped = pTlv->mTurnOffWhenStopped;

    PathLine* pathLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &pathLine,
            &hitX,
            &hitY,
            (GetScale() == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        mYPos = hitY;
    }
    else
    {
        mYPos += FP_FromInteger(20) * GetSpriteScale();
    }


    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mState = WheelStates::eIdle_0;
    mTlvInfo = tlvId;
}

WorkWheel::~WorkWheel()
{
    Path::TLV_Reset(mTlvInfo, -1, 0, 0);
}

s32 WorkWheel::CreateFromSaveState(const u8* pState)
{
    const WorkWheelSaveState* pData = reinterpret_cast<const WorkWheelSaveState*>(pState);

    relive::Path_WorkWheel* pTlv = static_cast<relive::Path_WorkWheel*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pData->field_4_tlvInfo));

    auto pWheel = relive_new WorkWheel(pTlv, pData->field_4_tlvInfo);
    if (pWheel)
    {
        if (pData->field_C_state == WheelStates::eTurning_1)
        {
            pWheel->VStartTurning();
        }

        pWheel->mTurningTime = pData->field_8_snd_counter;
    }
    return sizeof(WorkWheelSaveState);
}

s32 WorkWheel::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<WorkWheelSaveState*>(pSaveBuffer);

    pState->mType = AETypes::eWheel_148;
    pState->field_4_tlvInfo = mTlvInfo;
    pState->field_8_snd_counter = mTurningTime;
    pState->field_C_state = mState;
    return sizeof(WorkWheelSaveState);
}

void WorkWheel::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mState == WheelStates::eTurning_1)
    {
        ++mTurningTime;

        if (!(mTurningTime % 10)
            && gMap.Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            const s16 randomVol = Math_RandomRange(-30, 0);
            SND_SEQ_Play(SeqId::WheelSqueak_19, 1, randomVol + 127, randomVol + 127);
        }
    }
    else if (mState == WheelStates::eIdle_0)
    {
        mTurningTime = 0;
    }

    if (mSwitchId)
    {
        if (mTurningTime > mActivationTime)
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender && mSwitchId == 100)
            {
                CreateGameEnderController_43B7A0();
            }

            if (mTurnOffTime > 0 && mTurningTime > mTurnOffTime)
            {
                SwitchStates_Set(mSwitchId, 0);
            }
            else
            {
                SwitchStates_Set(mSwitchId, 1);
            }
        }
    }
}

void WorkWheel::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || mState == WheelStates::eIdle_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void WorkWheel::VStartTurning()
{
    if (mState == WheelStates::eIdle_0)
    {
        mState = WheelStates::eTurning_1;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Work_Wheel_Turning));
    }
}

void WorkWheel::VStopTurning(s16 bResetSwitch)
{
    if (mState == WheelStates::eTurning_1)
    {
        mState = WheelStates::eIdle_0;

        // Spin it.
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Work_Wheel_Idle));

        if (mTurnOffWhenStopped == relive::reliveChoice::eYes)
        {
            if (bResetSwitch)
            {
                SwitchStates_Set(mSwitchId, 0);
            }
        }
    }
}
