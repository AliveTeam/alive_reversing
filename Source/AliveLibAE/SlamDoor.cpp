#include "stdafx.h"
#include "SlamDoor.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Game.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Grid.hpp"

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

TintEntry sSlamDoorTints_5603B0[18] = {
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
    AETypes field_0_id;
    TlvItemInfoUnion field_4_tlv;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_Obj_SlamDoor, 8);

SlamDoor::SlamDoor(Path_SlamDoor* pTlv, TlvItemInfoUnion tlvInfo)
    : BaseAliveGameObject(0)
{
    mBaseGameObjectTlvInfo = tlvInfo.all; // todo: check this
    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);

    field_128_switch_id = pTlv->field_14_switch_id;

    field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit2_Open);
    field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit4_Inverted);
    field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit5_Delete);

    if (pTlv->field_10_bStart_closed == Choice_short::eNo_0)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit2_Open);
    }

    if (pTlv->field_16_bStart_inverted == Choice_short::eYes_1)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit4_Inverted);
    }

    if (pTlv->field_18_bDelete == Choice_short::eYes_1)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit5_Delete);
    }

    const s32 currentLevelId = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    const AnimRecord& rec = AnimRec(sSlamDoorAnimIds[currentLevelId][2]);
    Animation_Init(sSlamDoorAnimIds[currentLevelId][2], Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId));

    SetType(ReliveTypes::eSlamDoor);

    mXPos = FP_FromInteger(((pTlv->mTopLeft.x
                                     + pTlv->mBottomRight.x)
                                    / 2));

    mYPos = FP_FromInteger(pTlv->mTopLeft.y);
    field_12C_tlvInfo = tlvInfo;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
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

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
    {
        mAnim.mFlags.Set(AnimFlags::eBit6_FlipY);
        mYOffset = FP_GetExponent(mSpriteScale * FP_FromDouble(-68.0));
    }

    s32 switchState = SwitchStates_Get(field_128_switch_id);
    s32 bitIsOpen = static_cast<s32>(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit2_Open));

    if (switchState == bitIsOpen)
    {
        field_118_flags.Set(SlamDoor_Flags_118::e118_Bit1_bClosed);
    }
    else
    {
        field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit1_bClosed);
    }

    SetTint(sSlamDoorTints_5603B0, gMap.mCurrentLevel);

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
        field_124_x1 = FP_GetExponent((ScaleToGridSize(mSpriteScale) / FP_FromDouble(2.0)) + FP_FromInteger(FP_GetExponent(mXPos)));
    }
    else
    {
        field_124_x1 = FP_GetExponent(FP_FromInteger(FP_GetExponent(mXPos)) - (ScaleToGridSize(mSpriteScale) / FP_FromDouble(2.0)));
    }

    field_126_y1 = FP_GetExponent(mYPos);

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        PathLine* pPathLine = nullptr;

        if (mSpriteScale == FP_FromDouble(1.0))
        {
            const FP lineHeight = FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                eLineTypes::eWallRight_2);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize(mSpriteScale);
            const FP y1 = FP_FromInteger(field_126_y1)
                        - (mSpriteScale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(mSpriteScale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                eLineTypes::eWallLeft_1);
        }
        else
        {
            const FP lineHeight = mSpriteScale * FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                eLineTypes::eBackgroundWallRight_6);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize(mSpriteScale);
            const FP y1 = FP_FromInteger(field_126_y1) - (mSpriteScale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(mSpriteScale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                eLineTypes::eBackgroundWallLeft_5);
        }
        field_120_pCollisionLine_5_1 = pPathLine;

        mAnim.Set_Animation_Data(sSlamDoorAnimIds[currentLevelId][1], nullptr);
    }
    else
    {
        mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
        field_11C_pCollisionLine_6_2 = 0;
        field_120_pCollisionLine_5_1 = 0;
    }

    field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
}

SlamDoor::~SlamDoor()
{
    if (!(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit5_Delete)) || field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        Path::TLV_Reset(field_12C_tlvInfo.all, -1, 0, 0);
    }

    if (field_11C_pCollisionLine_6_2)
    {
        Rect_Clear(&field_11C_pCollisionLine_6_2->mRect);
    }

    if (field_120_pCollisionLine_5_1)
    {
        Rect_Clear(&field_120_pCollisionLine_5_1->mRect);
    }
}

void SlamDoor::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const bool stateUnchanged = SwitchStates_Get(field_128_switch_id) == static_cast<s32>(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit2_Open));
    if (!field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (mAnim.mFlags.Get(AnimFlags::eBit3_Render))
            {
                mAnim.mFlags.Clear(AnimFlags::eBit3_Render);

                if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit5_Delete))
                {
                    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                }
                SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 900);
                SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, -100);
                field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
            }
        }
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (!field_118_flags.Get(SlamDoor_Flags_118::e118_Bit3_bLastFrame))
            {
                field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
                SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 900);
                SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, -100);
            }
        }
    }

    if (stateUnchanged != field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        field_118_flags.Clear(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
        field_118_flags.Toggle(SlamDoor_Flags_118::e118_Bit1_bClosed);

        if (stateUnchanged)
        {
            mAnim.mFlags.Set(AnimFlags::eBit3_Render);

            mAnim.Set_Animation_Data(sSlamDoorAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][2], nullptr);

            if (mSpriteScale == FP_FromInteger(1))
            {
                field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * FP_FromInteger(1))),
                    field_124_x1,
                    field_126_y1,
                    eLineTypes::eWallLeft_1);
                field_120_pCollisionLine_5_1 = sCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(mSpriteScale) + FP_FromInteger(field_124_x1)),
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * mSpriteScale)),
                    FP_GetExponent(FP_FromInteger(field_124_x1) + ScaleToGridSize(mSpriteScale)),
                    field_126_y1,
                    eLineTypes::eWallRight_2);
            }
            else
            {
                field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * mSpriteScale)),
                    field_124_x1,
                    field_126_y1,
                    eLineTypes::eBackgroundWallLeft_5);
                field_120_pCollisionLine_5_1 = sCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(mSpriteScale) + FP_FromInteger(field_124_x1)),
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * mSpriteScale)),
                    FP_GetExponent(FP_FromInteger(field_124_x1) + ScaleToGridSize(mSpriteScale)),
                    field_126_y1,
                    eLineTypes::eBackgroundWallRight_6);
            }

            PSX_RECT bRect = VGetBoundingRect();

            if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
            {
                bRect.y += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale);
                bRect.h += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale);
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
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
            mAnim.Set_Animation_Data(sSlamDoorAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0], nullptr);
            Rect_Clear(&field_11C_pCollisionLine_6_2->mRect);
            field_11C_pCollisionLine_6_2 = nullptr;

            Rect_Clear(&field_120_pCollisionLine_5_1->mRect);
            field_120_pCollisionLine_5_1 = nullptr;
        }
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        PSX_RECT bRect = VGetBoundingRect();

        if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
        {
            bRect.y += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale);
            bRect.h += FP_GetExponent(FP_FromInteger(-110) * mSpriteScale) - FP_GetExponent(FP_FromInteger(20) * mSpriteScale);
        }

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
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

    pSaveState->field_0_id = AETypes::eSlamDoor_122;
    pSaveState->field_4_tlv = field_12C_tlvInfo;

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

    relive_new SlamDoor(static_cast<Path_SlamDoor*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pSaveState->field_4_tlv.all)), pSaveState->field_4_tlv);

    return sizeof(Quicksave_Obj_SlamDoor);
}
