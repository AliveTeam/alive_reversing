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
        mPalData = *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal;
    }

    // Note: Real game may leave a ptr un-inited depending on the count
    // we just do them all because its easier and safer.
    for (auto& pPalOverwriter : mPalOverwriters)
    {
        pPalOverwriter = nullptr;
    }
}

Electrocute::~Electrocute()
{
    for (auto& pPalOverwriter : mPalOverwriters)
    {
        relive_delete pPalOverwriter;
    }
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

// TODO all these need changing to RGB32
u32 Pal_Make_Colour(u8 r, u8 g, u8 b, s16 bOpaque)
{
    return (bOpaque != 0 ? 0x8000 : 0) + ((u32) r >> 3) + 4 * ((g & 0xF8) + 32 * (b & 0xF8));
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
                *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                static_cast<s16>(Pal_Make_Colour(255u, 255, 255, 1)));

            mPalOverwriters[1] = relive_new PalleteOverwriter(
                *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                static_cast<s16>(Pal_Make_Colour(64u, 64, 255, 1)));
            if (mPalOverwriters[1])
            {
                mPalOverwriters[1]->SetUpdateDelay(4);
            }

            if (mExtraOverwriter)
            {
                mPalOverwriters[2] = relive_new PalleteOverwriter(*pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                    static_cast<s16>(Pal_Make_Colour(0, 0, 0, 0)));
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

                if (pTargetObj->Type() == ReliveTypes::eAbe && GetGameType() == GameType::eAo ||
                    GetGameType() == GameType::eAe)
                {
                    /* TODO: update the pal
                    if (field_40_pPalData)
                    {
                        Pal_Set(
                            pTargetObj->mAnim.mPalVramXY,
                            pTargetObj->mAnim.mPalDepth,
                            reinterpret_cast<const u8*>(field_40_pPalData),
                            &field_4C_pal_rect);
                    }*/

                    // Restore the previous rgb values before the target was electrocuted
                    pTargetObj->mRGB.SetRGB(mTargetRed, mTargetGreen, mTargetBlue);
                    pTargetObj->SetElectrocuting(false);

                    mTargetObjId = Guid{};
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
    for (auto& pPalOverwriter : mPalOverwriters)
    {
        relive_delete pPalOverwriter;
        pPalOverwriter = nullptr;
    }

    SetDead(true);

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetObjId));
    if (pTarget)
    {
        if (GetGameType() == GameType::eAo && pTarget->Type() == ReliveTypes::eAbe ||
            GetGameType() == GameType::eAe)
        {
            /* TODO: Update pal
            Pal_Set(
                pTarget->mAnim.mPalVramXY,
                pTarget->mAnim.mPalDepth,
                reinterpret_cast<const u8*>(field_40_pPalData),
                &field_4C_pal_rect);
            */
            pTarget->mRGB.SetRGB(mTargetRed, mTargetGreen, mTargetBlue);
            pTarget->SetElectrocuting(false);
        }

        pTarget->VTakeDamage(this);
        mTargetObjId = Guid{};
    }
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
