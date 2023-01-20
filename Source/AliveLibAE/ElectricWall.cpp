#include "stdafx.h"
#include "ElectricWall.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Game.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/GameObjects/Flash.hpp"
#include "Abe.hpp"
#include "Electrocute.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "Math.hpp"

const s16 sElecticWallFrames_55165C[6] = {0, 6, 10, 18, 22, 0};

ElectricWall::ElectricWall(relive::Path_ElectricWall* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0),
    mGuid(tlvId),
    mSwitchId(pTlv->mSwitchId),
    mStartState(pTlv->mStartState)
{
    SetType(ReliveTypes::eElectricWall);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Electric_Wall));
    Animation_Init(GetAnimRes(AnimId::Electric_Wall));
    GetAnimation().SetSemiTrans(true);
    GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
    GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_36);

    if (GetAnimation().Get_Frame_Count() > 0)
    {
        GetAnimation().SetFrame(sElecticWallFrames_55165C[Math_RandomRange(0, 4)]);
    }

    SetApplyShadowZoneColour(false);
    mRGB.SetRGB(80, 80, 80);

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

    if (SwitchStates_Get(mSwitchId) == mStartState)
    {
        GetAnimation().SetRender(false);
    }
}

ElectricWall::~ElectricWall()
{
    Path::TLV_Reset(mGuid, -1, 0, 0);
}

void ElectricWall::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mNextLevel || gMap.mCurrentPath != gMap.mNextPath || gMap.GetDirection(mCurrentLevel, mCurrentPath, mXPos, mYPos) == CameraPos::eCamInvalid_m1)
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
        // If we are about to become visible set a random starting frame
        if (!(GetAnimation().GetRender()))
        {
            if (GetAnimation().Get_Frame_Count() > 0)
            {
                GetAnimation().SetFrame(sElecticWallFrames_55165C[Math_RandomRange(0, 4)]);
            }
        }

        GetAnimation().SetRender(true);

        // Keep flipping direction
        if (!(sGnFrame % 8))
        {
            GetAnimation().ToggleFlipX();
        }

        // Play sound every so often
        if (static_cast<s32>(sGnFrame) >= mSoundTimer)
        {
            SFX_Play_Camera(relive::SoundEffects::BirdPortalSpark, 45, soundDirection, GetSpriteScale());
            mSoundTimer = sGnFrame + Math_RandomRange(24, 40);
        }

        const PSX_RECT bRect = VGetBoundingRect();

        PSX_RECT bRectBigger = {};
        bRectBigger.x = FP_GetExponent(mXPos - FP_FromInteger(4));
        bRectBigger.y = static_cast<s16>(bRect.y + 5);
        bRectBigger.w = FP_GetExponent(mXPos + FP_FromInteger(4));
        bRectBigger.h = static_cast<s16>(bRect.h + 5);

        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
        {
            IBaseAliveGameObject* pObj = gBaseAliveGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            switch (pObj->Type())
            {
                // Can't zap this
                case ReliveTypes::eBone:
                case ReliveTypes::eRockSpawner:
                case ReliveTypes::eGrenade:
                case ReliveTypes::eMeat:
                case ReliveTypes::eRock:
                    break;

                default:
                    if (pObj->GetScale() == GetScale())
                    {
                        PSX_RECT objRect = pObj->VGetBoundingRect();

                        // If touching rect then we are fried
                        if (!RectsOverlap(bRectBigger, objRect))
                        {
                            // Not touching, so every so often check if we are near
                            if (!(sGnFrame % 3))
                            {
                                // Make each side of the rect wider
                                objRect.x -= 50;
                                objRect.w += 50;

                                if (RectsOverlap(bRectBigger, objRect) && pObj->mHealth > FP_FromInteger(0))
                                {
                                    // When near play the buzzing sound
                                    SFX_Play_Camera(relive::SoundEffects::ElectricGateLoud, 45, soundDirection, GetSpriteScale());
                                }
                            }
                        }
                        else
                        {
                            // Touching the wall, rip
                            if (!(pObj->GetElectrocuted()) && (!IsActiveHero(pObj) || !gAbeInvincible))
                            {
                                pObj->SetElectrocuted(true);

                                relive_new Electrocute(pObj, 1, 1);

                                pObj->VTakeDamage(this);

                                SFX_Play_Camera(relive::SoundEffects::ElectricZap, 127, soundDirection, GetSpriteScale());

                                relive_new Flash(Layer::eLayer_Above_FG1_39, 255, 255, 255, TPageAbr::eBlend_3, 1);
                            }
                        }
                    }
                    break;
            }
        }
    }
}
