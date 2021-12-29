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
#include "Sys_common.hpp"
#include "Renderer/IRenderer.hpp"

EXPORT InvisibleEffect* InvisibleEffect::ctor_45F280(BaseAliveGameObject* pTarget)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x545A60);

    SetType(AETypes::eInvisibleEffect_75);

    field_44_objId = pTarget->field_8_object_id;

    field_24_pPal1 = reinterpret_cast<u16*>(ae_malloc_non_zero_4954F0(pTarget->field_20_animation.field_90_pal_depth * sizeof(u16)));
    Pal_Copy_483560(
        pTarget->field_20_animation.field_8C_pal_vram_xy,
        pTarget->field_20_animation.field_90_pal_depth,
        field_24_pPal1,
        &field_28_pal_rect1);

    field_30_pPal2 = reinterpret_cast<u16*>(ae_malloc_non_zero_4954F0(pTarget->field_20_animation.field_90_pal_depth * sizeof(u16)));
    Pal_Copy_483560(
        pTarget->field_20_animation.field_8C_pal_vram_xy,
        pTarget->field_20_animation.field_90_pal_depth,
        field_30_pPal2,
        &field_34_pal_rect2);

    field_4A_flags.Clear();

    if (pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit15_bSemiTrans))
    {
        field_4A_flags.Set(Flags_4A::eSemiTrans_Bit1);
    }
    if (pTarget->field_20_animation.field_4_flags.Get(AnimFlags::eBit16_bBlending))
    {
        field_4A_flags.Set(Flags_4A::eBlending_Bit2);
    }

    field_4A_flags.Clear(Flags_4A::eIsInvisible_Bit3);
    field_48_old_render_mode = pTarget->field_20_animation.field_B_render_mode;
    field_20_state_or_op = InvisibleState::eSetRenderMode1_0;

    return this;
}

EXPORT void InvisibleEffect::dtor_45F410()
{
    SetVTable(this, 0x545A60);

    if (field_24_pPal1)
    {
        ae_non_zero_free_495560(field_24_pPal1);
    }

    if (field_30_pPal2)
    {
        ae_non_zero_free_495560(field_30_pPal2);
    }

    BaseGameObject_dtor_4DBEC0();
}

EXPORT void InvisibleEffect::InstantInvisibility_45FA00()
{
    field_4A_flags.Set(Flags_4A::eIsInvisible_Bit3);
    SetUpdateDelay(1);
    field_20_state_or_op = InvisibleState::eSetInvisibile_1;
}

EXPORT void InvisibleEffect::BecomeVisible_45FA30()
{
    field_20_state_or_op = InvisibleState::eBecomeVisible_4;
}

EXPORT void InvisibleEffect::ClearInvisibility_45FA50()
{
    SetUpdateDelay(1);
    field_20_state_or_op = InvisibleState::eClearInvisibility_5;
}

EXPORT void InvisibleEffect::BecomeInvisible_45F9E0()
{
    SetUpdateDelay(1);
    field_20_state_or_op = InvisibleState::eSetInvisibile_1;
}

EXPORT void InvisibleEffect::vUpdate_45F4A0()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(field_44_objId));
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!pTarget || pTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        switch (field_20_state_or_op)
        {
            case InvisibleState::eSetRenderMode1_0:
            {
                pTarget->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
                return;
            }
            case InvisibleState::eSetInvisibile_1:
            {
                for (s32 idx2 = 8; idx2 < pTarget->field_20_animation.field_90_pal_depth; idx2++)
                {
                    // Set transparent bit
                    field_30_pPal2[idx2] |= 0x8000u;
                }

                pTarget->field_114_flags.Set(Flags_114::e114_Bit8_bInvisible);

                pTarget->field_20_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);
                pTarget->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
                pTarget->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;

                SetUpdateDelay(1);
                field_20_state_or_op = InvisibleState::eBecomeInvisible_2;
                if (field_4A_flags.Get(Flags_4A::eIsInvisible_Bit3))
                {
                    field_20_state_or_op = InvisibleState::eUnknown_3;
                }
                return;
            }
            case InvisibleState::eBecomeInvisible_2:
            {
                if (pTarget->field_20_animation.field_90_pal_depth <= 8)
                {
                    field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
                    return;
                }

                bool v3 = false;
                for (s32 idx = 8; idx < pTarget->field_20_animation.field_90_pal_depth; idx++)
                {
                    // Red
                    if (field_30_pPal2[idx] & 0x1F)
                    {
                        v3 = true;
                        field_30_pPal2[idx] = field_30_pPal2[idx] - 1;
                    }

                    // Green
                    if (field_30_pPal2[idx] & 0x3E0)
                    {
                        v3 = true;
                        field_30_pPal2[idx] = field_30_pPal2[idx] - 32;
                    }

                    // Blue
                    if (field_30_pPal2[idx] & 0x7C00)
                    {
                        v3 = true;
                        field_30_pPal2[idx] = field_30_pPal2[idx] - 1024;
                    }

                    // Semi trans
                    if (field_30_pPal2[idx] == 0x8000u)
                    {
                        field_30_pPal2[idx] = 0;
                    }
                }

                if (!v3)
                {
                    field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
                }
                else
                {
                    Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (u8*) field_30_pPal2, &field_34_pal_rect2);
                    SetUpdateDelay(1);
                }


                break;
            }
            case InvisibleState::eUnknown_3:
            {
                LOG_ERROR("Unreachable case InvisibleState::eUnknown_3 hit");
                for (s32 i = 8; i < pTarget->field_20_animation.field_90_pal_depth; i++)
                {
                    // Clear transparent bit
                    field_30_pPal2[i] &= 0x8000u;
                }
                Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (u8*) field_30_pPal2, &field_34_pal_rect2);
                field_4A_flags.Clear(Flags_4A::eIsInvisible_Bit3);
                SetUpdateDelay(1);
                field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
                break;
            }
            case InvisibleState::eBecomeVisible_4:
            {
                if (pTarget->field_20_animation.field_90_pal_depth <= 1)
                {
                    field_20_state_or_op = InvisibleState::eClearInvisibility_5;
                    return;
                }

                bool v3 = false;
                for (s32 idx4 = 1; idx4 < pTarget->field_20_animation.field_90_pal_depth; idx4++)
                {
                    if ((field_30_pPal2[idx4] ^ (field_24_pPal1[idx4])) & 0x1F)
                    {
                        v3 = true;
                        field_30_pPal2[idx4] = field_30_pPal2[idx4] + 1;
                    }

                    if ((field_30_pPal2[idx4] ^ field_24_pPal1[idx4]) & 0x3E0)
                    {
                        v3 = true;
                        field_30_pPal2[idx4] = field_30_pPal2[idx4] + 32;
                    }

                    if ((field_30_pPal2[idx4] ^ field_24_pPal1[idx4]) & 0x7C00)
                    {
                        v3 = true;
                        field_30_pPal2[idx4] = field_30_pPal2[idx4] + 1024;
                    }
                }

                if (v3)
                {
                    Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (u8*) field_30_pPal2, &field_34_pal_rect2);
                    pTarget->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
                    SetUpdateDelay(5);
                }
                else
                {
                    field_20_state_or_op = InvisibleState::eClearInvisibility_5;
                }
                break;
            }
            case InvisibleState::eClearInvisibility_5:
            {
                Pal_Set_483510(pTarget->field_20_animation.field_8C_pal_vram_xy, pTarget->field_20_animation.field_90_pal_depth, (u8*) field_24_pPal1, &field_28_pal_rect1);

                pTarget->field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans, field_4A_flags.Get(Flags_4A::eSemiTrans_Bit1));
                pTarget->field_20_animation.field_4_flags.Set(AnimFlags::eBit16_bBlending, field_4A_flags.Get(Flags_4A::eBlending_Bit2));
                pTarget->field_20_animation.field_B_render_mode = field_48_old_render_mode;

                pTarget->field_114_flags.Clear(Flags_114::e114_Bit8_bInvisible);

                SetUpdateDelay(1);
                auto pFlicker = ae_new<PossessionFlicker>();
                if (pFlicker)
                {
                    pFlicker->ctor_4319E0(pTarget, 16, 255, 128, 128);
                }
                field_20_state_or_op = InvisibleState::eSetDead_6;
                break;
            }
            case InvisibleState::eSetDead_6:
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
                return;
            }
            default:
            {
                return;
            }
        }
    }
}

EXPORT BaseGameObject* InvisibleEffect::vdtor_45F3E0(s32 flags)
{
    dtor_45F410();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

BaseGameObject* InvisibleEffect::VDestructor(s32 flags)
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
