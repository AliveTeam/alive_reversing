#include "stdafx_ao.h"
#include "Function.hpp"
#include "Rock.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Shadow.hpp"
#include "Grenade.hpp"
#include "ThrowableArray.hpp"
#include "Math.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Collisions.hpp"
#include "Game.hpp"
#include "Particle.hpp"

namespace AO {

Rock* Rock::ctor_456960(FP xpos, FP ypos, s16 count)
{
    ctor_401090();
    SetVTable(this, 0x4BC0A8);
    field_4_typeId = Types::eRock_70;

    field_10E_bDead = 0;

    const AnimRecord rec = AO::AnimRec(AnimId::Rock);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_6_flags.Clear(Options::eInteractive_Bit8);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_A8_xpos = xpos;
    field_11C_xpos = xpos;
    field_AC_ypos = ypos;
    field_120_ypos = ypos;
    field_B4_velx = FP_FromInteger(0);
    field_B8_vely = FP_FromInteger(0);
    field_10C_count = count;
    field_110_state = States::eNone_0;

    u8** ppPal = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kAberockResID, 0, 0);
    if (ppPal)
    {
        field_10_anim.LoadPal_403090(ppPal, 0);
    }
    else
    {
        const FrameInfoHeader* pFrameInfo = field_10_anim.Get_FrameHeader_403A00(-1);

        const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_10_anim.field_20_ppBlock)[pFrameInfo->field_0_frame_header_offset]);

        field_10_anim.LoadPal_403090(
            field_10_anim.field_20_ppBlock,
            pFrameHeader->field_0_clut_offset);
    }

    field_118_vol = 0;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }
    return this;
}

BaseGameObject* Rock::dtor_456A90()
{
    SetVTable(this, 0x4BC0A8);
    if (!gInfiniteGrenades_5076EC && !field_10E_bDead)
    {
        if (gpThrowableArray_50E26C)
        {
            gpThrowableArray_50E26C->Remove_4540D0(field_10C_count >= 1u ? field_10C_count : 1);
        }
    }
    return dtor_401000();
}

BaseGameObject* Rock::VDestructor(s32 flags)
{
    return Vdtor_4573D0(flags);
}

Rock* Rock::Vdtor_4573D0(s32 flags)
{
    dtor_456A90();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Rock::VUpdate()
{
    VUpdate_456EC0();
}

void Rock::VUpdate_456EC0()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    switch (field_110_state)
    {
        case States::eFallingOutOfRockSack_1:
            InTheAir_456B60();
            break;

        case States::eRolling_2:
            if (FP_Abs(field_B4_velx) >= FP_FromInteger(1))
            {
                if (field_B4_velx < FP_FromInteger(0))
                {
                    field_B4_velx += FP_FromDouble(0.01);
                }
                else
                {
                    field_B4_velx -= FP_FromDouble(0.01);
                }

                field_114_pLine->MoveOnLine_40CA20(
                    &field_A8_xpos,
                    &field_AC_ypos,
                    field_B4_velx);

                if (!field_114_pLine)
                {
                    field_110_state = States::eBouncing_4;
                    field_10_anim.field_4_flags.Set(AnimFlags::eBit8_Loop);
                }
            }
            else
            {
                const s16 x_exp = FP_GetExponent(field_A8_xpos);
                const s32 xSnapped = (x_exp & 0xFC00) + SnapToXGrid_41FAA0(field_BC_sprite_scale, x_exp & 0x3FF);
                if (abs(xSnapped - x_exp) > 1)
                {
                    field_114_pLine = field_114_pLine->MoveOnLine_40CA20(
                        &field_A8_xpos,
                        &field_AC_ypos,
                        field_B4_velx);
                    if (!field_114_pLine)
                    {
                        field_110_state = States::eBouncing_4;
                        field_10_anim.field_4_flags.Set(AnimFlags::eBit8_Loop);
                    }
                }
                else
                {
                    field_B4_velx = FP_FromInteger(0);
                    field_D4_collection_rect.x = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
                    field_D4_collection_rect.w = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));

                    field_6_flags.Set(Options::eInteractive_Bit8);

                    field_10_anim.field_4_flags.Clear(AnimFlags::eBit8_Loop);
                    field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale);
                    field_D4_collection_rect.h = field_AC_ypos;
                    field_110_state = States::eOnGround_3;
                    field_124_shimmer_timer = gnFrameCount_507670;
                }
            }
            break;

        case States::eOnGround_3:
            if (static_cast<s32>(gnFrameCount_507670) > field_124_shimmer_timer)
            {
                New_Shiny_Particle_4199A0(
                    (field_BC_sprite_scale * FP_FromInteger(1)) + field_A8_xpos,
                    (field_BC_sprite_scale * FP_FromInteger(-7)) + field_AC_ypos,
                    FP_FromDouble(0.3),
                    Layer::eLayer_Foreground_36);
                field_124_shimmer_timer = (Math_NextRandom() % 16) + gnFrameCount_507670 + 60;
            }
            break;

        case States::eBouncing_4:
        {
            InTheAir_456B60();
            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);
            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(
                xy,
                wh,
                gBaseGameObject_list_9F2DF0,
                1,
                (TCollisionCallBack) &Rock::OnCollision_457240);

            if (field_B8_vely > FP_FromInteger(30))
            {
                field_110_state = States::eFallingOutOfWorld_5;
            }
        }
        break;

        case States::eFallingOutOfWorld_5:
            field_B8_vely += FP_FromInteger(1);
            field_A8_xpos += field_B4_velx;
            field_AC_ypos += field_B8_vely;
            if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                field_6_flags.Set(Options::eDead_Bit3);
            }
            break;
        default:
            return;
    }
}

void Rock::VScreenChanged()
{
    VScreenChanged_457310();
}


void Rock::VScreenChanged_457310()
{
    if (gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path
        || gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Rock::VThrow(FP velX, FP velY)
{
    VThrow_456B20(velX, velY);
}

void Rock::VThrow_456B20(FP velX, FP velY)
{
    field_B4_velx = velX;
    field_B8_vely = velY;

    field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);

    if (field_10C_count == 0)
    {
        field_110_state = States::eBouncing_4;
    }
    else
    {
        field_110_state = States::eFallingOutOfRockSack_1;
    }
}

s16 Rock::VCanThrow()
{
    return VCanThrow_4573C0();
}

s16 Rock::VCanThrow_4573C0()
{
    return field_110_state == States::eBouncing_4;
}

void Rock::InTheAir_456B60()
{
    field_11C_xpos = field_A8_xpos;
    field_120_ypos = field_AC_ypos;

    if (field_B8_vely > FP_FromInteger(30))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    field_B8_vely += FP_FromInteger(1);

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    u16 result = 0;
    field_A8_xpos = CamX_VoidSkipper_418590(field_A8_xpos, field_B4_velx, 8, &result);
    field_AC_ypos = CamY_VoidSkipper_418690(field_AC_ypos, field_B8_vely, 8, &result);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
            field_11C_xpos,
            field_120_ypos,
            field_A8_xpos,
            field_AC_ypos,
            &field_114_pLine,
            &hitX,
            &hitY,
            field_BC_sprite_scale != FP_FromInteger(1) ? 0x70 : 0x07))
    {
        switch (field_114_pLine->field_8_type)
        {
            case eLineTypes::eFloor_0:
            case eLineTypes::eBackGroundFloor_4:
            case eLineTypes::eUnknown_32:
            case eLineTypes::eUnknown_36:
                if (field_B8_vely > FP_FromInteger(0))
                {
                    if (field_110_state != States::eBouncing_4 || field_B8_vely >= FP_FromInteger(5))
                    {
                        if (field_110_state != States::eFallingOutOfRockSack_1 || field_B8_vely >= FP_FromInteger(1))
                        {
                            field_AC_ypos = hitY;
                            field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
                            field_B4_velx = (field_B4_velx / FP_FromInteger(2));
                            s32 vol = 20 * (4 - field_118_vol);
                            if (vol < 40)
                            {
                                vol = 40;
                            }
                            SFX_Play_43AD70(SoundEffect::RockBounce_31, vol, 0);
                            Event_Broadcast_417220(kEventNoise_0, this);
                            Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
                            field_118_vol++;
                        }
                        else
                        {
                            field_110_state = States::eRolling_2;
                            if (field_B4_velx >= FP_FromInteger(0) && field_B4_velx < FP_FromInteger(1))
                            {
                                field_B4_velx = FP_FromInteger(1);
                            }

                            if (field_B4_velx < FP_FromInteger(0) && field_B4_velx > FP_FromInteger(-1))
                            {
                                field_B4_velx = FP_FromInteger(-1);
                            }
                        }
                    }
                    else
                    {
                        field_110_state = States::eFallingOutOfWorld_5;
                    }
                }
                break;

            case eLineTypes::eWallLeft_1:
            case eLineTypes::eBackGroundWallLeft_5:
                if (field_B4_velx < FP_FromInteger(0))
                {
                    field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                    field_A8_xpos = hitX;
                    field_AC_ypos = hitY;
                    s32 vol = 20 * (4 - field_118_vol);
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_43AD70(SoundEffect::RockBounce_31, vol, 0);
                    Event_Broadcast_417220(kEventNoise_0, this);
                    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
                }
                break;
            case eLineTypes::eWallRight_2:
            case eLineTypes::eBackGroundWallRight_6:
                if (field_B4_velx > FP_FromInteger(0))
                {
                    field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
                    field_A8_xpos = hitX;
                    field_AC_ypos = hitY;
                    s32 vol = 20 * (4 - field_118_vol);
                    if (vol < 40)
                    {
                        vol = 40;
                    }

                    SFX_Play_43AD70(SoundEffect::RockBounce_31, vol, 0);
                    Event_Broadcast_417220(kEventNoise_0, this);
                    Event_Broadcast_417220(kEventSuspiciousNoise_10, this);
                }
                break;

            default:
                return;
        }
    }
}

s16 Rock::OnCollision_457240(BaseAnimatedWithPhysicsGameObject* pObj)
{
    if (!pObj->field_6_flags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pObj->VGetBoundingRect(&bRect, 1);

    if (field_11C_xpos < FP_FromInteger(bRect.x) || field_11C_xpos > FP_FromInteger(bRect.w))
    {
        field_A8_xpos -= field_B4_velx;
        field_B4_velx = (-field_B4_velx / FP_FromInteger(2));
    }
    else
    {
        field_AC_ypos -= field_B8_vely;
        field_B8_vely = (-field_B8_vely / FP_FromInteger(2));
    }

    pObj->VOnThrowableHit(this);

    SFX_Play_43AD70(SoundEffect::RockBounceOnMine_29, 80, 0);
    return 0;
}

s16 Rock::VIsFalling()
{
    // Same as meat falling func - compiler seems to have made them both
    // use the same func, or should it go in the base ??
    return field_110_state == States::eFallingOutOfWorld_5;
}

void Rock::VTimeToExplodeRandom()
{
    // Empty ?
}

} // namespace AO
