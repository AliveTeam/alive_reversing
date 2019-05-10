#include "stdafx.h"
#include "InvisibleEffect.hpp"
#include "Function.hpp"
#include "VRam.hpp"
#include "PossessionFlicker.hpp"
#include "Events.hpp"
#include "ObjectIds.hpp"
#include "VRam.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

EXPORT InvisibleEffect* InvisibleEffect::ctor_45F280(BaseAliveGameObject* pTarget)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545A60);

    field_4_typeId = Types::eInvisibleEffect_75;

    field_44_objId = pTarget->field_8_object_id;

    field_24_pAlloc = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(pTarget->field_20_animation.field_90_pal_depth * sizeof(WORD)));
    Pal_Copy_483560(
        pTarget->field_20_animation.field_8C_pal_vram_xy,
        pTarget->field_20_animation.field_90_pal_depth,
        field_24_pAlloc,
        &field_28);

    field_30_pPalAlloc = reinterpret_cast<WORD*>(malloc_non_zero_4954F0(pTarget->field_20_animation.field_90_pal_depth * sizeof(WORD)));
    Pal_Copy_483560(
        pTarget->field_20_animation.field_8C_pal_vram_xy,
        pTarget->field_20_animation.field_90_pal_depth,
        field_30_pPalAlloc,
        &field_34);

    field_4A_flags = 0;

    if (pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit15_bSemiTrans))
    {
        field_4A_flags |= 0x1;
    }
    if (pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
    {
        field_4A_flags |= 0x2;
    }

    field_4A_flags &= ~4u;
    field_48_old_render_mode = pTarget->field_20_animation.field_B_render_mode;
    field_20_state_or_op = 0;

    return this;
}

EXPORT void InvisibleEffect::dtor_45F410()
{
    SetVTable(this, 0x545A60);

    if (field_24_pAlloc)
    {
        Mem_Free_495560(field_24_pAlloc);
    }

    if (field_30_pPalAlloc)
    {
        Mem_Free_495560(field_30_pPalAlloc);
    }

    BaseGameObject_dtor_4DBEC0();
}

EXPORT void InvisibleEffect::sub_45FA00()
{
    field_4A_flags |= 4u;
    field_1C_update_delay = 1;
    field_20_state_or_op = 1;
}

EXPORT void InvisibleEffect::sub_45FA30()
{
    field_20_state_or_op = 4;
}

EXPORT void InvisibleEffect::sub_45FA50()
{
    field_1C_update_delay = 1;
    field_20_state_or_op = 5;
}

EXPORT void InvisibleEffect::sub_45F9E0()
{
    field_1C_update_delay = 1;
    field_20_state_or_op = 1;
}

EXPORT void InvisibleEffect::vUpdate_45F4A0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_44_objId));
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (!pTarget || pTarget->field_6_flags.Get(BaseGameObject::eDead))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    else
    {
        switch (field_20_state_or_op)
        {
        case 0u:
            pTarget->field_20_animation.field_B_render_mode = 1;
            return;

        case 1u:
            for (int idx2 = 8; idx2 < pTarget->field_20_animation.field_90_pal_depth; idx2++)
            {
                // Set transparent bit
                field_30_pPalAlloc[idx2] |= 0x8000u;
            }

            pTarget->field_114_flags.Set(Flags_114::e114_Bit8);

            pTarget->field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
            pTarget->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
            pTarget->field_20_animation.field_B_render_mode = 1;

            field_1C_update_delay = 1;
            field_20_state_or_op = (unsigned __int8)(field_4A_flags & 4 | 8) >> 2;
            return;

        case 3u:
            for (int i = 8; i < pTarget->field_20_animation.field_90_pal_depth; i++)
            {
                // Clear transparent bit
                field_30_pPalAlloc[i] &= 0x8000u;

            }
            Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (BYTE *)field_30_pPalAlloc, &field_34);
            field_4A_flags &= ~4u;
            field_1C_update_delay = 1;
            field_20_state_or_op = 0;
            break;

        case 2u:
        {
            if (pTarget->field_20_animation.field_90_pal_depth <= 8)
            {
                field_20_state_or_op = 0;
                return;
            }

            bool v3 = false;
            for (int idx = 8; idx < pTarget->field_20_animation.field_90_pal_depth; idx++)
            {
                // Red
                if (field_30_pPalAlloc[idx] & 0x1F)
                {
                    v3 = true;
                    field_30_pPalAlloc[idx] = field_30_pPalAlloc[idx] - 1;
                }

                // Green
                if (field_30_pPalAlloc[idx] & 0x3E0)
                {
                    v3 = true;
                    field_30_pPalAlloc[idx] = field_30_pPalAlloc[idx] - 32;
                }

                // Blue
                if (field_30_pPalAlloc[idx] & 0x7C00)
                {
                    v3 = true;
                    field_30_pPalAlloc[idx] = field_30_pPalAlloc[idx] - 1024;
                }

                // Semi trans
                if (field_30_pPalAlloc[idx] == 0x8000u)
                {
                    field_30_pPalAlloc[idx] = 0;
                }
            }

            if (!v3)
            {
                field_20_state_or_op = 0;
            }
            else
            {
                Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (BYTE *)field_30_pPalAlloc, &field_34);
                field_1C_update_delay = 1;
            }
        }
            break;


        case 4u:
        {
            if (pTarget->field_20_animation.field_90_pal_depth <= 1)
            {
                field_20_state_or_op = 5;
                return;
            }

            bool v3 = false;
            for (int idx4 = 1; idx4 < pTarget->field_20_animation.field_90_pal_depth; idx4++)
            {
                if ((field_30_pPalAlloc[idx4] ^ (field_24_pAlloc[idx4])) & 0x1F)
                {
                    v3 = 1;
                    field_30_pPalAlloc[idx4] = field_30_pPalAlloc[idx4] + 1;
                }

                if ((field_30_pPalAlloc[idx4] ^ field_24_pAlloc[idx4]) & 0x3E0)
                {
                    v3 = true;
                    field_30_pPalAlloc[idx4] = field_30_pPalAlloc[idx4] + 32;
                }

                if ((field_30_pPalAlloc[idx4] ^ field_24_pAlloc[idx4]) & 0x7C00)
                {
                    v3 = true;
                    field_30_pPalAlloc[idx4] = field_30_pPalAlloc[idx4] + 1024;
                }
            }

            if (v3)
            {
                Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (BYTE *)field_30_pPalAlloc, &field_34);
                pTarget->field_20_animation.field_B_render_mode = 1;
                field_1C_update_delay = 5;
            }
            else
            {
                field_20_state_or_op = 5;
            }
        }
            break;

        case 5u:
        {
            Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy,  pTarget->field_20_animation.field_90_pal_depth, (BYTE *)field_24_pAlloc, &field_28);

            pTarget->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans, !!(field_4A_flags & 1));
            pTarget->field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending, !!(field_4A_flags & 2));
            pTarget->field_20_animation.field_B_render_mode = field_48_old_render_mode;

            pTarget->field_114_flags.Clear(Flags_114::e114_Bit8);

            field_1C_update_delay = 1;
            auto pFlicker = alive_new<PossessionFlicker>();
            if (pFlicker)
            {
                pFlicker->ctor_4319E0(pTarget, 16, 255, 128, 128);
            }
            field_20_state_or_op = 6;
        }
            break;

        case 6u:
            field_6_flags.Set(BaseGameObject::eDead);
            return;

        default:
            return;
        }
    }

}

EXPORT BaseGameObject* InvisibleEffect::vdtor_45F3E0(signed int flags)
{
    dtor_45F410();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

BaseGameObject* InvisibleEffect::VDestructor(signed int flags)
{
    return vdtor_45F3E0(flags);
}

void InvisibleEffect::VUpdate()
{
    vUpdate_45F4A0();
}

void InvisibleEffect::VScreenChanged()
{
    // Null @ 45F9C0
}
