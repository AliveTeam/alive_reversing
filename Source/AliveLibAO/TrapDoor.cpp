#include "stdafx_ao.h"
#include "Function.hpp"
#include "TrapDoor.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "../relive_lib/Collisions.hpp"

namespace AO {

struct TrapDoor_Data final
{
    AnimId field_0_open;
    AnimId field_4_closed;
    AnimId field_8_opening;
    AnimId field_C_closing;
    //s16 field_10_maxW;
    //s16 field_12_maxH;
};
//ALIVE_ASSERT_SIZEOF(TrapDoor_Data, 0x14);


const TrapDoor_Data sTrapDoorData_4BD4A0[16] = {
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // menu
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // rapture farms
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // lines
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest temple
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // stock yards
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // stock yards return
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // removed
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // desert
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // desert temple
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // credits
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}, // removed
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // board room
    {AnimId::R1_TrapDoor_Open, AnimId::R1_TrapDoor_Closed, AnimId::R1_TrapDoor_Opening, AnimId::R1_TrapDoor_Closing}, // rupture farms return
    {AnimId::Lines_TrapDoor_Open, AnimId::Lines_TrapDoor_Closed, AnimId::Lines_TrapDoor_Opening, AnimId::Lines_TrapDoor_Closing}, // forest chase
    {AnimId::Desert_TrapDoor_Open, AnimId::Desert_TrapDoor_Closed, AnimId::Desert_TrapDoor_Opening, AnimId::Desert_TrapDoor_Closing}}; // desert escape


TrapDoor::TrapDoor(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo)
{
    mBaseGameObjectTypeId = ReliveTypes::eTrapDoor;
    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    AnimId animId = AnimId::None;
    if (mStartState == SwitchStates_Get(pTlv->mSwitchId))
    {
        mState = TrapDoorState::eOpen_2;
        animId = sTrapDoorData_4BD4A0[cur_lvl].field_0_open;
    }
    else
    {
        mState = TrapDoorState::eClosed_0;
        animId = sTrapDoorData_4BD4A0[cur_lvl].field_4_closed;
    }

    mSelfClosing = pTlv->mSelfClosing;
    if (pTlv->mScale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kP6c1trapAOResID, 1, 0);

    AddDynamicCollision(
        sTrapDoorData_4BD4A0[cur_lvl].field_4_closed,
        ppRes,
        pTlv,
        pMap,
        tlvInfo);

    mTrapDoorY = FP_FromInteger(pTlv->mTopLeft.y);
    mTrapDoorX = FP_FromInteger(pTlv->mTopLeft.x);

    mAnim.Set_Animation_Data(animId, nullptr);
    if (pTlv->mDirection == XDirection_short::eRight_1)
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mXPos);
    mTrapDoorXOffset = pTlv->mXOff;

    if (mState == TrapDoorState::eOpen_2)
    {
        // Inlined
        Open();
    }

    mBoundingRect.x = pTlv->mTopLeft.x;
    mBoundingRect.y = pTlv->mTopLeft.y;

    mBoundingRect.w = pTlv->mBottomRight.x;
    mBoundingRect.h = pTlv->mBottomRight.y;

    field_10C = 4;
}

void TrapDoor::VRender(PrimHeader** ppOt)
{
    mXPos += FP_FromInteger(mTrapDoorXOffset);
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    mXPos -= FP_FromInteger(mTrapDoorXOffset);
}

void TrapDoor::VRemove(BaseAliveGameObject* pObj)
{
    PlatformBase::VRemove(pObj);
}

void TrapDoor::VAdd(BaseAliveGameObject* pObj)
{
    PlatformBase::VAdd(pObj);
}

void TrapDoor::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);

        if (mSelfClosing == Choice_short::eYes_1)
        {
            SwitchStates_Set(mSwitchId, mStartState == 0);
        }
    }
}

PSX_RECT TrapDoor::VGetBoundingRect(s32 /*pointIdx*/)
{
    return mBoundingRect;
}

TrapDoor::~TrapDoor()
{
    Path::TLV_Reset(mPlatformBaseTlvInfo, -1, 0, 0);
}

void TrapDoor::Open()
{
    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        // Find alive objects..
        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // That are on this trap door
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (pAliveObj->mLiftPoint == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened
                if (mPlatformBaseCollisionLine == pAliveObj->BaseAliveGameObjectCollisionLine)
                {
                    pAliveObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }
        }
    }

    Rect_Clear(&mPlatformBaseCollisionLine->mRect);
    mPlatformBaseCollisionLine = nullptr;
    ObjListPlatforms_50766C->Remove_Item(this);
}

void TrapDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const CameraPos direction = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mTrapDoorX,
        mTrapDoorY);

    switch (mState)
    {
        case TrapDoorState::eClosed_0:
            if (SwitchStates_Get(mSwitchId) == mStartState)
            {
                Open();
                mState = TrapDoorState::eOpening_1;

                const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
                mAnim.Set_Animation_Data(
                    sTrapDoorData_4BD4A0[cur_lvl].field_8_opening,
                    nullptr);

                SFX_Play_Camera(SoundEffect::Trapdoor_49, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialTrigger_97, 45, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise1_93, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mState = TrapDoorState::eOpen_2;
                mStayOpenTimeTimer = 20; // NOTE: AE has another variable to save and re-use the path tlv value
            }
            break;

        case TrapDoorState::eOpen_2:
            mStayOpenTimeTimer--;
            if ((mSelfClosing == Choice_short::eYes_1 && !mStayOpenTimeTimer) || SwitchStates_Get(mSwitchId) != SwitchStates_Get(mStartState))
            {
                const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
                mAnim.Set_Animation_Data(
                    sTrapDoorData_4BD4A0[cur_lvl].field_C_closing,
                    nullptr);
                mState = TrapDoorState::eClosing_3;

                SFX_Play_Camera(SoundEffect::Trapdoor_49, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(SoundEffect::IndustrialNoise3_95, 60, direction);
                    SFX_Play_Camera(SoundEffect::IndustrialNoise2_94, 90, direction);
                }
            }
            break;

        case TrapDoorState::eClosing_3:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mPlatformBaseCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
                    mBoundingRect.x,
                    mBoundingRect.y,
                    mBoundingRect.w,
                    mBoundingRect.y,
                    eLineTypes::eDynamicCollision_32
                );
                ObjListPlatforms_50766C->Push_Back(this);
                mState = TrapDoorState::eClosed_0;
                SwitchStates_Set(mSwitchId, mStartState == 0);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
