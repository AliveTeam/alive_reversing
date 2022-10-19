#include "stdafx.h"
#include "Electrocute.hpp"
#include "BaseAliveGameObject.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "Function.hpp"
#include "Renderer/IRenderer.hpp"

// Overwrites a pallete 8 colours at a time one per update
class PalleteOverwriter final : public BaseGameObject
{
public:
    EXPORT PalleteOverwriter* ctor_4228D0(PSX_Point palXY, s16 palDepth, s16 colour)
    {
        BaseGameObject_ctor_4DBFA0(FALSE, 0);

        SetVTable(this, 0x544BC4); // vTbl_Class_544BC4

        SetType(AETypes::ePalOverwriter_44);

        gObjList_drawables_5C1124->Push_Back(this);

        field_20_pal_xy = palXY;
        field_24_pal_colours_count = palDepth;

        field_6_flags.Set(BaseGameObject::eDrawable_Bit4);

        for (auto& palBufferEntry : field_B8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_CA_pal_w = 8;
        field_C8_pal_x_index = 1;
        field_CC_bFirstUpdate = 1;
        field_CE_bDone = FALSE;

        return this;
    }

    virtual BaseGameObject* VDestructor(s32 flags) override
    {
        return vdtor_4229C0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_422A70();
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        vRender_422B30(ppOt);
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

private:
    EXPORT void vUpdate_422A70()
    {
        if (field_CC_bFirstUpdate || field_CE_bDone)
        {
            // First time round or when done do nothing
            field_CC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_C8_pal_x_index == field_24_pal_colours_count - 1)
            {
                // Got to the end
                field_CE_bDone = TRUE;
            }
            else
            {
                field_C8_pal_x_index += 8;

                if (field_C8_pal_x_index >= field_24_pal_colours_count - 1)
                {
                    field_C8_pal_x_index = field_24_pal_colours_count - 1;
                }

                if (field_C8_pal_x_index + field_CA_pal_w >= field_24_pal_colours_count - 1)
                {
                    field_CA_pal_w = field_24_pal_colours_count - field_C8_pal_x_index;
                }
            }
        }
    }

    EXPORT void dtor_4229F0()
    {
        SetVTable(this, 0x544BC4); // vTbl_Class_544BC4
        gObjList_drawables_5C1124->Remove_Item(this);
        BaseGameObject_dtor_4DBEC0();
    }

    EXPORT PalleteOverwriter* vdtor_4229C0(s32 flags)
    {
        dtor_4229F0();
        if (flags & 1)
        {
            ae_delete_free_495540(this);
        }
        return this;
    }

    EXPORT void vRender_422B30(PrimHeader** /*ppOt*/)
    {
        if (!field_CE_bDone)
        {
            // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
            const IRenderer::PalRecord palRec{ static_cast<s16>(field_20_pal_xy.field_0_x + field_C8_pal_x_index), field_20_pal_xy.field_2_y, field_CA_pal_w};

            IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&field_B8_palBuffer[0]));
        }
    }

private:
    PSX_Point field_20_pal_xy;
    s16 field_24_pal_colours_count;
    // pad
    s32 field_28_not_used[36]; // TODO: Probably something used in PSX but not PC?
    s16 field_B8_palBuffer[8];
    s16 field_C8_pal_x_index;
    s16 field_CA_pal_w;
    s16 field_CC_bFirstUpdate;

public:
    s16 field_CE_bDone;
};
ALIVE_ASSERT_SIZEOF(PalleteOverwriter, 0xD0);

BaseGameObject* Electrocute::VDestructor(s32 flags)
{
    return vdtor_4E6060(flags);
}

void Electrocute::VUpdate()
{
    vUpdate_4E6240();
}

void Electrocute::VScreenChanged()
{
    vScreenChanged_4E65E0();
}

void Electrocute::VStop_4E6150()
{
    vStop_4E6150();
}

s32 Electrocute::VSub_4E6630()
{
    return vSub_4E6630();
}

Electrocute* Electrocute::ctor_4E5E80(BaseAliveGameObject* pTargetObj, s16 bExtraOverwriter, s16 bKillTarget)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);

    SetVTable(this, 0x548100); // vTbl_Class_548100
    SetType(AETypes::eElectrocute_150);

    field_20_target_obj_id = pTargetObj->field_8_object_id;
    field_44_state = States::eSetNewColour_0;
    field_3C_extraOverwriter = bExtraOverwriter;
    field_2C_bKillTarget = bKillTarget;
    field_2E_overwriter_count = bExtraOverwriter ? 3 : 2;
    field_40_pPalData = nullptr;

    switch (pTargetObj->Type())
    {
        case AETypes::eFlyingSlig_54:
        case AETypes::eGlukkon_67:
        case AETypes::eAbe_69:
        case AETypes::eSlig_125:
            field_40_pPalData = reinterpret_cast<u16*>(ae_malloc_non_zero_4954F0(sizeof(u16) * pTargetObj->field_20_animation.field_90_pal_depth));
            Pal_Copy_483560(
                pTargetObj->field_20_animation.field_8C_pal_vram_xy,
                pTargetObj->field_20_animation.field_90_pal_depth,
                field_40_pPalData,
                &field_4C_pal_rect);
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

    return this;
}

Electrocute* Electrocute::vdtor_4E6060(s32 flags)
{
    dtor_4E6090();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void Electrocute::dtor_4E6090()
{
    SetVTable(this, 0x548100); // vTbl_Class_548100

    for (auto& pPalOverwriter : field_30_pPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->VDestructor(1);
        }
    }

    field_20_target_obj_id = -1;

    if (field_40_pPalData)
    {
        ae_non_zero_free_495560(field_40_pPalData);
    }

    // NOTE: omitted vtable vTbl_IClass_548128
    BaseGameObject_dtor_4DBEC0();
}

void Electrocute::vScreenChanged_4E65E0()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_20_target_obj_id));
    // If the map has changed or target we are tracking has died then..
    if (gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path || (pTargetObj && pTargetObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
    {
        VStop_4E6150();
    }
}

void Electrocute::vUpdate_4E6240()
{
    BaseAliveGameObject* pTargetObj = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_20_target_obj_id));
    if (!pTargetObj || pTargetObj->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        VStop_4E6150();
    }
    else
    {
        switch (field_44_state)
        {
            case States::eSetNewColour_0:
                if (pTargetObj->Type() == AETypes::eAbe_69)
                {
                    field_24_r = 127;
                    field_26_g = 127;
                    field_28_b = 127;
                }
                else
                {
                    field_24_r = pTargetObj->field_D0_r;
                    field_26_g = pTargetObj->field_D2_g;
                    field_28_b = pTargetObj->field_D4_b;
                }

                pTargetObj->field_114_flags.Set(Flags_114::e114_Bit11_Electrocuting);

                pTargetObj->field_D4_b = 255;
                pTargetObj->field_D2_g = 255;
                pTargetObj->field_D0_r = 255;
                field_44_state = States::eAlphaFadeout_1;
                break;

            case States::eAlphaFadeout_1:
                field_30_pPalOverwriters[0] = ae_new<PalleteOverwriter>();
                if (field_30_pPalOverwriters[0])
                {
                    field_30_pPalOverwriters[0]->ctor_4228D0(
                        pTargetObj->field_20_animation.field_8C_pal_vram_xy,
                        pTargetObj->field_20_animation.field_90_pal_depth,
                        static_cast<s16>(Pal_Make_Colour_4834C0(255u, 255, 255, 1)));
                }

                field_30_pPalOverwriters[1] = ae_new<PalleteOverwriter>();
                if (field_30_pPalOverwriters[1])
                {
                    field_30_pPalOverwriters[1]->ctor_4228D0(
                        pTargetObj->field_20_animation.field_8C_pal_vram_xy,
                        pTargetObj->field_20_animation.field_90_pal_depth,
                        static_cast<s16>(Pal_Make_Colour_4834C0(64u, 64, 255, 1)));

                    field_30_pPalOverwriters[1]->SetUpdateDelay(4);
                }

                if (field_3C_extraOverwriter)
                {
                    field_30_pPalOverwriters[2] = ae_new<PalleteOverwriter>();
                    if (field_30_pPalOverwriters[2])
                    {
                        field_30_pPalOverwriters[2]->ctor_4228D0(
                            pTargetObj->field_20_animation.field_8C_pal_vram_xy,
                            pTargetObj->field_20_animation.field_90_pal_depth,
                            static_cast<s16>(Pal_Make_Colour_4834C0(0, 0, 0, 0)));

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
                    if (field_40_pPalData)
                    {
                        Pal_Set_483510(
                            pTargetObj->field_20_animation.field_8C_pal_vram_xy,
                            pTargetObj->field_20_animation.field_90_pal_depth,
                            reinterpret_cast<const u8*>(field_40_pPalData),
                            &field_4C_pal_rect);
                    }

                    if (field_2C_bKillTarget)
                    {
                        pTargetObj->VTakeDamage_408730(this);
                    }
                    else
                    {
                        pTargetObj->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                    }

                    pTargetObj->field_D0_r = field_24_r;
                    pTargetObj->field_D2_g = field_26_g;
                    pTargetObj->field_D4_b = field_28_b;

                    pTargetObj->field_114_flags.Clear(Flags_114::e114_Bit11_Electrocuting);

                    field_20_target_obj_id = -1;
                    field_44_state = States::eKillElectrocute_3;
                }
            }
            break;

            case States::eKillElectrocute_3:
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
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

void Electrocute::vStop_4E6150()
{
    for (auto& pPalOverwriter : field_30_pPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->VDestructor(1);
            pPalOverwriter = nullptr;
        }
    }

    field_6_flags.Set(BaseGameObject::eDead_Bit3);

    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_20_target_obj_id));
    if (pTarget)
    {
        if (field_40_pPalData)
        {
            Pal_Set_483510(
                pTarget->field_20_animation.field_8C_pal_vram_xy,
                pTarget->field_20_animation.field_90_pal_depth,
                reinterpret_cast<const u8*>(field_40_pPalData),
                &field_4C_pal_rect);

            pTarget->field_D0_r = field_24_r;
            pTarget->field_D2_g = field_26_g;
            pTarget->field_D4_b = field_28_b;

            pTarget->field_114_flags.Clear(Flags_114::e114_Bit11_Electrocuting);
        }

        pTarget->VTakeDamage_408730(this);
        field_20_target_obj_id = -1;
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
