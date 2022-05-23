#include "stdafx_ao.h"
#include "Function.hpp"
#include "Gibs.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "stdlib.hpp"
#include "CameraSwapper.hpp"
#include "ScreenManager.hpp"

namespace AO {

const Gib_Data kGibData_4C30B0[6] = {
    {AnimId::Abe_Head_Gib, AnimId::Abe_Arm_Gib, AnimId::Abe_Body_Gib},
    {AnimId::Slig_Head_Gib, AnimId::Slig_Arm_Gib, AnimId::Slig_Body_Gib},
    {AnimId::Slog_Head_Gib, AnimId::Slog_Body_Gib, AnimId::Slog_Body_Gib},
    {AnimId::Elum_Head_Gib, AnimId::Elum_Arm_Gib, AnimId::Elum_Body_Gib},
    {AnimId::Abe_Head_Gib, AnimId::Abe_Arm_Gib, AnimId::Abe_Body_Gib},
    {AnimId::Metal_Gib, AnimId::Metal_Gib, AnimId::Metal_Gib}};

const TintEntry sAbeGibTints_4C6438[5] = {
    {LevelIds_s8::eStockYards_5, 25u, 25u, 25u},
    {LevelIds_s8::eStockYardsReturn_6, 25u, 25u, 25u},
    {LevelIds_s8::eDesert_8, 125u, 125u, 95u},
    {LevelIds_s8::eDesertTemple_9, 120u, 120u, 90u},
    {LevelIds_s8::eNone, 102u, 102u, 102u}};

const TintEntry sMudGibTints_4CD320[3] = {
    {LevelIds_s8::eStockYards_5, 25u, 25u, 25u},
    {LevelIds_s8::eStockYardsReturn_6, 25u, 25u, 25u},
    {LevelIds_s8::eNone , 87u, 103u, 67u},
};

static FP GibRand(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << 13) * scale;
}

Gibs* Gibs::ctor_407B20(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale)
{
    

    for (GibPart& part : field_F4_parts)
    {
        SetVTable(&part.field_18_anim, 0x4BA2B8);
    }
    SetVTable(this, 0x4BA280);

    field_E4_pGibData = &kGibData_4C30B0[gibType];
    const AnimRecord& headRec = AO::AnimRec(field_E4_pGibData->field_0_head);
    const AnimRecord& armRec = AO::AnimRec(field_E4_pGibData->field_4_arm);
    const AnimRecord& bodyRec = AO::AnimRec(field_E4_pGibData->field_8_body);
    u8** ppAnimData = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, headRec.mResourceId, 1, 0);

    // The base class renders the head gib
    Animation_Init_417FD0(
        headRec.mFrameTableOffset,
        static_cast<s16>(headRec.mMaxW),
        static_cast<s16>(headRec.mMaxH),
        ppAnimData,
        1);

    field_BC_sprite_scale = scale;
    field_A8_xpos = xpos;
    field_AC_ypos = ypos + FP_FromInteger(2);

    field_F0_timer = gnFrameCount_507670 + 91;

    if (scale == FP_FromInteger(1))
    {
        field_E8_z = FP_FromInteger(0);
        field_10_anim.field_C_layer = Layer::eLayer_FG1_37;
        field_C6_scale = 1;
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_E8_z = FP_FromInteger(100);
        field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
        field_C6_scale = 0;
    }
    else
    {
        // Not a valid scale
        mFlags.Set(BaseGameObject::eDead);
    }

    field_B4_velx = xOff + GibRand(scale);
    field_B8_vely = yOff + GibRand(scale);

    if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
    {
        field_EC_dz = -(GibRand(scale) / FP_FromInteger(2));
    }
    else
    {
        field_EC_dz = GibRand(scale) / FP_FromInteger(2);
    }

    u8** ppPal = nullptr;
    if (gMap.mCurrentLevel == LevelIds::eStockYards_5 || gMap.mCurrentLevel == LevelIds::eStockYardsReturn_6)
    {
        if (gibType == GibType::Abe_0 || gibType == GibType::Mud_4)
        {
            ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0);
        }
        else if (gibType == GibType::Slog_2)
        {
            ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0);
        }
    }

    if (gibType == GibType::Abe_0)
    {
        SetTint_418750(sAbeGibTints_4C6438, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::Mud_4)
    {
        SetTint_418750(sMudGibTints_4CD320, gMap.mCurrentLevel);
    }

    field_5C4_parts_used_count = 7;

    GibPart* pPart = &field_F4_parts[0];
    for (s16 i = 0; i < field_5C4_parts_used_count; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->field_18_anim.Init_402D20(
                    armRec.mFrameTableOffset,
                    gObjList_animations_505564,
                    this,
                    static_cast<s16>(armRec.mMaxW),
                    static_cast<s16>(armRec.mMaxH),
                    ppAnimData,
                    1,
                    0,
                    0))
            {
                field_5C4_parts_used_count = i;
                mFlags.Set(BaseGameObject::eDead);
                return this;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->field_18_anim.Init_402D20(
                    bodyRec.mFrameTableOffset,
                    gObjList_animations_505564,
                    this,
                    static_cast<s16>(bodyRec.mMaxW),
                    static_cast<s16>(bodyRec.mMaxH),
                    ppAnimData,
                    1,
                    0,
                    0))
            {
                field_5C4_parts_used_count = i;
                mFlags.Set(BaseGameObject::eDead);
                return this;
            }
        }

        pPart->field_18_anim.field_C_layer = field_10_anim.field_C_layer;
        pPart->field_18_anim.field_14_scale = scale;

        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit17_bFreeResource); // Else the gibs seem to kill drills and other objects ??
        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

        pPart->field_18_anim.field_8_r = static_cast<u8>(field_C0_r);
        pPart->field_18_anim.field_9_g = static_cast<u8>(field_C2_g);
        pPart->field_18_anim.field_A_b = static_cast<u8>(field_C4_b);

        pPart->field_0_x = field_A8_xpos;
        pPart->field_4_y = field_AC_ypos;
        pPart->field_8_z = field_E8_z;

        pPart->field_C_dx = xOff + GibRand(scale);
        pPart->field_10_dy = yOff + GibRand(scale);

        if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
        {
            pPart->field_14_dz = -(GibRand(scale) / FP_FromInteger(2));
        }
        else
        {
            pPart->field_14_dz = GibRand(scale) / FP_FromInteger(2);
        }

        if (ppPal)
        {
            pPart->field_18_anim.LoadPal_403090(ppPal, 0);
        }

        pPart++;
    }

    return this;
}

void Gibs::VUpdate()
{
    VUpdate_4080C0();
}

void Gibs::VUpdate_4080C0()
{
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;
    field_E8_z += field_EC_dz;

    field_B8_vely += FP_FromDouble(0.25);

    if (field_E8_z + FP_FromInteger(100) < FP_FromInteger(15))
    {
        field_E8_z -= field_EC_dz;
        field_EC_dz = -field_EC_dz;
    }

    for (s32 i = 0; i < field_5C4_parts_used_count; i++)
    {
        field_F4_parts[i].field_0_x += field_F4_parts[i].field_C_dx;
        field_F4_parts[i].field_4_y += field_F4_parts[i].field_10_dy;
        field_F4_parts[i].field_8_z += field_F4_parts[i].field_14_dz;

        field_F4_parts[i].field_10_dy += FP_FromDouble(0.25);

        if ((field_F4_parts[i].field_8_z + FP_FromInteger(100)) < FP_FromInteger(15))
        {
            field_F4_parts[i].field_8_z -= field_F4_parts[i].field_14_dz;
            field_F4_parts[i].field_14_dz = -field_F4_parts[i].field_14_dz;
        }
    }

    if (static_cast<s32>(gnFrameCount_507670) > field_F0_timer)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

BaseGameObject* Gibs::dtor_408040()
{
    SetVTable(this, 0x4BA280);

    for (s16 i = 0; i < field_5C4_parts_used_count; i++)
    {
        field_F4_parts[i].field_18_anim.vCleanUp();
    }
    return dtor_417D10();
}

BaseGameObject* Gibs::Vdtor_4083D0(u32 flags)
{
    dtor_408040();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}


void Gibs::VRender(PrimHeader** ppOt)
{
    VRender_408200(ppOt);
}


void Gibs::VRender_408200(PrimHeader** ppOt)
{
    if (sNumCamSwappers_507668 != 0)
    {
        // Don't do anything during screen change
        return;
    }

    field_BC_sprite_scale = FP_FromInteger(100) / (field_E8_z + FP_FromInteger(100));

    // Head part rendering
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

    const FP_Point* pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;
    const FP left = pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);
    const FP right = pCamPos->field_0_x + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);

    const FP up = pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos);
    const FP down = pCamPos->field_4_y + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos);

    for (s32 i = 0; i < field_5C4_parts_used_count; i++)
    {
        GibPart* pGib = &field_F4_parts[i];
        if (pGib->field_0_x >= left && pGib->field_0_x <= right)
        {
            if (pGib->field_4_y >= up && pGib->field_4_y <= down)
            {
                pGib->field_18_anim.field_14_scale = FP_FromInteger(100) / (pGib->field_8_z + FP_FromInteger(100));
                if (pGib->field_18_anim.field_14_scale < FP_FromInteger(1))
                {
                    pGib->field_18_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
                }
                else
                {
                    pGib->field_18_anim.field_C_layer = Layer::eLayer_FG1_37;
                }

                pGib->field_18_anim.VRender_403AE0(
                    FP_GetExponent(pGib->field_0_x - left),
                    FP_GetExponent(pGib->field_4_y - up),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                pGib->field_18_anim.Get_Frame_Rect_402B50(&frameRect);
                pScreenManager_4FF7C8->InvalidateRect_406E40(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h,
                    pScreenManager_4FF7C8->field_2E_idx);
            }
        }
    }
}

BaseGameObject* Gibs::VDestructor(s32 flags)
{
    return Vdtor_4083D0(flags);
}

} // namespace AO
