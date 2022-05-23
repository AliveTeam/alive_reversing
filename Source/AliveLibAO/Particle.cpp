#include "stdafx_ao.h"
#include "Function.hpp"
#include "Particle.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"

namespace AO {

Particle* CC New_DestroyOrCreateObject_Particle_419D00(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::DeathFlare_2);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);

    auto pParticle = ao_new<Particle>();
    if (!pParticle)
    {
        return nullptr;
    }

    pParticle->ctor_478880(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
    pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;
    pParticle->field_BC_sprite_scale = FP_FromInteger(2) * scale;

    if (scale == FP_FromInteger(1))
    {
        pParticle->field_10_anim.field_C_layer = Layer::eLayer_Above_FG1_39;
    }
    else
    {
        pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
    }

    pParticle->field_CC_bApplyShadows &= ~1u;

    return pParticle;
}

void CC New_Smoke_Particles_419A80(FP xpos, FP ypos, FP scale, s16 count, s16 type)
{
    FP velYCounter = {};
    for (s32 i = 0; i < count; i++)
    {
        FP randX = (FP_FromInteger(Math_RandomRange_450F20(-3, 3)) * scale) + xpos;
        FP particleY = (FP_FromInteger(6 * (i + 1) / 2 * (1 - 2 * (i % 2))) * scale) + ypos;
        const AnimRecord& rec = AO::AnimRec(AnimId::SquibSmoke_Particle);
        u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
        auto pParticle = ao_new<Particle>();
        if (pParticle)
        {
            pParticle->ctor_478880(randX, particleY, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
            pParticle->field_CC_bApplyShadows &= ~1u;
            pParticle->field_10_anim.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_3;

            if (type == 1)
            {
                pParticle->field_C0_r = 32;
                pParticle->field_C2_g = 128;
                pParticle->field_C4_b = 32;
            }
            else
            {
                pParticle->field_C0_r = 128;
                pParticle->field_C2_g = 128;
                pParticle->field_C4_b = 128;
            }

            pParticle->field_B4_velx = (scale * FP_FromInteger(Math_RandomRange_450F20(-10, 10))) / FP_FromInteger(10);
            pParticle->field_B8_vely = ((scale * velYCounter) * FP_FromInteger(Math_RandomRange_450F20(50, 50))) / FP_FromInteger(100);
            pParticle->field_BC_sprite_scale = scale;

            if (scale == FP_FromInteger(1))
            {
                pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;
            }
            else
            {
                pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
            }

            pParticle->field_E4_scale_amount = scale * FP_FromDouble(0.03);
            pParticle->field_10_anim.field_10_frame_delay = static_cast<u16>((i + 3) / 2);
            if (Math_NextRandom() < 127)
            {
                pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
            }
        }
        velYCounter -= FP_FromInteger(1);
    }
}

void CC New_Chant_Particle_4198E0(FP xpos, FP ypos, FP scale, Layer layer)
{
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
        pParticle->ctor_478880(xpos, ypos, orbRec.mFrameTableOffset, orbRec.mMaxW, orbRec.mMaxH, ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0));
        pParticle->field_CC_bApplyShadows &= ~1u;
        pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;

        if (layer != Layer::eLayer_0)
        {
            pParticle->field_10_anim.field_C_layer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->field_BC_sprite_scale = scale;
    }
}

void CC New_Shiny_Particle_4199A0(FP xpos, FP ypos, FP scale, Layer layer)
{
    const AnimRecord& orbRec = AO::AnimRec(AnimId::ChantOrb_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, orbRec.mResourceId, 1, 0);
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_478880(xpos, ypos, orbRec.mFrameTableOffset, orbRec.mMaxW, orbRec.mMaxH, ppRes);

        pParticle->field_CC_bApplyShadows &= ~1u;
        pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;
        pParticle->field_C0_r = 100;
        pParticle->field_C2_g = 100;
        pParticle->field_C4_b = 100;

        if (layer != Layer::eLayer_0)
        {
            pParticle->field_10_anim.field_C_layer = layer;
        }
        else if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->field_BC_sprite_scale = scale;
    }
}

void CC New_ShootingZFire_Particle_419810(FP xpos, FP ypos, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::ShootingZFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_478880(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);
        pParticle->field_CC_bApplyShadows &= ~1u;

        pParticle->field_C4_b = 55;
        pParticle->field_C2_g = 55;
        pParticle->field_C0_r = 55;

        pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->field_BC_sprite_scale = scale;
    }
}

void CC New_ShootingFire_Particle_419720(FP xpos, FP ypos, s8 direction, FP scale)
{
    const AnimRecord& rec = AO::AnimRec(AnimId::ShootingFire_Particle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    auto pParticle = ao_new<Particle>();
    if (pParticle)
    {
        pParticle->ctor_478880(xpos, ypos, rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes);

        pParticle->field_CC_bApplyShadows &= ~1u;
        pParticle->field_C4_b = 55;
        pParticle->field_C2_g = 55;
        pParticle->field_C0_r = 55;
        pParticle->field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;

        if (scale == FP_FromInteger(1))
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_36;
        }
        else
        {
            pParticle->field_10_anim.field_C_layer = Layer::eLayer_Foreground_Half_17;
        }

        pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, direction & 1);
        pParticle->field_BC_sprite_scale = scale;
    }
}

Particle* Particle::ctor_478880(FP xpos, FP ypos, s32 animFrameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData)
{
    
    SetVTable(this, 0x4BCDB0);

    field_C4_b = 128;
    field_C2_g = 128;
    field_C0_r = 128;

    field_4_typeId = Types::eParticle_93;

    Animation_Init_417FD0(
        animFrameTableOffset,
        maxW,
        maxH,
        ppAnimData,
        1);

    if (mFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;
    field_E4_scale_amount = FP_FromInteger(0);

    return this;
}

BaseGameObject* Particle::VDestructor(s32 flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Particle::VUpdate()
{
    VUpdate_478930();
}

void Particle::VUpdate_478930()
{
    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    field_BC_sprite_scale += field_E4_scale_amount;

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
    {
        mFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
