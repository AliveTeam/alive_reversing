#include "stdafx.h"
#include "Gibs.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Math.hpp"

const TintEntry stru_55C744[19] =
{
    { '\x01', 87u, 103u, 67u },
    { '\x02', 87u, 103u, 67u },
    { '\x03', 87u, 103u, 67u },
    { '\x04', 87u, 103u, 67u },
    { '\x05', 87u, 103u, 67u },
    { '\x06', 87u, 103u, 67u },
    { '\a', 87u, 103u, 67u },
    { '\b', 87u, 103u, 67u },
    { '\t', 87u, 103u, 67u },
    { '\n', 87u, 103u, 67u },
    { '\v', 87u, 103u, 67u },
    { '\f', 87u, 103u, 67u },
    { '\r', 87u, 103u, 67u },
    { '\x0E', 87u, 103u, 67u },
    { '\xFF', 87u, 103u, 67u },
    { '\0', 0u, 0u, 0u },
    { '\0', 0u, 0u, 0u },
    { '\0', 0u, 0u, 0u },
    { '\0', 0u, 0u, 0u }
};

const Gib_Data kGibData_550D78[11] =
{
    { 7732, 7772, 7812, 50, 25, 25 },
    { 6480, 6560, 6520, 44, 26, 423 },
    { 7504, 7544, 7544, 53, 28, 576 },
    { 7732, 7772, 7812, 50, 25, 25 },
    { 7732, 7772, 7812, 50, 25, 25 },
    { 2244, 2244, 2244, 38, 29, 365 },
    { 8140, 8188, 8188, 48, 28, 801 },
    { 8140, 8188, 8188, 48, 28, 803 },
    { 8140, 8188, 8188, 48, 28, 805 },
    { 8140, 8188, 8188, 48, 28, 807 },
    { 1088, 1128, 1128, 19, 8, 580 }
};

ALIVE_VAR(1, 0x550e80, short, word_550E80, 13);


EXPORT FP CC Random_40FAF0(FP /*scale*/)
{
    NOT_IMPLEMENTED();

    /*
    signed int v1; // eax

    v1 = (sRandomBytes_546744[sRandomSeed_5D1E10++] - 128) << word_550E80;
    return Math_FixedPoint_Multiply_496C50(v1, scale);
    */
    return FP_FromInteger(0);
}

EXPORT Gibs* Gibs::ctor_40FB40(int gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, __int16 bUnknown)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    
    for (GibPart& part : field_104_parts)
    {
        SetVTable(&part.field_18_anim, 0x544290); // gVtbl_animation_2a_544290
    }
    SetVTable(this, 0x544248); // vTbl_Gibs_544248

    field_F4_pGibData = &kGibData_550D78[gibType];
    BYTE** ppAnimData = Add_Resource_4DC130(ResourceManager::Resource_Animation, field_F4_pGibData->field_14_resource_id);
    
    BYTE** ppRes = nullptr;
    if (gibType == 4)
    {
        ppRes = Add_Resource_4DC130(ResourceManager::Resource_Palt, ResourceID::kMudblindResID);
    }

    // The base class renders the head gib
    Animation_Init_424E10(
        field_F4_pGibData->field_0_head,
        static_cast<short>(field_F4_pGibData->field_C_max_w),
        static_cast<short>(field_F4_pGibData->field_10_max_h),
        ppAnimData,
        1,
        1);

    if (field_6_flags.Get(BaseGameObject::eListAddFailed))
    {
        return this;
    }

    field_CC_sprite_scale = scale;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos + FP_FromInteger(2);

    field_100_timer = sGnFrame_5C1B84 + 91;

    if (scale == FP_FromInteger(1))
    {
        field_F8_z = FP_FromInteger(0);
        field_20_animation.field_C_render_layer = 37;
        field_D6_scale = 1;
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_F8_z = FP_FromInteger(100);
        field_20_animation.field_C_render_layer = 17;
        field_D6_scale = 0;
    }
    else
    {
        // Not a valid scale
        field_6_flags.Set(BaseGameObject::eDead);
    }

    field_5D6_bUnknown = bUnknown;
    field_C4_velx = xOff + Random_40FAF0(scale);

    // OG Bug? WTF?? Looks like somehow they didn't condition this param correctly
    // because field_C8_vely and field_FC_dz are always overwritten
    if (field_5D6_bUnknown == 0)
    {
        field_C8_vely = yOff + Random_40FAF0(scale);
        field_FC_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(2));
    }

    word_550E80 = 12;

    field_C8_vely = (yOff + Random_40FAF0(scale)) / FP_FromInteger(2);
    field_FC_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(4));

    if (gibType != 0)
    {
        if (gibType == 3)
        {
            SetTint_425600(stru_55C744, gMap_5C3030.sCurrentLevelId_5C3030);
        }
        else if (gibType == 4)
        {
            field_D0_r = 63;
            field_D2_g = 63;
            field_D4_b = 63;
        }
    }
    else
    {
        SetTint_425600(sTintTable_Abe_554D20, gMap_5C3030.sCurrentLevelId_5C3030);
    }

    field_5D4_parts_used_count = 4;
    
    GibPart* pPart = &field_104_parts[0];
    for (short i = 0; i < field_5D4_parts_used_count; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->field_18_anim.Init_40A030(
                field_F4_pGibData->field_4_arm,
                gObjList_animations_5C1A24,
                this,
                static_cast<short>(field_F4_pGibData->field_C_max_w),
                static_cast<short>(field_F4_pGibData->field_10_max_h),
                ppAnimData,
                1,
                0,
                0))
            {
                field_5D4_parts_used_count = i;
                field_6_flags.Set(BaseGameObject::eDead);
                return this;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->field_18_anim.Init_40A030(
                field_F4_pGibData->field_8_body,
                gObjList_animations_5C1A24,
                this,
                static_cast<short>(field_F4_pGibData->field_C_max_w),
                static_cast<short>(field_F4_pGibData->field_10_max_h),
                ppAnimData,
                1u,
                0,
                0))
            {
                field_5D4_parts_used_count = i;
                field_6_flags.Set(BaseGameObject::eDead);
                return this;
            }
        }

        pPart->field_18_anim.field_C_render_layer = field_20_animation.field_C_render_layer;
        pPart->field_18_anim.field_14_scale = scale;
        pPart->field_18_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

        pPart->field_18_anim.field_8_r = static_cast<BYTE>(field_D0_r);
        pPart->field_18_anim.field_9_g = static_cast<BYTE>(field_D2_g);
        pPart->field_18_anim.field_A_b = static_cast<BYTE>(field_D4_b); 

        pPart->field_0_x = field_B8_xpos;
        pPart->field_4_y = field_BC_ypos;
        pPart->field_8_z = field_F8_z;

        pPart->field_C_dx = xOff + Random_40FAF0(scale);

        if (field_5D6_bUnknown)
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

    return this;
}
