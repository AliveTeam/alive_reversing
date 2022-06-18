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

const AnimId sSlamDoorData_547168[15][3] = {
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

    const AnimRecord& rec = AnimRec(sSlamDoorData_547168[currentLevelId][2]);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId), 1);

    SetType(ReliveTypes::eSlamDoor);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger(((pTlv->field_8_top_left.field_0_x
                                     + pTlv->field_C_bottom_right.field_0_x)
                                    / 2));

    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_12C_tlvInfo = tlvInfo;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(1.0);
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit6_FlipY);
        mBaseAnimatedWithPhysicsGameObject_YOffset = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(-68.0));
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
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromDouble(24.0),
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            mBaseAnimatedWithPhysicsGameObject_Scale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
    {
        field_124_x1 = FP_GetExponent((ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromDouble(2.0)) + FP_FromInteger(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)));
    }
    else
    {
        field_124_x1 = FP_GetExponent(FP_FromInteger(FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_XPos)) - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) / FP_FromDouble(2.0)));
    }

    field_126_y1 = FP_GetExponent(mBaseAnimatedWithPhysicsGameObject_YPos);

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        PathLine* pPathLine = nullptr;

        if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromDouble(1.0))
        {
            const FP lineHeight = FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                2);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            const FP y1 = FP_FromInteger(field_126_y1)
                        - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                1);
        }
        else
        {
            const FP lineHeight = mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                6);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            const FP y1 = FP_FromInteger(field_126_y1) - (mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                5);
        }
        field_120_pCollisionLine_5_1 = pPathLine;

        const AnimRecord& animRec = AnimRec(sSlamDoorData_547168[currentLevelId][1]);
        mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, 0);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
        field_11C_pCollisionLine_6_2 = 0;
        field_120_pCollisionLine_5_1 = 0;
    }

    field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
    mApplyShadows |= 2u;
}

SlamDoor::~SlamDoor()
{
    if (!(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit5_Delete)) || field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        Path::TLV_Reset(field_12C_tlvInfo.all, -1, 0, 0);
    }

    if (field_11C_pCollisionLine_6_2)
    {
        Rect_Clear(&field_11C_pCollisionLine_6_2->field_0_rect);
    }

    if (field_120_pCollisionLine_5_1)
    {
        Rect_Clear(&field_120_pCollisionLine_5_1->field_0_rect);
    }
}

void SlamDoor::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const bool stateUnchanged = SwitchStates_Get(field_128_switch_id) == static_cast<s32>(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit2_Open));
    if (!field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);

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
        if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);

            const AnimRecord& animRec = AnimRec(sSlamDoorData_547168[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][2]);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
            {
                field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * FP_FromInteger(1))),
                    field_124_x1,
                    field_126_y1,
                    1);
                field_120_pCollisionLine_5_1 = sCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + FP_FromInteger(field_124_x1)),
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    FP_GetExponent(FP_FromInteger(field_124_x1) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    field_126_y1,
                    2);
            }
            else
            {
                field_11C_pCollisionLine_6_2 = sCollisions->Add_Dynamic_Collision_Line(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    field_124_x1,
                    field_126_y1,
                    5);
                field_120_pCollisionLine_5_1 = sCollisions->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) + FP_FromInteger(field_124_x1)),
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    FP_GetExponent(FP_FromInteger(field_124_x1) + ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale)),
                    field_126_y1,
                    6);
            }

            PSX_RECT bRect = VGetBoundingRect();

            if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
            {
                bRect.y += FP_GetExponent(FP_FromInteger(-110) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
                bRect.h += FP_GetExponent(FP_FromInteger(-110) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                if (pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
                {
                    if (pObj->Type() != ReliveTypes::eSlamDoor)
                    {
                        PSX_RECT bObjRect = pObj->VGetBoundingRect();

                        // Some hack that prevents Abe getting knocked back when rolling or falling near a closing slam door
                        bObjRect.x += 3;

                        if (PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect) && pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale)
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
        else
        {
            const AnimRecord& animRec = AnimRec(sSlamDoorData_547168[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]);
            mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(animRec.mFrameTableOffset, 0);
            Rect_Clear(&field_11C_pCollisionLine_6_2->field_0_rect);
            field_11C_pCollisionLine_6_2 = nullptr;

            Rect_Clear(&field_120_pCollisionLine_5_1->field_0_rect);
            field_120_pCollisionLine_5_1 = nullptr;
        }
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        PSX_RECT bRect = VGetBoundingRect();

        if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
        {
            bRect.y += FP_GetExponent(FP_FromInteger(-110) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
            bRect.h += FP_GetExponent(FP_FromInteger(-110) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) - FP_GetExponent(FP_FromInteger(20) * mBaseAnimatedWithPhysicsGameObject_SpriteScale);
        }

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render))
            {
                if (pObj->Type() != ReliveTypes::eSlamDoor && pObj->Type() != ReliveTypes::eGrenade)
                {
                    const PSX_RECT bObjRect = pObj->VGetBoundingRect();

                    if (FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos) > bRect.x && FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos) < bRect.w && PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect))
                    {
                        if (pObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale == mBaseAnimatedWithPhysicsGameObject_SpriteScale || (pObj->Type() == ReliveTypes::eSlog && mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1)))
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
    }

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render));
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
    if (FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos) - xPosition >= width - FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos))
    {
        pObj->mBaseAnimatedWithPhysicsGameObject_XPos = (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(0.5)) + FP_FromDouble(1.0) + pObj->mBaseAnimatedWithPhysicsGameObject_XPos;
    }
    else
    {
        pObj->mBaseAnimatedWithPhysicsGameObject_XPos = pObj->mBaseAnimatedWithPhysicsGameObject_XPos - (ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromDouble(0.5));
    }

    if (pObj->Type() == ReliveTypes::eRingOrLiftMud || pObj->Type() == ReliveTypes::eMudokon || pObj->Type() == ReliveTypes::eAbe)
    {
        pObj->VTakeDamage(this);
    }

    if (pObj->Type() == ReliveTypes::eBone)
    {
        pObj->mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-7);
        pObj->mBaseAnimatedWithPhysicsGameObject_VelX = -pObj->mBaseAnimatedWithPhysicsGameObject_VelX;
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

    relive_new SlamDoor(static_cast<Path_SlamDoor*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pSaveState->field_4_tlv.all)), pSaveState->field_4_tlv);

    return sizeof(Quicksave_Obj_SlamDoor);
}
