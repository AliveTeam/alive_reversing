#include "stdafx.h"
#include "Gibs.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "ScreenManager.hpp"
#include "AnimResources.hpp"

const TintEntry kGibTints_55C744[] = {
    {LevelIds_s8::eMines_1, 87u, 103u, 67u},
    {LevelIds_s8::eNecrum_2, 87u, 103u, 67u},
    {LevelIds_s8::eMudomoVault_3, 87u, 103u, 67u},
    {LevelIds_s8::eMudancheeVault_4, 87u, 103u, 67u},
    {LevelIds_s8::eFeeCoDepot_5, 87u, 103u, 67u},
    {LevelIds_s8::eBarracks_6, 87u, 103u, 67u},
    {LevelIds_s8::eMudancheeVault_Ender_7, 87u, 103u, 67u},
    {LevelIds_s8::eBonewerkz_8, 87u, 103u, 67u},
    {LevelIds_s8::eBrewery_9, 87u, 103u, 67u},
    {LevelIds_s8::eBrewery_Ender_10, 87u, 103u, 67u},
    {LevelIds_s8::eMudomoVault_Ender_11, 87u, 103u, 67u},
    {LevelIds_s8::eFeeCoDepot_Ender_12, 87u, 103u, 67u},
    {LevelIds_s8::eBarracks_Ender_13, 87u, 103u, 67u},
    {LevelIds_s8::eBonewerkz_Ender_14, 87u, 103u, 67u},
    {LevelIds_s8::eNone, 87u, 103u, 67u}};

ALIVE_VAR(1, 0x550e80, s16, sGibRandom_550E80, 13);

FP Random_40FAF0(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << sGibRandom_550E80) * scale;
}

Gibs::Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, bool bMakeSmaller)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_F4_not_used = nullptr;

    AnimRecord headGib = {};
    AnimRecord armGib = {};
    AnimRecord bodyGib = {};

    switch (gibType)
    {
        case GibType::Abe_0:
            [[fallthrough]];

        case GibType::Mud_3:
            headGib = AnimRec(AnimId::Abe_Head_Gib);
            armGib = AnimRec(AnimId::Abe_Arm_Gib);
            bodyGib = AnimRec(AnimId::Abe_Body_Gib);
            break;

        case GibType::Slig_1:
            headGib = AnimRec(AnimId::Slig_Head_Gib);
            armGib = AnimRec(AnimId::Slig_Arm_Gib);
            bodyGib = AnimRec(AnimId::Slig_Body_Gib);
            break;

        case GibType::Slog_2:
            headGib = AnimRec(AnimId::Slog_Head_Gib);
            armGib = AnimRec(AnimId::Slog_Body_Gib); // No arms
            bodyGib = AnimRec(AnimId::Slog_Body_Gib);
            break;

        case GibType::BlindMud_4:
            headGib = AnimRec(AnimId::BlindMud_Head_Gib);
            armGib = AnimRec(AnimId::BlindMud_Arm_Gib);
            bodyGib = AnimRec(AnimId::BlindMud_Body_Gib);
            break;

        case GibType::Metal_5:
            // No body parts, all metal
            headGib = AnimRec(AnimId::Metal_Gib);
            armGib = AnimRec(AnimId::Metal_Gib);
            bodyGib = AnimRec(AnimId::Metal_Gib);
            break;

        case GibType::Glukkon_6:
            headGib = AnimRec(AnimId::Glukkon_Head_Gib);
            armGib = AnimRec(AnimId::Glukkon_Arm_Gib);
            bodyGib = AnimRec(AnimId::Glukkon_Body_Gib);
            break;

        case GibType::Aslik_7:
            headGib = AnimRec(AnimId::Aslik_Head_Gib);
            armGib = AnimRec(AnimId::Aslik_Arm_Gib);
            bodyGib = AnimRec(AnimId::Aslik_Body_Gib);
            break;

        case GibType::Dripik_8:
            headGib = AnimRec(AnimId::Dripik_Head_Gib);
            armGib = AnimRec(AnimId::Dripik_Arm_Gib);
            bodyGib = AnimRec(AnimId::Dripik_Body_Gib);
            break;

        case GibType::Phleg_9:
            headGib = AnimRec(AnimId::Phleg_Head_Gib);
            armGib = AnimRec(AnimId::Phleg_Arm_Gib);
            bodyGib = AnimRec(AnimId::Phleg_Body_Gib);
            break;

        case GibType::Fleech_10:
            headGib = AnimRec(AnimId::Fleech_Head_Gib);
            armGib = AnimRec(AnimId::Fleech_Body_Gib); // No arms
            bodyGib = AnimRec(AnimId::Fleech_Body_Gib);
            break;
    }

    // TODO: It is assumed all 3 gib parts have the same resource id - might not be true for mods
    u8** ppAnimData = Add_Resource(ResourceManager::Resource_Animation, headGib.mResourceId);

    // TODO: It is assumed all 3 gib parts use the same pal - might not be true for mods
    u8** ppRes = nullptr;
    if (headGib.mPalOverride != PalId::Default)
    {
        ppRes = Add_Resource(ResourceManager::Resource_Palt, PalRec(headGib.mPalOverride).mResourceId);
    }

    // The base class renders the head gib
    Animation_Init(
        headGib.mFrameTableOffset,
        headGib.mMaxW,
        headGib.mMaxH,
        ppAnimData,
        1,
        1);

    if (mFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        return;
    }

    field_CC_sprite_scale = scale;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos + FP_FromInteger(2);

    field_100_timer = sGnFrame_5C1B84 + 91;

    if (scale == FP_FromInteger(1))
    {
        field_F8_z = FP_FromInteger(0);
        field_20_animation.field_C_render_layer = Layer::eLayer_FG1_37;
        field_D6_scale = 1;
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_F8_z = FP_FromInteger(100);
        field_20_animation.field_C_render_layer = Layer::eLayer_Foreground_Half_17;
        field_D6_scale = 0;
    }
    else
    {
        // Not a valid scale
        mFlags.Set(BaseGameObject::eDead);
    }

    field_5D6_bMakeSmaller = bMakeSmaller;
    field_C4_velx = xOff + Random_40FAF0(scale);

    // OG Bug? WTF?? Looks like somehow they didn't condition this param correctly
    // because field_C8_vely and field_FC_dz are always overwritten
    if (!field_5D6_bMakeSmaller)
    {
        field_C8_vely = yOff + Random_40FAF0(scale);
        field_FC_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(2));
    }

    sGibRandom_550E80 = 12;

    field_C8_vely = (yOff + Random_40FAF0(scale)) / FP_FromInteger(2);
    field_FC_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(4));

    if (gibType == GibType::Abe_0)
    {
        SetTint_425600(sTintTable_Abe_554D20, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::Mud_3)
    {
        SetTint_425600(kGibTints_55C744, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::BlindMud_4)
    {
        field_D0_r = 63;
        field_D2_g = 63;
        field_D4_b = 63;
    }

    field_5D4_parts_used_count = 4;

    GibPart* pPart = &field_104_parts[0];
    for (s16 i = 0; i < field_5D4_parts_used_count; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->field_18_anim.Init_40A030(
                    armGib.mFrameTableOffset,
                    gObjList_animations_5C1A24,
                    this,
                    armGib.mMaxW,
                    armGib.mMaxH,
                    ppAnimData,
                    1,
                    0,
                    0))
            {
                field_5D4_parts_used_count = i;
                mFlags.Set(BaseGameObject::eDead);
                return;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->field_18_anim.Init_40A030(
                    bodyGib.mFrameTableOffset,
                    gObjList_animations_5C1A24,
                    this,
                    bodyGib.mMaxW,
                    bodyGib.mMaxH,
                    ppAnimData,
                    1u,
                    0,
                    0))
            {
                field_5D4_parts_used_count = i;
                mFlags.Set(BaseGameObject::eDead);
                return;
            }
        }

        pPart->field_18_anim.field_C_render_layer = field_20_animation.field_C_render_layer;
        pPart->field_18_anim.field_14_scale = scale;
        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        pPart->field_18_anim.field_8_r = static_cast<u8>(field_D0_r);
        pPart->field_18_anim.field_9_g = static_cast<u8>(field_D2_g);
        pPart->field_18_anim.field_A_b = static_cast<u8>(field_D4_b);

        pPart->field_0_x = field_B8_xpos;
        pPart->field_4_y = field_BC_ypos;
        pPart->field_8_z = field_F8_z;

        pPart->field_C_dx = xOff + Random_40FAF0(scale);

        if (field_5D6_bMakeSmaller)
        {
            pPart->field_10_dy = (yOff + Random_40FAF0(scale)) / FP_FromInteger(2);
            pPart->field_14_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(4));
        }
        else
        {
            pPart->field_10_dy = yOff + Random_40FAF0(scale);
            pPart->field_14_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(2));
        }

        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

        if (ppRes)
        {
            pPart->field_18_anim.Load_Pal_40A530(ppRes, 0);
        }

        pPart++;
    }
}

Gibs::~Gibs()
{
    for (s32 i = 0; i < field_5D4_parts_used_count; i++)
    {
        field_104_parts[i].field_18_anim.vCleanUp_40C630();
    }
}

void Gibs::VUpdate()
{
    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;
    field_F8_z += field_FC_dz;

    field_C8_vely += FP_FromDouble(0.25);

    if (field_F8_z + FP_FromInteger(100) < FP_FromInteger(15))
    {
        const FP dz = -field_FC_dz;
        field_FC_dz = dz;
        field_F8_z += dz;
    }

    for (s32 i = 0; i < field_5D4_parts_used_count; i++)
    {
        field_104_parts[i].field_0_x += field_104_parts[i].field_C_dx;
        field_104_parts[i].field_4_y += field_104_parts[i].field_10_dy;
        field_104_parts[i].field_8_z += field_104_parts[i].field_14_dz;

        field_104_parts[i].field_10_dy += FP_FromDouble(0.25);

        if (field_104_parts[i].field_8_z + FP_FromInteger(100) < FP_FromInteger(15))
        {
            const FP dz = -field_104_parts[i].field_14_dz;
            field_104_parts[i].field_14_dz = dz;
            field_104_parts[i].field_8_z += dz;
        }
    }

    if (static_cast<s32>(sGnFrame_5C1B84) > field_100_timer)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

void Gibs::VRender(PrimHeader** ppOt)
{
    if (sNum_CamSwappers_5C1B66 > 0)
    {
        // Don't do anything during screen change
        return;
    }

    field_CC_sprite_scale = FP_FromInteger(100) / (field_F8_z + FP_FromInteger(100));
    if (field_5D6_bMakeSmaller)
    {
        field_CC_sprite_scale /= FP_FromInteger(2);
    }

    // Head part rendering
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

    const FP_Point* pCamPos = pScreenManager_5BB5F4->field_20_pCamPos;
    const FP camXPos = pCamPos->field_0_x;
    const FP camYPos = pCamPos->field_4_y;

    for (s32 i = 0; i < field_5D4_parts_used_count; i++)
    {
        // Part is within camera X?
        if (field_104_parts[i].field_0_x >= camXPos && field_104_parts[i].field_0_x <= camXPos + FP_FromInteger(640))
        {
            // Part is within camera Y?
            if (field_104_parts[i].field_4_y >= camYPos && field_104_parts[i].field_4_y <= camYPos + FP_FromInteger(240))
            {
                field_104_parts[i].field_18_anim.field_14_scale = FP_FromInteger(100) / (field_104_parts[i].field_8_z + FP_FromInteger(100));

                if (field_5D6_bMakeSmaller)
                {
                    field_104_parts[i].field_18_anim.field_14_scale /= FP_FromInteger(2);
                }

                if (field_104_parts[i].field_18_anim.field_14_scale < FP_FromInteger(1))
                {
                    field_104_parts[i].field_18_anim.field_C_render_layer = Layer::eLayer_Foreground_Half_17;
                }
                else
                {
                    field_104_parts[i].field_18_anim.field_C_render_layer = Layer::eLayer_FG1_37;
                }

                if (field_104_parts[i].field_18_anim.field_14_scale <= FP_FromInteger(1))
                {
                    const s32 xpos = FP_GetExponent(field_104_parts[i].field_0_x - camXPos);
                    const s32 ypos = FP_GetExponent(field_104_parts[i].field_4_y - camYPos);

                    field_104_parts[i].field_18_anim.vRender_40B820(xpos, ypos, ppOt, 0, 0);

                    PSX_RECT frameRect = {};
                    field_104_parts[i].field_18_anim.Get_Frame_Rect_409E10(&frameRect);
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h,
                        pScreenManager_5BB5F4->field_3A_idx);
                }
            }
        }
    }
}
