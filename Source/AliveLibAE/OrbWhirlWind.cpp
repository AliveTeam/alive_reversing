#include "stdafx.h"
#include "OrbWhirlWind.hpp"
#include "../relive_lib/Function.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

OrbWhirlWind::OrbWhirlWind(FP xpos, FP ypos, FP scale, bool bIsMudokonSpirit)
    : BaseGameObject(true, 0)
{
    SetType(ReliveTypes::eNone);

    mXPos = xpos;
    mYPos = ypos;
    mScale = scale;

    mState = ParticlesState::eCreating;
    mParticleSpawnCounter = 0;

    mIsMudokonSpirit = bIsMudokonSpirit;
    gObjListDrawables->Push_Back(this);
    SetDrawable(true);

    mParticleIdx = 0;

    memset(mOrbParticles, 0, sizeof(mOrbParticles));
}

void OrbWhirlWind::ToSpin(FP xpos, FP ypos, FP scale, BaseGameObject* pObj)
{
    for (s32 i = 0; i < mParticleIdx; i++)
    {
        if (mOrbParticles[i])
        {
            if (!mOrbParticles[i]->IsActive())
            {
                mOrbParticles[i]->Spin(xpos, ypos, scale, pObj);
            }
        }
    }
    mState = ParticlesState::eActive;
}

void OrbWhirlWind::ToStop()
{
    for (s32 i = 0; i < mParticleIdx; i++)
    {
        if (mOrbParticles[i])
        {
            if (!mOrbParticles[i]->IsActive())
            {
                mOrbParticles[i]->ToStop();
            }
        }
    }
    mState = ParticlesState::eActive;
}

void OrbWhirlWind::VRender(PrimHeader** ppOt)
{
    for (s32 i = 0; i < mParticleIdx; i++)
    {
        if (mOrbParticles[i])
        {
            if (!mOrbParticles[i]->IsActive())
            {
                mOrbParticles[i]->Render(ppOt);
            }
        }
    }
}

void OrbWhirlWind::VUpdate()
{
    if (mState == ParticlesState::eCreating)
    {
        if (!(mParticleSpawnCounter % 4))
        {
            auto pParticle = relive_new OrbWhirlWindParticle(
                mXPos,
                mYPos,
                mScale,
                mIsMudokonSpirit);
            mOrbParticles[mParticleIdx++] = pParticle;

            if (mParticleIdx >= ALIVE_COUNTOF(mOrbParticles))
            {
                mState = ParticlesState::eCreated;
            }
        }

        ++mParticleSpawnCounter;
    }
    else if (mState == ParticlesState::eActive)
    {
        bool hasInactiveParticles = false;
        for (s32 i = 0; i < mParticleIdx; i++)
        {
            if (mOrbParticles[i])
            {
                if (!mOrbParticles[i]->IsActive())
                {
                    hasInactiveParticles = true;
                    break;
                }
            }
        }

        if (!hasInactiveParticles)
        {
            SetDead(true);
        }
    }

    for (s32 i = 0; i < mParticleIdx; i++)
    {
        if (mOrbParticles[i])
        {
            if (!mOrbParticles[i]->IsActive())
            {
                mOrbParticles[i]->Update();
            }
        }
    }
}

OrbWhirlWind::~OrbWhirlWind()
{
    gObjListDrawables->Remove_Item(this);

    for (auto& obj : mOrbParticles)
    {
        delete obj;
    }
}

