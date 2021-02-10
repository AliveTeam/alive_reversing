#include "stdafx_ao.h"
#include "Function.hpp"
#include "MovingBomb.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "PlatformBase.hpp"
#include "Midi.hpp"
#include "Gibs.hpp"
#include "Explosion.hpp"
#include "Sfx.hpp"
#include "SwitchStates.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Math.hpp"

namespace AO {

const TintEntry stru_4CD310[4] =
{
    { 5, 30u, 30u, 55u },
    { 6, 30u, 30u, 55u },
    { -1, 127u, 127u, 127u },
    { 0, 0u, 0u, 0u }
};

ALIVE_VAR(1, 0x507B8C, MovingBomb*, gMovingBomb_507B8C, nullptr);

struct Path_MovingBombStopper : public Path_TLV
{
    __int16 field_18_min_delay;
    __int16 field_1A_max_delay;
};

MovingBomb* MovingBomb::ctor_43AFE0(Path_MovingBomb* pTlv, int tlvInfo)
{
    ctor_401090();
    field_6_flags.Set(Options::eCanExplode_Bit7);
    SetVTable(this, 0x4BBA68);
    field_4_typeId = Types::eTimedMine_8;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMovebombResID, 1, 0);
    Animation_Init_417FD0(
        17140,
        76,
        30,
        ppRes,
        1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = TPageAbr::eBlend_0;
    field_10C_state = States::eTriggeredBySwitch_1;

    if (pTlv->field_1E_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = Layer::eLayer_16;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = Layer::eLayer_35;
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);


    field_118_speed = FP_FromRaw(pTlv->field_18_speed << 8);
    field_B4_velx = FP_FromRaw(pTlv->field_24_start_speed << 8);
    field_11C_id = pTlv->field_1A_id;
    field_114_timer = gnFrameCount_507670;
    field_C8_yOffset = 0;
    field_110_tlvInfo = tlvInfo;
    field_120_min = 0;
    field_11E_max = 0;
    field_12A_persist_offscreen = pTlv->field_26_persist_offscreen;
    field_124_sound_channels = 0;

    if (pTlv->field_1C_bStart_type_triggered_by_alarm)
    {
        field_10C_state = States::eTriggeredByAlarm_0;
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint_418750(stru_4CD310, gMap_507BA8.field_0_current_level);

    field_128_disabled_resources = pTlv->field_22_disabled_resources;

    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 1, 0);
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, 1, 0);
    }

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMetalGib, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, 1, 0);

    field_F8_pLiftPoint = nullptr;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos + FP_FromInteger(24),
        field_AC_ypos + FP_FromInteger(24),
        &field_F4_pLine,
        &hitX,
        &hitY,
        0x100))
    {
        field_AC_ypos = hitY;
        field_A8_xpos = hitX;
    }

    field_D0_pShadow = ao_new<Shadow>();

    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    return this;
}

BaseGameObject* MovingBomb::dtor_43B2C0()
{
    SetVTable(this, 0x4BBA68);

    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kAbeblowResID, 0, 0));
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kElmblowResID_217, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kMetalGib, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kAbeblowResID, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, ResourceID::kSlogBlowResID, 0, 0));

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }

    if (field_10C_state >= States::eBlowingUp_6)
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 0);
    }

    if (gMovingBomb_507B8C == this)
    {
        if (field_124_sound_channels)
        {
            SND_Stop_Channels_Mask_4774A0(field_124_sound_channels);
            field_124_sound_channels = 0;
        }
        gMovingBomb_507B8C = nullptr;
    }

    return dtor_401000();
}

BaseGameObject* MovingBomb::VDestructor(signed int flags)
{
    return Vdtor_43BCC0(flags);
}

BaseGameObject* MovingBomb::Vdtor_43BCC0(signed int flags)
{
    dtor_43B2C0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void MovingBomb::VScreenChanged_43BC90()
{
    if (!field_12A_persist_offscreen ||
        gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

__int16 MovingBomb::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_43BB60(pFrom);
}

__int16 MovingBomb::VTakeDamage_43BB60(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 1;
    }

    if (field_100_health <= FP_FromInteger(0))
    {
        return 1;
    }

    if ( pFrom->field_4_typeId != Types::eAbilityRing_69 &&  pFrom->field_4_typeId != Types::eExplosion_74 &&  pFrom->field_4_typeId != Types::eShrykull_85)
    {
        return 0;
    }

    field_100_health = FP_FromInteger(0);

    auto pExplosion = ao_new<Explosion>();
    if (pExplosion)
    {
        pExplosion->ctor_458B80(
            field_A8_xpos,
            field_AC_ypos,
            field_BC_sprite_scale);
    }

    auto pGibs = ao_new<Gibs>();
    if (pGibs)
    {
        pGibs->ctor_407B20(
            5,
            field_A8_xpos,
            field_AC_ypos,
            FP_FromInteger(0),
            FP_FromInteger(5),
            field_BC_sprite_scale);
    }

    field_10C_state = States::eKillMovingBomb_7;
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_114_timer = gnFrameCount_507670 + 4;
    return 0;
}

void MovingBomb::VRender(PrimHeader** ppOt)
{
    VRender_43B910(ppOt);
}

void MovingBomb::VRender_43B910(PrimHeader** ppOt)
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render))
    {
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void MovingBomb::VOnThrowableHit(BaseGameObject* pFrom)
{
    VOnThrowableHit_43B930(pFrom);
}

void MovingBomb::VOnThrowableHit_43B930(BaseGameObject* /*pFrom*/)
{
    field_6_flags.Clear(Options::eCanExplode_Bit7);
    field_10C_state = States::eBlowingUp_6;
    field_B8_vely = FP_FromInteger(0);
    field_114_timer = gnFrameCount_507670 + 1;
    SFX_Play_43AD70(SoundEffect::GreenTick_3, 100, 0);
}

__int16 MovingBomb::HitObject_43B970()
{
    PSX_RECT ourRect = {};
    VGetBoundingRect(&ourRect, 1);
    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter != this)
        {
            if (pObjIter->field_10A_flags.Get(Flags_10A::e10A_Bit4_SetOffExplosives))
            {
                if (pObjIter->field_100_health > FP_FromInteger(0))
                {
                    PSX_RECT objRect = {};
                    pObjIter->VGetBoundingRect(&objRect, 1);

                    if (RectsOverlap(ourRect, objRect) &&
                        pObjIter->field_BC_sprite_scale == field_BC_sprite_scale)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

void MovingBomb::FollowLine_43BA40()
{
    if (field_F4_pLine)
    {
        const FP oldX = field_A8_xpos;
        const FP oldY = field_AC_ypos;

        field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(&field_A8_xpos, &field_AC_ypos, field_B4_velx);
        if (field_F4_pLine)
        {
            WORD a4 = 0;
            const FP screen_x = CamX_VoidSkipper_418590(oldX, field_A8_xpos - oldX, 12, &a4);
            if (a4)
            {
                FP hitX = {};
                FP hitY = {};
                field_A8_xpos = screen_x;
                if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos,
                    field_AC_ypos - FP_FromInteger(20),
                    field_A8_xpos,
                    field_AC_ypos + FP_FromInteger(20),
                    &field_F4_pLine,
                    &hitX,
                    &hitY,
                    0x100))
                {
                    field_AC_ypos = hitY;
                }
            }

            // OG bug? Why y = oldx, surely y-oldy ?
            const FP screen_y = CamY_VoidSkipper_418690(oldY, field_AC_ypos - oldX, 12, &a4);
            if (a4)
            {
                FP hitX = {};
                FP hitY = {};
                field_AC_ypos = screen_y;
                if (sCollisions_DArray_504C6C->RayCast_40C410(
                    field_A8_xpos - FP_FromInteger(20),
                    field_AC_ypos,
                    field_A8_xpos + FP_FromInteger(20),
                    field_AC_ypos,
                    &field_F4_pLine,
                    &hitX,
                    &hitY,
                    0x100))
                {
                    field_A8_xpos = hitX;
                }
            }
        }
    }
}

void MovingBomb::VUpdate()
{
    VUpdate_43B440();
}

void MovingBomb::VUpdate_43B440()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_10C_state < States::eBlowingUp_6)
    {
        if (HitObject_43B970())
        {
            field_6_flags.Clear(Options::eCanExplode_Bit7);
            field_10C_state = States::eBlowingUp_6;
            field_B8_vely = FP_FromInteger(0);
            field_114_timer = gnFrameCount_507670 + 1;
            SFX_Play_43AD70(3u, 100, 0);
        }
    }

    if (!gMovingBomb_507B8C || gMovingBomb_507B8C == this)
    {
        if (field_10_anim.field_92_current_frame != 0 && field_10_anim.field_92_current_frame != 7)
        {
            gMovingBomb_507B8C = this;
        }
        else
        {
            if (field_124_sound_channels)
            {
                SND_Stop_Channels_Mask_4774A0(field_124_sound_channels);
            }

            if (VIsObjNearby(FP_FromInteger(700), sActiveHero_507678))
            {
                if (FP_Abs(sActiveHero_507678->field_AC_ypos - field_AC_ypos) <= FP_FromInteger(700))
                {
                    if (field_10C_state == States::eWaitABit_4)
                    {
                        field_124_sound_channels = SFX_Play_43AD70(SoundEffect::SecurityOrb_56, 55);
                    }
                    else
                    {
                        field_124_sound_channels = SFX_Play_43AD70(SoundEffect::SecurityOrb_56, 80);
                    }
                }
            }
            else
            {
                if (field_10C_state == States::eWaitABit_4)
                {
                    field_124_sound_channels = 0;
                    gMovingBomb_507B8C = this;
                }
                else
                {
                    field_124_sound_channels = SFX_Play_43AD70(SoundEffect::SecurityOrb_56, 12, 0);
                    gMovingBomb_507B8C = this;
                }
            }
        }
    }

    switch (field_10C_state)
    {
    case States::eTriggeredByAlarm_0:
        if (Event_Get_417250(kEvent_Alarm_17))
        {
            field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
            field_10C_state = States::eMoving_2;
        }
        break;

    case States::eTriggeredBySwitch_1:
        if (SwitchStates_Get(field_11C_id))
        {
            field_10C_state = States::eMoving_2;
        }
        break;

    case States::eMoving_2:
        if (field_B4_velx < field_118_speed)
        {
            field_B4_velx += (field_BC_sprite_scale * FP_FromDouble(0.5));
        }

        FollowLine_43BA40();

        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            TlvTypes::MovingBombStopper_87);

        if (field_F0_pTlv)
        {
            auto pStopper = static_cast<Path_MovingBombStopper*>(field_F0_pTlv);
            field_11E_max = pStopper->field_18_min_delay;
            field_120_min = pStopper->field_1A_max_delay;
            field_10C_state = States::eStopMoving_3;
        }
        break;

    case States::eStopMoving_3:
        field_B4_velx -= (field_BC_sprite_scale * FP_FromDouble(0.5));
        if (field_B4_velx < FP_FromInteger(0))
        {
            field_10C_state = States::eWaitABit_4;
            field_114_timer = gnFrameCount_507670 + Math_RandomRange_450F20(field_11E_max, field_120_min);
        }

        FollowLine_43BA40();
        break;

    case States::eWaitABit_4:
        if (field_114_timer <= static_cast<int>(gnFrameCount_507670))
        {
            field_10C_state = States::eToMoving_5;
        }
        break;

    case States::eToMoving_5:
        if (field_B4_velx < field_118_speed)
        {
            field_B4_velx += (field_BC_sprite_scale * FP_FromDouble(0.5));
        }

        FollowLine_43BA40();

        field_F0_pTlv = gMap_507BA8.TLV_Get_At_446260(
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            FP_GetExponent(field_A8_xpos),
            FP_GetExponent(field_AC_ypos),
            TlvTypes::MovingBombStopper_87);
        if (!field_F0_pTlv)
        {
            field_10C_state = States::eMoving_2;
        }
        break;

    case States::eBlowingUp_6:
        if (field_114_timer <= static_cast<int>(gnFrameCount_507670))
        {
            SFX_Play_43AD70(SoundEffect::GreenTick_3, 100, 0);

            field_100_health = FP_FromInteger(0);

            auto pExplosion = ao_new<Explosion>();
            if (pExplosion)
            {
                pExplosion->ctor_458B80(
                    field_A8_xpos,
                    field_AC_ypos,
                    field_BC_sprite_scale);
            }

            auto pGibs = ao_new<Gibs>();
            if (pGibs)
            {
                pGibs->ctor_407B20(
                    5,
                    field_A8_xpos,
                    field_AC_ypos,
                    FP_FromInteger(0),
                    FP_FromInteger(5),
                    field_BC_sprite_scale);
            }

            field_10C_state = States::eKillMovingBomb_7;
            field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
            field_114_timer = gnFrameCount_507670 + 4;
        }
        break;

    case States::eKillMovingBomb_7:
        if (field_114_timer <= static_cast<int>(gnFrameCount_507670))
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
        break;

    default:
        break;
    }
}

void MovingBomb::VScreenChanged()
{
    VScreenChanged_43BC90();
}

}
