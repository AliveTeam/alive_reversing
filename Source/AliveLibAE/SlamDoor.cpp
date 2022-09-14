#include "stdafx.h"
#include "SlamDoor.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Grid.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

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

TintEntry sSlamDoorTints[18] = {
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
    {EReliveLevelIds::eNone, 102u, 87u, 118u}};

struct Quicksave_Obj_SlamDoor final
{
    AETypes mType;
    Guid mTlvInfo;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_Obj_SlamDoor, 8);

void SlamDoor::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Industrial_Closing));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Industrial_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Industrial_Opening));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Vault_Closing));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Vault_Closed));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slam_Door_Vault_Opening));
}

SlamDoor::SlamDoor(relive::Path_SlamDoor* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    LoadAnimations();

    mBaseGameObjectTlvInfo = tlvId; // todo: check this
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    mSwitchId = pTlv->mSwitchId;

    mSlamDoorFlags.Clear(SlamDoorFlags::eOpen);
    mSlamDoorFlags.Clear(SlamDoorFlags::eFlipY);
    mSlamDoorFlags.Clear(SlamDoorFlags::eDelete);

    if (pTlv->mStartClosed == relive::reliveChoice::eNo)
    {
        mSlamDoorFlags.Set(SlamDoorFlags::eOpen);
    }

    if (pTlv->mFlipY == relive::reliveChoice::eYes)
    {
        mSlamDoorFlags.Set(SlamDoorFlags::eFlipY);
    }

    if (pTlv->mDelete == relive::reliveChoice::eYes)
    {
        mSlamDoorFlags.Set(SlamDoorFlags::eDelete);
    }

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
        mSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mScale = Scale::Bg;
    }
    else
    {
        mSpriteScale = FP_FromDouble(1.0);
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mScale = Scale::Fg;
    }

    if (mSlamDoorFlags.Get(SlamDoorFlags::eFlipY))
    {
        mAnim.mFlags.Set(AnimFlags::eBit6_FlipY);
        mYOffset = FP_GetExponent(mSpriteScale * FP_FromDouble(-68.0));
    }

    s32 switchState = SwitchStates_Get(mSwitchId);
    s32 bitIsOpen = static_cast<s32>(mSlamDoorFlags.Get(SlamDoorFlags::eOpen));

    if (switchState == bitIsOpen)
    {
        mSlamDoorFlags.Set(SlamDoorFlags::eClosed);
    }
    else
    {
        mSlamDoorFlags.Clear(SlamDoorFlags::eClosed);
    }

    SetTint(sSlamDoorTints, gMap.mCurrentLevel);

    FP hitX;
    FP hitY;

    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromDouble(24.0),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mScale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    if (mAnim.mFlags.Get(AnimFlags::eBit5_FlipX))
    {
        mCollisionX = FP_GetExponent((ScaleToGridSize(mSpriteScale) / FP_FromDouble(2.0)) + FP_FromInteger(FP_GetExponent(mXPos)));
    }
    else
    {
        mCollisionX = FP_GetExponent(FP_FromInteger(FP_GetExponent(mXPos)) - (ScaleToGridSize(mSpriteScale) / FP_FromDouble(2.0)));
    }

    mCollisionY = FP_GetExponent(mYPos);

    if (mSlamDoorFlags.Get(SlamDoorFlags::eClosed))
    {
        PathLine* pPathLine = nullptr;

        if (mSpriteScale == FP_FromDouble(1.0))
        {
            const FP lineHeight = FP_FromDouble(80.0);

            mCollisionLine1 = sCollisions->Add_Dynamic_Collision_Line(
                mCollisionX,
                FP_GetExponent(FP_FromInteger(mCollisionY) - lineHeight),
                mCollisionX,
                mCollisionY,
                eLineTypes::eWallRight_2);
            const FP x2 = FP_FromInteger(mCollisionX) + ScaleToGridSize(mSpriteScale);
            const FP y1 = FP_FromInteger(mCollisionY)
                        - (mSpriteScale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(mSpriteScale) + FP_FromInteger(mCollisionX);
            pPathLine = sCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                mCollisionY,
                eLineTypes::eWallLeft_1);
        }
        else
        {
            const FP lineHeight = mSpriteScale * FP_FromDouble(80.0);

            mCollisionLine1 = sCollisions->Add_Dynamic_Collision_Line(
                mCollisionX,
                FP_GetExponent(FP_FromInteger(mCollisionY) - lineHeight),
                mCollisionX,
                mCollisionY,
                eLineTypes::eBackgroundWallRight_6);
            const FP x2 = FP_FromInteger(mCollisionX) + ScaleToGridSize(mSpriteScale);
            const FP y1 = FP_FromInteger(mCollisionY) - (mSpriteScale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(mSpriteScale) + FP_FromInteger(mCollisionX);
            pPathLine = sCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                mCollisionY,
                eLineTypes::eBackgroundWallLeft_5);
        }
        mCollisionLine2 = pPathLine;

        mAnim.Set_Animation_Data(GetAnimRes(sSlamDoorAnimIds[currentLevelId][1]));
    }
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        mCollisionLine1 = 0;
        mCollisionLine2 = 0;
    }

    mSlamDoorFlags.Set(SlamDoorFlags::eLastFrame);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

SlamDoor::~SlamDoor()
{
    if (!(mSlamDoorFlags.Get(SlamDoorFlags::eDelete)) || mSlamDoorFlags.Get(SlamDoorFlags::eClosed))
    {
        Path::TLV_Reset(mTlvInfo, -1, 0, 0);
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
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const bool stateUnchanged = SwitchStates_Get(mSwitchId) == static_cast<s32>(mSlamDoorFlags.Get(SlamDoorFlags::eOpen));
    if (!mSlamDoorFlags.Get(SlamDoorFlags::eClosed))
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

                if (mSlamDoorFlags.Get(SlamDoorFlags::eDelete))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 900);
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, -100);
                mSlamDoorFlags.Set(SlamDoorFlags::eLastFrame);
            }
        }
    }

    if (mSlamDoorFlags.Get(SlamDoorFlags::eClosed))
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!mSlamDoorFlags.Get(SlamDoorFlags::eLastFrame))
            {
                mSlamDoorFlags.Set(SlamDoorFlags::eLastFrame);
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, 900);
                SFX_Play_Pitch(relive::SoundEffects::DoorEffect, 100, -100);
            }
        }
    }

    if (stateUnchanged != mSlamDoorFlags.Get(SlamDoorFlags::eClosed))
    {
        mSlamDoorFlags.Clear(SlamDoorFlags::eLastFrame);
        mSlamDoorFlags.Toggle(SlamDoorFlags::eClosed);

        if (stateUnchanged)
        {
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);

            mAnim.Set_Animation_Data(GetAnimRes(sSlamDoorAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][2]));

            if (mSpriteScale == FP_FromInteger(1))
            {
                mCollisionLine1 = sCollisions->Add_Dynamic_Collision_Line(
                    mCollisionX,
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * FP_FromInteger(1))),
                    mCollisionX,
                    mCollisionY,
                    eLineTypes::eWallLeft_1);
                mCollisionLine2 = sCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(mSpriteScale) + FP_FromInteger(mCollisionX)),
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * mSpriteScale)),
                    FP_GetExponent(FP_FromInteger(mCollisionX) + ScaleToGridSize(mSpriteScale)),
                    mCollisionY,
                    eLineTypes::eWallRight_2);
            }
            else
            {
                mCollisionLine1 = sCollisions->Add_Dynamic_Collision_Line(
                    mCollisionX,
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * mSpriteScale)),
                    mCollisionX,
                    mCollisionY,
                    eLineTypes::eBackgroundWallLeft_5);
                mCollisionLine2 = sCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(mSpriteScale) + FP_FromInteger(mCollisionX)),
                    FP_GetExponent(FP_FromInteger(mCollisionY) - (FP_FromInteger(80) * mSpriteScale)),
                    FP_GetExponent(FP_FromInteger(mCollisionX) + ScaleToGridSize(mSpriteScale)),
                    mCollisionY,
                    eLineTypes::eBackgroundWallRight_6);
            }

            PSX_RECT bRect = VGetBoundingRect();

            if (mSlamDoorFlags.Get(SlamDoorFlags::eFlipY))
            {
                bRect.y += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale);
                bRect.h += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale);
            }

            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                if (pObj->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
                {
                    if (pObj->Type() != ReliveTypes::eSlamDoor)
                    {
                        PSX_RECT bObjRect = pObj->VGetBoundingRect();

                        // Some hack that prevents Abe getting knocked back when rolling or falling near a closing slam door
                        bObjRect.x += 3;

                        if (PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect) && pObj->mSpriteScale == mSpriteScale)
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
        else
        {
            mAnim.Set_Animation_Data(GetAnimRes(sSlamDoorAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]));
            Rect_Clear(&mCollisionLine1->mRect);
            mCollisionLine1 = nullptr;

            Rect_Clear(&mCollisionLine2->mRect);
            mCollisionLine2 = nullptr;
        }
    }

    if (mSlamDoorFlags.Get(SlamDoorFlags::eClosed))
    {
        PSX_RECT bRect = VGetBoundingRect();

        if (mSlamDoorFlags.Get(SlamDoorFlags::eFlipY))
        {
            bRect.y += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale);
            bRect.h += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale) - FP_GetExponent(FP_FromInteger(20) * mSpriteScale);
        }

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                if (pObj->Type() != ReliveTypes::eSlamDoor && pObj->Type() != ReliveTypes::eGrenade)
                {
                    const PSX_RECT bObjRect = pObj->VGetBoundingRect();

                    if (FP_GetExponent(pObj->mXPos) > bRect.x && FP_GetExponent(pObj->mXPos) < bRect.w && PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect))
                    {
                        if (pObj->mSpriteScale == mSpriteScale || (pObj->Type() == ReliveTypes::eSlog && mSpriteScale == FP_FromInteger(1)))
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7, mAnim.mFlags.Get(AnimFlags::eBit3_Render));
}

s32 SlamDoor::VGetSaveState(u8* pSaveBuffer)
{
    Quicksave_Obj_SlamDoor* pSaveState = reinterpret_cast<Quicksave_Obj_SlamDoor*>(pSaveBuffer);

    pSaveState->mType = AETypes::eSlamDoor_122;
    pSaveState->mTlvInfo = mTlvInfo;

    return sizeof(Quicksave_Obj_SlamDoor);
}

void SlamDoor::ClearInsideSlamDoor(BaseAliveGameObject* pObj, s16 xPosition, s16 width)
{
    if (FP_GetExponent(pObj->mXPos) - xPosition >= width - FP_GetExponent(pObj->mXPos))
    {
        pObj->mXPos = (ScaleToGridSize(mSpriteScale) * FP_FromDouble(0.5)) + FP_FromDouble(1.0) + pObj->mXPos;
    }
    else
    {
        pObj->mXPos = pObj->mXPos - (ScaleToGridSize(mSpriteScale) * FP_FromDouble(0.5));
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

s32 SlamDoor::CreateFromSaveState(const u8* pData)
{
    const Quicksave_Obj_SlamDoor* pSaveState = reinterpret_cast<const Quicksave_Obj_SlamDoor*>(pData);

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlamResID, 0, 0))
    {
        switch (gMap.mCurrentLevel)
        {
            case EReliveLevelIds::eNecrum:
            case EReliveLevelIds::eMudomoVault:
            case EReliveLevelIds::eMudancheeVault:
            case EReliveLevelIds::eMudancheeVault_Ender:
            case EReliveLevelIds::eMudomoVault_Ender:
                ResourceManager::LoadResourceFile_49C170("SLAMVLTS.BAN", nullptr);
                break;
            default:
                ResourceManager::LoadResourceFile_49C170("SLAM.BAN", nullptr);
                break;
        }
    }

    relive_new SlamDoor(static_cast<relive::Path_SlamDoor*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->mTlvInfo)), pSaveState->mTlvInfo);

    return sizeof(Quicksave_Obj_SlamDoor);
}
