#include "stdafx_ao.h"
#include "Function.hpp"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Abe.hpp"
#include "ScreenClipper.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Flash.hpp"
#include "Midi.hpp"
#include "Dove.hpp"
#include "OrbWhirlWind.hpp"
#include "Particle.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"
#include "PsxDisplay.hpp"
#include "BaseGameObject.hpp"
#include "AbilityRing.hpp"

START_NS_AO

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
}

BirdPortalTerminator* BirdPortalTerminator::ctor_451F70(FP xpos, FP ypos, FP scale, PortalType /*portalType*/)
{
    ctor_417C10();
    SetVTable(this, 0x4BBFB0);

    field_4_typeId = Types::eClaw_48;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 1, 0);
    Animation_Init_417FD0(3860, 31, 18, ppRes, 1);
    field_10_anim.field_B_render_mode = 1;
    field_BC_sprite_scale = scale;
    if (scale == FP_FromInteger(1))
    {
        field_10_anim.field_C_layer = 39;
    }
    else
    {
        field_10_anim.field_C_layer = 20;
    }

    field_CC_bApplyShadows &= ~1u;

    field_AC_ypos = ypos;
    field_A8_xpos = xpos;

    field_C0_r = 255;
    field_C2_g = 128;
    field_C4_b = 64;

    return this;
}

BaseGameObject* BirdPortalTerminator::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

// ==========================================================================

BaseGameObject* BirdPortal::VDestructor(signed int flags)
{
    return Vdtor_453990(flags);
}

BaseGameObject* BirdPortal::dtor_452230()
{
    SetVTable(this, 0x4BBFE8);

    if (field_3C_pTerminator1)
    {
        field_3C_pTerminator1->field_6_flags.Set(Options::eDead_Bit3);
    }
    if (field_40_pTerminator2)
    {
        field_40_pTerminator2->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_44_pScreenClipper)
    {
        field_44_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_48_pScreenClipper)
    {
        field_48_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_4C_pDovesArray)
    {
        for (int i = 0; i < field_4C_pDovesArray->Size(); i++)
        {
            Dove* pObj = field_4C_pDovesArray->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            pObj->field_C_refCount--;
            pObj->field_6_flags.Set(Options::eDead_Bit3);
        }

        field_4C_pDovesArray->field_4_used_size = 0;

        if (field_4C_pDovesArray)
        {
            field_4C_pDovesArray->dtor_404440();
            ao_delete_free_447540(field_4C_pDovesArray);
        }
    }

    if (field_5C)
    {
        field_5C->field_6_flags.Set(Options::eDead_Bit3);
        field_5C->field_C_refCount--;
        field_5C = nullptr;
    }

    BYTE** ppRes = nullptr;
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 351, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 353, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);
    ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 0, 0);
    ResourceManager::FreeResource_455550(ppRes);

    if (field_68_sfx_ret)
    {
        SND_Stop_Channels_Mask_4774A0(field_68_sfx_ret);
        field_68_sfx_ret = 0;
    }

    gMap_507BA8.TLV_Reset_446870(field_2C_tlvInfo, -1, 0, 0);
    if (field_14_state >= States::State_17)
    {
        if (sActiveHero_507678)
        {
            if (field_34_scale == FP_FromInteger(1))
            {
                sActiveHero_507678->field_10_anim.field_C_layer = 32;
            }
            else
            {
                sActiveHero_507678->field_10_anim.field_C_layer = 13;
            }
        }
    }
    return dtor_487DF0();
}

BirdPortal* BirdPortal::ctor_4520A0(Path_BirdPortal* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBFE8);

    field_4_typeId = Types::eBirdPortal_65;

    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 313, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 60, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 351, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 353, 1, 0);

    field_2C_tlvInfo = tlvInfo;


    field_50_dest_level = pTlv->field_1A_dest_level;
    field_12_side = pTlv->field_18_side;
    field_54_dest_camera = pTlv->field_1E_dest_camera;
    field_52_dest_path = pTlv->field_1C_dest_path;
    field_10_portal_type = pTlv->field_24_portal_type;
    field_38_movie_id = pTlv->field_22_movie_id;
    field_56_num_muds_for_shrykul = pTlv->field_26_num_muds_for_shrykul;
    if (pTlv->field_20_scale == 1)
    {
        field_34_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_34_scale = FP_FromInteger(1);
    }

    field_66_path = gMap_507BA8.field_2_current_path;
    field_64_level = gMap_507BA8.field_0_current_level;
    field_14_state = States::State_0;
    field_30_timer = 0;

    field_3C_pTerminator1 = nullptr;
    field_40_pTerminator2 = nullptr;
    field_44_pScreenClipper = nullptr;
    field_48_pScreenClipper = nullptr;
    field_4C_pDovesArray = nullptr;
    field_60_pOrbWhirlWind = nullptr;
    field_5C = 0;

    PathLine* pLine = nullptr;
    FP hitX = {};
    sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &pLine,
        &hitX,
        &field_28_ypos,
        0xFFFFFFFF);  // -1 ??

    field_18_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_68_sfx_ret = 0;
    field_1C_ypos = field_28_ypos - (FP_FromInteger(55) * field_34_scale);
    return this;
}

BirdPortal* BirdPortal::Vdtor_453990(signed int flags)
{
    dtor_452230();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BirdPortal::VUpdate()
{
    VUpdate_4523D0();
}

void BirdPortal::VUpdate_4523D0()
{
    const CameraPos direction = gMap_507BA8.GetDirection(
        field_64_level,
        field_66_path,
        field_18_xpos,
        field_1C_ypos);

    if (!field_68_sfx_ret)
    {
        if (field_14_state > States::State_4 && field_14_state < States::State_11)
        {
            field_68_sfx_ret = SFX_Play_43AED0(67u, 45, direction);
        }
    }

    switch (field_14_state)
    {
    case States::State_0:
        field_4C_pDovesArray = ao_new<DynamicArrayT<Dove>>();
        if (field_4C_pDovesArray)
        {
            field_4C_pDovesArray->ctor_4043E0(6);
        }

        for (BYTE i = 0; i < 6; i++)
        {
            auto pDove = ao_new<Dove>();
            if (pDove)
            {
                pDove->ctor_40EFF0(
                    4988,
                    41,
                    20,
                    60,
                    field_18_xpos,
                    field_1C_ypos,
                    field_34_scale);
                pDove->field_C_refCount++;
            }

            if (field_10_portal_type == PortalType::eAbe_0)
            {
                pDove->AsAlmostACircle_40F300(field_18_xpos, field_1C_ypos + (field_34_scale * FP_FromInteger(30)), 42 * i);

            }
            else
            {
                pDove->AsACircle_40F280(field_18_xpos, field_1C_ypos + (field_34_scale * FP_FromInteger(30)), 42 * i);
            }

            pDove->field_BC_sprite_scale = field_34_scale;
            field_4C_pDovesArray->Push_Back(pDove);
        }

        if (field_10_portal_type == PortalType::eShrykull_2)
        {
            field_5C = ao_new<ThrowableTotalIndicator>();
            if (field_5C)
            {
                field_5C->ctor_41B520(
                    field_18_xpos,
                    field_1C_ypos + FP_FromInteger(10),
                    field_34_scale != FP_FromDouble(0.5) ? 27 : 8,
                    field_34_scale,
                    field_56_num_muds_for_shrykul,
                    0);
            }
            field_5C->field_C_refCount++;
        }

        field_14_state = States::State_1;
        break;

    case States::State_1:
    {
        if (static_cast<int>(gnFrameCount_507670) >= field_30_timer)
        {
            SFX_Play_43AD70(16u, 35, 0);
            field_30_timer = gnFrameCount_507670 + Math_RandomRange_450F20(24, 40);
        }

        auto pTarget = Abe::FindObjectToPosses_421410();
        if (!Event_Get_417250(8) || pTarget)
        {
            if (IsScaredAway_4532E0() || Event_Get_417250(2) || Event_Get_417250(8) && pTarget)
            {
                for (int i = 0; i < field_4C_pDovesArray->Size(); i++)
                {
                    Dove* pDove = field_4C_pDovesArray->ItemAt(i);
                    if (!pDove)
                    {
                        break;
                    }
                    pDove->field_C_refCount--;
                    pDove->FlyAway_40F8F0(1);
                }

                field_4C_pDovesArray->field_4_used_size = 0;
                if (field_4C_pDovesArray)
                {
                    field_4C_pDovesArray->dtor_404440();
                    ao_delete_free_447540(field_4C_pDovesArray);
                }

                field_4C_pDovesArray = nullptr;

                if (field_5C)
                {
                    field_5C->field_6_flags.Set(Options::eDead_Bit3);
                    field_5C->field_C_refCount--;
                    field_5C = nullptr;
                }

                SFX_Play_43AD70(16u, 70, 0);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        else
        {
            if (field_5C)
            {
                field_5C->field_6_flags.Set(Options::eDead_Bit3);
                field_5C->field_C_refCount--;
                field_5C = nullptr;
            }

            for (int i = 0; i < field_4C_pDovesArray->Size(); i++)
            {
                Dove* pDove = field_4C_pDovesArray->ItemAt(i);
                if (!pDove)
                {
                    break;
                }
                pDove->AsJoin_40F250(field_18_xpos, field_1C_ypos + (field_34_scale * FP_FromInteger(20)));
            }

            field_30_timer = gnFrameCount_507670 + 15;
            field_14_state = States::State_2;
            Event_Broadcast_417220(18, this);
            SFX_Play_43AD70(16u, 70, 0);
        }
    }
        break;

    case States::State_2:
        Event_Broadcast_417220(18, this);
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            field_3C_pTerminator1 = ao_new<BirdPortalTerminator>();
            if (field_3C_pTerminator1)
            {
                field_3C_pTerminator1->ctor_451F70(
                    field_18_xpos,
                    field_1C_ypos,
                    field_34_scale,
                    field_10_portal_type);
            }

            field_40_pTerminator2 = ao_new<BirdPortalTerminator>();
            if (field_40_pTerminator2)
            {
                field_40_pTerminator2->ctor_451F70(
                    field_18_xpos,
                    field_1C_ypos,
                    field_34_scale,
                    field_10_portal_type);
            }

            field_30_timer = gnFrameCount_507670 + 5;
            field_14_state = States::State_3;
        }
        break;

    case States::State_3:
        Event_Broadcast_417220(18, this);
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            for (int i = 0; i < field_4C_pDovesArray->Size(); i++)
            {
                Dove* pDove = field_4C_pDovesArray->ItemAt(i);
                if (!pDove)
                {
                    break;
                }

                pDove->field_C_refCount--;
                pDove->field_6_flags.Set(Options::eDead_Bit3);
            }

            field_4C_pDovesArray->field_4_used_size = 0;
            if (field_4C_pDovesArray)
            {
                field_4C_pDovesArray->dtor_404440();
                ao_delete_free_447540(field_4C_pDovesArray);
            }

            field_4C_pDovesArray = nullptr;
            field_14_state = States::State_4;
        }
        break;

    case States::State_4:
        Event_Broadcast_417220(18, this);
        if (field_3C_pTerminator1->field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_3C_pTerminator1->field_10_anim.Set_Animation_Data_402A40(3784, 0);
            field_40_pTerminator2->field_10_anim.Set_Animation_Data_402A40(3784, 0);
            field_30_timer = gnFrameCount_507670 + 12;
            field_14_state = States::State_5;
            field_68_sfx_ret = SFX_Play_43AD70(67u, 0, 0);
            SFX_Play_43AE60(107u, 115, 300, 0);
        }
        break;

    case States::State_5:
        Event_Broadcast_417220(18, this);
        field_3C_pTerminator1->field_AC_ypos -= (FP_FromDouble(3.5) * field_34_scale);
        field_40_pTerminator2->field_AC_ypos += (FP_FromDouble(3.5) * field_34_scale);
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            field_14_state = States::State_6;
        }
        break;

    case States::State_6:
        Event_Broadcast_417220(18, this);
        if (field_10_portal_type != PortalType::eWorker_1 && field_10_portal_type != PortalType::eShrykull_2 || Event_Get_417250(8))
        {
            if ((Math_NextRandom() % 8) == 0)
            {
                BYTE** ppLightRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 351, 1, 0);
                if (ppLightRes)
                {
                    auto pParticle = ao_new<Particle>();
                    if (pParticle)
                    {
                        pParticle->ctor_478880(
                            field_40_pTerminator2->field_A8_xpos,
                            (FP_FromInteger(10) * field_34_scale) + field_40_pTerminator2->field_AC_ypos,
                            3708,
                            31,
                            69,
                            ppLightRes);
                    }
                    pParticle->field_CC_bApplyShadows &= ~1u;
                    pParticle->field_10_anim.field_B_render_mode = 1;
                    pParticle->field_4_typeId = Types::e66;
                    pParticle->field_BC_sprite_scale = field_34_scale;

                    if (gnFrameCount_507670 % 2)
                    {
                        pParticle->field_10_anim.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                        pParticle->field_10_anim.SetFrame_402AC0(pParticle->field_10_anim.Get_Frame_Count_403540());
                    }

                    if (direction == CameraPos::eCamCurrent_0)
                    {
                        SFX_Play_43AE60(48u, 50, 2400, 0);
                    }
                }
            }
            if (!(gnFrameCount_507670 % 8))
            {
                AbilityRing::Factory_447590(
                    field_3C_pTerminator1->field_A8_xpos,
                    field_3C_pTerminator1->field_AC_ypos + FP_FromInteger(7),
                    6);
                AbilityRing::Factory_447590(
                    field_40_pTerminator2->field_A8_xpos,
                    field_40_pTerminator2->field_AC_ypos + FP_FromInteger(7),
                    6);
            }
        }
        else
        {
            VGiveShrukul_4535A0(0);
            if (field_14_state != States::State_7)
            {
                field_14_state = States::State_22;
                field_30_timer = gnFrameCount_507670 + 30;
            }
        }
        break;

    case States::State_7:
        if (field_58 >= 7)
        {
            field_14_state = States::State_9;
            field_30_timer = gnFrameCount_507670 + 20;
        }
        else
        {
            FP xOff = {};
            if (field_12_side == PortalSide::eLeft_1)
            {
                xOff = (field_34_scale * FP_FromInteger(16));
            }
            else
            {
                xOff = (field_34_scale * FP_FromInteger(-16));
            }

            const short scale32 = FP_GetExponent(field_34_scale * FP_FromInteger(32));
            if (static_cast<int>(gnFrameCount_507670) >= field_30_timer)
            {
                field_30_timer = gnFrameCount_507670 + Math_RandomRange_450F20(4, 12);
                auto pDoveMem = ao_new<Dove>();
                if (pDoveMem)
                {
                    pDoveMem->ctor_40EFF0(
                        4988,
                        41,
                        20,
                        60,
                        field_18_xpos + FP_FromInteger(FP_GetExponent(xOff)),
                        field_1C_ypos + FP_FromInteger(Math_RandomRange_450F20(-scale32, scale32)),
                        field_34_scale);
                }

                SFX_Play_43AD70(16u, 70, 0);
                pDoveMem->field_BC_sprite_scale = field_34_scale;
                pDoveMem->AsJoin_40F250(sActiveHero_507678->field_A8_xpos,
                    FP_FromInteger(Math_RandomRange_450F20(-36, 4)) + sActiveHero_507678->field_AC_ypos);

                if (++field_58 == 6)
                {
                    field_60_pOrbWhirlWind->ToSpin_48BAD0(
                        sActiveHero_507678->field_A8_xpos,
                        sActiveHero_507678->field_AC_ypos - (sActiveHero_507678->field_BC_sprite_scale * FP_FromInteger(38)),
                        sActiveHero_507678);
                    field_60_pOrbWhirlWind = nullptr;
                }
            }
        }
        break;

    case States::State_9:
        if (static_cast<int>(gnFrameCount_507670) >= field_30_timer)
        {
            Abe::Get_Shrykull_Resources_42F480();
            field_14_state = States::State_10;
            sActiveHero_507678->field_168_ring_pulse_timer = gnFrameCount_507670 + 32000;
            sActiveHero_507678->field_16C_bHaveShrykull = TRUE;
            if (sActiveHero_507678->field_FC_current_motion == eAbeStates::State_150_Chant_42FD50)
            {
                sActiveHero_507678->sub_430510(0);
            }
        }
        break;

    case States::State_10:
        field_3C_pTerminator1->field_AC_ypos += FP_FromDouble(3.5) * field_34_scale;
        field_40_pTerminator2->field_AC_ypos -= FP_FromDouble(3.5) * field_34_scale;

        if (FP_GetExponent(field_3C_pTerminator1->field_AC_ypos) >= FP_GetExponent(field_40_pTerminator2->field_AC_ypos))
        {
            BYTE** ppLightRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 353, 1, 0);
            if (ppLightRes)
            {
                auto pParticle_1 = ao_new<Particle>();
                if (pParticle_1)
                {
                    pParticle_1->ctor_478880(
                        field_40_pTerminator2->field_A8_xpos,
                        field_40_pTerminator2->field_AC_ypos,
                        13352,
                        145,
                        74,
                        ppLightRes);
                }
                pParticle_1->field_CC_bApplyShadows &= ~1u;
                pParticle_1->field_10_anim.field_B_render_mode = 1;
                pParticle_1->field_BC_sprite_scale = field_34_scale;
            }

            field_14_state = States::State_11;
            field_30_timer = gnFrameCount_507670 + 5;
            field_3C_pTerminator1->field_6_flags.Set(Options::eDead_Bit3);
            field_40_pTerminator2->field_6_flags.Set(Options::eDead_Bit3);
            field_3C_pTerminator1 = nullptr;
            field_40_pTerminator2 = nullptr;
            SFX_Play_43AE60(61u, 100, -1800, 0);
        }
        break;

    case States::State_11:
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            if (field_68_sfx_ret)
            {
                SND_Stop_Channels_Mask_4774A0(field_68_sfx_ret);
                field_68_sfx_ret = 0;
            }
            field_14_state = States::State_12;
        }
        break;

    case States::State_12:
    {
        auto pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(40, 255u, 255u, 255u, 1, 3u, 1);
        }
        field_14_state = States::State_13;
    }
    break;

    case States::State_13:
    {
        auto pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(40, 255u, 255u, 255u, 1, 0, 1);
        }
        field_14_state = States::State_14;
    }
    break;

    case States::State_14:
    {
        auto pFlash = ao_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_41A810(40, 255u, 255u, 255u, 0, 0, 1);
        }
        field_14_state = States::State_15;
        field_30_timer = gnFrameCount_507670 + 5;
    }
    break;

    case States::State_15:
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            if (field_10_portal_type == PortalType::eWorker_1 || field_10_portal_type == PortalType::eShrykull_2)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            else
            {
                field_14_state = States::State_16;
            }
        }
        break;

    case States::State_17:
    {
        pScreenManager_4FF7C8->field_36_flags = pScreenManager_4FF7C8->field_36_flags & ~1 ^ 1;
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            0,
            0,
            gPsxDisplay_504C78.field_0_width,
            gPsxDisplay_504C78.field_2_height,
            pScreenManager_4FF7C8->field_2E_idx);

        field_3C_pTerminator1 = ao_new<BirdPortalTerminator>();
        if (field_3C_pTerminator1)
        {
            field_3C_pTerminator1->ctor_451F70(
                field_18_xpos,
                field_1C_ypos,
                field_34_scale,
                field_10_portal_type);
        }

        field_40_pTerminator2 = ao_new<BirdPortalTerminator>();
        if (field_40_pTerminator2)
        {
            field_40_pTerminator2->ctor_451F70(
                field_18_xpos,
                field_1C_ypos,
                field_34_scale,
                field_10_portal_type);
        }

        field_14_state = States::State_18;
        field_68_sfx_ret = SFX_Play_43AD70(67, 0, 0);
    }
    break;

    case States::State_18:
        if (field_3C_pTerminator1->field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_3C_pTerminator1->field_10_anim.Set_Animation_Data_402A40(3784, nullptr);
            field_40_pTerminator2->field_10_anim.Set_Animation_Data_402A40(3784, nullptr);
            field_14_state = States::State_19;
            field_30_timer = gnFrameCount_507670 + 12;
        }
        break;

    case States::State_19:
        field_3C_pTerminator1->field_AC_ypos -= (FP_FromDouble(3.5) * field_34_scale);
        field_40_pTerminator2->field_AC_ypos += (FP_FromDouble(3.5) * field_34_scale);
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            field_14_state = States::State_20;
        }
        break;

    case States::State_21:
        if (static_cast<int>(gnFrameCount_507670) > field_30_timer)
        {
            field_3C_pTerminator1->field_10_anim.Set_Animation_Data_402A40(3884, nullptr);
            field_40_pTerminator2->field_10_anim.Set_Animation_Data_402A40(3884, nullptr);
            field_14_state = States::State_22;
            field_30_timer = gnFrameCount_507670 + 30;

            if (field_44_pScreenClipper)
            {
                field_44_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
                field_44_pScreenClipper = nullptr;
            }

            if (field_68_sfx_ret)
            {
                SND_Stop_Channels_Mask_4774A0(field_68_sfx_ret);
                field_68_sfx_ret = 0;
            }
        }
        break;

    case States::State_22:
        if (static_cast<int>(gnFrameCount_507670) <= field_30_timer)
        {
            field_3C_pTerminator1->field_C0_r = (field_3C_pTerminator1->field_C0_r >> 1) + (field_3C_pTerminator1->field_C0_r >> 2);
            field_3C_pTerminator1->field_C2_g = (field_3C_pTerminator1->field_C2_g >> 2) + (field_3C_pTerminator1->field_C2_g >> 1);;
            field_3C_pTerminator1->field_C4_b = (field_3C_pTerminator1->field_C4_b >> 1) + (field_3C_pTerminator1->field_C4_b >> 2);

            field_40_pTerminator2->field_C0_r = (field_40_pTerminator2->field_C0_r >> 2) + (field_40_pTerminator2->field_C0_r >> 1);
            field_40_pTerminator2->field_C2_g = (field_40_pTerminator2->field_C2_g >> 1) + (field_40_pTerminator2->field_C2_g >> 2);
            field_40_pTerminator2->field_C4_b = (field_40_pTerminator2->field_C4_b >> 2) + (field_40_pTerminator2->field_C4_b >> 1);
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
        break;

    default:
        break;
    }

    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

__int16 BirdPortal::IsScaredAway_4532E0()
{
    for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObj)
        {
            return FALSE;
        }

        switch (pObj->field_4_typeId)
        {
        case Types::eElum_26:
        case Types::eAbe_43:
        case Types::eMudokon_52:
        case Types::eParamite_62:
        case Types::eScrab_77:
        case Types::eSlig_88:
            if (pObj->field_B0_path_number != field_66_path)
            {
                continue;
            }

            if (FP_Abs(pObj->field_A8_xpos - field_18_xpos) >= FP_FromInteger(75) ||
                FP_Abs(pObj->field_AC_ypos - field_28_ypos) >= FP_FromInteger(30) ||
                pObj->field_BC_sprite_scale != field_34_scale)
            {
                continue;
            }
            return TRUE;

        default:
            continue;
        }
    }
    return FALSE;
}

void BirdPortal::VGiveShrukul_4535A0(__int16 bPlaySound)
{
    if (field_10_portal_type == PortalType::eShrykull_2 && field_56_num_muds_for_shrykul <= 0)
    {
        field_14_state = States::State_6;
        field_30_timer = gnFrameCount_507670 + 12;
        field_58 = 0;

        field_60_pOrbWhirlWind = ao_new<OrbWhirlWind>();
        if (field_60_pOrbWhirlWind)
        {
            field_60_pOrbWhirlWind->ctor_48B870(
                sActiveHero_507678->field_A8_xpos,
                sActiveHero_507678->field_AC_ypos - (sActiveHero_507678->field_BC_sprite_scale * FP_FromInteger(38)),
                sActiveHero_507678->field_BC_sprite_scale);
        }

        if (sActiveHero_507678->field_FC_current_motion ==  eAbeStates::State_150_Chant_42FD50)
        {
            sActiveHero_507678->sub_430510(1);
        }
    }
    else
    {
        field_14_state = States::State_10;
        if ((field_10_portal_type == PortalType::eWorker_1 || field_10_portal_type == PortalType::eShrykull_2) &&
            sActiveHero_507678->field_FC_current_motion == eAbeStates::State_150_Chant_42FD50)
        {
            sActiveHero_507678->sub_430510(0);
        }
    }

    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::e66)
        {
            pObj->field_6_flags.Set(Options::eDead_Bit3);
        }
    }

    if (bPlaySound)
    {
        SFX_Play_43AE60(61u, 70, -1600, 0);
    }
}

void BirdPortal::VScreenChanged()
{
    VScreenChanged_4538E0();
}

void BirdPortal::VScreenChanged_4538E0()
{
    if (field_14_state <= States::State_1 || field_14_state >= States::State_21 ||
        (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path) &&
        (field_14_state != States::State_16 || field_10_portal_type != PortalType::eAbe_0 ||
            gMap_507BA8.field_A_level != field_50_dest_level ||
            gMap_507BA8.field_C_path != field_52_dest_path))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        if (field_3C_pTerminator1)
        {
            field_3C_pTerminator1->field_6_flags.Set(Options::eDead_Bit3);
            field_3C_pTerminator1 = nullptr;
        }

        if (field_40_pTerminator2)
        {
            field_40_pTerminator2->field_6_flags.Set(Options::eDead_Bit3);
            field_40_pTerminator2 = nullptr;
        }

        if (field_44_pScreenClipper)
        {
            field_44_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
            field_44_pScreenClipper = nullptr;
        }

        if (field_48_pScreenClipper)
        {
            field_48_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
            field_48_pScreenClipper = nullptr;
        }
    }
    else
    {
        if (field_68_sfx_ret)
        {
            SND_Stop_Channels_Mask_4774A0(field_68_sfx_ret);
            field_68_sfx_ret = 0;
        }
    }
}

void BirdPortal::VStopAudio()
{
    VStopAudio_4532C0();
}

void BirdPortal::VStopAudio_4532C0()
{
    if (field_68_sfx_ret)
    {
        SND_Stop_Channels_Mask_4774A0(field_68_sfx_ret);
        field_68_sfx_ret = 0;
    }
}

void BirdPortal::Vsub_453570()
{
    if (field_44_pScreenClipper)
    {
        field_44_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
        field_44_pScreenClipper = nullptr;

        field_48_pScreenClipper->field_6_flags.Set(Options::eDead_Bit3);
        field_48_pScreenClipper = nullptr;
    }
}

BOOL BirdPortal::VStateIs6_453700()
{
    return field_14_state == States::State_6;
}

BOOL BirdPortal::VStateIs16_453710()
{
    return field_14_state == States::State_16;
}

void BirdPortal::VExitPortal_453720()
{
    field_66_path = gMap_507BA8.field_2_current_path;
    field_64_level = gMap_507BA8.field_0_current_level;
    
    auto pPortalExitTlv = static_cast<Path_BirdPortalExit*>(gMap_507BA8.TLV_First_Of_Type_In_Camera_4464A0(TlvTypes::BirdPortalExit_53, 0));

    PathLine* pLine = nullptr;
    sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pPortalExitTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pPortalExitTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pPortalExitTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pPortalExitTlv->field_14_bottom_right.field_2_y),
        &pLine,
        &field_20_exit_x,
        &field_24_exit_y,
        0xFFFFFFFF);  // -1 ??

    field_1C_ypos = field_24_exit_y - FP_FromInteger(55);

    field_20_exit_x = FP_FromInteger(pPortalExitTlv->field_10_top_left.field_0_x);
    field_18_xpos = field_20_exit_x;
    field_12_side = pPortalExitTlv->field_18_side;

    if (pPortalExitTlv->field_1A_scale == 1)
    {
        field_34_scale = FP_FromDouble(0.5);
        sActiveHero_507678->field_10_anim.field_C_layer = 11;
    }
    else
    {
        field_34_scale = FP_FromInteger(1);
        sActiveHero_507678->field_10_anim.field_C_layer = 30;
    }

    sActiveHero_507678->field_BC_sprite_scale = field_34_scale;
    field_14_state = States::State_17;
    sActiveHero_507678->field_B2_lvl_number = gMap_507BA8.field_0_current_level;
    sActiveHero_507678->field_B0_path_number = gMap_507BA8.field_2_current_path;
}

BOOL BirdPortal::VStateIs20_453800()
{
    return field_14_state == States::State_20;
}

void BirdPortal::Vsub_453810()
{
    field_14_state = States::State_21;
    field_30_timer = gnFrameCount_507670 + 30;
}

void BirdPortal::VMudSaved_453830()
{
    field_56_num_muds_for_shrykul--;
}

END_NS_AO
