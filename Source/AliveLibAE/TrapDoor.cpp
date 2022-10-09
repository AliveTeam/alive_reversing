#include "stdafx.h"
#include "TrapDoor.hpp"
#include "Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "Path.hpp"

struct TrapDoor_Data final
{
    AnimId field_0_open;
    AnimId field_4_closed;
    AnimId field_8_opening;
    AnimId field_C_closing;
    //s16 field_10_maxW;
    //s16 field_12_maxH;
};

const TrapDoor_Data sTrapDoorData_547B78[18] = {
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Tribal_Open, AnimId::Trap_Door_Tribal_Closed, AnimId::Trap_Door_Tribal_Opening, AnimId::Trap_Door_Tribal_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing},
    {AnimId::Trap_Door_Open, AnimId::Trap_Door_Closed, AnimId::Trap_Door_Opening, AnimId::Trap_Door_Closing}};

const TintEntry sTrapDoorTints_5639AC[18] = {
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
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
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};

void TrapDoor::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Opening));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Closing));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Open));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Opening));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Closing));
}

TrapDoor::TrapDoor(relive::Path_TrapDoor* pTlv, const Guid& tlvId)
{
    SetType(ReliveTypes::eTrapDoor);
    mBaseGameObjectTlvInfo = tlvId;

    LoadAnimations();

    mStayOpenTimeTimer = mStayOpenTime;
    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;

    const s32 levelIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    AnimId animId = AnimId::None;
    if (mStartState == SwitchStates_Get(mSwitchId))
    {
        mState = TrapDoorState::eOpen_2;
        animId = sTrapDoorData_547B78[levelIdx].field_0_open;
    }
    else
    {
        mState = TrapDoorState::eClosed_0;
        animId = sTrapDoorData_547B78[levelIdx].field_4_closed;
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
        sTrapDoorData_547B78[levelIdx].field_4_closed,
        pTlv,
        tlvId);

    if (mSpriteScale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_Shadow_26;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_Shadow_Half_7;
        field_124_pCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    SetTint(sTrapDoorTints_5639AC, gMap.mCurrentLevel);

    mTrapDoorX = FP_FromInteger(pTlv->mTopLeftX);
    mTrapDoorY = FP_FromInteger(pTlv->mTopLeftY);

    mAnim.Set_Animation_Data(GetAnimRes(animId));

    if (pTlv->mDirection == relive::reliveXDirection::eRight) // TODO: check if this is the correct direction
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeftX) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRightX) - mXPos);
    mTrapDoorXOffset = pTlv->mXOff;

    if (mState == TrapDoorState::eOpen_2)
    {
        Open();
    }

    mBoundingRect.x = pTlv->mTopLeftX;
    mBoundingRect.y = pTlv->mTopLeftY;

    mBoundingRect.w = pTlv->mBottomRightX;
    mBoundingRect.h = pTlv->mBottomRightY;

    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mStayOpenTime = pTlv->mStayOpenTime;
}

void TrapDoor::VRender(PrimHeader** ppOt)
{
    mXPos += FP_FromInteger(mTrapDoorXOffset);
    BaseAliveGameObject::VRender(ppOt);
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

        // Find alive objects.
        if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            // That are on this trap door.
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (sObjectIds.Find_Impl(pAliveObj->BaseAliveGameObject_PlatformId) == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened.
                if (field_124_pCollisionLine == pAliveObj->BaseAliveGameObjectCollisionLine)
                {
                    pAliveObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }
        }
    }

    Rect_Clear(&field_124_pCollisionLine->mRect);
    field_124_pCollisionLine = nullptr;
    ObjList_5C1B78->Remove_Item(this);
}

void TrapDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const CameraPos direction = gMap.GetDirection(mCurrentLevel, mCurrentPath, mTrapDoorX, mTrapDoorY);

    switch (mState)
    {
    case TrapDoorState::eClosed_0:
        if (SwitchStates_Get(mSwitchId) == mStartState)
        {
            Open();
            mState = TrapDoorState::eOpening_1;
            mAnim.Set_Animation_Data(GetAnimRes(sTrapDoorData_547B78[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].field_8_opening));

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Camera(relive::SoundEffects::IndustrialTrigger, 45, direction);
                SFX_Play_Camera(relive::SoundEffects::IndustrialNoise1, 90, direction);
            }
        }
        break;

    case TrapDoorState::eOpening_1:
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_Camera(relive::SoundEffects::TrapdoorOpen, 70, direction);
            mState = TrapDoorState::eOpen_2;
            mStayOpenTimeTimer = mStayOpenTime;
        }
        break;

    case TrapDoorState::eOpen_2:
        mStayOpenTimeTimer--;

        if ((mSelfClosing == relive::reliveChoice::eYes && mStayOpenTimeTimer + 1 <= 0) || SwitchStates_Get(mSwitchId) != mStartState)
        {
            mAnim.Set_Animation_Data(GetAnimRes(sTrapDoorData_547B78[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].field_C_closing));

            mState = TrapDoorState::eClosing_3;

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Camera(relive::SoundEffects::IndustrialNoise3, 60, direction);
                SFX_Play_Camera(relive::SoundEffects::IndustrialNoise2, 90, direction);
            }
        }
        break;

    case TrapDoorState::eClosing_3:
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_Camera(relive::SoundEffects::TrapdoorClose, 70, direction);
            Add_To_Collisions_Array();
            mState = TrapDoorState::eClosed_0;
            SwitchStates_Set(mSwitchId, mStartState == 0);
        }
        break;

    default:
        return;
    }
}

s32 TrapDoor::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const TrapDoor_State*>(pData);
    auto pTlv = static_cast<relive::Path_TrapDoor*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_8_tlvInfo));

    auto pTrapDoor = relive_new TrapDoor(pTlv, pState->field_8_tlvInfo);
    if (pTrapDoor)
    {
        pTrapDoor->mStayOpenTimeTimer = pState->field_4_open_time;
        pTrapDoor->mState = pState->field_2_state;

        if (pState->field_2_state == TrapDoorState::eClosing_3 || pState->field_2_state == TrapDoorState::eOpening_1)
        {
            pTrapDoor->Open();
        }
    }

    return sizeof(TrapDoor_State);
}

s32 TrapDoor::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<TrapDoor_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eTrapDoor_142;
    pState->field_4_open_time = mStayOpenTimeTimer;
    pState->field_2_state = mState;
    pState->field_8_tlvInfo = mPlatformBaseTlvInfo;
    return sizeof(TrapDoor_State);
}

void TrapDoor::Add_To_Collisions_Array()
{
    field_124_pCollisionLine = sCollisions->Add_Dynamic_Collision_Line(
        mBoundingRect.x,
        mBoundingRect.y,
        mBoundingRect.w,
        mBoundingRect.y,
        eLineTypes::eDynamicCollision_32);

    if (mSpriteScale != FP_FromInteger(1))
    {
        field_124_pCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    ObjList_5C1B78->Push_Back(this);
}
