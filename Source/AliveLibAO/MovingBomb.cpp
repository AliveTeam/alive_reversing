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

START_NS_AO

const TintEntry stru_4CD310[4] =
{
    { 5, 30u, 30u, 55u },
    { 6, 30u, 30u, 55u },
    { -1, 127u, 127u, 127u },
    { 0, 0u, 0u, 0u }
};

ALIVE_VAR(1, 0x507B8C, MovingBomb*, gMovingBomb_507B8C, nullptr);

MovingBomb* MovingBomb::ctor_43AFE0(Path_MovingBomb* pTlv, int tlvInfo)
{
    ctor_401090();
    field_6_flags.Set(Options::eCanExplode_Bit7);
    SetVTable(this, 0x4BBA68);
    field_4_typeId = Types::eTimedMine_8;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 3006, 1, 0);
    Animation_Init_417FD0(
        17140,
        76,
        30,
        ppRes,
        1);

    field_10_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_10_anim.field_B_render_mode = 0;
    field_10C_state = 1;

    if (pTlv->field_1E_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 16;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 35;
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

    if (pTlv->field_1C_start_type != 0)
    {
        field_10C_state = 0;
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    }

    SetTint_418750(stru_4CD310, gMap_507BA8.field_0_current_level);

    field_128_disabled_resources = pTlv->field_22_disabled_resources;

    if (!(field_128_disabled_resources & 1))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 1, 0);
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 217, 1, 0);
    }

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 25, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 576, 1, 0);

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
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 25, 0, 0));
    }

    if (!(field_128_disabled_resources & 2))
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 217, 0, 0));
    }

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 301, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 365, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 25, 0, 0));
    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, 576, 0, 0));

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }

    if (field_10C_state >= 6u)
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

    field_10C_state = 7;
    field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_114_timer = gnFrameCount_507670 + 4;
    return 0;
}

void MovingBomb::VRender(int** pOrderingTable)
{
    VRender_43B910(pOrderingTable);
}

void MovingBomb::VRender_43B910(int** ppOt)
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
    field_10C_state = 6;
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

void MovingBomb::VScreenChanged()
{
    VScreenChanged_43BC90();
}

END_NS_AO
