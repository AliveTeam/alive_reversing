#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Electrocute.hpp"
#include "../relive_lib/GameObjects/BaseAliveGameObject.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Map.hpp"

#define kPalDepth 64

namespace AO {

class PalleteOverwriter final : public ::BaseGameObject
{
public:
    PalleteOverwriter(AnimationPal& /*pal*/, s16 colour)
        : BaseGameObject(true, 0)
    {
        SetType(ReliveTypes::ePalOverwriter);

        gObjListDrawables->Push_Back(this);

        SetDrawable(true);

        for (auto& palBufferEntry : field_A8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_BA_pal_w = 8;
        field_B8_pal_x_index = 1;
        field_BC_bFirstUpdate = 1;
        field_BE_bDone = false;
    }

    ~PalleteOverwriter()
    {
        gObjListDrawables->Remove_Item(this);
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

    virtual void VRender(OrderingTable& /*ot*/) override
    {
        if (!field_BE_bDone)
        {
            // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
            /*
            const IRenderer::PalRecord palRec{ static_cast<s16>(field_10_pal_xy.x + field_B8_pal_x_index), field_10_pal_xy.y, field_BA_pal_w };
            IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&field_A8_palBuffer[0]));*/

            // TODO: Set the next 8 pal entries + update anim
        }
    }

    virtual void VUpdate() override
    {
        if (field_BC_bFirstUpdate || field_BE_bDone)
        {
            // First time round or when done do nothing
            field_BC_bFirstUpdate = false;
        }
        else
        {
            if (field_B8_pal_x_index == kPalDepth - 1)
            {
                // Got to the end
                field_BE_bDone = true;
            }
            else
            {
                field_B8_pal_x_index += 8;

                if (field_B8_pal_x_index >= kPalDepth - 1)
                {
                    field_B8_pal_x_index = kPalDepth - 1;
                }

                if (field_BA_pal_w + field_B8_pal_x_index >= kPalDepth - 1)
                {
                    field_BA_pal_w = kPalDepth - field_B8_pal_x_index;
                }
            }
        }
    }

    s16 field_A8_palBuffer[8] = {};
    s16 field_B8_pal_x_index = 0;
    s16 field_BA_pal_w = 0;
    s16 field_BC_bFirstUpdate = 0;
    s16 field_BE_bDone = 0;
};

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
            if (pOverwritter && pOverwritter->field_BE_bDone)
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
