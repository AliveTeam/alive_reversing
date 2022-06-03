#include "stdafx.h"
#include "Grenade.hpp"
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
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "Grid.hpp"

Grenade::Grenade(FP xpos, FP ypos, s32 numGrenades, bool bBlowUpOnCollision, s32 unused, BaseGameObject* pOwner)
    : BaseThrowable(0)
{
    field_11A_bDead = 0;

    Init(xpos, ypos);

    field_118_count = static_cast<s16>(numGrenades);
    field_132_bBlowUpOnCollision = bBlowUpOnCollision;

    if (bBlowUpOnCollision)
    {
        field_120_state = GrenadeStates::eDoesNothing_8;
        field_11A_bDead = 1;
    }
    else if (numGrenades)
    {
        field_120_state = GrenadeStates::eFallingToBeCollected_0;
    }
    else
    {
        field_120_state = GrenadeStates::eCountingDown_3;
        field_122_explode_timer = 90;
    }

    field_138_pOwner = pOwner;
    field_130_unused = static_cast<s16>(unused);
}

s32 Grenade::CreateFromSaveState(const u8* pBuffer)
{
    auto pState = reinterpret_cast<const Grenade_SaveState*>(pBuffer);
    auto pGrenade = ae_new<Grenade>(pState->field_8_xpos, pState->field_C_ypos, pState->field_2A_savedcount, 0, 0, nullptr);

    pGrenade->field_C_objectId = pState->field_4_obj_id;

    pGrenade->field_B8_xpos = pState->field_8_xpos;
    pGrenade->field_BC_ypos = pState->field_C_ypos;

    pGrenade->field_E4_collection_rect.x = pGrenade->field_B8_xpos - (ScaleToGridSize(pGrenade->field_CC_sprite_scale) / FP_FromInteger(2));
    pGrenade->field_E4_collection_rect.w = pGrenade->field_B8_xpos + (ScaleToGridSize(pGrenade->field_CC_sprite_scale) / FP_FromInteger(2));
    pGrenade->field_E4_collection_rect.h = pGrenade->field_BC_ypos;
    pGrenade->field_E4_collection_rect.y = pGrenade->field_BC_ypos - ScaleToGridSize(pGrenade->field_CC_sprite_scale);

    pGrenade->field_C4_velx = pState->field_10_velx;
    pGrenade->field_C8_vely = pState->field_14_vely;
    pGrenade->field_C0_path_number = pState->field_1C_path_number;
    pGrenade->field_C2_lvl_number = pState->field_1E_lvl_number;
    pGrenade->field_CC_sprite_scale = pState->field_18_sprite_scale;

    pGrenade->field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop, pState->field_20_flags.Get(Grenade_SaveState::eBit3_bLoop));
    pGrenade->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render, pState->field_20_flags.Get(Grenade_SaveState::eBit1_bRender));

    pGrenade->mFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_20_flags.Get(Grenade_SaveState::eBit2_bDrawable));
    pGrenade->mFlags.Set(BaseGameObject::eInteractive_Bit8, pState->field_20_flags.Get(Grenade_SaveState::eBit4_bInteractive));

    pGrenade->field_114_flags.Set(Flags_114::e114_Bit9_RestoredFromQuickSave);
    pGrenade->field_104_collision_line_type = pState->field_28_line_type;
    pGrenade->field_118_count = pState->field_2A_savedcount;
    pGrenade->field_120_state = pState->field_2C_state;
    pGrenade->field_124 = pState->field_2E;
    pGrenade->field_128_xpos = pState->field_34_xpos;
    pGrenade->field_12C_ypos = pState->field_38_ypos;

    pGrenade->field_130_unused = pState->field_20_flags.Get(Grenade_SaveState::eBit5_unused);
    pGrenade->field_134_bExplodeNow = pState->field_20_flags.Get(Grenade_SaveState::eBit6_bExplodeNow);
    pGrenade->field_132_bBlowUpOnCollision = pState->field_20_flags.Get(Grenade_SaveState::eBit7_bBlowUpOnCollision);

    pGrenade->field_122_explode_timer = pState->field_30_explode_timer;

    return sizeof(Grenade_SaveState);
}

s32 Grenade::VGetSaveState(u8* pSaveBuffer)
{
    auto pState = reinterpret_cast<Grenade_SaveState*>(pSaveBuffer);

    pState->field_0_type = AETypes::eGrenade_65;

    pState->field_4_obj_id = field_C_objectId;

    pState->field_8_xpos = field_B8_xpos;
    pState->field_C_ypos = field_BC_ypos;
    pState->field_10_velx = field_C4_velx;
    pState->field_14_vely = field_C8_vely;

    pState->field_1C_path_number = field_C0_path_number;
    pState->field_1E_lvl_number = field_C2_lvl_number;
    pState->field_18_sprite_scale = field_CC_sprite_scale;

    pState->field_20_flags.Set(Grenade_SaveState::eBit3_bLoop, field_20_animation.field_4_flags.Get(AnimFlags::eBit8_Loop));
    pState->field_20_flags.Set(Grenade_SaveState::eBit2_bDrawable, mFlags.Get(BaseGameObject::eDrawable_Bit4));
    pState->field_20_flags.Set(Grenade_SaveState::eBit1_bRender, field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render));
    pState->field_20_flags.Set(Grenade_SaveState::eBit4_bInteractive, mFlags.Get(BaseGameObject::eInteractive_Bit8));

    if (field_100_pCollisionLine)
    {
        pState->field_28_line_type = field_100_pCollisionLine->field_8_type;
    }
    else
    {
        pState->field_28_line_type = -1;
    }

    pState->field_24_base_id = field_110_id;
    pState->field_2A_savedcount = field_118_count;
    pState->field_2C_state = field_120_state;
    pState->field_2E = field_124;
    pState->field_34_xpos = field_128_xpos;
    pState->field_38_ypos = field_12C_ypos;

    pState->field_20_flags.Set(Grenade_SaveState::eBit5_unused, field_130_unused & 1);
    pState->field_20_flags.Set(Grenade_SaveState::eBit6_bExplodeNow, field_134_bExplodeNow & 1);
    pState->field_20_flags.Set(Grenade_SaveState::eBit7_bBlowUpOnCollision, field_132_bBlowUpOnCollision & 1);

    pState->field_30_explode_timer = field_122_explode_timer;

    return sizeof(Grenade_SaveState);
}

void Grenade::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mFlags.Set(BaseGameObject::eDead);
    }
    field_11C_explosion_id = -1;
}

void Grenade::Init(FP xpos, FP ypos)
{
    SetType(AETypes::eGrenade_65);

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kGrenadeResID, 0, 0))
    {
        LoadRockTypes_49AB30(field_C2_lvl_number, field_C0_path_number);
    }

    const AnimRecord& rec = AnimRec(AnimId::Grenade);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);
    mFlags.Clear(BaseGameObject::eInteractive_Bit8);

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

    field_20_animation.field_B_render_mode = TPageAbr::eBlend_0;

    field_11C_explosion_id = -1;
    field_B8_xpos = xpos;
    field_BC_ypos = ypos;
    field_128_xpos = xpos;
    field_12C_ypos = ypos;
    field_C4_velx = FP_FromInteger(0);
    field_C8_vely = FP_FromInteger(0);
    field_118_count = 0;
    field_124 = 0;
    field_134_bExplodeNow = FALSE;
}

void Grenade::VOnTrapDoorOpen()
{
    auto pPlatform = static_cast<PlatformBase*>(sObjectIds.Find_Impl(field_110_id));
    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
        if (field_120_state == GrenadeStates::eWaitToBeCollected_1 || field_120_state == GrenadeStates::eDoesNothing_2)
        {
            field_120_state = GrenadeStates::eFallingToBeCollected_0;
        }
        else if (field_120_state != GrenadeStates::eWaitForExplodeEnd_6)
        {
            field_120_state = GrenadeStates::eFalling_4;
        }
    }
}

void Grenade::VThrow(FP velX, FP velY)
{
    field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);

    field_C4_velx = velX;
    field_C8_vely = velY;

    if (field_132_bBlowUpOnCollision == 0)
    {
        if (field_118_count == 0)
        {
            field_120_state = GrenadeStates::eFalling_4;
        }
    }
    else
    {
        field_120_state = GrenadeStates::eFallingBlowUpOnGround_9;
    }
}

Bool32 Grenade::VCanThrow()
{
    return FALSE;
}

Bool32 Grenade::VIsFalling()
{
    return FALSE;
}

void Grenade::VTimeToExplodeRandom()
{
    field_122_explode_timer -= Math_NextRandom() % 16;
}

void Grenade::BlowUp(s16 bSmallExplosion)
{
    auto pExplosion = ae_new<Explosion>(
        field_B8_xpos,
        field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
        field_CC_sprite_scale,
        bSmallExplosion);
    if (pExplosion)
    {
        field_11C_explosion_id = pExplosion->field_8_object_id;
    }

    field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);

    field_120_state = GrenadeStates::eWaitForExplodeEnd_6;

    ae_new<Gibs>(GibType::Metal_5, field_B8_xpos, field_BC_ypos, FP_FromInteger(0), FP_FromInteger(5), field_CC_sprite_scale, bSmallExplosion);
}

Grenade::~Grenade()
{
    if (!gInfiniteGrenades_5C1BDE && !field_11A_bDead)
    {
        if (gpThrowableArray_5D1E2C)
        {
            gpThrowableArray_5D1E2C->Remove(field_118_count >= 1 ? field_118_count : 1);
        }
    }
}

s16 Grenade::TimeToBlowUp()
{
    field_122_explode_timer--;
    const s16 timer = field_122_explode_timer;
    if (!(timer % 16))
    {
        SFX_Play_Mono(SoundEffect::GreenTick_2, 0);
    }

    if (timer)
    {
        return 0;
    }

    BlowUp(0);
    return 1;
}

void Grenade::VUpdate()
{
    auto pExplosion = sObjectIds.Find_Impl(field_11C_explosion_id);


    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (field_114_flags.Get(Flags_114::e114_Bit9_RestoredFromQuickSave))
    {
        field_114_flags.Clear(Flags_114::e114_Bit9_RestoredFromQuickSave);
        if (field_104_collision_line_type == -1)
        {
            field_100_pCollisionLine = nullptr;
        }
        else
        {
            sCollisions_DArray_5C1128->Raycast_417A60(
                field_B8_xpos,
                field_BC_ypos - FP_FromInteger(10),
                field_B8_xpos,
                field_BC_ypos + FP_FromInteger(10),
                &field_100_pCollisionLine,
                &field_B8_xpos,
                &field_BC_ypos,
                1 << field_104_collision_line_type);
        }
        field_104_collision_line_type = 0;
    }

    switch (field_120_state)
    {
        case GrenadeStates::eFallingToBeCollected_0:
            if (!InTheAir(FALSE))
            {
                field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize(field_CC_sprite_scale);
                field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.h = field_BC_ypos;

                mFlags.Set(BaseGameObject::eInteractive_Bit8);
                field_120_state = GrenadeStates::eWaitToBeCollected_1;
            }
            break;

        case GrenadeStates::eWaitToBeCollected_1:
            if (FP_Abs(field_C4_velx) >= FP_FromInteger(1))
            {
                if (field_C4_velx <= FP_FromInteger(0))
                {
                    field_C4_velx += FP_FromDouble(0.01);
                }
                else
                {
                    field_C4_velx -= FP_FromDouble(0.01);
                }

                auto oldLine = field_100_pCollisionLine;
                field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
                if (field_110_id != -1 && field_100_pCollisionLine != oldLine)
                {
                    VOnTrapDoorOpen();
                }

                if (!field_100_pCollisionLine)
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                    field_120_state = GrenadeStates::eFallingToBeCollected_0;
                }
            }
            else if (abs(SnapToXGrid(field_CC_sprite_scale, FP_GetExponent(field_B8_xpos)) - FP_GetExponent(field_B8_xpos)) > 1)
            {
                auto oldLine = field_100_pCollisionLine;
                field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
                if (field_110_id != -1 && field_100_pCollisionLine != oldLine)
                {
                    VOnTrapDoorOpen();
                }

                if (!field_100_pCollisionLine)
                {
                    field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                    field_120_state = GrenadeStates::eFalling_4;
                }
            }
            else
            {
                field_C4_velx = FP_FromInteger(0);

                field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize(field_CC_sprite_scale);
                field_E4_collection_rect.w = field_B8_xpos + (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
                field_E4_collection_rect.h = field_BC_ypos;

                mFlags.Set(BaseGameObject::eInteractive_Bit8);
                field_120_state = GrenadeStates::eDoesNothing_2;
            }
            break;

        case GrenadeStates::eDoesNothing_2:
            break;

        case GrenadeStates::eCountingDown_3:
            TimeToBlowUp();
            break;

        case GrenadeStates::eFalling_4:
            if (InTheAir(FALSE))
            {
                if (!TimeToBlowUp())
                {
                    PSX_RECT bRect = {};
                    VGetBoundingRect(&bRect, 1);

                    const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
                    const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
                    VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Grenade::OnCollision_BounceOff);
                }
            }
            else
            {
                field_120_state = GrenadeStates::eHitGround_5;
            }
            break;

        case GrenadeStates::eHitGround_5:
            field_C4_velx = FP_FromRaw(field_C4_velx.fpValue / 2);

            field_100_pCollisionLine = field_100_pCollisionLine->MoveOnLine_418260(&field_B8_xpos, &field_BC_ypos, field_C4_velx);
            if (!field_100_pCollisionLine)
            {
                field_20_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
                field_120_state = GrenadeStates::eFalling_4;
            }

            TimeToBlowUp();
            break;

        case GrenadeStates::eWaitForExplodeEnd_6:
            if (!pExplosion || pExplosion->mFlags.Get(BaseGameObject::eDead))
            {
                field_120_state = GrenadeStates::eExploded_7;
                field_11C_explosion_id = -1;
            }
            break;

        case GrenadeStates::eExploded_7:
            mFlags.Set(BaseGameObject::eDead);
            break;

        case GrenadeStates::eDoesNothing_8:
            break;

        case GrenadeStates::eFallingBlowUpOnGround_9:
        {
            InTheAir(TRUE);

            PSX_RECT bRect = {};
            VGetBoundingRect(&bRect, 1);

            const PSX_Point xy = {bRect.x, static_cast<s16>(bRect.y + 5)};
            const PSX_Point wh = {bRect.w, static_cast<s16>(bRect.h + 5)};
            VOnCollisionWith(xy, wh, gBaseGameObjects, 1, (TCollisionCallBack) &Grenade::OnCollision_InstantExplode);

            if (field_134_bExplodeNow)
            {
                BlowUp(TRUE);
            }
        }
        break;
    }
}

s16 Grenade::InTheAir(s16 blowUpOnFloorTouch)
{
    sObjectIds.Find_Impl(field_110_id);

    field_128_xpos = field_B8_xpos;
    field_12C_ypos = field_BC_ypos;

    field_C8_vely += FP_FromInteger(1);
    field_B8_xpos += field_C4_velx;
    field_BC_ypos = field_C8_vely + field_BC_ypos;

    field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
        nullptr,
        field_B8_xpos,
        field_BC_ypos,
        field_B8_xpos,
        field_BC_ypos);

    while (field_FC_pPathTLV)
    {
        if (field_FC_pPathTLV->field_4_type == TlvTypes::DeathDrop_4)
        {
            mFlags.Set(BaseGameObject::eDead);
            return 1;
        }

        field_FC_pPathTLV = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            field_FC_pPathTLV,
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos);
    }

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_128_xpos,
            field_12C_ypos,
            field_B8_xpos,
            field_BC_ypos,
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale == 0 ? 0x10 : 0x01)
        == 1)
    {
        if (field_C8_vely <= FP_FromInteger(0))
        {
            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
            s16 vol = 75 - 20 * field_124;
            if (vol < 40)
            {
                vol = 40;
            }
            SFX_Play_Mono(SoundEffect::GrenadeBounce_68, vol);
            Event_Broadcast_422BC0(kEventNoise, this);
            Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
            Event_Broadcast_422BC0(kEventSpeaking, this);
        }
        else
        {
            if (field_C8_vely < FP_FromInteger(1))
            {
                if (field_110_id == -1)
                {
                    AddToPlatform();
                }
                return 0;
            }

            field_B8_xpos = hitX;
            field_BC_ypos = hitY;
            field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
            field_C4_velx = (field_C4_velx / FP_FromInteger(2));

            if (blowUpOnFloorTouch)
            {
                field_134_bExplodeNow = TRUE;
                return 1;
            }

            if (field_124 <= 4)
            {
                s16 vol = 75 - 20 * field_124;
                if (vol < 40)
                {
                    vol = 40;
                }

                SFX_Play_Mono(SoundEffect::GrenadeBounce_68, vol);
                ++field_124;

                Event_Broadcast_422BC0(kEventNoise, this);
                Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                Event_Broadcast_422BC0(kEventSpeaking, this);
            }
        }
    }

    if (sCollisions_DArray_5C1128->Raycast_417A60(
            field_128_xpos,
            field_12C_ypos,
            field_B8_xpos,
            field_BC_ypos,
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale == 0 ? 0x60 : 0x06)
        == 1)
    {
        switch (field_100_pCollisionLine->field_8_type)
        {
            case 1u:
            case 5u:
                if (field_C4_velx < FP_FromInteger(0))
                {
                    field_BC_ypos = hitY;
                    field_B8_xpos = hitX;
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                    s16 vol = 75 - 20 * field_124;
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::GrenadeBounce_68, vol);
                    Event_Broadcast_422BC0(kEventNoise, this);
                    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                    Event_Broadcast_422BC0(kEventSpeaking, this);
                }
                break;

            case 2u:
            case 6u:
                if (field_C4_velx > FP_FromInteger(0))
                {
                    field_B8_xpos = hitX;
                    field_BC_ypos = hitY;
                    field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
                    s16 vol = 75 - 20 * field_124;
                    if (vol < 40)
                    {
                        vol = 40;
                    }
                    SFX_Play_Mono(SoundEffect::GrenadeBounce_68, vol);
                    Event_Broadcast_422BC0(kEventNoise, this);
                    Event_Broadcast_422BC0(kEventSuspiciousNoise, this);
                    Event_Broadcast_422BC0(kEventSpeaking, this);
                }
                break;

            default:
                return 1;
        }
    }

    return 1;
}

void Grenade::AddToPlatform()
{
    BaseAddToPlatform([](AETypes type)
                      { return type == AETypes::eLiftPoint_78 || type == AETypes::eTrapDoor_142; });
}

s16 Grenade::OnCollision_BounceOff(BaseGameObject* pHit)
{
    if (!pHit->mFlags.Get(BaseGameObject::eCanExplode_Bit7))
    {
        return 1;
    }

    auto pHit2 = static_cast<BaseAliveGameObject*>(pHit);
    if (pHit2->field_CC_sprite_scale != field_CC_sprite_scale)
    {
        return 1;
    }

    PSX_RECT bRect = {};
    pHit2->VGetBoundingRect(&bRect, 1);

    if (field_128_xpos < FP_FromInteger(bRect.x + 12) || field_128_xpos > FP_FromInteger(bRect.w - 12))
    {
        field_B8_xpos = field_128_xpos;
        field_C4_velx = (-field_C4_velx / FP_FromInteger(2));
    }
    else
    {
        field_BC_ypos = field_12C_ypos;
        field_C8_vely = (-field_C8_vely / FP_FromInteger(2));
    }

    pHit2->VOnThrowableHit(this);

    SFX_Play_Mono(SoundEffect::RockBounceOnMine_24, 0);
    return 0;
}

s16 Grenade::OnCollision_InstantExplode(BaseGameObject* pHit)
{
    if (pHit == field_138_pOwner)
    {
        // Don't do anything if hit the person who threw it
        return 1;
    }

    if (pHit->mFlags.Get(BaseGameObject::eCanExplode_Bit7) && static_cast<BaseAliveGameObject*>(pHit)->field_CC_sprite_scale == field_CC_sprite_scale)
    {
        field_134_bExplodeNow = 1;
        return 0;
    }

    return 1;
}
