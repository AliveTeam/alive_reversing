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
#include "Path.hpp"
#include "stdlib.hpp"

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

TrapDoor::TrapDoor(Path_TrapDoor* pTlv, s32 tlvInfo)
{
    SetType(ReliveTypes::eTrapDoor);
    mBaseGameObjectTlvInfo = tlvInfo;

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
    if (pTlv->field_16_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromInteger(1);
        mScale = Scale::Fg;
    }

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID);

    AddDynamicCollision(
        sTrapDoorData_547B78[levelIdx].field_4_closed,
        ppRes,
        pTlv,
        tlvInfo);

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

    mTrapDoorX = FP_FromInteger(pTlv->mTopLeft.x);
    mTrapDoorY = FP_FromInteger(pTlv->mTopLeft.y);

    mAnim.Set_Animation_Data(animId, nullptr);

    if (pTlv->mDirection == XDirection_short::eRight_1) // TODO: check if this is the correct direction
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mPlatformBaseXOffset = FP_GetExponent(FP_FromInteger(pTlv->mTopLeft.x) - mXPos);
    mPlatformBaseWidthOffset = FP_GetExponent(FP_FromInteger(pTlv->mBottomRight.x) - mXPos);
    mTrapDoorXOffset = pTlv->field_1C_xOff;

    if (mState == TrapDoorState::eOpen_2)
    {
        Open();
    }

    mBoundingRect.x = pTlv->mTopLeft.x;
    mBoundingRect.y = pTlv->mTopLeft.y;

    mBoundingRect.w = pTlv->mBottomRight.x;
    mBoundingRect.h = pTlv->mBottomRight.y;

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
        if (mSelfClosing == Choice_short::eYes_1)
        {
            SwitchStates_Set(mSwitchId, mStartState == 0);
        }
    }
}

PSX_RECT TrapDoor::VGetBoundingRect(s32 /*not_used*/)
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
            mAnim.Set_Animation_Data(sTrapDoorData_547B78[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].field_8_opening, nullptr);

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Camera(SoundEffect::IndustrialTrigger_80, 45, direction);
                SFX_Play_Camera(SoundEffect::IndustrialNoise1_76, 90, direction);
            }
        }
        break;

    case TrapDoorState::eOpening_1:
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_Camera(SoundEffect::TrapdoorOpen_43, 70, direction);
            mState = TrapDoorState::eOpen_2;
            mStayOpenTimeTimer = mStayOpenTime;
        }
        break;

    case TrapDoorState::eOpen_2:
        mStayOpenTimeTimer--;

        if ((mSelfClosing == Choice_short::eYes_1 && mStayOpenTimeTimer + 1 <= 0) || SwitchStates_Get(mSwitchId) != mStartState)
        {
            mAnim.Set_Animation_Data(sTrapDoorData_547B78[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))].field_C_closing, nullptr);

            mState = TrapDoorState::eClosing_3;

            if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz_Ender || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBarracks_Ender || gMap.mCurrentLevel == EReliveLevelIds::eBrewery || gMap.mCurrentLevel == EReliveLevelIds::eBrewery_Ender)
            {
                SFX_Play_Camera(SoundEffect::IndustrialNoise3_78, 60, direction);
                SFX_Play_Camera(SoundEffect::IndustrialNoise2_77, 90, direction);
            }
        }
        break;

    case TrapDoorState::eClosing_3:
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            SFX_Play_Camera(SoundEffect::TrapdoorClose_42, 70, direction);
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
    auto pTlv = static_cast<Path_TrapDoor*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->field_8_tlvInfo));

    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eMudomoVault:
        case EReliveLevelIds::eMudancheeVault:
        case EReliveLevelIds::eMudancheeVault_Ender:
        case EReliveLevelIds::eMudomoVault_Ender:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("VLTSTRAP.BAN", nullptr);
            }
            break;

        default:
            if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kP6c1trapResID, FALSE, FALSE))
            {
                ResourceManager::LoadResourceFile_49C170("TRAPDOOR.BAN", nullptr);
            }
            break;
    }

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
