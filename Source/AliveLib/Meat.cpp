#include "stdafx.h"
#include "Meat.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "Shadow.hpp"
#include "PlatformBase.hpp"
#include "Sfx.hpp"
#include "ObjectIds.hpp"
#include "Events.hpp"
#include "Abe.hpp"

Meat* Meat::ctor_4694A0(FP xpos, FP ypos, __int16 count)
{
    ctor_408240(0);
    SetVTable(this, 0x546040);

    field_11A_bDead = 0;
    field_4_typeId = Types::eMeat_84;

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, 2023, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 2023);
    Animation_Init_424E10(488, 17, 9, ppRes, 1, 1);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_128 = 0;
    field_6_flags.Clear(BaseGameObject::eInteractive);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_12C = sGnFrame_5C1B84 + 600;
    field_130_pLine = 0;
    field_118_count = count;
    field_11C_state = 0;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }
    return this;
}

BaseGameObject* Meat::VDestructor(signed int flags)
{
    return vdtor_4696C0(flags);
}

void Meat::VScreenChanged()
{
    vScreenChanged_46A130();
}

void Meat::VOnTrapDoorOpen()
{
    vOnTrapDoorOpen_46A2E0();
}

void Meat::VThrow_49E460(FP velX, FP velY)
{
    vThrow_469790(velX, velY);
}

BOOL Meat::VCanThrow_49E350()
{
    return vCanThrow_469680();
}

BOOL Meat::VIsFalling_49E330()
{
    return vIsFalling_469660();
}

void Meat::VTimeToExplodeRandom_411490()
{
    // TODO
}

__int16 Meat::VGetCount_448080()
{
    return vGetCount_46A350();
}

void Meat::vScreenChanged_46A130()
{
    if (gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId)
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}

void Meat::AddToPlatform_46A170()
{
    // TODO: OG bug - why doesn't meat check for trap doors ??
    BaseAddToPlatform([](Types type) { return type == Types::eLiftPoint_78; });
}

void Meat::vOnTrapDoorOpen_46A2E0()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds_5C1B70.Find_449CF0(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_11C_state == 3 || field_11C_state == 4)
        {
            field_11C_state = 1;
        }
    }
}

BOOL Meat::vIsFalling_469660()
{
    return field_11C_state == 5;
}

BOOL Meat::vCanThrow_469680()
{
    return field_11C_state == 2;
}

void Meat::dtor_4696F0()
{
    SetVTable(this, 0x546040);
    if (!field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove_49AA00(field_118_count >= 1 ? field_118_count : 1);
        }
    }
    dtor_4080B0();
}

Meat* Meat::vdtor_4696C0(signed int flags)
{
    dtor_4696F0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void Meat::vThrow_469790(FP velX, FP velY)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_118_count == 0)
    {
        field_11C_state = 2;
    }
    else
    {
        field_11C_state = 1;
    }
}

__int16 Meat::vGetCount_46A350()
{
    if (field_11C_state == 4 && field_118_count == 0)
    {
        return 1;
    }

    return field_118_count;
}

void Meat::InTheAir_4697E0()
{
    const auto xpos = field_B8_xpos;
    const auto ypos = field_BC_ypos;

    field_120_xpos = xpos;
    field_124_ypos = ypos;

    if (field_C8_vely < FP_FromInteger(18))
    {
        field_C8_vely += FP_FromInteger(1);
    }

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(field_120_xpos, field_124_ypos, field_B8_xpos, field_BC_ypos, &field_130_pLine, &hitX, &hitY, field_D6_scale == 0 ? 0x10F : 0xF0) == 1)
    {
        switch (field_130_pLine->field_8_type)
        {
        case 0u:
        case 4u:
        case 32u:
        case 36u:
            if (field_C8_vely > FP_FromInteger(0))
            {
                field_B8_xpos = FP_FromInteger(SnapToXGrid_449930(field_CC_sprite_scale, FP_GetExponent(hitX)));
                field_BC_ypos = hitY;
                field_11C_state = 3;
                field_C8_vely = FP_FromInteger(0);
                field_C4_velx = FP_FromInteger(0);
                SFX_Play_46FBA0(36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                AddToPlatform_46A170();
            }
            break;

        case 1u:
        case 5u:
            if (field_C4_velx < FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(4));
                SFX_Play_46FBA0(36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }

            if (field_C8_vely < FP_FromInteger(0))
            {
                field_C8_vely = FP_FromInteger(0);
            }

            field_130_pLine = nullptr;
            break;

        case 2u:
        case 6u:
            if (field_C4_velx > FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY;
                field_C4_velx = (-field_C4_velx / FP_FromInteger(4));
                SFX_Play_46FBA0(36, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                if (field_C8_vely < FP_FromInteger(0))
                {
                    field_C8_vely = FP_FromInteger(0);
                }
            }
            field_130_pLine = nullptr;
            break;

        case 3u:
        case 7u:
            if (field_C8_vely < FP_FromInteger(0))
            {
                field_B8_xpos = hitX;
                field_BC_ypos = hitY + FP_FromInteger(1);
                field_C8_vely = FP_FromInteger(0);
                SFX_Play_46FBA0(36u, 0, -650);
                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            }
            break;
        }
    }
}

// =================================================================================



TintEntry stru_55C254[] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 11u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
};


MeatSack* MeatSack::ctor_46A410(Path_MeatSack* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x5460D4);

    field_4_typeId = Types::eMeatSack_85;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 4002);
    Animation_Init_424E10(15848, 93, 86, ppRes, 1, 1);
    SetTint_425600(&stru_55C254[0], gMap_5C3030.sCurrentLevelId_5C3030);

    field_DC_bApplyShadows &= ~1u;
    field_118_tlvInfo = tlvInfo;

    field_11C = 0;

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_124_velX = FP_FromRaw(pTlv->field_12_xVel << 8);
    field_128_velY = -FP_FromRaw(256 * pTlv->field_14_yVel);

    if (!pTlv->field_10_side)
    {
        field_124_velX = -field_124_velX;
    }

    if (pTlv->field_16_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_20_animation.field_C_render_layer = 8;
        field_D6_scale = 0;
    }
    else if (pTlv->field_16_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_20_animation.field_C_render_layer = 27;
        field_D6_scale = 1;
    }

    field_11E_num_items = pTlv->field_18_num_items;

    field_E0_pShadow = alive_new<Shadow>();
    if (field_E0_pShadow)
    {
        field_E0_pShadow->ctor_4AC990();
    }

    return this;
}

BaseGameObject* MeatSack::VDestructor(signed int flags)
{
    return vdtor_46A5E0(flags);
}

void MeatSack::VScreenChanged()
{
    vScreenChanged_46A9C0();
}

void MeatSack::VUpdate()
{
    vUpdate_46A6A0();
}

MeatSack* MeatSack::vdtor_46A5E0(signed int flags)
{
    dtor_46A610();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void MeatSack::dtor_46A610()
{
    SetVTable(this, 0x5460D4);
    Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    dtor_4080B0();
}

void MeatSack::vScreenChanged_46A9C0()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

void MeatSack::vUpdate_46A6A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_20_animation.field_92_current_frame == 2)
    {
        if (field_120)
        {
            if (Math_NextRandom() < 40u || field_122)
            {
                field_120 = 0;
                field_122 = 0;
                SFX_Play_46FBA0(29, 24, Math_RandomRange_496AB0(-2400, -2200));
            }
        }
    }
    else
    {
        field_120 = 1;
    }

    if (field_11C)
    {
        if (field_11C == 1 && field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_20_animation.Set_Animation_Data_409C80(15848, 0);
            field_11C = 0;
        }
    }
    else
    {
        PSX_RECT bPlayerRect = {};
        sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bPlayerRect, 1);

        PSX_RECT bRect = {};
        vGetBoundingRect_424FD0(&bRect, 1);

        if (bRect.x <= bPlayerRect.w &&
            bRect.w >= bPlayerRect.x &&
            bRect.h >= bPlayerRect.y &&
            bRect.y <= bPlayerRect.h &&
            field_CC_sprite_scale == sActiveHero_5C1B68->field_CC_sprite_scale)
        {
            if (gpThrowableArray_5D1E2C)
            {
                if (gpThrowableArray_5D1E2C->field_20_count)
                {
                    field_20_animation.Set_Animation_Data_409C80(15888, 0);
                    field_11C = 1;
                    return;
                }
            }
            else
            {
                gpThrowableArray_5D1E2C = alive_new<ThrowableArray>();
                gpThrowableArray_5D1E2C->ctor_49A630();
            }

            gpThrowableArray_5D1E2C->Add_49A7A0(field_11E_num_items);

            auto pMeat = alive_new<Meat>();
            if (pMeat)
            {
                pMeat->ctor_4694A0(field_B8_xpos, field_BC_ypos - FP_FromInteger(30), field_11E_num_items);
            }

            pMeat->VThrow_49E460(field_124_velX, field_128_velY);
            pMeat->field_CC_sprite_scale = field_CC_sprite_scale;

            SFX_Play_46FA90(25, 0);
            Abe_SFX_2_457A40(7, 0, 0x7FFF, 0);

            field_20_animation.Set_Animation_Data_409C80(15888, 0);
            field_11C = 1;
        }
    }
}
