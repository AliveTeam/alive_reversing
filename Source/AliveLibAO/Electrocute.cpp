#include "stdafx_ao.h"
#include "Function.hpp"
#include "Electrocute.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/VRam.hpp"
#include "Map.hpp"

#define kPalDepth 64

namespace AO {

class PalleteOverwriter final : public ::BaseGameObject
{
public:
    PalleteOverwriter(AnimationPal& /*pal*/, s16 colour)
        : BaseGameObject(TRUE, 0)
    {
        SetType(ReliveTypes::ePalOverwriter);

        gObjListDrawables->Push_Back(this);

        mBaseGameObjectFlags.Set(Options::eDrawable_Bit4);

        for (auto& palBufferEntry : field_A8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_BA_pal_w = 8;
        field_B8_pal_x_index = 1;
        field_BC_bFirstUpdate = 1;
        field_BE_bDone = FALSE;
    }

    ~PalleteOverwriter()
    {
        gObjListDrawables->Remove_Item(this);
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

    virtual void VRender(PrimHeader** /*ppOt*/) override
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
            field_BC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_B8_pal_x_index == kPalDepth - 1)
            {
                // Got to the end
                field_BE_bDone = TRUE;
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
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eElectrocute);

    pTargetObj->mBaseGameObjectRefCount++;
    field_10_obj_target = pTargetObj;
    field_32_state = States::eSetNewColour_0;
    field_24_extraOverwriter = static_cast<s16>(bExtraOverwriter);
    field_14_overwriter_count = bExtraOverwriter ? 3 : 2;

    if (pTargetObj->Type() == ReliveTypes::eAbe)
    {
        mPalData = *pTargetObj->mAnim.mAnimRes.mTgaPtr->mPal;
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
            pPalOverwriter->mBaseGameObjectFlags.Set(Options::eDead);
        }
    }

    if (field_10_obj_target)
    {
        field_10_obj_target->mBaseGameObjectRefCount--;
    }

   // relive_delete[] field_28_pPalData;
}

void Electrocute::VScreenChanged()
{
    // If the map has changed or target we are tracking has died then..
    if (gMap.mOverlayId != gMap.GetOverlayId()
        || (field_10_obj_target && field_10_obj_target->mBaseGameObjectFlags.Get(BaseGameObject::eDead)))
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
            pPalOverwriter->mBaseGameObjectFlags.Set(Options::eDead);
            pPalOverwriter = nullptr;
        }
    }

    if (field_10_obj_target)
    {
        if (field_10_obj_target->Type() == ReliveTypes::eAbe)
        {
            /* TODO: Set anim
            Pal_Set(
                field_10_obj_target->mAnim.mPalVramXY,
                field_10_obj_target->mAnim.mPalDepth,
                reinterpret_cast<const u8*>(field_28_pPalData),
                &field_38_pal_rect);*/
            field_10_obj_target->mRGB.r = field_2C_r;
            field_10_obj_target->mRGB.g = field_2E_g;
            field_10_obj_target->mRGB.b = field_30_b;
        }

        field_10_obj_target->VTakeDamage(this);
        field_10_obj_target->mBaseGameObjectRefCount--;
        field_10_obj_target = nullptr;

        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void Electrocute::VUpdate()
{
    if (field_10_obj_target && field_10_obj_target->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        Stop();
        return;
    }

    switch (field_32_state)
    {
        case States::eSetNewColour_0:
            field_2C_r = field_10_obj_target->mRGB.r;
            field_2E_g = field_10_obj_target->mRGB.g;
            field_30_b = field_10_obj_target->mRGB.b;

            field_10_obj_target->mRGB.SetRGB(255, 255, 255);

            field_32_state = States::eAlphaFadeout_1;
            return;

        case States::eAlphaFadeout_1:
            field_18_pPalOverwriters[0] = relive_new PalleteOverwriter(
                *field_10_obj_target->mAnim.mAnimRes.mTgaPtr->mPal,
                static_cast<s16>(Pal_Make_Colour(255u, 255, 255, 1)));

            field_18_pPalOverwriters[1] = relive_new PalleteOverwriter(
                *field_10_obj_target->mAnim.mAnimRes.mTgaPtr->mPal,
                static_cast<s16>(Pal_Make_Colour(64u, 64, 255, 1)));
            if (field_18_pPalOverwriters[1])
            {
                field_18_pPalOverwriters[1]->SetUpdateDelay(4);
            }

            if (field_24_extraOverwriter)
            {
                field_18_pPalOverwriters[2] = relive_new PalleteOverwriter(
                    *field_10_obj_target->mAnim.mAnimRes.mTgaPtr->mPal,

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
                if (field_10_obj_target->Type() == ReliveTypes::eAbe)
                {
                    field_10_obj_target->VTakeDamage(this);
                    /* TODO: Set pal
                    Pal_Set(
                        field_10_obj_target->mAnim.mPalVramXY,
                        field_10_obj_target->mAnim.mPalDepth,
                        reinterpret_cast<const u8*>(field_28_pPalData),
                        &field_38_pal_rect);
                    */
                    field_10_obj_target->mRGB.r = field_2C_r;
                    field_10_obj_target->mRGB.g = field_2E_g;
                    field_10_obj_target->mRGB.b = field_30_b;

                    field_32_state = States::eKillElectrocute_3;
                }
                else
                {
                    field_10_obj_target->VTakeDamage(this);
                    field_10_obj_target->mBaseGameObjectRefCount--;
                    field_10_obj_target = nullptr;
                    field_32_state = States::eKillElectrocute_3;
                }
            }
        }
        break;

        case States::eKillElectrocute_3:
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            break;

        default:
            return;
    }
}

} // namespace AO
