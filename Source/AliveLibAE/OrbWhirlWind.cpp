#include "stdafx.h"
#include "OrbWhirlWind.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

BaseGameObject* OrbWhirlWind::VDestructor(signed int flags)
{
    return vdtor_4E3D50(flags);
}

void OrbWhirlWind::VUpdate()
{
    return vUpdate_4E3E20();
}

void OrbWhirlWind::VRender(int** pOrderingTable)
{
    return vRender_4E3F80(pOrderingTable);
}

OrbWhirlWind* OrbWhirlWind::ctor_4E3C90(FP xpos, FP ypos, FP scale, __int16 bIsMudokonSpirit)
{
    field_4_typeId = Types::eNone_0;

    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x5480B8); // vTbl_OrbWhirlWind_5480B8


    field_6C_xpos = xpos;
    field_70_ypos = ypos;
    field_74_scale = scale;

    field_24_particles_state = ParticlesState::eCreating;
    field_20_particle_spawn_counter = 0;

    field_26_is_mudokon_spirit = bIsMudokonSpirit;
    gObjList_drawables_5C1124->Push_Back(this);
    field_6_flags.Set(BaseGameObject::eDrawable_Bit4);

    field_28_particleIdx = 0;
    memset(field_2C_particles, 0, sizeof(field_2C_particles));

    return this;
}

void OrbWhirlWind::ToSpin_4E3FD0(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    for (int i = 0; i < field_28_particleIdx; i++)
    {
        if (field_2C_particles[i])
        {
            if (!field_2C_particles[i]->IsActive_4E4370())
            {
                field_2C_particles[i]->Spin_4E4A10(xpos, ypos, scale, pObj);
            }
        }
    }
    field_24_particles_state = ParticlesState::eActive;
}

void OrbWhirlWind::ToStop_4E4050()
{
    for (int i = 0; i < field_28_particleIdx; i++)
    {
        if (field_2C_particles[i])
        {
            if (!field_2C_particles[i]->IsActive_4E4370())
            {
                field_2C_particles[i]->ToStop_4E4AD0();
            }
        }
    }
    field_24_particles_state = ParticlesState::eActive;
}

void OrbWhirlWind::vRender_4E3F80(int** ppOt)
{
    for (int i = 0; i < field_28_particleIdx; i++)
    {
        if (field_2C_particles[i])
        {
            if (!field_2C_particles[i]->IsActive_4E4370())
            {
                field_2C_particles[i]->VRender_4E4B10(ppOt);
            }
        }
    }
}

void OrbWhirlWind::vUpdate_4E3E20()
{
    if (field_24_particles_state == ParticlesState::eCreating)
    {
        if (!(field_20_particle_spawn_counter % 4))
        {
            auto pParticle = ae_new<OrbWhirlWindParticle>();
            if (pParticle)
            {
                pParticle->ctor_4E40C0(
                    field_6C_xpos,
                    field_70_ypos,
                    field_74_scale,
                    field_26_is_mudokon_spirit);
            }

            field_2C_particles[field_28_particleIdx++] = pParticle;

            if (field_28_particleIdx >= ALIVE_COUNTOF(field_2C_particles))
            {
                field_24_particles_state = ParticlesState::eCreated;
            }
        }

        ++field_20_particle_spawn_counter;
    }
    else if (field_24_particles_state == ParticlesState::eActive)
    {
        bool hasInactiveParticles = false;
        for (int i = 0; i < field_28_particleIdx; i++)
        {
            if (field_2C_particles[i])
            {
                if (!field_2C_particles[i]->IsActive_4E4370())
                {
                    hasInactiveParticles = true;
                    break;
                }
            }
        }

        if (!hasInactiveParticles)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    for (int i = 0; i < field_28_particleIdx; i++)
    {
        if (field_2C_particles[i])
        {
            if (!field_2C_particles[i]->IsActive_4E4370())
            {
                field_2C_particles[i]->VUpdate_4E4510();
            }
        }
    }
}

void OrbWhirlWind::dtor_4E3D80()
{
    SetVTable(this, 0x5480B8);

    gObjList_drawables_5C1124->Remove_Item(this);
    for (auto& obj : field_2C_particles)
    {
        if (obj)
        {
            obj->vdtor_4E42D0(1);
        }
    }
    BaseGameObject_dtor_4DBEC0();
}


OrbWhirlWind* OrbWhirlWind::vdtor_4E3D50(signed int flags)
{
    dtor_4E3D80();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}
