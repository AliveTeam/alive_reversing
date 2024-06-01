#include "stdafx.h"
#include "Electrocute.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "../relive_lib/Function.hpp"
#include "Map.hpp"
#include "PaletteOverwriter.hpp"

s32 Electrocute::VSub_4E6630()
{
    return vSub_4E6630();
}

Electrocute::Electrocute(BaseAliveGameObject* pTargetObj, bool bExtraOverwriter, bool bKillTarget)
    : BaseGameObject(true, 0),
    field_20_target_obj_id(pTargetObj->mBaseGameObjectId),
    field_3C_extraOverwriter(bExtraOverwriter),
    field_2C_bKillTarget(bKillTarget),
    field_2E_overwriter_count(bExtraOverwriter ? 3 : 2)
{
    SetType(ReliveTypes::eElectrocute);

    //field_40_pPalData = nullptr;

    switch (pTargetObj->Type())
    {
        case ReliveTypes::eFlyingSlig:
        case ReliveTypes::eGlukkon:
        case ReliveTypes::eAbe:
        case ReliveTypes::eSlig:
            mPalData = *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal;
            /*
            field_40_pPalData = relive_new u16[pTargetObj->mAnim.mPalDepth];
            Pal_Copy(
                pTargetObj->mAnim.mPalVramXY,
                pTargetObj->mAnim.mPalDepth,
                field_40_pPalData,
                &field_4C_pal_rect);
            */
            break;
        default:
            break;
    }

    // Note: Real game may leave a ptr un-inited depending on the count
    // we just do them all because its easier and safer.
    for (auto& pPalOverwriter : field_30_pPalOverwriters)
    {
        pPalOverwriter = nullptr;
    }
}

Electrocute::~Electrocute()
{
    for (auto& pPalOverwriter : field_30_pPalOverwriters)
    {
        relive_delete pPalOverwriter;
    }

    //relive_delete[] field_40_pPalData;
}

void Electrocute::VScreenChanged()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    // If the map has changed or target we are tracking has died then..
    if (gMap.LevelChanged() || gMap.PathChanged() || (pTargetObj && pTargetObj->GetDead()))
    {
        VStop();
    }
}

// TODO all these need changing to RGB32
u32 Pal_Make_Colour(u8 r, u8 g, u8 b, s16 bOpaque)
{
    return (bOpaque != 0 ? 0x8000 : 0) + ((u32) r >> 3) + 4 * ((g & 0xF8) + 32 * (b & 0xF8));
}

void Electrocute::VUpdate()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    if (!pTargetObj || pTargetObj->GetDead())
    {
        VStop();
    }
    else
    {
        switch (field_44_state)
        {
            case States::eSetNewColour_0:
                if (pTargetObj->Type() == ReliveTypes::eAbe)
                {
                    field_24_r = 127;
                    field_26_g = 127;
                    field_28_b = 127;
                }
                else
                {
                    field_24_r = pTargetObj->mRGB.r;
                    field_26_g = pTargetObj->mRGB.g;
                    field_28_b = pTargetObj->mRGB.b;
                }

                pTargetObj->SetElectrocuting(true);

                pTargetObj->mRGB.SetRGB(255, 255, 255);
                field_44_state = States::eAlphaFadeout_1;
                break;

            case States::eAlphaFadeout_1:
                field_30_pPalOverwriters[0] = relive_new PalleteOverwriter(
                    *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                    static_cast<s16>(Pal_Make_Colour(255u, 255, 255, 1)));

                field_30_pPalOverwriters[1] = relive_new PalleteOverwriter(
                    *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                    static_cast<s16>(Pal_Make_Colour(64u, 64, 255, 1)));
                if (field_30_pPalOverwriters[1])
                {
                    field_30_pPalOverwriters[1]->SetUpdateDelay(4);
                }

                if (field_3C_extraOverwriter)
                {
                    field_30_pPalOverwriters[2] = relive_new PalleteOverwriter(*pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,

                        static_cast<s16>(Pal_Make_Colour(0, 0, 0, 0)));
                    if (field_30_pPalOverwriters[2])
                    {
                        field_30_pPalOverwriters[2]->SetUpdateDelay(8);
                    }
                }

                field_44_state = States::eHandleDamage_2;
                break;

            case States::eHandleDamage_2:
            {
                PalleteOverwriter* pPalOverwriter = field_30_pPalOverwriters[field_2E_overwriter_count - 1];
                if (!pPalOverwriter || pPalOverwriter->field_CE_bDone)
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

                    if (field_2C_bKillTarget)
                    {
                        pTargetObj->VTakeDamage(this);
                    }
                    else
                    {
                        pTargetObj->GetAnimation().SetRender(false);
                    }

                    pTargetObj->mRGB.SetRGB(field_24_r, field_26_g, field_28_b);
                    pTargetObj->SetElectrocuting(false);

                    field_20_target_obj_id = Guid{};
                    field_44_state = States::eKillElectrocute_3;
                }
            }
            break;

            case States::eKillElectrocute_3:
                SetDead(true);
                break;

            default:
                break;
        }

        for (auto& pPalOverwriter : field_30_pPalOverwriters)
        {
            if (pPalOverwriter)
            {
                pPalOverwriter->VUpdate();
            }
        }
    }
}

void Electrocute::VStop()
{
    for (auto& pPalOverwriter : field_30_pPalOverwriters)
    {
        delete pPalOverwriter;
        pPalOverwriter = nullptr;
    }

    SetDead(true);

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    if (pTarget)
    {
        //if (field_40_pPalData)
        {
            /* TODO: Update pal
            Pal_Set(
                pTarget->mAnim.mPalVramXY,
                pTarget->mAnim.mPalDepth,
                reinterpret_cast<const u8*>(field_40_pPalData),
                &field_4C_pal_rect);
            */
            pTarget->mRGB.SetRGB(field_24_r, field_26_g, field_28_b);
            pTarget->SetElectrocuting(false);
        }

        pTarget->VTakeDamage(this);
        field_20_target_obj_id = Guid{};
    }
}

s32 Electrocute::vSub_4E6630()
{
    s32 counter = field_2E_overwriter_count - 1;
    if (counter < 0)
    {
        return 0;
    }

    PalleteOverwriter** pIter = &field_30_pPalOverwriters[counter];
    while (!*pIter || !(*pIter)->field_CE_bDone)
    {
        --counter;
        --pIter;
        if (counter < 0)
        {
            return 0;
        }
    }

    return counter + 1;
}
