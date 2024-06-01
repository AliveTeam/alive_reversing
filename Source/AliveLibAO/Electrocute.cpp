#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Electrocute.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"
#include "PaletteOverwriter.hpp"

namespace AO {

Electrocute::Electrocute(BaseAliveGameObject* pTargetObj, s32 bExtraOverwriter)
    : BaseGameObject(true, 0),
      field_20_target_obj_id(pTargetObj->mBaseGameObjectId)
{
    SetType(ReliveTypes::eElectrocute);

    field_32_state = States::eSetNewColour_0;
    field_24_extraOverwriter = static_cast<s16>(bExtraOverwriter);
    field_14_overwriter_count = bExtraOverwriter ? 3 : 2;

    if (pTargetObj->Type() == ReliveTypes::eAbe)
    {
        mPalData = *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal;
        /*
        field_28_pPalData = relive_new u16[pTargetObj->mAnim.mPalDepth];
        Pal_Copy(
            pTargetObj->mAnim.mPalVramXY,
            pTargetObj->mAnim.mPalDepth,
            field_28_pPalData,
            &field_38_pal_rect);*/
    }

    // Note: Real game may leave a ptr un-inited depending on the count
    // we just do them all because its easier and safer.
    for (auto& pPalOverwriter : field_18_pPalOverwriters)
    {
        pPalOverwriter = nullptr;
    }
}

Electrocute::~Electrocute()
{
    for (auto& pPalOverwriter : field_18_pPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->SetDead(true);
        }
    }

   // relive_delete[] field_28_pPalData;
}

void Electrocute::VScreenChanged()
{
    ::BaseAliveGameObject* pTargetObj = static_cast<::BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));

    // If the map has changed or target we are tracking has died then..
    if (gMap.LevelChanged() || gMap.PathChanged() || (pTargetObj && pTargetObj->GetDead()))
    {
        Stop();
    }
}

void Electrocute::Stop()
{
    for (auto& pPalOverwriter : field_18_pPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->SetDead(true);
            pPalOverwriter = nullptr;
        }
    }

    SetDead(true);

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    if (pTarget)
    {
        if (pTarget->Type() == ReliveTypes::eAbe)
        {
            /* TODO: Set anim
            Pal_Set(
                field_10_obj_target->mAnim.mPalVramXY,
                field_10_obj_target->mAnim.mPalDepth,
                reinterpret_cast<const u8*>(field_28_pPalData),
                &field_38_pal_rect);*/
            pTarget->mRGB.SetRGB(field_2C_r, field_2E_g, field_30_b);
        }

        pTarget->VTakeDamage(this);
        field_20_target_obj_id = Guid{};
    }
}

// TODO all these need changing to RGB32
u32 Pal_Make_Colour(u8 r, u8 g, u8 b, s16 bOpaque)
{
    return (bOpaque != 0 ? 0x8000 : 0) + ((u32) r >> 3) + 4 * ((g & 0xF8) + 32 * (b & 0xF8));
}

void Electrocute::VUpdate()
{
    ::BaseAliveGameObject* pTargetObj = static_cast<::BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    if (!pTargetObj || pTargetObj->GetDead())
    {
        Stop();
        return;
    }

    switch (field_32_state)
    {
        case States::eSetNewColour_0:
            field_2C_r = pTargetObj->mRGB.r;
            field_2E_g = pTargetObj->mRGB.g;
            field_30_b = pTargetObj->mRGB.b;

            pTargetObj->mRGB.SetRGB(255, 255, 255);

            field_32_state = States::eAlphaFadeout_1;
            return;

        case States::eAlphaFadeout_1:
            field_18_pPalOverwriters[0] = relive_new PalleteOverwriter(
                *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                static_cast<s16>(Pal_Make_Colour(255u, 255, 255, 1)));

            field_18_pPalOverwriters[1] = relive_new PalleteOverwriter(
                *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,
                static_cast<s16>(Pal_Make_Colour(64u, 64, 255, 1)));
            if (field_18_pPalOverwriters[1])
            {
                field_18_pPalOverwriters[1]->SetUpdateDelay(4);
            }

            if (field_24_extraOverwriter)
            {
                field_18_pPalOverwriters[2] = relive_new PalleteOverwriter(
                    *pTargetObj->GetAnimation().mAnimRes.mPngPtr->mPal,

                    static_cast<s16>(Pal_Make_Colour(0, 0, 0, 0)));
                if (field_18_pPalOverwriters[2])
                {
                    field_18_pPalOverwriters[2]->SetUpdateDelay(8);
                    field_32_state = States::eHandleDamage_2;
                }
            }
            field_32_state = States::eHandleDamage_2;
            break;

        case States::eHandleDamage_2:
        {
            PalleteOverwriter* pOverwritter = field_18_pPalOverwriters[field_14_overwriter_count - 1];
            if (pOverwritter && pOverwritter->mDone)
            {
                pTargetObj->VTakeDamage(this);
                if (pTargetObj->Type() == ReliveTypes::eAbe)
                {
                    /* TODO: Set pal
                    Pal_Set(
                        field_10_obj_target->mAnim.mPalVramXY,
                        field_10_obj_target->mAnim.mPalDepth,
                        reinterpret_cast<const u8*>(field_28_pPalData),
                        &field_38_pal_rect);
                    */
                    pTargetObj->mRGB.SetRGB(field_2C_r, field_2E_g, field_30_b);


                }
                else
                {
                    field_20_target_obj_id = Guid{};
                }

                field_32_state = States::eKillElectrocute_3;
            }
        }
        break;

        case States::eKillElectrocute_3:
            SetDead(true);
            break;

        default:
            return;
    }
}

} // namespace AO
