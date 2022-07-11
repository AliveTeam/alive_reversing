#include "stdafx.h"
#include "WorkWheel.hpp"
#include "Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "PathData.hpp"
#include "GameEnderController.hpp"

WorkWheel::WorkWheel(Path_WorkWheel* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eWheel);

    mBaseGameObjectTlvInfo = tlvInfo;

    const AnimRecord& rec = AnimRec(AnimId::Work_Wheel_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);

    mAnim.mFlags.Set(eBit15_bSemiTrans);

    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeft.y);

    if (pTlv->field_10_scale != Scale_short::eFull_0)
    {
        if (pTlv->field_10_scale == Scale_short::eHalf_1)
        {
            mSpriteScale = FP_FromDouble(0.5);
            mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
            mScale = Scale::Bg;
        }
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = Scale::Fg;
    }

    field_F8_switch_id = pTlv->field_12_switch_id;
    field_FE_activation_time = pTlv->field_14_activation_time;
    field_102_off_time = pTlv->field_16_off_time;
    field_100_on_counter = 0;
    field_104_turn_off_when_stopped = pTlv->field_18_turn_off_when_stopped;

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
            (mScale == Scale::Fg) ? kFgFloorCeilingOrWalls : kBgFloorCeilingOrWalls))
    {
        mYPos = hitY;
    }
    else
    {
        mYPos += FP_FromInteger(20) * mSpriteScale;
    }


    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    field_FC_state = WheelStates::eIdle_0;
    field_F4_tlv_info = tlvInfo;
}

WorkWheel::~WorkWheel()
{
    Path::TLV_Reset(field_F4_tlv_info, -1, 0, 0);
}

s32 WorkWheel::CreateFromSaveState(const u8* pState)
{
    const WorkWheel_SaveState* pData = reinterpret_cast<const WorkWheel_SaveState*>(pState);

    Path_WorkWheel* pTlv = static_cast<Path_WorkWheel*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pData->field_4_tlvInfo));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kAbeworkResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("ABEWORK.BAN", nullptr);
    }

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kWorkwhelResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("WORKWHEL.BAN", nullptr);
    }

    auto pWheel = relive_new WorkWheel(pTlv, pData->field_4_tlvInfo);
    if (pWheel)
    {
        if (pData->field_C_state == WheelStates::eTurning_1)
        {
            pWheel->VStartTurning();
        }

        pWheel->field_100_on_counter = pData->field_8_snd_counter;
    }
    return sizeof(WorkWheel_SaveState);
}

s32 WorkWheel::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<WorkWheel_SaveState*>(pSaveBuffer);

    pState->field_0_id = AETypes::eWheel_148;
    pState->field_4_tlvInfo = field_F4_tlv_info;
    pState->field_8_snd_counter = field_100_on_counter;
    pState->field_C_state = field_FC_state;
    return sizeof(WorkWheel_SaveState);
}

void WorkWheel::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_FC_state == WheelStates::eTurning_1)
    {
        ++field_100_on_counter;

        if (!(field_100_on_counter % 10)
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
    else if (field_FC_state == WheelStates::eIdle_0)
    {
        field_100_on_counter = 0;
    }

    if (field_F8_switch_id)
    {
        if (field_100_on_counter > field_FE_activation_time)
        {
            if (gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender && field_F8_switch_id == 100)
            {
                CreateGameEnderController_43B7A0();
            }

            if (field_102_off_time > 0 && field_100_on_counter > field_102_off_time)
            {
                SwitchStates_Set(field_F8_switch_id, 0);
            }
            else
            {
                SwitchStates_Set(field_F8_switch_id, 1);
            }
        }
    }
}

void WorkWheel::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || field_FC_state == WheelStates::eIdle_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void WorkWheel::VStartTurning()
{
    if (field_FC_state == WheelStates::eIdle_0)
    {
        field_FC_state = WheelStates::eTurning_1;
        mAnim.Set_Animation_Data(AnimId::Work_Wheel_Turning, nullptr);
    }
}

void WorkWheel::VStopTurning(s16 bResetSwitch)
{
    if (field_FC_state == WheelStates::eTurning_1)
    {
        field_FC_state = WheelStates::eIdle_0;

        // Spin it.
        mAnim.Set_Animation_Data(AnimId::Work_Wheel_Idle, nullptr);

        if (field_104_turn_off_when_stopped == Choice_short::eYes_1)
        {
            if (bResetSwitch)
            {
                SwitchStates_Set(field_F8_switch_id, 0);
            }
        }
    }
}
