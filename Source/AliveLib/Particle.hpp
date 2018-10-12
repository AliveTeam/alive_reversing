#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"

class Particle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT void ctor_4CC4C0(FP xpos, FP ypos, int animFrameTableOffset, int maxW, int maxH, BYTE** ppAnimData)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

        SetVTable(this, 0x547858); // vTbl_Particle_547858
        field_4_typeId = Types::eParticle;

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
        field_F4_scale_amount = 0;
    }

    EXPORT void vUpdate_4CC620()
    {
        field_B8_xpos.fpValue += field_C4_velx;
        field_BC_ypos.fpValue += field_C8_vely;
        field_CC_sprite_scale.fpValue += field_F4_scale_amount;

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_6_flags.Set(Options::eDead);
        }
    }

    EXPORT void vdtor_4CC5D0(signed int flags)
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
    }

    virtual void VUpdate() override
    {
        vUpdate_4CC620();
    }

    virtual void VDestructor(signed int flags) override
    {
        vdtor_4CC5D0(flags);
    }

private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4_scale_amount;
};

ALIVE_ASSERT_SIZEOF(Particle, 0xF8);
