#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "ElectricWall.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Electrocute.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Map.hpp"

namespace AO {

ElectricWall::ElectricWall(relive::Path_ElectricWall* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eElectricWall);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Electric_Wall));
    Animation_Init(GetAnimRes(AnimId::Electric_Wall));

    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

    if (GetAnimation().Get_Frame_Count() > 0)
    {
        GetAnimation().SetFrame(Math_NextRandom() % GetAnimation().Get_Frame_Count());
    }

    mRGB.SetRGB(80, 80, 80);

    mTlvInfo = tlvId;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    mSwitchId = pTlv->mSwitchId;
    mStartState = pTlv->mStartState;

    if (SwitchStates_Get(pTlv->mSwitchId) == mStartState)
    {
        GetAnimation().SetRender(false);
    }

    mSoundTimer = 0;
}

ElectricWall::~ElectricWall()
{
    Path::TLV_Reset(mTlvInfo);
}

void ElectricWall::VScreenChanged()
{
    if (gMap.LevelChanged() || gMap.PathChanged() || gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) == CameraPos::eCamInvalid_m1)
    {
        SetDead(true);
    }
}

void ElectricWall::VUpdate()
{
    const CameraPos soundDirection = gMap.GetDirection(
        mCurrentLevel,
        mCurrentPath,
        mXPos,
        mYPos);

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (SwitchStates_Get(mSwitchId) == mStartState)
    {
        GetAnimation().SetRender(false);
    }
    else
    {
        GetAnimation().SetRender(true);

        // Keep flipping direction
        if (!(sGnFrame % 8))
        {
            GetAnimation().ToggleFlipX();
        }

        // Play sound every so often
        if (static_cast<s32>(sGnFrame) >= mSoundTimer)
        {
            // set a random starting frame
            SFX_Play_Camera(relive::SoundEffects::BirdPortalSpark, 45, soundDirection);
            mSoundTimer = MakeTimer(Math_RandomRange(24, 40));
        }


        const PSX_RECT bRect = VGetBoundingRect();

        PSX_RECT bRectBigger = {};
        bRectBigger.x = FP_GetExponent(mXPos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(mXPos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            IBaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
            if (!pObjIter)
            {
                break;
            }

            // Don't kill nades
            if (pObjIter->Type() != ReliveTypes::eGrenade)
            {
                PSX_RECT objRect = pObjIter->VGetBoundingRect();

                if (!RectsOverlap(bRectBigger, objRect))
                {
                    // Not touching, so every so often check if we are near
                    if (!(sGnFrame % 3))
                    {
                        // Make each side of the rect wider
                        objRect.x -= 50;
                        objRect.w += 50;

                        if (RectsOverlap(bRectBigger, objRect) && pObjIter->mHealth > FP_FromInteger(0))
                        {
                            SFX_Play_Camera(relive::SoundEffects::ElectricGateLoud, 45, soundDirection);
                        }
                    }
                }
                else
                {
                    // Touching the wall, rip
                    if (!pObjIter->GetElectrocuted())
                    {
                        pObjIter->SetElectrocuted(true);
                        relive_new Electrocute(pObjIter, 1);

                        pObjIter->VTakeDamage(this);

                        SFX_Play_Camera(relive::SoundEffects::ElectricZap, 127, soundDirection);

                        relive_new Flash(Layer::eLayer_Above_FG1_39, 255u, 255u, 255u);
                    }
                }
            }
        }
    }
}

} // namespace AO
