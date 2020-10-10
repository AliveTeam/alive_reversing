#include "stdafx_ao.h"
#include "Function.hpp"
#include "Grenade.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "LiftPoint.hpp"
#include "Map.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Explosion.hpp"
#include "Gibs.hpp"
#include "Events.hpp"
#include "Collisions.hpp"

START_NS_AO

ALIVE_VAR(1, 0x5076EC, short, gInfiniteGrenades_5076EC, 0);

void Grenade::VOnTrapDoorOpen()
{
    VOnTrapDoorOpen_41F920();
}

void Grenade::VUpdate()
{
    VUpdate_41F240();
}

void Grenade::VScreenChanged()
{
    VScreenChanged_41F720();
}

Grenade* Grenade::ctor_41EBD0(FP xpos, FP ypos, __int16 numGrenades)
{
    ctor_401090();
    field_10E_bDead = 0;
    SetVTable(this, 0x4BB0A0);
    field_4_typeId = Types::eGrenade_40;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6005, 1, 0);
    Animation_Init_417FD0(
        1132,
        17,
        11,
        ppRes,
        1);

    field_6_flags.Clear(Options::eInteractive_Bit8);
    field_10_anim.field_B_render_mode = 0;

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

    field_A8_xpos = xpos;
    field_120_xpos = xpos;

    field_AC_ypos = ypos;
    field_124_ypos = ypos;

    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_10C_count = numGrenades;

    if (numGrenades > 0)
    {
        field_110_state = GrenadeStates::eState_0_FallingToBeCollected;
    }
    else
    {
        field_110_state = GrenadeStates::eState_3_CountingDown;
        field_112_explode_timer = 90;
    }

    field_118 = 0;
    field_11C = 0;
    return this;
}

BaseGameObject* Grenade::dtor_41ECD0()
{
    SetVTable(this, 0x4BB0A0);

    if (field_11C)
    {
        field_11C->field_C_refCount--;
    }

    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (field_F8_pLiftPoint)
        {
            field_F8_pLiftPoint->VRemove(this);
            field_F8_pLiftPoint->field_C_refCount--;
            field_F8_pLiftPoint = nullptr;
        }

        if (gpThrowableArray_50E26C)
        {
            __int16 count = 0;
            if (field_10C_count >= 1u)
            {
                count = field_10C_count;
            }
            else
            {
                count = 1;
            }
            gpThrowableArray_50E26C->Remove_4540D0(count);
        }
    }
    return dtor_401000();
}

BaseGameObject* Grenade::VDestructor(signed int flags)
{
    return Vdtor_41F9F0(flags);
}

Grenade* Grenade::Vdtor_41F9F0(signed int flags)
{
    dtor_41ECD0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Grenade::AddToPlatform_41F7C0()
{
    NOT_IMPLEMENTED();
}

void Grenade::VTimeToExplodeRandom_41F9B0()
{
    field_112_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::VThrow_41ED90(FP velX, FP velY)
{
    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_B4_velx = velX;
    field_B8_vely = velY;

    if (field_10C_count == 0)
    {
        field_110_state = GrenadeStates::eState_4_Falling;
    }
}

void Grenade::VScreenChanged_41F720()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Grenade::VUpdate_41F240()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    switch (field_110_state)
    {
    case GrenadeStates::eState_0_FallingToBeCollected:
        if (!InTheAir_41EF10())
        {
            field_D4_collection_rect.x = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
            field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale);
            field_D4_collection_rect.w = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
            field_D4_collection_rect.h = field_AC_ypos;

            field_6_flags.Set(Options::eInteractive_Bit8);
            field_110_state = GrenadeStates::eState_1_WaitToBeCollected;
        }
        break;

    case GrenadeStates::eState_1_WaitToBeCollected:
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_B4_velx = FP_FromInteger(0);
        }

        if (FP_Abs(field_B4_velx) >= FP_FromInteger(1))
        {
            if (field_B4_velx <= FP_FromInteger(0))
            {
                field_B4_velx += FP_FromDouble(0.01);
            }
            else
            {
                field_B4_velx -= FP_FromDouble(0.01);
            }

            const auto oldLine = field_114_pCollisionLine;
            field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine_40CA20(&field_A8_xpos, &field_AC_ypos, field_B4_velx);
            if (field_F8_pLiftPoint)
            {
                if (field_114_pCollisionLine != oldLine)
                {
                    field_F8_pLiftPoint->VRemove(this);
                    field_F8_pLiftPoint->field_C_refCount--;
                    field_F8_pLiftPoint = nullptr;
                }
            }

            if (!field_114_pCollisionLine)
            {
                field_110_state = GrenadeStates::eState_0_FallingToBeCollected;
            }
        }
        else if (abs(SnapToXGrid_41FAA0(field_BC_sprite_scale, FP_GetExponent(field_A8_xpos)) - FP_GetExponent(field_A8_xpos)) > 1)
        {
            const auto oldLine = field_114_pCollisionLine;
            field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine_40CA20(&field_A8_xpos, &field_AC_ypos, field_B4_velx);
            if (field_F8_pLiftPoint)
            {
                if (field_114_pCollisionLine != oldLine)
                {
                    field_F8_pLiftPoint->VRemove(this);
                    field_F8_pLiftPoint->field_C_refCount--;
                    field_F8_pLiftPoint = nullptr;
                }
            }

            if (!field_114_pCollisionLine)
            {
                field_110_state = GrenadeStates::eState_4_Falling;
            }
        }
        else
        {
            field_B4_velx = FP_FromInteger(0);

            field_D4_collection_rect.x = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
            field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale);
            field_D4_collection_rect.w = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
            field_D4_collection_rect.h = field_AC_ypos;

            field_6_flags.Set(Options::eInteractive_Bit8);
            field_110_state = GrenadeStates::eState_2;
        }
        break;

    case GrenadeStates::eState_2:
        break;

    case GrenadeStates::eState_3_CountingDown:
        BlowUp_41EDD0();
        break;

    case GrenadeStates::eState_4_Falling:
        if (InTheAir_41EF10())
        {
            if (!BlowUp_41EDD0())
            {
                PSX_RECT bRect = {};
                VGetBoundingRect(&bRect, 1);

                const PSX_Point xy = { bRect.x, static_cast<short>(bRect.y + 5) };
                const PSX_Point wh = { bRect.w, static_cast<short>(bRect.h + 5) };

                VOnCollisionWith(xy, wh, gBaseGameObject_list_9F2DF0, 1, (TCollisionCallBack)&Grenade::OnCollision_BounceOff_41F650);
            }
        }
        else
        {
            field_110_state = GrenadeStates::eState_5_HitGround;
        }
        break;

    case GrenadeStates::eState_5_HitGround:
    {
        field_B4_velx = FP_FromRaw(field_B4_velx.fpValue / 2);

        const auto oldLine = field_114_pCollisionLine;
        field_114_pCollisionLine = field_114_pCollisionLine->MoveOnLine_40CA20(&field_A8_xpos, &field_AC_ypos, field_B4_velx);
        if (field_F8_pLiftPoint)
        {
            if (field_114_pCollisionLine != oldLine)
            {
                field_F8_pLiftPoint->VRemove(this);
                field_F8_pLiftPoint->field_C_refCount--;
                field_F8_pLiftPoint = nullptr;
            }
        }

        if (!field_114_pCollisionLine)
        {
            field_110_state = GrenadeStates::eState_4_Falling;
        }

        BlowUp_41EDD0();
        break;
    }

    case GrenadeStates::eState_6_WaitForExplodeEnd:
        if (field_11C->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_110_state = GrenadeStates::eState_7_Exploded;
            field_11C->field_C_refCount--;
            field_11C = nullptr;
        }
        break;

    case GrenadeStates::eState_7_Exploded:
        field_6_flags.Set(Options::eDead_Bit3);
        break;

    default:
        return;
    }
}

void Grenade::VOnTrapDoorOpen_41F920()
{
    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;

        if (field_110_state == GrenadeStates::eState_1_WaitToBeCollected || field_110_state == GrenadeStates::eState_2)
        {
            field_110_state = GrenadeStates::eState_0_FallingToBeCollected;
        }
        else if (field_110_state != GrenadeStates::eState_6_WaitForExplodeEnd)
        {
            field_110_state = GrenadeStates::eState_4_Falling;
        }
    }
}

signed __int16 Grenade::InTheAir_41EF10()
{
    NOT_IMPLEMENTED();
    return 0;
}

signed __int16 Grenade::OnCollision_BounceOff_41F650(BaseGameObject* pHit)
{
    if (!pHit->field_6_flags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);

    PSX_RECT bRect = {};
    pHit2->VGetBoundingRect(&bRect, 1);

    if (field_A8_xpos < FP_FromInteger(bRect.x + 12) || field_A8_xpos > FP_FromInteger(bRect.w - 12))
    {
        field_A8_xpos = field_120_xpos;
        field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
    }
    else
    {
        field_AC_ypos = field_124_ypos;
        field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SFX_Play_43AD70(SoundEffect::RockBounceOnMine_29, 0, 0);
    return 0;
}

signed __int16 Grenade::BlowUp_41EDD0()
{
    const short timer = field_112_explode_timer--;
    if (!(timer % 16))
    {
        SFX_Play_43AD70(SoundEffect::GreenTick_3, 0, 0);
    }

    if (timer)
    {
        return 0;
    }

    auto pExplosion = ao_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_458B80(
            field_A8_xpos,
            field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(5)),
            field_BC_sprite_scale);
    }

    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_11C = pExplosion;
    pExplosion->field_C_refCount++;
    field_110_state = GrenadeStates::eState_6_WaitForExplodeEnd;

    auto pGibs = ao_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_407B20(5, field_A8_xpos, field_AC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_BC_sprite_scale);
    }
    return 1;
}

__int16 Grenade::VCanThrow_453EC0()
{
    return FALSE;
}

__int16 Grenade::VIsFalling_453ED0()
{
    return FALSE;
}

END_NS_AO
