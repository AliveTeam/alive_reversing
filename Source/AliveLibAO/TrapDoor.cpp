#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "TrapDoor.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Game.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

struct TrapDoor_Data final
{
    AnimId mOpen;
    AnimId mClosed;
    AnimId mOpening;
    AnimId mClosing;
};


static const TrapDoor_Data sTrapDoorData[16] = {
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
    for (u32 i = 0; i < ALIVE_COUNTOF(sTrapDoorData); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData[i].mOpen));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData[i].mClosed));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData[i].mOpening));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sTrapDoorData[i].mClosing));
    }
}

TrapDoor::TrapDoor(relive::Path_TrapDoor* pTlv, const Guid& tlvId)
{
    SetType(ReliveTypes::eTrapDoor);
    mBaseGameObjectTlvInfo = tlvId;
    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;

    LoadAnimations();

    const s32 cur_lvl = static_cast<s32>(MapWrapper::ToAO(gMap.mCurrentLevel));

    AnimId animId = AnimId::None;
    if (mStartState == SwitchStates_Get(pTlv->mSwitchId))
    {
        mState = TrapDoorState::eOpen_2;
        animId = sTrapDoorData[cur_lvl].mOpen;
    }
    else
    {
        mState = TrapDoorState::eClosed_0;
        animId = sTrapDoorData[cur_lvl].mClosed;
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
        sTrapDoorData[cur_lvl].mClosed,
        pTlv,
        tlvId);

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
        // Inlined
        Open();
    }

    mBoundingRect.x = pTlv->mTopLeftX;
    mBoundingRect.y = pTlv->mTopLeftY;

    mBoundingRect.w = pTlv->mBottomRightX;
    mBoundingRect.h = pTlv->mBottomRightY;
}

void TrapDoor::VRender(OrderingTable& ot)
{
    mXPos += FP_FromInteger(mTrapDoorXOffset);
    BaseAnimatedWithPhysicsGameObject::VRender(ot);
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
                GetAnimation().Set_Animation_Data(
                    GetAnimRes(sTrapDoorData[cur_lvl].mOpening));

                SFX_Play_Camera(relive::SoundEffects::Trapdoor, 70, direction);

                if (gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarms || gMap.mCurrentLevel == EReliveLevelIds::eBoardRoom || gMap.mCurrentLevel == EReliveLevelIds::eRuptureFarmsReturn)
                {
                    SFX_Play_Camera(relive::SoundEffects::IndustrialTrigger, 45, direction);
                    SFX_Play_Camera(relive::SoundEffects::IndustrialNoise1, 90, direction);
                }
            }
            break;

        case TrapDoorState::eOpening_1:
            if (GetAnimation().GetIsLastFrame())
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
                GetAnimation().Set_Animation_Data(
                    GetAnimRes(sTrapDoorData[cur_lvl].mClosing));
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
            if (GetAnimation().GetIsLastFrame())
            {
                mPlatformBaseCollisionLine = gCollisions->Add_Dynamic_Collision_Line(
                    mBoundingRect.x,
                    mBoundingRect.y,
                    mBoundingRect.w,
                    mBoundingRect.y,
                    eLineTypes::eDynamicCollision_32
                );
                gPlatformsArray->Push_Back(this);
                mState = TrapDoorState::eClosed_0;
                SwitchStates_Set(mSwitchId, mStartState == 0);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
