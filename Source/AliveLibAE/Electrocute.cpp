#include "stdafx.h"
#include "Electrocute.hpp"
#include "BaseAliveGameObject.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "stdlib.hpp"
#include "../relive_lib/VRam.hpp"
#include "Game.hpp"
#include "Function.hpp"
#include "Renderer/IRenderer.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

// Overwrites a pallete 8 colours at a time one per update
class PalleteOverwriter final : public BaseGameObject
{
public:
    PalleteOverwriter(AnimationPal& pal, s16 colour)
        : BaseGameObject(FALSE, 0)
    {
        SetType(ReliveTypes::ePalOverwriter);

        gObjListDrawables->Push_Back(this);

        mPal = pal;

        mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4);

        for (auto& palBufferEntry : field_B8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_CA_pal_w = 8;
        field_C8_pal_x_index = 1;
        field_CC_bFirstUpdate = 1;
        field_CE_bDone = FALSE;
    }

    ~PalleteOverwriter()
    {
        gObjListDrawables->Remove_Item(this);
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

    void VUpdate()
    {
        if (field_CC_bFirstUpdate || field_CE_bDone)
        {
            // First time round or when done do nothing
            field_CC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_C8_pal_x_index == 256 - 1)
            {
                // Got to the end
                field_CE_bDone = TRUE;
            }
            else
            {
                field_C8_pal_x_index += 8;

                if (field_C8_pal_x_index >= 256 - 1)
                {
                    field_C8_pal_x_index = 256 - 1;
                }

                if (field_C8_pal_x_index + field_CA_pal_w >= 256 - 1)
                {
                    field_CA_pal_w = 256 - field_C8_pal_x_index;
                }
            }
        }
    }

    void VRender(PrimHeader** /*ppOt*/)
    {
        if (!field_CE_bDone)
        {
            // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
            /*
            const IRenderer::PalRecord palRec{ static_cast<s16>(field_20_pal_xy.x + field_C8_pal_x_index), field_20_pal_xy.y, field_CA_pal_w};

            IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&field_B8_palBuffer[0]));
            */

            // TODO: Copy in the 8 new entries

            // TODO: Actually set this pal back on the anim
        }
    }

private:
    AnimationPal mPal;
    s16 field_B8_palBuffer[8] = {};
    s16 field_C8_pal_x_index = 0;
    s16 field_CA_pal_w = 0;
    s16 field_CC_bFirstUpdate = 0;

public:
    s16 field_CE_bDone = 0;
};
ALIVE_ASSERT_SIZEOF(PalleteOverwriter, 0xD0);

s32 Electrocute::VSub_4E6630()
{
    return vSub_4E6630();
}

Electrocute::Electrocute(BaseAliveGameObject* pTargetObj, bool bExtraOverwriter, bool bKillTarget)
    : BaseGameObject(TRUE, 0)
{
    SetType(ReliveTypes::eElectrocute);

    field_20_target_obj_id = pTargetObj->mBaseGameObjectId;
    field_44_state = States::eSetNewColour_0;
    field_3C_extraOverwriter = bExtraOverwriter;
    field_2C_bKillTarget = bKillTarget;
    field_2E_overwriter_count = bExtraOverwriter ? 3 : 2;
    //field_40_pPalData = nullptr;

    switch (pTargetObj->Type())
    {
        case ReliveTypes::eFlyingSlig:
        case ReliveTypes::eGlukkon:
        case ReliveTypes::eAbe:
        case ReliveTypes::eSlig:
            mPalData = pTargetObj->mAnim.mAnimRes.mTgaPtr->mPal;
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

    field_20_target_obj_id = Guid{};

    //relive_delete[] field_40_pPalData;
}

void Electrocute::VScreenChanged()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    // If the map has changed or target we are tracking has died then..
    if (gMap.mOverlayId != gMap.GetOverlayId() || (pTargetObj && pTargetObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead)))
    {
        VStop();
    }
}

void Electrocute::VUpdate()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_20_target_obj_id));
    if (!pTargetObj || pTargetObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
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

                pTargetObj->mBaseAliveGameObjectFlags.Set(Flags_114::e114_Bit11_Electrocuting);

                pTargetObj->mRGB.SetRGB(255, 255, 255);
                field_44_state = States::eAlphaFadeout_1;
                break;

            case States::eAlphaFadeout_1:
                field_30_pPalOverwriters[0] = relive_new PalleteOverwriter(
                    pTargetObj->mAnim.mAnimRes.mTgaPtr->mPal,
                    static_cast<s16>(Pal_Make_Colour(255u, 255, 255, 1)));

                field_30_pPalOverwriters[1] = relive_new PalleteOverwriter(
                    pTargetObj->mAnim.mAnimRes.mTgaPtr->mPal,
                    static_cast<s16>(Pal_Make_Colour(64u, 64, 255, 1)));
                if (field_30_pPalOverwriters[1])
                {
                    field_30_pPalOverwriters[1]->SetUpdateDelay(4);
                }

                if (field_3C_extraOverwriter)
                {
                    field_30_pPalOverwriters[2] = relive_new PalleteOverwriter(pTargetObj->mAnim.mAnimRes.mTgaPtr->mPal,

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
                        pTargetObj->mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                    }

                    pTargetObj->mRGB.SetRGB(field_24_r, field_26_g, field_28_b);
                    pTargetObj->mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit11_Electrocuting);

                    field_20_target_obj_id = Guid{};
                    field_44_state = States::eKillElectrocute_3;
                }
            }
            break;

            case States::eKillElectrocute_3:
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

    mBaseGameObjectFlags.Set(BaseGameObject::eDead);

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
            pTarget->mBaseAliveGameObjectFlags.Clear(Flags_114::e114_Bit11_Electrocuting);
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
