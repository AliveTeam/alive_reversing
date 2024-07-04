#include "stdafx_ao.h"
#include "BoomMachinePipe.hpp"
#include "../relive_lib/GameObjects/Grenade.hpp"
#include "../relive_lib/GameObjects/ThrowableArray.hpp"
#include "Sfx.hpp"

namespace AO {

BoomMachinePipe::BoomMachinePipe(FP xpos, FP ypos, FP scale, s16 numGrenades)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::BoomMachine_Pipe_Idle));

    GetAnimation().SetSemiTrans(false);
    SetApplyShadowZoneColour(false);

    SetSpriteScale(scale);

    mXPos = xpos;
    mYPos = ypos;

    mGrenadeCount = numGrenades;

    mState = BoomMachinePipeStates::eInactive;
}

void BoomMachinePipe::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BoomMachine_Pipe_DropGrenade));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BoomMachine_Pipe_Idle));
}

void BoomMachinePipe::DropGrenadeAnimation()
{
    if (mState == BoomMachinePipeStates::eInactive)
    {
        mState = BoomMachinePipeStates::eDropGrenadeAnimation;
        mTimer = MakeTimer(10);
    }
}

void BoomMachinePipe::AlreadyUsed()
{
    if (mState == BoomMachinePipeStates::eInactive)
    {
        mState = BoomMachinePipeStates::eAlreadyUsed;
        mTimer = MakeTimer(10);
    }
}

void BoomMachinePipe::VUpdate()
{
    switch (mState)
    {
        case BoomMachinePipeStates::eInactive:
            // do nothing
            break;
        case BoomMachinePipeStates::eAlreadyUsed:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                SFX_Play_Pitch(relive::SoundEffects::ZPop, 60, -1800);
                mState = BoomMachinePipeStates::eInactive;
            }
            break;

        case BoomMachinePipeStates::eDropGrenadeAnimation:
            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                mState = BoomMachinePipeStates::eDropGrenade;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Pipe_DropGrenade));
            }
            break;

        case BoomMachinePipeStates::eDropGrenade:
            if (GetAnimation().GetForwardLoopCompleted())
            {
                SFX_Play_Pitch(relive::SoundEffects::PickupItem, 127, -900);

                if (!gThrowableArray)
                {
                    gThrowableArray = relive_new ThrowableArray();
                }

                gThrowableArray->Add(mGrenadeCount);

                FP directedScale = {};
                if (GetAnimation().GetFlipX())
                {
                    directedScale = -GetSpriteScale();
                }
                else
                {
                    directedScale = GetSpriteScale();
                }
                auto pGrenade = relive_new Grenade(
                    mXPos + (FP_FromInteger(6) * directedScale),
                    mYPos + (-FP_FromInteger(6) * GetSpriteScale()),
                    mGrenadeCount,
                    false,
                    nullptr);
                if (pGrenade)
                {
                    pGrenade->VThrow(GetAnimation().GetFlipX() ? -FP_FromDouble(0.75) : FP_FromDouble(0.75), FP_FromInteger(3));
                }

                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::BoomMachine_Pipe_Idle));
                mState = BoomMachinePipeStates::eInactive;
            }
            break;
        default:
            return;
    }
}

void BoomMachinePipe::VScreenChanged()
{
    // Empty
}

} // namespace AO
