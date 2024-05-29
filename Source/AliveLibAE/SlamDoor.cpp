#include "stdafx.h"
#include "SlamDoor.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "../relive_lib/Collisions.hpp"
#include "QuikSave.hpp"

struct SlamDoor_Data final
{
    s32 field_0_frameTableOffset;
    s32 field_4_ppRes;
    s32 field_8_frameTableOffset;
    s16 field_C_maxH;
    s16 field_E_maxW;
};

const AnimId sSlamDoorAnimIds[15][3] = {
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Vault_Closing, AnimId::Slam_Door_Vault_Closed, AnimId::Slam_Door_Vault_Opening},
    {AnimId::Slam_Door_Vault_Closing, AnimId::Slam_Door_Vault_Closed, AnimId::Slam_Door_Vault_Opening},
    {AnimId::Slam_Door_Vault_Closing, AnimId::Slam_Door_Vault_Closed, AnimId::Slam_Door_Vault_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Vault_Closing, AnimId::Slam_Door_Vault_Closed, AnimId::Slam_Door_Vault_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Vault_Closing, AnimId::Slam_Door_Vault_Closed, AnimId::Slam_Door_Vault_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
    {AnimId::Slam_Door_Industrial_Closing, AnimId::Slam_Door_Industrial_Closed, AnimId::Slam_Door_Industrial_Opening},
};

static const TintEntry sSlamDoorTints[16] = {
    {EReliveLevelIds::eMenu, 102u, 87u, 118u},
    {EReliveLevelIds::eMines, 102u, 87u, 118u},
    {EReliveLevelIds::eNecrum, 102u, 87u, 118u},
    {EReliveLevelIds::eMudomoVault, 102u, 87u, 118u},
    {EReliveLevelIds::eMudancheeVault, 102u, 87u, 118u},
    {EReliveLevelIds::eFeeCoDepot, 102u, 87u, 118u},
    {EReliveLevelIds::eBarracks, 102u, 87u, 118u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 87u, 118u},
    {EReliveLevelIds::eBonewerkz, 102u, 87u, 118u},
    {EReliveLevelIds::eBrewery, 102u, 87u, 118u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 87u, 118u},
    {EReliveLevelIds::eMudomoVault_Ender, 102u, 87u, 118u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 102u, 87u, 118u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 87u, 118u},
    {EReliveLevelIds::eBonewerkz_Ender, 102u, 87u, 118u},
    {EReliveLevelIds::eCredits, 102u, 87u, 118u}};

void SlamDoor::LoadAnimations()
{
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eNecrum:
        case EReliveLevelIds::eMudomoVault:
        case EReliveLevelIds::eMudancheeVault:
        case EReliveLevelIds::eMudancheeVault_Ender:
        case EReliveLevelIds::eMudomoVault_Ender:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Vault_Closing));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Vault_Closed));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Vault_Opening));
            break;
        default:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Industrial_Closing));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Industrial_Closed));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Industrial_Opening));
            break;
    }
}

SlamDoor::SlamDoor(relive::Path_SlamDoor* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    LoadAnimations();

    mBaseGameObjectTlvInfo = tlvId; // todo: check this
    SetCanExplode(true);

    mSwitchId = pTlv->mSwitchId;

    mOpen = !pTlv->mStartClosed;
    mSlamDoorFlipY = pTlv->mFlipY;
    mDelete = pTlv->mDelete;

    const s32 currentLevelId = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    Animation_Init(GetAnimRes(sSlamDoorAnimIds[currentLevelId][2]));

    SetType(ReliveTypes::eSlamDoor);

    mXPos = FP_FromInteger(((pTlv->mTopLeftX
                                     + pTlv->mBottomRightX)
                                    / 2));

    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    mTlvInfo = tlvId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromDouble(1.0));
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);
        SetScale(Scale::Fg);
    }

    if (mSlamDoorFlipY)
    {
        GetAnimation().SetFlipY(true);
        mYOffset = FP_GetExponent(GetSpriteScale() * FP_FromDouble(-68.0));
    }

    s32 switchState = SwitchStates_Get(mSwitchId);
    s32 bitIsOpen = static_cast<s32>(mOpen);

    if (switchState == bitIsOpen)
    {
        mClosed = true;
    }
    else
    {
        mClosed = false;
    }

    SetTint(sSlamDoorTints, gMap.mCurrentLevel);

    FP hitX;
    FP hitY;

    if (gCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromDouble(24.0),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    if (GetAnimation().GetFlipX())
    {
        mCollisionX = FP_GetExponent((ScaleToGridSize(GetSpriteScale()) / FP_FromDouble(2.0)) + FP_FromInteger(FP_GetExponent(mXPos)));
    }
    else
    {
        mCollisionX = FP_GetExponent(FP_FromInteger(FP_GetExponent(mXPos)) - (ScaleToGridSize(GetSpriteScale()) / FP_FromDouble(2.0)));
    }

    mCollisionY = FP_GetExponent(mYPos);

    if (mClosed)
    {
        PathLine* pPathLine = nullptr;

        if (GetSpriteScale() == FP_FromDouble(1.0))
        {
            const FP lineHeight = FP_FromDouble(80.0);

            mCollisionLine1 = gCollisions->Add_Dynamic_Collision_Line(
                mCollisionX,
                FP_GetExponent(FP_FromInteger(mCollisionY) - lineHeight),
                mCollisionX,
                mCollisionY,
                eLineTypes::eWallRight_2);
            const FP x2 = FP_FromInteger(mCollisionX) + ScaleToGridSize(GetSpriteScale());
            const FP y1 = FP_FromInteger(mCollisionY)
                        - (GetSpriteScale() * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(GetSpriteScale()) + FP_FromInteger(mCollisionX);
            pPathLine = gCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                mCollisionY,
                eLineTypes::eWallLeft_1);
        }
        else
        {
            const FP lineHeight = GetSpriteScale() * FP_FromDouble(80.0);

            mCollisionLine1 = gCollisions->Add_Dynamic_Collision_Line(
                mCollisionX,
                FP_GetExponent(FP_FromInteger(mCollisionY) - lineHeight),
                mCollisionX,
                mCollisionY,
                eLineTypes::eBackgroundWallRight_6);
            const FP x2 = FP_FromInteger(mCollisionX) + ScaleToGridSize(GetSpriteScale());
            const FP y1 = FP_FromInteger(mCollisionY) - (GetSpriteScale() * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(GetSpriteScale()) + FP_FromInteger(mCollisionX);
            pPathLine = gCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                mCollisionY,
                eLineTypes::eBackgroundWallLeft_5);
        }
        mCollisionLine2 = pPathLine;

        GetAnimation().Set_Animation_Data(GetAnimRes(sSlamDoorAnimIds[currentLevelId][1]));
    }
    else
    {
        GetAnimation().SetRender(false);
        mCollisionLine1 = 0;
        mCollisionLine2 = 0;
    }

    mLastFrame = true;
    SetDoPurpleLightEffect(true);
}

SlamDoor::~SlamDoor()
{
    if (!mDelete || mClosed)
    {
        Path::TLV_Reset(mTlvInfo);
    }

    if (mCollisionLine1)
    {
        Rect_Clear(&mCollisionLine1->mRect);
    }

    if (mCollisionLine2)
    {
        Rect_Clear(&mCollisionLine2->mRect);
    }
}

void SlamDoor::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    const bool stateUnchanged = SwitchStates_Get(mSwitchId) == static_cast<s32>(mOpen);
    if (!mClosed)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (GetAnimation().GetRender())
            {
                GetAnimation().SetRender(false);

                if (mDelete)
                {
                    SetDead(true);
                }
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 900);
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, -100);
                mLastFrame = true;
            }
        }
    }

    if (mClosed)
    {
        if (GetAnimation().GetIsLastFrame())
        {
            if (!mLastFrame)
            {
                mLastFrame = true;
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 900);
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, -100);
            }
        }
    }

    if (stateUnchanged != mClosed)
    {
        mLastFrame = false;
        mClosed = !mClosed;

        if (stateUnchanged)
        {
            GetAnimation().SetRender(true);

            GetAnimation().Set_Animation_Data(GetAnimRes(sSlamDoorAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][2]));

            if (GetSpriteScale() == FP_FromInteger(1))
            {
                mCollisionLine1 = gCollisions->Add_Dynamic_Collision_Line(
                    mCollisionX,
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * FP_FromInteger(1))),
                    mCollisionX,
                    mCollisionY,
                    eLineTypes::eWallLeft_1);
                mCollisionLine2 = gCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(GetSpriteScale()) + FP_FromInteger(mCollisionX)),
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * GetSpriteScale())),
                    FP_GetExponent(FP_FromInteger(mCollisionX) + ScaleToGridSize(GetSpriteScale())),
                    mCollisionY,
                    eLineTypes::eWallRight_2);
            }
            else
            {
                mCollisionLine1 = gCollisions->Add_Dynamic_Collision_Line(
                    mCollisionX,
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * GetSpriteScale())),
                    mCollisionX,
                    mCollisionY,
                    eLineTypes::eBackgroundWallLeft_5);
                mCollisionLine2 = gCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(GetSpriteScale()) + FP_FromInteger(mCollisionX)),
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * GetSpriteScale())),
                    FP_GetExponent(FP_FromInteger(mCollisionX) + ScaleToGridSize(GetSpriteScale())),
                    mCollisionY,
                    eLineTypes::eBackgroundWallRight_6);
            }

            PSX_RECT bRect = VGetBoundingRect();

            if (mSlamDoorFlipY)
            {
                bRect.y += FP_GetExponent(FP_FromInteger(-110) * GetSpriteScale());
                bRect.h += FP_GetExponent(FP_FromInteger(-110) * GetSpriteScale());
            }

            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                if (pObj->GetAnimation().GetRender())
                {
                    if (pObj->Type() != ReliveTypes::eSlamDoor)
                    {
                        PSX_RECT bObjRect = pObj->VGetBoundingRect();

                        // Some hack that prevents Abe getting knocked back when rolling or falling near a closing slam door
                        bObjRect.x += 3;

                        if (PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect) && pObj->GetSpriteScale() == GetSpriteScale())
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
        else
        {
            GetAnimation().Set_Animation_Data(GetAnimRes(sSlamDoorAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]));
            Rect_Clear(&mCollisionLine1->mRect);
            mCollisionLine1 = nullptr;

            Rect_Clear(&mCollisionLine2->mRect);
            mCollisionLine2 = nullptr;
        }
    }

    if (mClosed)
    {
        PSX_RECT bRect = VGetBoundingRect();

        if (mSlamDoorFlipY)
        {
            bRect.y += FP_GetExponent(FP_FromInteger(-110) * GetSpriteScale());
            bRect.h += FP_GetExponent(FP_FromInteger(-110) * GetSpriteScale()) - FP_GetExponent(FP_FromInteger(20) * GetSpriteScale());
        }

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->GetAnimation().GetRender())
            {
                if (pObj->Type() != ReliveTypes::eSlamDoor && pObj->Type() != ReliveTypes::eGrenade)
                {
                    const PSX_RECT bObjRect = pObj->VGetBoundingRect();

                    if (FP_GetExponent(pObj->mXPos) > bRect.x && FP_GetExponent(pObj->mXPos) < bRect.w && PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect))
                    {
                        if (pObj->GetSpriteScale() == GetSpriteScale() || (pObj->Type() == ReliveTypes::eSlog && GetSpriteScale() == FP_FromInteger(1)))
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
    }

    SetCanExplode(GetAnimation().GetRender()); // wot?
}

void SlamDoor::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    SlamDoorSaveState data = {};

    data.mType = ReliveTypes::eSlamDoor;
    data.mTlvInfo = mTlvInfo;

    pSaveBuffer.Write(data);
}

void SlamDoor::ClearInsideSlamDoor(BaseAliveGameObject* pObj, s16 xPosition, s16 width)
{
    if (FP_GetExponent(pObj->mXPos) - xPosition >= width - FP_GetExponent(pObj->mXPos))
    {
        pObj->mXPos = (ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5)) + FP_FromDouble(1.0) + pObj->mXPos;
    }
    else
    {
        pObj->mXPos = pObj->mXPos - (ScaleToGridSize(GetSpriteScale()) * FP_FromDouble(0.5));
    }

    if (pObj->Type() == ReliveTypes::eRingOrLiftMud || pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eAbe)
    {
        pObj->VTakeDamage(this);
    }

    if (pObj->Type() == ReliveTypes::eBone)
    {
        pObj->mVelY = FP_FromInteger(-7);
        pObj->mVelX = -pObj->mVelX;
    }
}

void SlamDoor::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pSaveState = pData.ReadTmpPtr<SlamDoorSaveState>();

    relive_new SlamDoor(static_cast<relive::Path_SlamDoor*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvInfo)), pSaveState->mTlvInfo);
}
