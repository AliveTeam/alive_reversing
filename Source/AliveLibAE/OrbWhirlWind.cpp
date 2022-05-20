#include "stdafx.h"
#include "OrbWhirlWind.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "stdlib.hpp"

OrbWhirlWind::OrbWhirlWind(FP xpos, FP ypos, FP scale, bool bIsMudokonSpirit)
    : BaseGameObject(TRUE, 0)
{
    SetType(AETypes::eNone_0);

    field_6C_xpos = xpos;
    field_70_ypos = ypos;
    field_74_scale = scale;

    field_24_particles_state = ParticlesState::eCreating;
    field_20_particle_spawn_counter = 0;

    field_26_is_mudokon_spirit = bIsMudokonSpirit;
    gObjList_drawables_5C1124->Push_Back(this);
    mFlags.Set(BaseGameObject::eDrawable_Bit4);

    field_28_particleIdx = 0;
    memset(field_2C_particles, 0, sizeof(field_2C_particles));
}

void OrbWhirlWind::ToSpin_4E3FD0(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    for (s32 i = 0; i < field_28_particleIdx; i++)
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
    for (s32 i = 0; i < field_28_particleIdx; i++)
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

void OrbWhirlWind::VRender(PrimHeader** ppOt)
{
    for (s32 i = 0; i < field_28_particleIdx; i++)
    {
        if (field_2C_particles[i])
        {
            if (!field_2C_particles[i]->IsActive_4E4370())
            {
                field_2C_particles[i]->Render(ppOt);
            }
        }
    }
}

void OrbWhirlWind::VUpdate()
{
    if (field_24_particles_state == ParticlesState::eCreating)
    {
        if (!(field_20_particle_spawn_counter % 4))
        {
            auto pParticle = ae_new<OrbWhirlWindParticle>(field_6C_xpos,
                field_70_ypos,
                field_74_scale,
                field_26_is_mudokon_spirit);
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
        for (s32 i = 0; i < field_28_particleIdx; i++)
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
            mFlags.Set(BaseGameObject::eDead);
        }
    }

    for (s32 i = 0; i < field_28_particleIdx; i++)
    {
        if (field_2C_particles[i])
        {
            if (!field_2C_particles[i]->IsActive_4E4370())
            {
                field_2C_particles[i]->Update();
            }
        }
    }
}

OrbWhirlWind::~OrbWhirlWind()
{
    gObjList_drawables_5C1124->Remove_Item(this);
    for (auto& obj : field_2C_particles)
    {
        delete obj;
    }
}

