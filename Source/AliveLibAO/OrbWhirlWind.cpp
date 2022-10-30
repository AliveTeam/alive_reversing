#include "stdafx_ao.h"
#include "Function.hpp"
#include "OrbWhirlWind.hpp"
#include "OrbWhirlWindParticle.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

s32 gOrbWhirlWind_instace_count_9F30A0 = 0;

void OrbWhirlWind::ToSpin(FP xpos, FP ypos, IBaseAliveGameObject* pObj)
{
    for (s32 i = 0; i < field_16_particleIdx; i++)
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

void OrbWhirlWind::VRender(PrimHeader** ppOt)
{
    for (s32 i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->Render(ppOt);
            }
        }
    }
}

void OrbWhirlWind::VUpdate()
{
    if (field_14_particles_state == OrbWhirlWind::ParticlesState::eCreating)
    {
        if (!(field_10_particle_spawn_counter % 4))
        {
            auto pParticle = relive_new OrbWhirlWindParticle(
                field_58_xpos,
                field_5C_ypos,
                field_60_scale);

            if (pParticle)
            {
                field_18_particles[field_16_particleIdx++] = pParticle;

                if (field_16_particleIdx >= ALIVE_COUNTOF(field_18_particles))
                {
                    field_14_particles_state = ParticlesState::eCreated;
                }
               
            }
         }
         ++field_10_particle_spawn_counter;
    }
    else if (field_14_particles_state == ParticlesState::eActive)
    {
        bool unknown = false;
        for (s32 i = 0; i < field_16_particleIdx; i++)
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
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }

    for (s32 i = 0; i < field_16_particleIdx; i++)
    {
        if (field_18_particles[i])
        {
            if (!field_18_particles[i]->IsActive())
            {
                field_18_particles[i]->Update();
            }
        }
    }
}

OrbWhirlWind::~OrbWhirlWind()
{
    gObjListDrawables->Remove_Item(this);

    for (auto& obj : field_18_particles)
    {
        if (obj)
        {
            delete obj;
        }
    }

    gOrbWhirlWind_instace_count_9F30A0--;
}

OrbWhirlWind::OrbWhirlWind(FP xpos, FP ypos, FP scale)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eNone);

    field_58_xpos = xpos;
    field_5C_ypos = ypos;
    field_60_scale = scale;

    field_14_particles_state = ParticlesState::eCreating;
    field_10_particle_spawn_counter = 0;

    gObjListDrawables->Push_Back(this);
    mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

    field_16_particleIdx = 0;

    memset(field_18_particles, 0, sizeof(field_18_particles));

    gOrbWhirlWind_instace_count_9F30A0++;
}

void OrbWhirlWind::ToStop()
{
    for (s32 i = 0; i < field_16_particleIdx; i++)
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

} // namespace AO
