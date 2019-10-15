#include "stdafx.h"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ScreenManager.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

struct ParticleBurst_Item
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_z;
    FP field_C_x_speed;
    FP field_10_y_speed;
    FP field_14_z_speed;
    AnimationUnknown field_18_anim;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst_Item, 0x88);


ParticleBurst* ParticleBurst::ctor_41CF50(FP xpos, FP ypos, unsigned int unknown_count1, FP scale, BurstType type, signed __int16 a7)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5447DC);

    field_4_typeId = Types::eParticleBurst_29;

    // TODO: Check it
    if (unknown_count1 > 5)
    {
        unknown_count1 /= 2;
    }

    if (a7 > 13)
    {
        a7 = 13;
    }
    else if (a7 <= 0)
    {
        a7 = 1;
    }

    field_106_count = a7;
    field_CC_sprite_scale = scale;
    field_F4_ppRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::ResourceType::Resource_3DGibs, 0, sizeof(ParticleBurst_Item) * unknown_count1);
    if (field_F4_ppRes)
    {
        field_F8_pRes = reinterpret_cast<ParticleBurst_Item*>(*field_F4_ppRes);
        for (DWORD i = 0; i < unknown_count1; i++)
        {
            // Placement new each element
            new (&field_F8_pRes[i]) ParticleBurst_Item();
            SetVTable(&field_F8_pRes[i], 0x5447CC);
        }

        field_104_type = type;
        switch (field_104_type)
        {
        case BurstType::eFallingRocks_0:
            Animation_Init_424E10(6484, 71, 36, Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDebrisID00), 1, 1u);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            break;

        case BurstType::eSticks_1:
            Animation_Init_424E10(1704, 49, 29, Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kStickResID), 1, 1u);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            break;

        case BurstType::eBigPurpleSparks_2:
            Animation_Init_424E10(9912, 122, 43, Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID), 1, 1u);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending);
            field_20_animation.field_B_render_mode = 1;
            break;

        case BurstType::eBigRedSparks_3:
        case BurstType::eGreenSparks_5:
        case BurstType::eSmallPurpleSparks_6:
            Animation_Init_424E10(9912, 122, 43, Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID), 1, 1u);
            field_20_animation.field_B_render_mode = 1;
            field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

            if (field_104_type == BurstType::eBigRedSparks_3)
            {
                field_20_animation.field_8_r = 254;
                field_20_animation.field_9_g = 148;
                field_20_animation.field_A_b = 18;
            }
            else if (field_104_type == BurstType::eSmallPurpleSparks_6)
            {
                field_20_animation.field_8_r = 127;
                field_20_animation.field_9_g = 127;
                field_20_animation.field_A_b = 127;
            }
            else
            {
                field_20_animation.field_8_r = 0;
                field_20_animation.field_9_g = 255;
                field_20_animation.field_A_b = 32;
            }
            break;

        default:
            break;
        }

        if (field_6_flags.Get(BaseGameObject::eListAddFailed))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        else
        {
            if (field_CC_sprite_scale == FP_FromInteger(1))
            {
                field_D6_scale = 1;
                field_20_animation.field_C_render_layer = 39;
            }
            else
            {
                field_D6_scale = 0;
                field_20_animation.field_C_render_layer = 20;
            }

            field_FC_count = static_cast<short>(unknown_count1);
            field_100_timer = sGnFrame_5C1B84 + 91;
            field_B8_xpos = xpos;
            field_BC_ypos = ypos;

            for (DWORD i = 0; i < unknown_count1; i++)
            {
                field_F8_pRes[i].field_18_anim.field_68_anim_ptr = &field_20_animation;
                field_F8_pRes[i].field_18_anim.field_C_render_layer = field_20_animation.field_C_render_layer;
                field_F8_pRes[i].field_18_anim.field_6C_scale = FP_FromDouble(0.95) * field_CC_sprite_scale;

                field_F8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
                field_F8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit25_bDecompressDone); // TODO: HIWORD &= ~0x0100u ??

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit15_bSemiTrans))
                {
                    field_F8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
                }
                else
                {
                    field_F8_pRes[i].field_18_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

                }

                if (field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
                {
                    field_F8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
                }
                else
                {
                    field_F8_pRes[i].field_18_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

                }

                if (type == BurstType::eBigPurpleSparks_2)
                {
                    if (i % 2)
                    {
                        field_F8_pRes[i].field_18_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
                    }
                }

                field_F8_pRes[i].field_18_anim.field_8_r = field_20_animation.field_8_r;
                field_F8_pRes[i].field_18_anim.field_9_g = field_20_animation.field_9_g;
                field_F8_pRes[i].field_18_anim.field_A_b = field_20_animation.field_A_b;

                field_F8_pRes[i].field_0_x = field_B8_xpos;
                field_F8_pRes[i].field_4_y = field_BC_ypos;
                field_F8_pRes[i].field_8_z = FP_FromInteger(0);

                Random_Speed_41CEE0(&field_F8_pRes[i].field_C_x_speed);
                Random_Speed_41CEE0(&field_F8_pRes[i].field_10_y_speed);
                // OG bug sign could be wrong here as it called random again to Abs() it!
                FP zRandom = {};
                field_F8_pRes[i].field_14_z_speed = FP_Abs(*Random_Speed_41CEE0(&zRandom));
            }
        }
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    return this;
}

BaseGameObject* ParticleBurst::VDestructor(signed int flags)
{
    return vdtor_41D4E0(flags);
}

void ParticleBurst::VUpdate()
{
    vUpdate_41D590();
}

void ParticleBurst::VRender(int** pOrderingTable)
{
    vRender_41D7B0(pOrderingTable);
}

FP* ParticleBurst::Random_Speed_41CEE0(FP* random)
{
    const FP v2 = FP_FromRaw((Math_NextRandom() - 128) << LOBYTE(field_106_count));
    *random = v2 * field_CC_sprite_scale;
    return random;
}

ParticleBurst* ParticleBurst::vdtor_41D4E0(signed int flags)
{
    dtor_41D510();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ParticleBurst::dtor_41D510()
{
    SetVTable(this, 0x5447DC);
    if (field_F4_ppRes)
    {
        ResourceManager::FreeResource_49C330(field_F4_ppRes);
    }
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void ParticleBurst::vRender_41D7B0(int** pOt)
{
    bool bFirst = true;
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        field_20_animation.field_14_scale = field_CC_sprite_scale;
        const FP camX = pScreenManager_5BB5F4->field_20_pCamPos->field_0_x;
        const FP camY = pScreenManager_5BB5F4->field_20_pCamPos->field_4_y;
      
        for (int i = 0; i < field_FC_count; i++)
        {
            if (field_F8_pRes[i].field_0_x < camX)
            {
                continue;
            }

            if (field_F8_pRes[i].field_0_x > camX + FP_FromInteger(640))
            {
                continue;
            }

            if (field_F8_pRes[i].field_4_y < camY)
            {
                continue;
            }
            
            if (field_F8_pRes[i].field_4_y > camY + FP_FromInteger(240))
            {
                continue;
            }

            const FP zPos = field_F8_pRes[i].field_8_z;

            // TODO: Much duplicated code in each branch
            if (bFirst)
            {
                field_20_animation.field_14_scale =  FP_FromInteger(100) /  (zPos + FP_FromInteger(300));
                field_20_animation.field_14_scale *= field_CC_sprite_scale;
                field_20_animation.field_14_scale *= FP_FromInteger(field_106_count) / FP_FromInteger(13);

                if (field_20_animation.field_14_scale <= FP_FromInteger(1))
                {
                    field_20_animation.vRender_40B820(
                        FP_GetExponent(field_F8_pRes[i].field_0_x - camX),
                        FP_GetExponent(field_F8_pRes[i].field_4_y - camY),
                        pOt,
                        0,
                        0);
                    
                    bFirst = false;

                    PSX_RECT frameRect = {};
                    field_20_animation.Get_Frame_Rect_409E10(&frameRect);
                    if (field_106_count == 9)
                    {
                        if (field_20_animation.field_8_r > 5)
                        {
                            field_20_animation.field_8_r -= 6;
                        }
                        else
                        {
                            field_20_animation.field_8_r = 0;
                        }

                        if (field_20_animation.field_9_g > 5)
                        {
                            field_20_animation.field_9_g -= 6;
                        }
                        else
                        {
                            field_20_animation.field_9_g = 0;
                        }

                        if (field_20_animation.field_A_b > 5)
                        {
                            field_20_animation.field_A_b -= 6;
                        }
                        else
                        {
                            field_20_animation.field_A_b = 0;
                        }
                    }
                    pScreenManager_5BB5F4->InvalidateRect_40EC90(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h,
                        pScreenManager_5BB5F4->field_3A_idx);
                }
            }
            else
            {
                field_F8_pRes[i].field_18_anim.field_6C_scale = FP_FromInteger(100) / (zPos + FP_FromInteger(300));
                field_F8_pRes[i].field_18_anim.field_6C_scale *= field_CC_sprite_scale;
                field_F8_pRes[i].field_18_anim.field_6C_scale *= FP_FromInteger(field_106_count) / FP_FromInteger(13);

                if (field_F8_pRes[i].field_18_anim.field_6C_scale <= FP_FromInteger(1))
                {
                    field_F8_pRes[i].field_18_anim.vRender_40B820(
                        FP_GetExponent(field_F8_pRes[i].field_0_x - camX),
                        FP_GetExponent(field_F8_pRes[i].field_4_y - camY),
                        pOt,
                        0,
                        0);
                    
                    PSX_RECT frameRect = {};
                    field_F8_pRes[i].field_18_anim.GetRenderedSize_40C980(&frameRect);

                    if (field_106_count == 9)
                    {
                        if (field_F8_pRes[i].field_18_anim.field_8_r > 5)
                        {
                            field_F8_pRes[i].field_18_anim.field_8_r -= 6;
                        }
                        else
                        {
                            field_F8_pRes[i].field_18_anim.field_8_r = 0;
                        }

                        if (field_F8_pRes[i].field_18_anim.field_9_g > 5)
                        {
                            field_F8_pRes[i].field_18_anim.field_9_g -= 6;
                        }
                        else
                        {
                            field_F8_pRes[i].field_18_anim.field_9_g = 0;
                        }

                        if (field_F8_pRes[i].field_18_anim.field_A_b > 5)
                        {
                            field_F8_pRes[i].field_18_anim.field_A_b -= 6;
                        }
                        else
                        {
                            field_F8_pRes[i].field_18_anim.field_A_b = 0;
                        }

                    }
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

void ParticleBurst::vUpdate_41D590()
{
    const int v3 = field_CC_sprite_scale != FP_FromInteger(1) ? 2 : 4;
    for (int i = 0; i < field_FC_count; i++)
    {
        field_F8_pRes[i].field_0_x += field_F8_pRes[i].field_C_x_speed;
        field_F8_pRes[i].field_4_y += field_F8_pRes[i].field_10_y_speed;
        field_F8_pRes[i].field_8_z += field_F8_pRes[i].field_14_z_speed;

        field_F8_pRes[i].field_10_y_speed += FP_FromDouble(0.25);

        if (field_106_count == 9)
        {
            if ((sGnFrame_5C1B84 + i) & v3)
            {
                field_F8_pRes[i].field_0_x -= FP_FromInteger(1);
            }
            else
            {
                field_F8_pRes[i].field_0_x += FP_FromInteger(1);
            }
        }

        if (field_F8_pRes[i].field_8_z + FP_FromInteger(300) < FP_FromInteger(15))
        {
            field_F8_pRes[i].field_14_z_speed = -field_F8_pRes[i].field_14_z_speed;
            field_F8_pRes[i].field_8_z += field_F8_pRes[i].field_14_z_speed;

            // TODO: Never used by OG ??
            //Math_RandomRange_496AB0(-64, 46);

            // TODO: This might be wrong
            const short volume = static_cast<short>(Math_RandomRange_496AB0(-10, 10) + ((field_100_timer - sGnFrame_5C1B84) / 91) + 25);

            const BYTE next_rand = Math_NextRandom();
            if (next_rand < 43)
            {
                SFX_Play_46FC20(27u, volume, CameraPos::eCamLeft_3);
            }
            else if (next_rand >= 85)
            {
                SFX_Play_46FC20(27u, volume, CameraPos::eCamRight_4);
            }
            else
            {
                SFX_Play_46FC20(27u, volume, CameraPos::eCamCurrent_0);
            }
        }
    }

    if (static_cast<int>(sGnFrame_5C1B84) > field_100_timer)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}
