#include "stdafx.h"
#include "Particle.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"

EXPORT Particle* Particle::ctor_4CC4C0(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x547858); // vTbl_Particle_547858
    field_4_typeId = Types::eParticle_134;

    ResourceManager::Inc_Ref_Count_49C310(ppAnimData);

    field_10_resources_array.Push_Back(ppAnimData);

    field_D4_b = 128;
    field_D2_g = 128;
    field_D0_r = 128;

    Animation_Init_424E10(animFrameTableOffset, static_cast<short>(maxW), static_cast<short>(maxH), ppAnimData, 1, 1);

    if (field_6_flags.Get(Options::eListAddFailed))
    {
        field_6_flags.Set(Options::eDead);
    }

    field_BC_ypos = ypos;
    field_B8_xpos = xpos;
    field_F4_scale_amount = FP_FromInteger(0);
    return this;
}

EXPORT void Particle::vUpdate_4CC620()
{
    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;
    field_CC_sprite_scale += field_F4_scale_amount;

    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        field_6_flags.Set(Options::eDead);
    }
}

EXPORT BaseGameObject* Particle::vdtor_4CC5D0(signed int flags)
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Particle::VUpdate()
{
    vUpdate_4CC620();
}

BaseGameObject* Particle::VDestructor(signed int flags)
{
    return vdtor_4CC5D0(flags);
}

EXPORT Particle* CC New_Particle_426F40(FP xpos, FP ypos, FP scale)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kDeathFlareResID, FALSE, FALSE);
    auto pParticle = alive_new<Particle>();

    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->ctor_4CC4C0(xpos, ypos, 9912, 122, 43, ppRes);
    pParticle->field_20_animation.field_B_render_mode = 1;
    pParticle->field_CC_sprite_scale = FP_FromRaw(scale.fpValue * 2);

    if (scale == FP_FromInteger(1))
    {
        pParticle->field_20_animation.field_C_render_layer = 39;
    }
    else
    {
        pParticle->field_20_animation.field_C_render_layer = 17;
    }

    pParticle->field_DC_bApplyShadows &= ~1u;

    return pParticle;
}

EXPORT Particle* CC New_Particle_426AA0(FP xpos, FP ypos, FP velY, FP velX, FP scale, __int16 layer, BYTE r, BYTE b, BYTE g)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kOmmflareResID, 0, 0);
    auto pParticle = alive_new<Particle>();
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->ctor_4CC4C0(xpos, ypos, 1632, 39, 21, ppRes);
    pParticle->field_D0_r = r;
    pParticle->field_D4_b = g;
    pParticle->field_D2_g = b;

    pParticle->field_C8_vely = velX;
    pParticle->field_C4_velx = velY;

    pParticle->field_DC_bApplyShadows &= ~1u;

    pParticle->field_20_animation.field_B_render_mode = 1;

    if (layer != 0)
    {
        pParticle->field_20_animation.field_C_render_layer = layer;
    }
    else
    {
        if (scale == FP_FromInteger(1))
        {
            pParticle->field_20_animation.field_C_render_layer = 36;
        }
        else
        {
            pParticle->field_20_animation.field_C_render_layer = 17;
        }
    }

    pParticle->field_CC_sprite_scale = scale;

    return pParticle;
}

EXPORT Particle* CC New_Chant_Particle_426BE0(FP xpos, FP ypos, FP scale, __int16 layer)
{
    return New_Particle_426AA0(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, 128u, 128u, 128u);
}

// Fart/dust cloud particle spawner
EXPORT void CC New_Particles_426C70(FP xpos, FP ypos, FP scale, __int16 count, BYTE r, BYTE g, BYTE b)
{
    FP velYCounter = {};
    for (int i=0; i < count; i++)
    {
        FP randX = (FP_FromInteger(Math_RandomRange_496AB0(-3, 3)) * scale) + xpos;
        FP particleY = (FP_FromInteger(6 * (i + 1) / 2 * (1 - 2 * (i % 2))) * scale) + ypos;
        BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSquibSmokeResID, 0, 0);
        auto pParticle = alive_new<Particle>();
        if (pParticle)
        {
            pParticle->ctor_4CC4C0(randX, particleY, 5084, 61, 44, ppRes);
            pParticle->field_DC_bApplyShadows &= ~1u;
            pParticle->field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_20_animation.field_B_render_mode = 3;

            pParticle->field_D0_r = r;
            pParticle->field_D2_g = g;
            pParticle->field_D4_b = b;

            pParticle->field_C4_velx = (scale * FP_FromInteger(Math_RandomRange_496AB0(-10, 10))) / FP_FromInteger(10);
            pParticle->field_C8_vely = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange_496AB0(50, 50))) / FP_FromInteger(100);
            pParticle->field_CC_sprite_scale = scale;

            if (scale == FP_FromInteger(1))
            {
                pParticle->field_20_animation.field_C_render_layer = 36;
            }
            else
            {
                pParticle->field_20_animation.field_C_render_layer = 17;
            }

            pParticle->field_F4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->field_20_animation.field_10_frame_delay = static_cast<WORD>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

void New_Particle_45BC70(BaseAliveGameObject* pObj)
{
    const FP ypos = pObj->field_BC_ypos - (pObj->field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(30, 60)));
    const FP xpos = (pObj->field_CC_sprite_scale * FP_FromInteger(Math_RandomRange_496AB0(-20, 20))) + pObj->field_B8_xpos;
    New_Chant_Particle_426BE0(xpos, ypos, pObj->field_CC_sprite_scale, 0);
}

Particle* CC New_Particle_426C30(FP xpos, FP ypos, FP scale, __int16 layer)
{
    return New_Particle_426AA0(xpos, ypos, FP_FromInteger(0), FP_FromInteger(0), scale, layer, 100u, 100u, 100u);
}

void CC New_Particle_4269B0(FP xpos, FP ypos, FP scale)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 344, 0, 0);

    auto pParticle = alive_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_4CC4C0(xpos, ypos, 2832, 126, 44, ppRes);
        pParticle->field_DC_bApplyShadows &= ~1u;
        pParticle->field_D4_b = 55;
        pParticle->field_D2_g = 55;
        pParticle->field_D0_r = 55;
        pParticle->field_20_animation.field_B_render_mode = 1;
        if (scale == FP_FromInteger(1))
        {
            pParticle->field_20_animation.field_C_render_layer = 36;
        }
        else
        {
            pParticle->field_20_animation.field_C_render_layer = 17;
        }
        pParticle->field_CC_sprite_scale = scale;
    }
}

void CC New_Particle_426890(FP xpos, FP ypos, char direction, FP scale)
{
    BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, kBigflashResID, 0, 0);
    auto pParticle = alive_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_4CC4C0(xpos, ypos, 960, 86, 17, ppRes);
        pParticle->field_DC_bApplyShadows &= ~1u;
        pParticle->field_D4_b = 55;
        pParticle->field_D2_g = 55;
        pParticle->field_D0_r = 55;
        pParticle->field_20_animation.field_B_render_mode = 1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->field_20_animation.field_C_render_layer = 36;
        }
        else
        {
            pParticle->field_20_animation.field_C_render_layer = 17;
        }

        pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, direction & 1);
        pParticle->field_CC_sprite_scale = scale;
    }
}
