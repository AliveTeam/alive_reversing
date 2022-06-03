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
    {LevelIds_s8::eMines_1, 102u, 87u, 118u},
    {LevelIds_s8::eNecrum_2, 102u, 87u, 118u},
    {LevelIds_s8::eMudomoVault_3, 102u, 87u, 118u},
    {LevelIds_s8::eMudancheeVault_4, 102u, 87u, 118u},
    {LevelIds_s8::eFeeCoDepot_5, 102u, 87u, 118u},
    {LevelIds_s8::eBarracks_6, 102u, 87u, 118u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 102u, 87u, 118u},
    {LevelIds_s8::eBonewerkz_8, 102u, 87u, 118u},
    {LevelIds_s8::eBrewery_9, 102u, 87u, 118u},
    {LevelIds_s8::eBrewery_Ender_10, 102u, 87u, 118u},
    {LevelIds_s8::eMudomoVault_Ender_11, 102u, 87u, 118u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 102u, 87u, 118u},
    {LevelIds_s8::eBarracks_Ender_13, 102u, 87u, 118u},
    {LevelIds_s8::eBonewerkz_Ender_14, 102u, 87u, 118u},
    {LevelIds_s8::eNone, 102u, 87u, 118u}};

struct Quicksave_Obj_SlamDoor final
{
    AETypes field_0_id;
    TlvItemInfoUnion field_4_tlv;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Quicksave_Obj_SlamDoor, 8);

SlamDoor::SlamDoor(Path_SlamDoor* pTlv, TlvItemInfoUnion tlvInfo)
    : BaseAliveGameObject(0)
{
    field_C_objectId = tlvInfo.all; // todo: check this
    mFlags.Set(Options::eCanExplode_Bit7);

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

    const s32 currentLevelId = static_cast<s32>(gMap.mCurrentLevel);

    const AnimRecord& rec = AnimRec(sSlamDoorData_547168[currentLevelId][2]);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId), 1, 1u);

    SetType(AETypes::eSlamDoor_122);

    field_B8_xpos = FP_FromInteger(((pTlv->field_8_top_left.field_0_x
                                     + pTlv->field_C_bottom_right.field_0_x)
                                    / 2));

    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);
    field_12C_tlvInfo = tlvInfo;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_Half_6;
        field_D6_scale = 0;
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1.0);
        field_20_animation.field_C_render_layer = Layer::eLayer_BeforeShadow_25;
        field_D6_scale = 1;
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit6_FlipY);
        field_D8_yOffset = FP_GetExponent(field_CC_sprite_scale * FP_FromDouble(-68.0));
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

    if (sCollisions_DArray_5C1128->Raycast(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos + FP_FromDouble(24.0),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        field_BC_ypos = hitY;
    }

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_124_x1 = FP_GetExponent((ScaleToGridSize(field_CC_sprite_scale) / FP_FromDouble(2.0)) + FP_FromInteger(FP_GetExponent(field_B8_xpos)));
    }
    else
    {
        field_124_x1 = FP_GetExponent(FP_FromInteger(FP_GetExponent(field_B8_xpos)) - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromDouble(2.0)));
    }

    field_126_y1 = FP_GetExponent(field_BC_ypos);

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        PathLine* pPathLine = nullptr;

        if (field_CC_sprite_scale == FP_FromDouble(1.0))
        {
            const FP lineHeight = FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                2);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize(field_CC_sprite_scale);
            const FP y1 = FP_FromInteger(field_126_y1)
                        - (field_CC_sprite_scale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(field_CC_sprite_scale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                1);
        }
        else
        {
            const FP lineHeight = field_CC_sprite_scale * FP_FromDouble(80.0);

            field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                field_124_x1,
                FP_GetExponent(FP_FromInteger(field_126_y1) - lineHeight),
                field_124_x1,
                field_126_y1,
                6);
            const FP x2 = FP_FromInteger(field_124_x1) + ScaleToGridSize(field_CC_sprite_scale);
            const FP y1 = FP_FromInteger(field_126_y1) - (field_CC_sprite_scale * FP_FromDouble(80.0));
            const FP x1 = ScaleToGridSize(field_CC_sprite_scale) + FP_FromInteger(field_124_x1);
            pPathLine = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                FP_GetExponent(x1),
                FP_GetExponent(y1),
                FP_GetExponent(x2),
                field_126_y1,
                5);
        }
        field_120_pCollisionLine_5_1 = pPathLine;

        const AnimRecord& animRec = AnimRec(sSlamDoorData_547168[currentLevelId][1]);
        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, 0);
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_11C_pCollisionLine_6_2 = 0;
        field_120_pCollisionLine_5_1 = 0;
    }

    field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
    field_DC_bApplyShadows |= 2u;
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
        mFlags.Set(BaseGameObject::eDead);
    }

    const bool stateUnchanged = SwitchStates_Get(field_128_switch_id) == static_cast<s32>(field_118_flags.Get(SlamDoor_Flags_118::e118_Bit2_Open));
    if (!field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

                if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit5_Delete))
                {
                    mFlags.Set(BaseGameObject::eDead);
                }
                SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, 900);
                SFX_Play_Pitch(SoundEffect::DoorEffect_57, 100, -100);
                field_118_flags.Set(SlamDoor_Flags_118::e118_Bit3_bLastFrame);
            }
        }
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
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
            field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

            const AnimRecord& animRec = AnimRec(sSlamDoorData_547168[static_cast<s32>(gMap.mCurrentLevel)][2]);
            field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

            if (field_CC_sprite_scale == FP_FromInteger(1))
            {
                field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * FP_FromInteger(1))),
                    field_124_x1,
                    field_126_y1,
                    1);
                field_120_pCollisionLine_5_1 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(field_CC_sprite_scale) + FP_FromInteger(field_124_x1)),
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * field_CC_sprite_scale)),
                    FP_GetExponent(FP_FromInteger(field_124_x1) + ScaleToGridSize(field_CC_sprite_scale)),
                    field_126_y1,
                    2);
            }
            else
            {
                field_11C_pCollisionLine_6_2 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                    field_124_x1,
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * field_CC_sprite_scale)),
                    field_124_x1,
                    field_126_y1,
                    5);
                field_120_pCollisionLine_5_1 = sCollisions_DArray_5C1128->Add_Dynamic_Collision_Line(
                    FP_GetExponent(ScaleToGridSize(field_CC_sprite_scale) + FP_FromInteger(field_124_x1)),
                    FP_GetExponent(FP_FromInteger(field_126_y1) - (FP_FromInteger(80) * field_CC_sprite_scale)),
                    FP_GetExponent(FP_FromInteger(field_124_x1) + ScaleToGridSize(field_CC_sprite_scale)),
                    field_126_y1,
                    6);
            }

            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
            {
                bRect.y += FP_GetExponent(FP_FromInteger(-110) * field_CC_sprite_scale);
                bRect.h += FP_GetExponent(FP_FromInteger(-110) * field_CC_sprite_scale);
            }

            for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
            {
                auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
                {
                    if (pObj->Type() != AETypes::eSlamDoor_122)
                    {
                        PSX_RECT bObjRect = {};
                        pObj->VGetBoundingRect(&bObjRect, 1);

                        // Some hack that prevents Abe getting knocked back when rolling or falling near a closing slam door
                        bObjRect.x += 3;

                        if (PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect) && pObj->field_CC_sprite_scale == field_CC_sprite_scale)
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
        else
        {
            const AnimRecord& animRec = AnimRec(sSlamDoorData_547168[static_cast<s32>(gMap.mCurrentLevel)][0]);
            field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, 0);
            Rect_Clear(&field_11C_pCollisionLine_6_2->field_0_rect);
            field_11C_pCollisionLine_6_2 = nullptr;

            Rect_Clear(&field_120_pCollisionLine_5_1->field_0_rect);
            field_120_pCollisionLine_5_1 = nullptr;
        }
    }

    if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit1_bClosed))
    {
        PSX_RECT bRect = {};
        VGetBoundingRect(&bRect, 1);

        if (field_118_flags.Get(SlamDoor_Flags_118::e118_Bit4_Inverted))
        {
            bRect.y += FP_GetExponent(FP_FromInteger(-110) * field_CC_sprite_scale);
            bRect.h += FP_GetExponent(FP_FromInteger(-110) * field_CC_sprite_scale) - FP_GetExponent(FP_FromInteger(20) * field_CC_sprite_scale);
        }

        for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
        {
            auto pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
            {
                if (pObj->Type() != AETypes::eSlamDoor_122 && pObj->Type() != AETypes::eGrenade_65)
                {
                    PSX_RECT bObjRect = {};
                    pObj->VGetBoundingRect(&bObjRect, 1);

                    if (FP_GetExponent(pObj->field_B8_xpos) > bRect.x && FP_GetExponent(pObj->field_B8_xpos) < bRect.w && PSX_Rects_overlap_no_adjustment(&bRect, &bObjRect))
                    {
                        if (pObj->field_CC_sprite_scale == field_CC_sprite_scale || (pObj->Type() == AETypes::eSlog_126 && field_CC_sprite_scale == FP_FromInteger(1)))
                        {
                            ClearInsideSlamDoor(pObj, bRect.x, bRect.w);
                        }
                    }
                }
            }
        }
    }

    mFlags.Set(BaseGameObject::eCanExplode_Bit7, field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render));
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
    if (FP_GetExponent(pObj->field_B8_xpos) - xPosition >= width - FP_GetExponent(pObj->field_B8_xpos))
    {
        pObj->field_B8_xpos = (ScaleToGridSize(field_CC_sprite_scale) * FP_FromDouble(0.5)) + FP_FromDouble(1.0) + pObj->field_B8_xpos;
    }
    else
    {
        pObj->field_B8_xpos = pObj->field_B8_xpos - (ScaleToGridSize(field_CC_sprite_scale) * FP_FromDouble(0.5));
    }

    if (pObj->Type() == AETypes::eMudokon2_81 || pObj->Type() == AETypes::eMudokon_110 || pObj->Type() == AETypes::eAbe_69)
    {
        pObj->VTakeDamage(this);
    }

    if (pObj->Type() == AETypes::eBone_11)
    {
        pObj->field_C8_vely = FP_FromInteger(-7);
        pObj->field_C4_velx = -pObj->field_C4_velx;
    }
}

s32 SlamDoor::CreateFromSaveState(const u8* pData)
{
    const Quicksave_Obj_SlamDoor* pSaveState = reinterpret_cast<const Quicksave_Obj_SlamDoor*>(pData);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlamResID, 0, 0))
    {
        switch (gMap.mCurrentLevel)
        {
            case LevelIds::eNecrum_2:
            case LevelIds::eMudomoVault_3:
            case LevelIds::eMudancheeVault_4:
            case LevelIds::eMudancheeVault_Ender_7:
            case LevelIds::eMudomoVault_Ender_11:
                ResourceManager::LoadResourceFile_49C170("SLAMVLTS.BAN", nullptr);
                break;
            default:
                ResourceManager::LoadResourceFile_49C170("SLAM.BAN", nullptr);
                break;
        }
    }

    ae_new<SlamDoor>(static_cast<Path_SlamDoor*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pSaveState->field_4_tlv.all)), pSaveState->field_4_tlv);

    return sizeof(Quicksave_Obj_SlamDoor);
}
