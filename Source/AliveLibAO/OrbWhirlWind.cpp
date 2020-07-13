#include "stdafx_ao.h"
#include "Function.hpp"
#include "OrbWhirlWind.hpp"
#include "OrbWhirlWindParticle.hpp"
#include "stdlib.hpp"
#include "Game.hpp"

START_NS_AO

ALIVE_VAR(1, 0x9F30A0, int, gOrbWhirlWind_instace_count_9F30A0, 0);

void OrbWhirlWind::ToSpin_48BAD0(FP xpos, FP ypos, BaseAliveGameObject* pObj)
{
    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->Spin(xpos, ypos, pObj);
            }
        }
    }
    field_14_particles_state = ParticlesState::eActive;
}

void OrbWhirlWind::VRender_48BA90(int** ppOt)
{
    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->VRender_48C430(ppOt);
            }
        }
    }
}

void OrbWhirlWind::VRender(int** ppOt)
{
    VRender_48BA90(ppOt);
}

void OrbWhirlWind::VUpdate_48B990()
{
    if (field_14_particles_state == OrbWhirlWind::ParticlesState::eCreating)
    {
        if (!(field_10_particle_spawn_counter % 4))
        {
            auto pParticle = ao_new<OrbWhirlWindParticle>();
            if (pParticle)
            {
                pParticle->ctor_48BC10(
                    field_58_xpos,
                    field_5C_ypos,
                    field_60_scale);
            }
            field_18_particles[field_16_particleIdx++] = pParticle;

            if (field_16_particleIdx >= ALIVE_COUNTOF(field_18_particles))
            {
                field_14_particles_state = ParticlesState::eCreated;
            }
        }
        ++field_10_particle_spawn_counter;
    }
    else if (field_14_particles_state == ParticlesState::eActive)
    {
        bool unknown = false;
        for (int i = 0; i < field_16_particleIdx; i++)
        {
            if (field_18_particles[i])
            {
                if (!field_18_particles[i]->IsActive())
                {
                    unknown = true;
                    break;
                }
            }
        }

        if (!unknown)
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }

    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->VUpdate_48BF00();
            }
        }
    }
}

void OrbWhirlWind::VUpdate()
{
    VUpdate_48B990();
}

OrbWhirlWind* OrbWhirlWind::Vdtor_48C4F0(signed int flags)
{
    dtor_48B910();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* OrbWhirlWind::VDestructor(signed int flags)
{
    return Vdtor_48C4F0(flags);
}

BaseGameObject* OrbWhirlWind::dtor_48B910()
{
    SetVTable(this, 0x4BD7A0);
    gObjList_drawables_504618->Remove_Item(this);

    for (auto& obj : field_18_particles)
    {
        obj->Vdtor_48C510(1);
    }

    gOrbWhirlWind_instace_count_9F30A0--;
    return dtor_487DF0();
}

OrbWhirlWind* OrbWhirlWind::ctor_48B870(FP xpos, FP ypos, FP scale)
{
    ctor_487E10(1);

    SetVTable(this, 0x4BD7A0);

    field_4_typeId = Types::eNone_0;

    field_58_xpos = xpos;
    field_5C_ypos = ypos;
    field_60_scale = scale;

    field_14_particles_state = ParticlesState::eCreating;
    field_10_particle_spawn_counter = 0;

    gObjList_drawables_504618->Push_Back(this);
    field_6_flags.Set(Options::eDrawable_Bit4);

    field_16_particleIdx = 0;

    memset(field_18_particles, 0, sizeof(field_18_particles));

    gOrbWhirlWind_instace_count_9F30A0++;
    return this;
}

void OrbWhirlWind::ToStop_48BBB0()
{
    for (int i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->ToStop();
            }
        }
    }
    field_14_particles_state = ParticlesState::eActive;
}

END_NS_AO

