#include "stdafx_ao.h"
#include "Function.hpp"
#include "TrapDoor.hpp"
#include "../AliveLibAE/stdlib.hpp"
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


void TrapDoor::LoadAnimations()
{
    for (u32 i = 0; i < ALIVE_COUNTOF(sTrapDoorData_4BD4A0); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData_4BD4A0[i].field_0_open));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData_4BD4A0[i].field_4_closed));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData_4BD4A0[i].field_8_opening));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData_4BD4A0[i].field_C_closing));
    }
}

TrapDoor::TrapDoor(relive::Path_TrapDoor* pTlv, Map* pMap, const Guid& tlvId)
{
    SetType(ReliveTypes::eTrapDoor);
    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;

    LoadAnimations();

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
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    AddDynamicCollision(
        sTrapDoorData_4BD4A0[cur_lvl].field_4_closed,
        pTlv,
        pMap,
        tlvId);

    mTrapDoorY = FP_FromInteger(pTlv->mTopLeftY);
    mTrapDoorX = FP_FromInteger(pTlv->mTopLeftX);

    mAnim.Set_Animation_Data(GetAnimRes(animId));
    if (pTlv->mDirection == relive::reliveXDirection::eRight)
    {
        mAnim.mFlags.Set(AnimFlags::eFlipX);
    }

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRightX) - mXPos);
    mTrapDoorXOffset = pTlv->mXOff;

    if (mState == TrapDoorState::eOpen_2)
    {
        // Inlined
        Open();
    }

    mBoundingRect.x = pTlv->mTopLeftX;
    mBoundingRect.y = pTlv->mTopLeftY;

    mBoundingRect.w = pTlv->mBottomRightX;
    mBoundingRect.h = pTlv->mBottomRightY;

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

        if (mSelfClosing == relive::reliveChoice::eYes)
        {
            SwitchStates_Set(mSwitchId, mStartState == 0);
        }
    }
}

PSX_RECT TrapDoor::VGetBoundingRect()
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
                    GetAnimRes(sTrapDoorData_4BD4A0[cur_lvl].field_8_opening));

                SFX_Play_Camera(relive::SoundEffects::Trapdoor, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(relive::SoundEffects::IndustrialTrigger, 45, direction);
                    SFX_Play_Camera(relive::SoundEffects::IndustrialNoise1, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
            {
                mState = TrapDoorState::eOpen_2;
                mStayOpenTimeTimer = 20; // NOTE: AE has another variable to save and re-use the path tlv value
            }
            break;

        case TrapDoorState::eOpen_2:
            mStayOpenTimeTimer--;
            if ((mSelfClosing == relive::reliveChoice::eYes && !mStayOpenTimeTimer) || SwitchStates_Get(mSwitchId) != SwitchStates_Get(mStartState))
            {
                const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));
                mAnim.Set_Animation_Data(
                    GetAnimRes(sTrapDoorData_4BD4A0[cur_lvl].field_C_closing));
                mState = TrapDoorState::eClosing_3;

                SFX_Play_Camera(relive::SoundEffects::Trapdoor, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(relive::SoundEffects::IndustrialNoise3, 60, direction);
                    SFX_Play_Camera(relive::SoundEffects::IndustrialNoise2, 90, direction);
                }
            }
            break;

        case TrapDoorState::eClosing_3:
            if (mAnim.mFlags.Get(AnimFlags::eIsLastFrame))
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
