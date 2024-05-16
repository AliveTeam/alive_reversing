#include "stdafx.h"
#include "TrapDoor.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"

struct TrapDoor_Data final
{
    AnimId mOpen;
    AnimId mClosed;
    AnimId mOpening;
    AnimId mClosing;
};

static const TrapDoor_Data sTrapDoorData[18] = {
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

static const TintEntry sTrapDoorTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
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
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};

void TrapDoor::LoadAnimations()
{
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eMudomoVault:
        case EReliveLevelIds::eMudancheeVault:
        case EReliveLevelIds::eMudancheeVault_Ender:
        case EReliveLevelIds::eMudomoVault_Ender:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Open));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Closed));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Opening));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Tribal_Closing));
            break;

        default:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Open));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Closed));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Opening));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Trap_Door_Closing));
            break;
    }
}

TrapDoor::TrapDoor(relive::Path_TrapDoor* pTlv, const Guid& tlvId)
{
    SetType(ReliveTypes::eTrapDoor);
    mBaseGameObjectTlvInfo = tlvId;

    LoadAnimations();

    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;
    mStayOpenTime = pTlv->mStayOpenTime;

    const s32 levelIdx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    AnimId animId = AnimId::None;
    if (mStartState == SwitchStates_Get(pTlv->mSwitchId))
    {
        mState = TrapDoorState::eOpen_2;
        animId = sTrapDoorData[levelIdx].mOpen;
    }
    else
    {
        mState = TrapDoorState::eClosed_0;
        animId = sTrapDoorData[levelIdx].mClosed;
    }

    mSelfClosing = pTlv->mSelfClosing;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    AddDynamicCollision(
        sTrapDoorData[levelIdx].mClosed,
        pTlv,
        tlvId);

    if (GetSpriteScale() == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_26);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_Shadow_Half_7);
        mPlatformBaseCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    SetTint(sTrapDoorTints, gMap.mCurrentLevel);

    mTrapDoorX = FP_FromInteger(pTlv->mTopLeftX);
    mTrapDoorY = FP_FromInteger(pTlv->mTopLeftY);

    GetAnimation().Set_Animation_Data(GetAnimRes(animId));

    if (pTlv->mDirection == relive::reliveXDirection::eRight)
    {
        GetAnimation().SetFlipX(true);
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

    SetDoPurpleLightEffect(true);
}

void TrapDoor::VRender(OrderingTable& ot)
{
    mXPos += FP_FromInteger(mTrapDoorXOffset);
    BaseAliveGameObject::VRender(ot);
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
    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
        if (mSelfClosing)
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
    Path::TLV_Reset(mPlatformBaseTlvInfo);
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
        if (pObj->GetIsBaseAliveGameObject())
        {
            // That are on this trap door.
            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);
            if (sObjectIds.Find_Impl(pAliveObj->BaseAliveGameObject_PlatformId) == this)
            {
                pAliveObj->VOnTrapDoorOpen();

                // Clear their collision line if they are on this trap door that has opened.
                if (mPlatformBaseCollisionLine == pAliveObj->BaseAliveGameObjectCollisionLine)
                {
                    pAliveObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }
        }
    }

    Rect_Clear(&mPlatformBaseCollisionLine->mRect);
    mPlatformBaseCollisionLine = nullptr;
    gPlatformsArray->Remove_Item(this);
}

void TrapDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    const CameraPos direction = gMap.GetDirection(mCurrentLevel, mCurrentPath, mTrapDoorX, mTrapDoorY);

    switch (mState)
    {
    case TrapDoorState::eClosed_0:
        if (SwitchStates_Get(mSwitchId) == mStartState)
        {
            Open();
            mState = TrapDoorState::eOpening_1;
            GetAnimation().Set_Animation_Data(GetAnimRes(sTrapDoorData[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].mOpening));

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Camera(relive::SoundEffects::IndustrialTrigger, 45, direction);
                SFX_Play_Camera(relive::SoundEffects::IndustrialNoise1, 90, direction);
            }
        }
        break;

    case TrapDoorState::eOpening_1:
        if (GetAnimation().GetIsLastFrame())
        {
            SFX_Play_Camera(relive::SoundEffects::TrapdoorOpen, 70, direction);
            mState = TrapDoorState::eOpen_2;
            mStayOpenTimeTimer = mStayOpenTime;
        }
        break;

    case TrapDoorState::eOpen_2:
        mStayOpenTimeTimer--;
        if ((mSelfClosing && mStayOpenTimeTimer <= 0) || SwitchStates_Get(mSwitchId) != mStartState)
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(sTrapDoorData[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].mClosing));

            mState = TrapDoorState::eClosing_3;

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Camera(relive::SoundEffects::IndustrialNoise3, 60, direction);
                SFX_Play_Camera(relive::SoundEffects::IndustrialNoise2, 90, direction);
            }
        }
        break;

    case TrapDoorState::eClosing_3:
        if (GetAnimation().GetIsLastFrame())
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

void TrapDoor::Add_To_Collisions_Array()
{
    mPlatformBaseCollisionLine = gCollisions->Add_Dynamic_Collision_Line(
        mBoundingRect.x,
        mBoundingRect.y,
        mBoundingRect.w,
        mBoundingRect.y,
        eLineTypes::eDynamicCollision_32);

    if (GetSpriteScale() != FP_FromInteger(1))
    {
        mPlatformBaseCollisionLine->mLineType = eLineTypes::eBackgroundDynamicCollision_36;
    }

    gPlatformsArray->Push_Back(this);
}

void TrapDoor::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pState = pData.ReadTmpPtr<TrapDoorSaveState>();
    auto pTlv = static_cast<relive::Path_TrapDoor*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId));

    auto pTrapDoor = relive_new TrapDoor(pTlv, pState->mTlvId);
    if (pTrapDoor)
    {
        pTrapDoor->mStayOpenTimeTimer = pState->mOpenTime;
        pTrapDoor->mState = pState->mState;

        if (pState->mState == TrapDoorState::eClosing_3 || pState->mState == TrapDoorState::eOpening_1)
        {
            pTrapDoor->Open();
        }
    }
}

void TrapDoor::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    TrapDoorSaveState data = {};

    data.mType = ReliveTypes::eTrapDoor;
    data.mOpenTime = mStayOpenTimeTimer;
    data.mState = mState;
    data.mTlvId = mPlatformBaseTlvInfo;
    pSaveBuffer.Write(data);
}
