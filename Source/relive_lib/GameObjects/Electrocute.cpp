#include "stdafx.h"
#include "Electrocute.hpp"
#include "BaseAliveGameObject.hpp"
#include "../ObjectIds.hpp"
#include "../Function.hpp"
#include "MapWrapper.hpp"
#include "PaletteOverwriter.hpp"
#include "../GameType.hpp"

Electrocute::Electrocute(BaseAliveGameObject* pTargetObj, bool bExtraOverwriter, bool bKillTarget)
    : BaseGameObject(true, 0),
    mTargetObjId(pTargetObj->mBaseGameObjectId),
    mExtraOverwriter(bExtraOverwriter),
    mKillTarget(bKillTarget),
    mPalOverwritersCount(bExtraOverwriter ? 3 : 2)
{
    SetType(ReliveTypes::eElectrocute);

    if (pTargetObj->Type() == ReliveTypes::eFlyingSlig ||
        pTargetObj->Type() == ReliveTypes::eGlukkon ||
        pTargetObj->Type() == ReliveTypes::eAbe ||
        (pTargetObj->Type() == ReliveTypes::eSlig && GetGameType() == GameType::eAe))
    {
        // TODO: This probably isn't working, idea is that we take 2 copies of the pal
        // set the animation to be using one of these copies that that overwrite updates
        // then at the end set back the pal to be one we didn't mess with
        mOldPalData = *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal;
        mPalData = *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal;
        *pTargetObj->GetAnimation().mAnimRes.mCurPal = mPalData;
    }
}

Electrocute::~Electrocute()
{
    KillPalOverwriters();
}

void Electrocute::VScreenChanged()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetObjId));
    // If the map has changed or target we are tracking has died then..
    if (GetMap().LevelChanged() || GetMap().PathChanged() || (pTargetObj && pTargetObj->GetDead()))
    {
        Stop();
    }
}

static RGBA32 Pal_Make_Colour(u8 r, u8 g, u8 b, bool bOpaque)
{
    RGBA32 rgba;
    rgba.r = r;
    rgba.g = g;
    rgba.b = b;
    rgba.a = bOpaque ? 254 : 0; // TODO: Wrong ?
    return rgba;
}

void Electrocute::VUpdate()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetObjId));
    if (!pTargetObj || pTargetObj->GetDead())
    {
        Stop();
        return;
    }

    switch (mState)
    {
        case States::eSetNewColour:
            if (pTargetObj->Type() == ReliveTypes::eAbe && GetGameType() == GameType::eAe)
            {
                mTargetRed = 127;
                mTargetGreen = 127;
                mTargetBlue = 127;
            }
            else
            {
                mTargetRed = pTargetObj->mRGB.r;
                mTargetGreen = pTargetObj->mRGB.g;
                mTargetBlue = pTargetObj->mRGB.b;
            }

            pTargetObj->SetElectrocuting(true);

            pTargetObj->mRGB.SetRGB(255, 255, 255);
            mState = States::eAlphaFadeout;
            break;

        case States::eAlphaFadeout:
            mPalOverwriters[0] = relive_new PalleteOverwriter(
                mPalData,
                Pal_Make_Colour(255u, 255, 255, true));

            mPalOverwriters[1] = relive_new PalleteOverwriter(
                mPalData,
                Pal_Make_Colour(64u, 64, 255, true));
            if (mPalOverwriters[1])
            {
                mPalOverwriters[1]->SetUpdateDelay(4);
            }

            if (mExtraOverwriter)
            {
                mPalOverwriters[2] = relive_new PalleteOverwriter(mPalData,
                    Pal_Make_Colour(0, 0, 0, false));
                if (mPalOverwriters[2])
                {
                    mPalOverwriters[2]->SetUpdateDelay(8);
                }
            }

            mState = States::eHandleDamage;
            break;

        case States::eHandleDamage:
        {
            PalleteOverwriter* pPalOverwriter = mPalOverwriters[mPalOverwritersCount - 1];
            if (pPalOverwriter && pPalOverwriter->mDone)
            {
                if (mKillTarget)
                {
                    pTargetObj->VTakeDamage(this);
                }
                else
                {
                    pTargetObj->GetAnimation().SetRender(false);
                }

                if (DeElectrocuteTarget(false))
                {
                    mState = States::eKillElectrocute;
                }
            }
        }
        break;

        case States::eKillElectrocute:
            SetDead(true);
            break;

        default:
            break;
    }

    for (auto& pPalOverwriter : mPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->VUpdate();
        }
    }
}

void Electrocute::Stop()
{
    KillPalOverwriters();

    SetDead(true);

    DeElectrocuteTarget(true);
}

void Electrocute::KillPalOverwriters()
{
    for (auto& pPalOverwriter : mPalOverwriters)
    {
        relive_delete pPalOverwriter;
        pPalOverwriter = nullptr;
    }
}

bool Electrocute::DeElectrocuteTarget(bool dealDamage)
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetObjId));
    if (pTarget)
    {
        if (GetGameType() == GameType::eAo && pTarget->Type() == ReliveTypes::eAbe ||
            GetGameType() == GameType::eAe)
        {
            // Restore old pal
            *pTarget->GetAnimation().mAnimRes.mPngPtr->mPal = mOldPalData;

            pTarget->mRGB.SetRGB(mTargetRed, mTargetGreen, mTargetBlue);
            pTarget->SetElectrocuting(false);
        }

        if (dealDamage)
        {
            pTarget->VTakeDamage(this);
        }

        mTargetObjId = Guid{};
        return true;
    }
    return false;
}

bool Electrocute::IsLastOverwriterFinished()
{
    for (s32 i = mPalOverwritersCount - 1; i >= 0; i--)
    {
        if (mPalOverwriters[i] && mPalOverwriters[i]->mDone)
        {
            return true;
        }
    }
    return false;
}
