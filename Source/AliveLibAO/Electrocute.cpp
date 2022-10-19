#include "stdafx_ao.h"
#include "Function.hpp"
#include "Electrocute.hpp"
#include "Psx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "VRam.hpp"
#include "Map.hpp"
#include "Renderer/IRenderer.hpp"

namespace AO {

class PalleteOverwriter final : public BaseGameObject
{
public:
    EXPORT PalleteOverwriter* ctor_416FF0(PSX_Point palXY, s16 palDepth, s16 colour)
    {
        ctor_487E10(1);

        SetVTable(this, 0x4BA9E8);

        field_4_typeId = Types::ePalOverwriter_29;

        gObjList_drawables_504618->Push_Back(this);

        field_10_pal_xy = palXY;
        field_14_pal_colours_count = palDepth;

        field_6_flags.Set(Options::eDrawable_Bit4);

        for (auto& palBufferEntry : field_A8_palBuffer)
        {
            palBufferEntry = colour;
        }

        field_BA_pal_w = 8;
        field_B8_pal_x_index = 1;
        field_BC_bFirstUpdate = 1;
        field_BE_bDone = FALSE;

        return this;
    }

    EXPORT BaseGameObject* dtor_4170B0()
    {
        SetVTable(this, 0x4BA9E8);
        gObjList_drawables_504618->Remove_Item(this);
        return dtor_487DF0();
    }

    virtual BaseGameObject* VDestructor(s32 flags) override
    {
        return Vdtor_417200(flags);
    }

    EXPORT PalleteOverwriter* Vdtor_417200(s32 flags)
    {
        dtor_4170B0();
        if (flags & 1)
        {
            ao_delete_free_447540(this);
        }
        return this;
    }

    virtual void VScreenChanged() override
    {
        // Stayin' alive
    }

    virtual void VRender(PrimHeader** ppOt) override
    {
        VRender_4171A0(ppOt);
    }

    EXPORT void VRender_4171A0(PrimHeader** /*ppOt*/)
    {
        if (!field_BE_bDone)
        {
            // TODO: FIX ME - abstraction break, the x value is used as an offset as to how much to overwrite, the width isn't isn't the pal depth in this case
            const IRenderer::PalRecord palRec{ static_cast<s16>(field_10_pal_xy.field_0_x + field_B8_pal_x_index), field_10_pal_xy.field_2_y, field_BA_pal_w };
            IRenderer::GetRenderer()->PalSetData(palRec, reinterpret_cast<u8*>(&field_A8_palBuffer[0]));
        }
    }

    virtual void VUpdate() override
    {
        VUpdate_417110();
    }

    EXPORT void VUpdate_417110()
    {
        if (field_BC_bFirstUpdate || field_BE_bDone)
        {
            // First time round or when done do nothing
            field_BC_bFirstUpdate = FALSE;
        }
        else
        {
            if (field_B8_pal_x_index == field_14_pal_colours_count - 1)
            {
                // Got to the end
                field_BE_bDone = TRUE;
            }
            else
            {
                field_B8_pal_x_index += 8;

                if (field_B8_pal_x_index >= field_14_pal_colours_count - 1)
                {
                    field_B8_pal_x_index = field_14_pal_colours_count - 1;
                }

                if (field_BA_pal_w + field_B8_pal_x_index >= field_14_pal_colours_count - 1)
                {
                    field_BA_pal_w = field_14_pal_colours_count - field_B8_pal_x_index;
                }
            }
        }
    }

    PSX_Point field_10_pal_xy;
    s16 field_14_pal_colours_count;
    // pad
    s32 field_18_not_used[36]; // TODO: Probably something used in PSX but not PC?
    s16 field_A8_palBuffer[8];
    s16 field_B8_pal_x_index;
    s16 field_BA_pal_w;
    s16 field_BC_bFirstUpdate;
    s16 field_BE_bDone;
};
ALIVE_ASSERT_SIZEOF(PalleteOverwriter, 0xC0);

Electrocute* Electrocute::ctor_48D3A0(BaseAliveGameObject* pTargetObj, s16 bExtraOverwriter)
{
    ctor_487E10(1);

    SetVTable(this, 0x4BD7E0);
    field_4_typeId = Types::eElectrocute_103;

    pTargetObj->field_C_refCount++;
    field_10_obj_target = pTargetObj;
    field_32_state = States::eSetNewColour_0;
    field_24_extraOverwriter = bExtraOverwriter;
    field_14_overwriter_count = bExtraOverwriter ? 3 : 2;
    field_28_pPalData = nullptr;

    if (pTargetObj->field_4_typeId == Types::eAbe_43)
    {
        field_28_pPalData = reinterpret_cast<u16*>(alloc_450740(sizeof(u16) * pTargetObj->field_10_anim.field_90_pal_depth));
        Pal_Copy_4479D0(
            pTargetObj->field_10_anim.field_8C_pal_vram_xy,
            pTargetObj->field_10_anim.field_90_pal_depth,
            field_28_pPalData,
            &field_38_pal_rect);
    }

    // Note: Real game may leave a ptr un-inited depending on the count
    // we just do them all because its easier and safer.
    for (auto& pPalOverwriter : field_18_pPalOverwriters)
    {
        pPalOverwriter = nullptr;
    }

    return this;
}

BaseGameObject* Electrocute::dtor_48D480()
{
    SetVTable(this, 0x4BD7E0);

    for (auto& pPalOverwriter : field_18_pPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->field_6_flags.Set(Options::eDead_Bit3);
        }
    }

    if (field_10_obj_target)
    {
        field_10_obj_target->field_C_refCount--;
    }

    if (field_28_pPalData)
    {
        ao_delete_free_450770(field_28_pPalData);
    }

    return dtor_487DF0();
}

BaseGameObject* Electrocute::VDestructor(s32 flags)
{
    return Vdtor_48D8E0(flags);
}

Electrocute* Electrocute::Vdtor_48D8E0(s32 flags)
{
    dtor_48D480();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Electrocute::VScreenChanged_48D8B0()
{
    // If the map has changed or target we are tracking has died then..
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || (field_10_obj_target && field_10_obj_target->field_6_flags.Get(BaseGameObject::eDead_Bit3)))
    {
        Stop_48D510();
    }
}

void Electrocute::Stop_48D510()
{
    for (auto& pPalOverwriter : field_18_pPalOverwriters)
    {
        if (pPalOverwriter)
        {
            pPalOverwriter->field_6_flags.Set(Options::eDead_Bit3);
            pPalOverwriter = nullptr;
        }
    }

    if (field_10_obj_target)
    {
        if (field_10_obj_target->field_4_typeId == Types::eAbe_43)
        {
            Pal_Set_447990(
                field_10_obj_target->field_10_anim.field_8C_pal_vram_xy,
                field_10_obj_target->field_10_anim.field_90_pal_depth,
                reinterpret_cast<const u8*>(field_28_pPalData),
                &field_38_pal_rect);
            field_10_obj_target->field_C0_r = field_2C_r;
            field_10_obj_target->field_C2_g = field_2E_g;
            field_10_obj_target->field_C4_b = field_30_b;
        }

        field_10_obj_target->VTakeDamage(this);
        field_10_obj_target->field_C_refCount--;
        field_10_obj_target = nullptr;

        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void Electrocute::VUpdate()
{
    VUpdate_48D5C0();
}

void Electrocute::VUpdate_48D5C0()
{
    if (field_10_obj_target && field_10_obj_target->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        Stop_48D510();
        return;
    }

    switch (field_32_state)
    {
        case States::eSetNewColour_0:
            field_2C_r = field_10_obj_target->field_C0_r;
            field_2E_g = field_10_obj_target->field_C2_g;
            field_30_b = field_10_obj_target->field_C4_b;

            field_10_obj_target->field_C0_r = 255;
            field_10_obj_target->field_C4_b = 255;
            field_10_obj_target->field_C2_g = 255;

            field_32_state = States::eAlphaFadeout_1;
            return;

        case States::eAlphaFadeout_1:
            field_18_pPalOverwriters[0] = ao_new<PalleteOverwriter>();
            if (field_18_pPalOverwriters[0])
                field_18_pPalOverwriters[0]->ctor_416FF0(
                    field_10_obj_target->field_10_anim.field_8C_pal_vram_xy,
                    field_10_obj_target->field_10_anim.field_90_pal_depth,
                    static_cast<s16>(Pal_Make_Colour_447950(255u, 255, 255, 1)));

            field_18_pPalOverwriters[1] = ao_new<PalleteOverwriter>();
            if (field_18_pPalOverwriters[1])
            {
                field_18_pPalOverwriters[1]->ctor_416FF0(
                    field_10_obj_target->field_10_anim.field_8C_pal_vram_xy,
                    field_10_obj_target->field_10_anim.field_90_pal_depth,
                    static_cast<s16>(Pal_Make_Colour_447950(64u, 64, 255, 1)));

                field_18_pPalOverwriters[1]->field_8_update_delay = 4;
            }

            if (field_24_extraOverwriter)
            {
                field_18_pPalOverwriters[2] = ao_new<PalleteOverwriter>();
                if (field_18_pPalOverwriters[2])
                {
                    field_18_pPalOverwriters[2]->ctor_416FF0(
                        field_10_obj_target->field_10_anim.field_8C_pal_vram_xy,
                        field_10_obj_target->field_10_anim.field_90_pal_depth,
                        static_cast<s16>(Pal_Make_Colour_447950(0, 0, 0, 0)));

                    field_18_pPalOverwriters[2]->field_8_update_delay = 8;
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
                if (field_10_obj_target->field_4_typeId == Types::eAbe_43)
                {
                    field_10_obj_target->VTakeDamage(this);
                    Pal_Set_447990(
                        field_10_obj_target->field_10_anim.field_8C_pal_vram_xy,
                        field_10_obj_target->field_10_anim.field_90_pal_depth,
                        reinterpret_cast<const u8*>(field_28_pPalData),
                        &field_38_pal_rect);

                    field_10_obj_target->field_C0_r = field_2C_r;
                    field_10_obj_target->field_C2_g = field_2E_g;
                    field_10_obj_target->field_C4_b = field_30_b;

                    field_32_state = States::eKillElectrocute_3;
                }
                else
                {
                    field_10_obj_target->VTakeDamage(this);
                    field_10_obj_target->field_C_refCount--;
                    field_10_obj_target = nullptr;
                    field_32_state = States::eKillElectrocute_3;
                }
            }
        }
        break;

        case States::eKillElectrocute_3:
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            break;

        default:
            return;
    }
}

void Electrocute::VScreenChanged()
{
    VScreenChanged_48D8B0();
}

} // namespace AO
