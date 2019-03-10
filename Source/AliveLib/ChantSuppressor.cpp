#include "stdafx.h"
#include "ChantSuppressor.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Flash.hpp"
#include "ParticleBurst.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "PossessionFlicker.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Function.hpp"

class Class_544534 : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Class_544534* ctor_416390(FP xpos, FP ypos, FP scale)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x544534);
        field_4_typeId = Types::eType_22;

        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSparksResID);
        Animation_Init_424E10(808, 19, 11, ppRes, 1, 1);
        
        field_DC_bApplyShadows &= ~1u;

        field_20_animation.field_C_render_layer = 37;
        field_20_animation.field_B_render_mode = 1;

        field_20_animation.field_A_b = 80;
        field_20_animation.field_9_g = 80;
        field_20_animation.field_8_r = 80;

        field_CC_sprite_scale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
        field_FA_16_random = Math_RandomRange_496AB0(0, 16);

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        field_C4_velx = FP_FromInteger(Math_RandomRange_496AB0(-8, 8));
        field_C8_vely = FP_FromInteger(Math_RandomRange_496AB0(-6, -3));
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_416520(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_416570();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_416720();
    }

private:

    EXPORT void vUpdate_416570()
    {
        if (field_FA_16_random > 0)
        {
            field_FA_16_random--;
        }

        if (field_FA_16_random == 0)
        {
            field_20_animation.Set_Animation_Data_409C80(808, 0);
            field_FA_16_random = -1;
        }

        field_C8_vely += FP_FromDouble(0.8);

        field_C4_velx *= FP_FromDouble(0.9);
        field_C8_vely *= FP_FromDouble(0.9);

        field_C4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
        field_C8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

        field_BC_ypos += field_C8_vely;
        field_B8_xpos += field_C4_velx;

        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    EXPORT void vScreenChanged_416720()
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    EXPORT void dtor_416550()
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT Class_544534* vdtor_416520(signed int flags)
    {
        dtor_416550();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

private:
    int field_E4_not_used[4];
    int field_F4_not_used1;
    __int16 field_F8_not_used2;
    __int16 field_FA_16_random;
};
ALIVE_ASSERT_SIZEOF(Class_544534, 0xFC);

ALIVE_VAR(1, 0x5C1BB6, short, word_5C1BB6, 0);

struct Path_Slig : public Path_TLV
{
    __int16 field_10_scale;
    // TODO: Check these - GameEnder is probably only AO for instance ?
    enum class StartState : __int16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        RunOffScreen_4 = 4,
        GameEnder_5 = 5,
    };
    StartState field_12_start_state;
    __int16 field_14_pause_time;
    __int16 field_16_pause_left_min;
    __int16 field_18_pause_left_max;
    __int16 field_1A_pause_right_min;
    __int16 field_1C_pause_right_max;
    __int16 field_1E_chal_number;
    __int16 field_20_chal_timer;
    __int16 field_22_num_times_to_shoot;
    __int16 field_24_unknown;
    __int16 field_26_code1;
    __int16 field_28_code2;
    __int16 field_2A_chase_abe;
    __int16 field_2C_start_direction;
    __int16 field_2E_panic_timeout;
    __int16 field_30_num_panic_sounds;
    __int16 field_32_panic_sound_timeout;
    __int16 field_34_stop_chase_delay;
    __int16 field_36_time_to_wait_before_chase;
    __int16 field_38_slig_id;
    __int16 field_3A_listen_time;
    __int16 field_3C_percent_say_what;
    __int16 field_3E_percent_beat_mud;
    __int16 field_40_talk_to_abe;
    __int16 field_42_dont_shoot;
    __int16 field_44_stay_awake;
    __int16 field_46_disable_resources;
    __int16 field_48_noise_wake_up_distance;
    __int16 field_4A_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x4C);

class Explosion : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Explosion* ctor_4A1200(FP xpos, FP ypos, FP scale, __int16 bSmall)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x546CB8);
        field_4_typeId = Types::eExplosion_109;

        field_F4_bSmall = bSmall;
        if (field_F4_bSmall)
        {
            BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSmallExplo2ResID);
            Animation_Init_424E10(14108, 99, 46, ppRes, 1, 1);
        }
        else
        {
            BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
            Animation_Init_424E10(51156, 202, 91, ppRes, 1, 1);
        }

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
        field_20_animation.field_B_render_mode = 1;
        field_F8_scale = scale;
        field_D6_scale = scale == FP_FromInteger(1);
        field_CC_sprite_scale = scale * FP_FromInteger(2);
       
        if (field_F4_bSmall)
        {
            field_FC = scale * FP_FromDouble(0.5);
        }
        else
        {
            field_FC = scale;
        }
        field_DC_bApplyShadows &= ~1u;
        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        auto pScreenShake = alive_new<ScreenShake>();
        if (pScreenShake)
        {
            pScreenShake->ctor_4ACF70(word_5C1BB6 ? 0 : 1, field_F4_bSmall);
        }

        PSX_RECT rect = {};
        rect.x = FP_GetExponent(FP_FromInteger(-10) * field_FC);
        rect.y = FP_GetExponent(FP_FromInteger(10) * field_FC);
        rect.w = FP_GetExponent(FP_FromInteger(-10) * field_FC);
        rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC);

        DealBlastDamage_4A1BD0(&rect);

        SND_SEQ_PlaySeq_4CA960(14, 1, 1);

        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4A14C0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_4A1510();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_4A1EE0();
    }

private:

    EXPORT void DealBlastDamage_4A1BD0(PSX_RECT* pRect)
    {
        if (!gBaseAliveGameObjects_5C1B7C)
        {
            return;
        }

        PSX_RECT expandedRect = {};
        expandedRect.x = std::min(pRect->x, pRect->w);
        expandedRect.w = std::max(pRect->x, pRect->w);

        expandedRect.y = std::min(pRect->y, pRect->h);
        expandedRect.h = std::max(pRect->y, pRect->h);

        expandedRect.x += FP_GetExponent(field_B8_xpos);
        expandedRect.w += FP_GetExponent(field_B8_xpos);

        expandedRect.y += FP_GetExponent(field_BC_ypos);
        expandedRect.h += FP_GetExponent(field_BC_ypos);

        for (int idx = 0; idx < gBaseAliveGameObjects_5C1B7C->Size(); idx++)
        {
            BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(idx);
            if (!pObj)
            {
                break;
            }

            if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject))
            {
                PSX_RECT boundRect = {};
                pObj->vGetBoundingRect_424FD0(&boundRect, 1);

                if (PSX_Rects_overlap_no_adjustment(&boundRect, &expandedRect) && /*boundRect.x <= expandedRect.w &&
                    boundRect.w >= expandedRect.x && 
                    boundRect.h >= expandedRect.y && 
                    boundRect.y <= expandedRect.h &&*/
                    field_D6_scale == pObj->field_D6_scale)
                {
                    pObj->VTakeDamage_408730(this);
                }
            }
        }

        auto pTlv = static_cast<Path_Slig*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            expandedRect.x,
            expandedRect.y,
            expandedRect.w,
            expandedRect.h,
            TlvTypes::Slig_15));

        if (pTlv)
        {
            if (!pTlv->field_0_flags.Get(TLV_Flags::eBit2_Unknown) && pTlv->field_12_start_state == Path_Slig::StartState::Sleeping_2)
            {
                pTlv->field_0_flags.Set(TLV_Flags::eBit2_Unknown);

               const signed __int16 direction = gMap_5C3030.sub_4811A0(
                    gMap_5C3030.sCurrentLevelId_5C3030,
                    gMap_5C3030.sCurrentPathId_5C3032,
                    FP_FromInteger(pTlv->field_8_top_left.field_0_x),
                    FP_FromInteger(pTlv->field_8_top_left.field_2_y));

                if (direction == 3)
                {
                    auto pGibs = alive_new<Gibs>();
                    if (pGibs)
                    {
                        pGibs->ctor_40FB40(1, field_B8_xpos + FP_FromInteger(656), field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
                    }
                }
                else if (direction == 4)
                {
                    auto pGibs = alive_new<Gibs>();
                    if (pGibs)
                    {
                        pGibs->ctor_40FB40(1, field_B8_xpos - FP_FromInteger(656), field_BC_ypos, FP_FromInteger(0), FP_FromInteger(0), FP_FromInteger(1), 0);
                    }
                }

                Start_Exploision_sounds_4CBA70(direction, 0);
            }
        }
    }

    EXPORT void vUpdate_4A1510()
    {
        Event_Broadcast_422BC0(kEventShooting, this);
        Event_Broadcast_422BC0(kEventLoudNoise, this);
        Event_Broadcast_422BC0(kEventSuspiciousNoise, this);

        PSX_RECT rect = {};

        switch (field_20_animation.field_92_current_frame)
        {
        case 2:
            rect.x = FP_GetExponent(FP_FromInteger(-20) * field_FC);
            rect.w = FP_GetExponent(FP_FromInteger(20) * field_FC);
            rect.y = FP_GetExponent(FP_FromInteger(-20) * field_FC);
            rect.h = FP_GetExponent(FP_FromInteger(10) * field_FC);
            DealBlastDamage_4A1BD0(&rect);
            break;

        case 3:
        {
            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 6 : 20, field_F8_scale, 3, field_F4_bSmall ? 11 : 13);
            }

            auto pFlash = alive_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 3u, 1);
            }
        }
        break;

        case 4:
        {
            auto pFlash = alive_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 1, 1);
            }
            rect.x = FP_GetExponent(FP_FromInteger(-38) * field_FC);
            rect.w = FP_GetExponent(FP_FromInteger(38) * field_FC);
            rect.y = FP_GetExponent(FP_FromInteger(-38) * field_FC);
            rect.h = FP_GetExponent(FP_FromInteger(19) * field_FC);
            DealBlastDamage_4A1BD0(&rect);
        }
        break;

        case 6:
            rect.x = FP_GetExponent(FP_FromInteger(-60) * field_FC);
            rect.w = FP_GetExponent(FP_FromInteger(60) * field_FC);
            rect.y = FP_GetExponent(FP_FromInteger(-60) * field_FC);
            rect.h = FP_GetExponent(FP_FromInteger(30) * field_FC);
            DealBlastDamage_4A1BD0(&rect);
            break;

        case 8:
        {
            auto pParticleBurst = alive_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_41CF50(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 6 : 20, field_F8_scale, 3, field_F4_bSmall ? 11 : 13);
            }

            auto pFlash = alive_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(39, 255u, 255u, 255u, 1, 3u, 1);
            }
        }
        break;

        default:
            break;
        }

        if (field_20_animation.field_92_current_frame > 9)
        {
            if (field_F4_bSmall)
            {
                field_CC_sprite_scale -= FP_FromDouble(0.065);
            }
            else
            {
                field_CC_sprite_scale -= FP_FromDouble(0.2);
            }
        }

        if (field_20_animation.field_92_current_frame == 1)
        {
            BYTE** ppRes = field_F4_bSmall ?
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSmallExplo2ResID) :
                Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kExplo2ResID);
            if (ppRes)
            {
                auto pParticle = alive_new<Particle>();
                if (pParticle)
                {
                    pParticle->ctor_4CC4C0(field_B8_xpos, field_BC_ypos, field_F4_bSmall ? 14108 : 51156,
                        202, // Same size for both for some reason
                        91,
                        ppRes);

                    if (pParticle->field_6_flags.Get(BaseGameObject::eListAddFailed))
                    {
                        pParticle->field_6_flags.Set(BaseGameObject::eDead);
                    }

                    pParticle->field_DC_bApplyShadows &= ~1u;
                    pParticle->field_20_animation.field_B_render_mode = 1;

                    if (field_20_animation.field_92_current_frame == 3)
                    {
                        pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX);
                        pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.5);
                    }
                    else
                    {
                        pParticle->field_20_animation.field_4_flags.Clear(AnimFlags::eBit5_FlipX);
                        pParticle->field_CC_sprite_scale = field_CC_sprite_scale * FP_FromDouble(0.25);
                    }
                }
            }
        }

        if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    EXPORT void vScreenChanged_4A1EE0()
    {
        if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    EXPORT void dtor_4A14F0()
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT Explosion* vdtor_4A14C0(signed int flags)
    {
        dtor_4A14F0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

private:
    int field_E4_not_used[4];
    __int16 field_F4_bSmall;
    //__int16 field_F6_pad;
    FP field_F8_scale;
    FP field_FC;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0x100);

class ZapLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZapLine* ctor_4CC690(FP /*xpos*/, FP /*ypos*/, FP /*a4*/, FP /*a5*/, __int16 /*a6*/, signed __int16 /*type*/, __int16 /*layer*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

private:
    __int16 field_E4;
    __int16 field_E6;
    __int16 field_E8;
    __int16 field_EA;
    __int16 field_EC;
    __int16 field_EE;
    __int16 field_F0;
    __int16 field_F2;
    __int16 field_F4_state;
    __int16 field_F6;
    int field_F8_ppRes;
    __int16 field_FC_tPage_p8;
    __int16 field_FE;
    __int16 field_100;
    __int16 field_102;
    __int16 field_104;
    __int16 field_106;
    __int16 field_108;
    __int16 field_10A;
    __int16 field_10C;
    __int16 field_10E;
    __int16 field_110;
    __int16 field_112;
    __int16 field_114;
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    __int16 field_128;
    __int16 field_12A_type;
    __int16 field_12C;
    __int16 field_12E_count;
    __int16 field_130_count;
    __int16 field_132_total_count;
    void *field_134_pSprts;
    int field_138_buf1;
    int field_13C_buf2;
    int field_140_buf3;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    __int16 field_150;
    __int16 field_152;
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x154);

const TintEntry sChantOrbTints_55C1EC[18] =
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
    { 3u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


ChantSuppressor* ChantSuppressor::ctor_466350(Path_ChantSuppressor* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x545F3C);
    
    field_4_typeId = Types::eAntiChant_83;

    field_6_flags.Set(BaseGameObject::eCanExplode);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMaimGameResID);
    Animation_Init_424E10(2228, 53, 28u, ppRes, 1, 1u);
    
    SetTint_425600(sChantOrbTints_55C1EC, gMap_5C3030.sCurrentLevelId_5C3030);

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_118_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 8;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 27;
    }

    field_DC_bApplyShadows |= 2u;
    field_11C_state = 0;
    field_124_sound_channels_mask = 0;

    return this;
}

ChantSuppressor* ChantSuppressor::vdtor_4664B0(signed int flags)
{
    dtor_4664E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ChantSuppressor::dtor_4664E0()
{
    SetVTable(this, 0x545F3C);

    if (field_124_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_124_sound_channels_mask);
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    dtor_4080B0();
}

void ChantSuppressor::vScreenChanged_466D20()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

signed __int16 ChantSuppressor::vTakeDamage_466BB0(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    field_6_flags.Set(BaseGameObject::eDead);
    field_10C_health = FP_FromInteger(0);

    if (pFrom->field_4_typeId == Types::eMineCar_89 || pFrom->field_4_typeId == Types::eType_104 || pFrom->field_4_typeId == Types::eType_121)
    {
        auto pExplosionMem = alive_new<Explosion>();
        if (pExplosionMem)
        {
            pExplosionMem->ctor_4A1200(
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
                field_CC_sprite_scale,
                0);
        }

        auto pGibsMem = alive_new<Gibs>();
        if (pGibsMem)
        {
            pGibsMem->ctor_40FB40(
                5,
                field_B8_xpos,
                field_BC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);
        }
    }

    return 1;
}

void ChantSuppressor::vUpdate_4665A0()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    if (field_11C_state)
    {
        const int stateM1 = field_11C_state - 1;
        if (stateM1)
        {
            if (stateM1 == 1)
            {
                if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 5 || static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 1)
                {
                    auto pFlash1 = alive_new<Flash>();
                    if (pFlash1)
                    {
                        pFlash1->ctor_428570(39, 255, 0, 0, 1, 3, 1);
                    }
                }
                if (static_cast<int>(sGnFrame_5C1B84) == field_120_timer - 4)
                {
                    auto pFlash2 = alive_new<Flash>();
                    if (pFlash2)
                    {
                        pFlash2->ctor_428570(39, 255, 0, 0, 1, 1, 1);
                    }
                }

                const int timerFrame = field_120_timer - sGnFrame_5C1B84;
                if (timerFrame == 4)
                {
                    SFX_Play_46FA90(0x31u, 0, field_CC_sprite_scale.fpValue);
                }
                else if (timerFrame == 1)
                {
                    SFX_Play_46FA90(0x32u, 0, field_CC_sprite_scale.fpValue);
                }

                if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
                {
                    field_11C_state = 0;
                }
            }
        }
        else if (static_cast<int>(sGnFrame_5C1B84) > field_120_timer)
        {
            PSX_RECT bRect = {};
            sActiveHero_5C1B68->vGetBoundingRect_424FD0(&bRect, 1);

            const FP xpos = FP_FromInteger((bRect.x + bRect.w) / 2);
            const FP ypos = FP_FromInteger((bRect.y + bRect.h) / 2);

            auto pZapLine = alive_new<ZapLine>();
            if (pZapLine)
            {
                pZapLine->ctor_4CC690(
                    field_B8_xpos,
                    field_BC_ypos - (FP_FromInteger(8) * field_CC_sprite_scale),
                    xpos,
                    ypos,
                    8,
                    0,
                    28);
            }

            auto v14 = alive_new<PossessionFlicker>();
            if (v14)
            {
                v14->ctor_4319E0(sActiveHero_5C1B68, 8, 255, 100, 100);
            }

            if (sActiveHero_5C1B68->field_10C_health > FP_FromInteger(0))
            {
                sActiveHero_5C1B68->VTakeDamage_408730(this);
            }

            field_120_timer = sGnFrame_5C1B84 + 8;
            field_11C_state = 2;

            auto pScreenShake = alive_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4ACF70(1, 0);
            }

            auto pUnknownObj = alive_new<Class_544534>();
            if (pUnknownObj)
            {
                pUnknownObj->ctor_416390(field_B8_xpos, field_BC_ypos - (FP_FromInteger(8) * field_CC_sprite_scale), field_CC_sprite_scale);

                pUnknownObj->field_D2_g = 65;
                pUnknownObj->field_D4_b = 65;
                pUnknownObj->field_D0_r = 255;
            }

            auto pUnknownObj2 = alive_new<Class_544534>();
            if (pUnknownObj2)
            {
                pUnknownObj2->ctor_416390(field_B8_xpos, field_BC_ypos - (FP_FromInteger(8) * field_CC_sprite_scale), field_CC_sprite_scale);

                pUnknownObj2->field_D2_g = 65;
                pUnknownObj2->field_D4_b = 65;
                pUnknownObj2->field_D0_r = 255;
            }

            for (int i = 0; i < 9; i++)
            {
                auto pUnknownObj4 = alive_new<Class_544534>();
                if (pUnknownObj4)
                {
                    pUnknownObj4->ctor_416390(xpos, ypos, field_CC_sprite_scale);
                    pUnknownObj4->field_D2_g = 65;
                    pUnknownObj4->field_D4_b = 65;
                    pUnknownObj4->field_D0_r = 255;
                }
            }
        }
    }
    else
    {
        if (field_20_animation.field_92_current_frame == 2 ||
            field_20_animation.field_92_current_frame == 6 ||
            field_20_animation.field_92_current_frame == 10)
        {
            if (field_124_sound_channels_mask)
            {
                SND_Stop_Channels_Mask_4CA810(field_124_sound_channels_mask);
            }

            if (field_CC_sprite_scale == FP_FromDouble(0.5))
            {
                field_124_sound_channels_mask = SFX_Play_46FBA0(48, 35, 720, 0x8000);
            }
            else
            {
                field_124_sound_channels_mask = SFX_Play_46FBA0(48, 55, 700, field_CC_sprite_scale.fpValue);
            }
        }

        if (Event_Get_422C00(kEventAbeOhm))
        {
            if (!sActiveHero_5C1B68->field_168_ring_pulse_timer ||
                !sActiveHero_5C1B68->field_16C_bHaveShrykull ||
                sActiveHero_5C1B68->field_CC_sprite_scale != FP_FromInteger(1))
            {
                field_11C_state = 1;
                field_120_timer = sGnFrame_5C1B84 + 20;
            }
        }
    }
}
