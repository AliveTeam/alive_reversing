#include "stdafx.h"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Sound/Midi.hpp"
#include "Function.hpp"
#include "BaseAliveGameObject.hpp"
#include "Dove.hpp"
#include "ThrowableTotalIndicator.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Flash.hpp"
#include "Particle.hpp"
#include "Abe.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "OrbWhirlWind.hpp"
#include "ScreenClipper.hpp"
#include "Sys_common.hpp"
#include "AbilityRing.hpp"

BaseGameObject* BirdPortal::ctor_497E00(Path_BirdPortal* pTlv, s32 tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x546970);
    field_68_doves_exist = 0;
    field_4_typeId = AETypes::eBirdPortal_99;
    field_C_objectId = tlvInfo;
    field_40_throwable_indicator_id = -1;

    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPortalTerminatorID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kDovbasicResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPortliteResID);
    Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSplineResID);

    field_20_tlvInfo = tlvInfo;

    field_26_side = pTlv->field_10_side;
    field_7C_dest_level = pTlv->field_12_dest_level;
    field_7E_dest_path = pTlv->field_14_dest_path;
    field_80_dest_camera = pTlv->field_16_dest_camera;
    field_64_movie_id = pTlv->field_1A_movie_id;
    field_24_portal_type = pTlv->field_1C_portal_type;
    field_82_num_muds_for_shrykull = pTlv->field_1E_mudokon_amount_for_shrykull;
    field_66_delete_id = pTlv->field_22_delete_id;

    if (pTlv->field_18_scale == Scale_short::eHalf_1)
    {
        field_60_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_60_scale = FP_FromInteger(1);
    }

    field_8E_path = gMap_5C3030.field_2_current_path;
    field_8C_level = gMap_5C3030.field_0_current_level;
    field_28_state = PortalStates::CreatePortal_0;
    field_5C_timer = 0;
    field_6C_terminator_id = -1;
    field_70_terminator_id = -1;
    field_74_screen_clipper_id = -1;
    field_78_screen_clipper_id = -1;
    field_88_pWhirlWind = nullptr;

    FP hitX = {};
    PathLine* pLine = nullptr;
    sCollisions_DArray_5C1128->Raycast_417A60(
        FP_FromInteger(pTlv->field_8_top_left.field_0_x),
        FP_FromInteger(pTlv->field_8_top_left.field_2_y),
        FP_FromInteger(pTlv->field_C_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_C_bottom_right.field_2_y),
        &pLine,
        &hitX,
        &field_3C_YPos,
        field_60_scale > FP_FromDouble(0.5) ? 1 : 16);

    field_2C_xpos = FP_FromInteger((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x) / 2);
    field_90_sfx_ret = 0;
    field_30_ypos = field_3C_YPos - (FP_FromInteger(55) * field_60_scale);
    return this;
}

BaseGameObject* BirdPortal::VDestructor(s32 flags)
{
    return vdtor_498050(flags);
}

void BirdPortal::VUpdate()
{
    vUpdate_498280();
}

BirdPortal* BirdPortal::vdtor_498050(s32 flags)
{
    dtor_4980A0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void BirdPortal::vUpdate_498280()
{
    const CameraPos direction = gMap_5C3030.GetDirection_4811A0(
        field_8C_level,
        field_8E_path,
        field_2C_xpos,
        field_30_ypos);

    auto pTerminator1 = static_cast<BirdPortalTerminator*>(sObjectIds_5C1B70.Find_449CF0(field_6C_terminator_id));
    auto pTerminator2 = static_cast<BirdPortalTerminator*>(sObjectIds_5C1B70.Find_449CF0(field_70_terminator_id));
    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);

    if (!field_90_sfx_ret)
    {
        if (field_28_state > PortalStates::CreateTerminators_4 && field_28_state < PortalStates::StopSound_11)
        {
            field_90_sfx_ret = SFX_Play_46FC20(SoundEffect::PortalOpening_58, 45, direction);
        }
    }

    switch (field_28_state)
    {
        case PortalStates::CreatePortal_0:
            CreateDovesAndShrykullNumber_497B50();
            field_28_state = PortalStates::IdlePortal_1;
            break;

        case PortalStates::IdlePortal_1:
            if (static_cast<s32>(sGnFrame_5C1B84) >= field_5C_timer)
            {
                SFX_Play_46FA90(SoundEffect::Dove_13, 35, field_60_scale);
                field_5C_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(24, 40);
            }

            if (Event_Get_422C00(kEventAbeOhm))
            {
                BaseGameObject* pShrykullNumMuds = sObjectIds_5C1B70.Find_449CF0(field_40_throwable_indicator_id);
                if (pShrykullNumMuds)
                {
                    pShrykullNumMuds->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }

                for (auto& id : field_44_dove_ids)
                {
                    auto pDove = static_cast<Dove*>(sObjectIds_5C1B70.Find_449CF0(id));
                    if (pDove)
                    {
                        pDove->AsJoin_41F940(field_2C_xpos, field_30_ypos + (field_60_scale * FP_FromInteger(20)));
                    }
                }

                field_28_state = PortalStates::JoinBirdsInCenter_2;
                field_5C_timer = sGnFrame_5C1B84 + 15;
                Event_Broadcast_422BC0(GetEvent_499A70(), this);
                SFX_Play_46FA90(SoundEffect::Dove_13, 70, field_60_scale);
            }
            else if (IsScaredAway_4992A0() || Event_Get_422C00(kEventShooting))
            {
                for (auto& id : field_44_dove_ids)
                {
                    Dove* pDove = static_cast<Dove*>(sObjectIds_5C1B70.Find_449CF0(id));
                    if (pDove)
                    {
                        pDove->FlyAway_420020(1);
                    }
                }

                field_68_doves_exist = 0;

                BaseGameObject* pThrowableIndicator = sObjectIds_5C1B70.Find_449CF0(field_40_throwable_indicator_id);
                if (pThrowableIndicator)
                {
                    pThrowableIndicator->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
                SFX_Play_46FA90(SoundEffect::Dove_13, 70, field_60_scale);
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        case PortalStates::JoinBirdsInCenter_2:
            Event_Broadcast_422BC0(GetEvent_499A70(), this);
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                CreateTerminators_497D10();
                field_28_state = PortalStates::KillBirds_3;
                field_5C_timer = sGnFrame_5C1B84 + 5;
            }
            break;

        case PortalStates::KillBirds_3:
            Event_Broadcast_422BC0(GetEvent_499A70(), this);
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                for (auto& id : field_44_dove_ids)
                {
                    BaseGameObject* pDove = sObjectIds_5C1B70.Find_449CF0(id);
                    if (pDove)
                    {
                        pDove->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                    }
                }
                field_28_state = PortalStates::CreateTerminators_4;
            }
            break;

        case PortalStates::CreateTerminators_4:
            Event_Broadcast_422BC0(GetEvent_499A70(), this);
            if (pTerminator1->field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                pTerminator1->field_20_animation.Set_Animation_Data_409C80(4068, 0);
                pTerminator2->field_20_animation.Set_Animation_Data_409C80(4068, 0);
                field_5C_timer = sGnFrame_5C1B84 + 12;
                field_28_state = PortalStates::ExpandTerminators_5;
                field_90_sfx_ret = SFX_Play_46FA90(SoundEffect::PortalOpening_58, 0, field_60_scale);
                SFX_Play_46FBA0(SoundEffect::IngameTransition_84, 115, 300, field_60_scale);
            }
            break;

        case PortalStates::ExpandTerminators_5:
            Event_Broadcast_422BC0(GetEvent_499A70(), this);
            pTerminator1->field_BC_ypos -= (FP_FromDouble(3.5) * field_60_scale);
            pTerminator2->field_BC_ypos += (FP_FromDouble(3.5) * field_60_scale);
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                field_28_state = PortalStates::ActivePortal_6;
            }
            break;

        case PortalStates::ActivePortal_6:
            Event_Broadcast_422BC0(GetEvent_499A70(), this);
            if ((field_24_portal_type != PortalType::eWorker_1 && field_24_portal_type != PortalType::eShrykull_2) || Event_Get_422C00(kEventAbeOhm))
            {
                if ((Math_NextRandom() % 8) == 0)
                {
                    u8** ppLightRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kPortliteResID, TRUE, FALSE);
                    if (ppLightRes)
                    {
                        auto pParticle = ae_new<Particle>();
                        if (pParticle)
                        {
                            pParticle->ctor_4CC4C0(
                                pTerminator2->field_B8_xpos,
                                (FP_FromInteger(10) * field_60_scale) + pTerminator2->field_BC_ypos,
                                4256,
                                32,
                                69,
                                ppLightRes);
                        }
                        pParticle->field_DC_bApplyShadows &= ~1u;
                        pParticle->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
                        pParticle->field_4_typeId = AETypes::eBirdPortalTerminator_100;
                        pParticle->field_CC_sprite_scale = field_60_scale;

                        if (static_cast<s32>(sGnFrame_5C1B84) % 2)
                        {
                            pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                            pParticle->field_20_animation.SetFrame_409D50(pParticle->field_20_animation.Get_Frame_Count_40AC70());
                        }

                        if (direction == CameraPos::eCamCurrent_0)
                        {
                            SFX_Play_46FBA0(SoundEffect::BirdPortalSpark_41, 50, 2400, field_60_scale);
                        }
                    }
                    ResourceManager::FreeResource_49C330(ppLightRes);
                }
                // add the ring effects from AO because why not :)
                if (!(sGnFrame_5C1B84 % 8))
                {
                    AbilityRing::Factory_482F80(
                        pTerminator1->field_B8_xpos,
                        pTerminator1->field_BC_ypos,
                        RingTypes::eShrykull_Pulse_Orange_6,
                        field_60_scale);
                    AbilityRing::Factory_482F80(
                        pTerminator2->field_B8_xpos,
                        pTerminator2->field_BC_ypos,
                        RingTypes::eShrykull_Pulse_Orange_6,
                        field_60_scale);
                }
            }
            else
            {
                VGiveShrukull_499680(0);
                if (field_28_state != PortalStates::ShrykullGetDoves_7)
                {
                    field_28_state = PortalStates::FadeoutTerminators_22;
                    field_5C_timer = sGnFrame_5C1B84 + 30;
                }
            }
            break;

        case PortalStates::ShrykullGetDoves_7:
            BirdPortal::KillTerminators_499220();
            if (field_84_received_doves >= 7)
            {
                field_28_state = PortalStates::GetShrykull_9;
                field_5C_timer = sGnFrame_5C1B84 + 20;
            }
            else
            {
                FP xOff = {};
                if (field_26_side == PortalSide::eLeft_1)
                {
                    xOff = field_60_scale * FP_FromInteger(16);
                }
                else
                {
                    xOff = field_60_scale * FP_FromInteger(-16);
                }
                const s16 xOffExp = FP_GetExponent(xOff);
                const s16 scale32 = FP_GetExponent(field_60_scale * FP_FromInteger(32));

                if (static_cast<s32>(sGnFrame_5C1B84) >= field_5C_timer)
                {
                    field_5C_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(4, 12);
                    auto pDove_1 = ae_new<Dove>();
                    if (pDove_1)
                    {
                        pDove_1->ctor_41F660(
                            5516,
                            41,
                            20u,
                            60,
                            FP_FromInteger(xOffExp) + field_2C_xpos,
                            field_30_ypos + FP_FromInteger(Math_RandomRange_496AB0(-scale32, scale32)),
                            field_60_scale);
                    }

                    SFX_Play_46FA90(SoundEffect::Dove_13, 70, field_60_scale);
                    pDove_1->field_CC_sprite_scale = field_60_scale;
                    pDove_1->AsJoin_41F940(sActiveHero_5C1B68->field_B8_xpos, FP_FromInteger(Math_RandomRange_496AB0(-36, 4)) + sActiveHero_5C1B68->field_BC_ypos);
                    field_84_received_doves++;
                    if (field_84_received_doves == 6)
                    {
                        field_88_pWhirlWind->ToSpin_4E3FD0(
                            sActiveHero_5C1B68->field_B8_xpos,
                            sActiveHero_5C1B68->field_BC_ypos - (sActiveHero_5C1B68->field_CC_sprite_scale * FP_FromInteger(38)),
                            sActiveHero_5C1B68->field_CC_sprite_scale,
                            sActiveHero_5C1B68);
                        field_88_pWhirlWind = nullptr;
                    }
                }
            }
            break;

        case PortalStates::GetShrykull_9:
            if (static_cast<s32>(sGnFrame_5C1B84) >= field_5C_timer)
            {
                sActiveHero_5C1B68->Get_Shrykull_Resources_45AA20();
                sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 32000;
                sActiveHero_5C1B68->field_16C_bHaveShrykull = TRUE;
                sActiveHero_5C1B68->field_16E_bHaveInvisiblity = 0;
                field_28_state = PortalStates::CollapseTerminators_10;
                if (sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_112_Chant_45B1C0)
                {
                    sActiveHero_5C1B68->ChangeChantState_45BB90(0);
                }
            }
            break;

        case PortalStates::CollapseTerminators_10:
            BirdPortal::KillTerminators_499220();
            pTerminator1->field_BC_ypos += (FP_FromDouble(3.5) * field_60_scale);
            pTerminator2->field_BC_ypos -= (FP_FromDouble(3.5) * field_60_scale);
            if (FP_GetExponent(pTerminator1->field_BC_ypos) >= FP_GetExponent(pTerminator2->field_BC_ypos))
            {
                u8** ppLightRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPortlitResID);
                if (ppLightRes)
                {
                    auto pParticle = ae_new<Particle>();
                    if (pParticle)
                    {
                        pParticle->ctor_4CC4C0(
                            pTerminator2->field_B8_xpos,
                            pTerminator2->field_BC_ypos,
                            13576,
                            145,
                            74,
                            ppLightRes);
                    }
                    pParticle->field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
                    pParticle->field_DC_bApplyShadows &= ~1u;
                    pParticle->field_CC_sprite_scale = field_60_scale;
                }

                field_28_state = PortalStates::StopSound_11;
                field_5C_timer = sGnFrame_5C1B84 + 5;
                pTerminator1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                pTerminator2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                field_6C_terminator_id = -1;
                field_70_terminator_id = -1;
                SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 100, -1800, field_60_scale);
            }
            break;

        case PortalStates::StopSound_11:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                if (field_90_sfx_ret)
                {
                    SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
                    field_90_sfx_ret = 0;
                }
                field_28_state = PortalStates::CreateFlash1_12;
            }
            break;

        case PortalStates::CreateFlash1_12:
        {
            auto pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, 1, TPageAbr::eBlend_3, 1);
            }
            field_28_state = PortalStates::CreateFlash2_13;
        }
        break;

        case PortalStates::CreateFlash2_13:
        {
            auto pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, 1, TPageAbr::eBlend_0, 1);
            }
            field_28_state = PortalStates::CreateFlash3_14;
        }
        break;

        case PortalStates::CreateFlash3_14:
        {
            auto pFlash = ae_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_428570(Layer::eLayer_FadeFlash_40, 255u, 255u, 255u, 0, TPageAbr::eBlend_0, 1);
            }
            field_28_state = PortalStates::KillPortal_15;
            field_5C_timer = sGnFrame_5C1B84 + 5;
        }
        break;

        case PortalStates::KillPortal_15:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                if (field_24_portal_type == PortalType::eWorker_1 || field_24_portal_type == PortalType::eShrykull_2)
                {
                    field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
                else
                {
                    field_28_state = PortalStates::AbeInsidePortal_16;
                }
            }
            break;

        case PortalStates::PortalExit_SetPosition_17:
        {
            pScreenManager_5BB5F4->field_40_flags |= 0x10000;
            pScreenManager_5BB5F4->InvalidateRect_40EC90(
                0,
                0,
                gPsxDisplay_5C1130.field_0_width,
                gPsxDisplay_5C1130.field_2_height,
                pScreenManager_5BB5F4->field_3A_idx);

            CreateTerminators_497D10();

            field_28_state = PortalStates::PortalExit_CreateTerminators_18;
            field_90_sfx_ret = SFX_Play_46FA90(SoundEffect::PortalOpening_58, 0, field_60_scale);
        }
        break;

        case PortalStates::PortalExit_CreateTerminators_18:
            if (pTerminator1->field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                pTerminator1->field_20_animation.Set_Animation_Data_409C80(4068, 0);
                pTerminator2->field_20_animation.Set_Animation_Data_409C80(4068, 0);
                field_28_state = PortalStates::PortalExit_ExpandTerminators_19;
                field_5C_timer = sGnFrame_5C1B84 + 12;
            }
            break;

        case PortalStates::PortalExit_ExpandTerminators_19:
            pTerminator1->field_BC_ypos -= (FP_FromDouble(3.5) * field_60_scale);
            pTerminator2->field_BC_ypos += (FP_FromDouble(3.5) * field_60_scale);
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                field_28_state = PortalStates::PortalExit_AbeExitting_20;
            }
            break;

        case PortalStates::KillPortalClipper_21:
            if (static_cast<s32>(sGnFrame_5C1B84) > field_5C_timer)
            {
                pTerminator1->field_20_animation.Set_Animation_Data_409C80(4168, 0);
                pTerminator2->field_20_animation.Set_Animation_Data_409C80(4168, 0);
                field_28_state = PortalStates::FadeoutTerminators_22;
                field_5C_timer = sGnFrame_5C1B84 + 30;

                if (pClipper1)
                {
                    pClipper1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }

                if (pClipper2)
                {
                    pClipper2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
                field_74_screen_clipper_id = -1;
                field_78_screen_clipper_id = -1;

                if (field_60_scale == FP_FromInteger(1))
                {
                    sActiveHero_5C1B68->field_20_animation.field_C_render_layer = Layer::eLayer_AbeMenu_32;
                }
                else
                {
                    sActiveHero_5C1B68->field_20_animation.field_C_render_layer = Layer::eLayer_AbeMenu_Half_13;
                }
                if (field_90_sfx_ret)
                {
                    SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
                    field_90_sfx_ret = 0;
                }
            }
            break;

        case PortalStates::FadeoutTerminators_22:
            if (static_cast<s32>(sGnFrame_5C1B84) <= field_5C_timer)
            {
                pTerminator1->Fadeout_497AC0();
                pTerminator2->Fadeout_497AC0();
            }
            else
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        default:
            break;
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}


void BirdPortal::vScreenChanged_499B50()
{
    if (field_28_state <= PortalStates::IdlePortal_1 || field_28_state >= PortalStates::KillPortalClipper_21 || ((gMap_5C3030.field_0_current_level != gMap_5C3030.field_A_level || gMap_5C3030.field_2_current_path != gMap_5C3030.field_C_path) &&

                                                                                                                 (field_28_state != PortalStates::AbeInsidePortal_16 || field_24_portal_type != PortalType::eAbe_0 || gMap_5C3030.field_A_level != field_7C_dest_level || gMap_5C3030.field_C_path != field_7E_dest_path)))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    BaseGameObject* pTerminator1 = sObjectIds_5C1B70.Find_449CF0(field_6C_terminator_id);
    BaseGameObject* pTerminator2 = sObjectIds_5C1B70.Find_449CF0(field_70_terminator_id);
    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);

    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        if (pTerminator1)
        {
            pTerminator1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        if (pTerminator2)
        {
            pTerminator2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        if (pClipper1)
        {
            pClipper1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        if (pClipper2)
        {
            pClipper2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }

        field_6C_terminator_id = -1;
        field_70_terminator_id = -1;
        field_74_screen_clipper_id = -1;
        field_78_screen_clipper_id = -1;
    }
    else if (field_90_sfx_ret)
    {
        SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
        field_90_sfx_ret = 0;
    }
}

void BirdPortal::vStopAudio_499260()
{
    if (field_90_sfx_ret)
    {
        SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
        field_90_sfx_ret = 0;
    }
}

s32 BirdPortal::vGetSaveState_499F50(u8* pBuffer)
{
    auto pState = reinterpret_cast<BirdPortal_State*>(pBuffer);
    auto pTlv = static_cast<Path_BirdPortal*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_20_tlvInfo));

    s16 numMudsForShrykull = 0;
    if (pTlv)
    {
        numMudsForShrykull = pTlv->field_1E_mudokon_amount_for_shrykull;
    }

    pState->field_0_type = AETypes::eBirdPortal_99;
    pState->field_4_tlvInfo = field_20_tlvInfo;
    pState->field_2_state = static_cast<u8>(field_28_state);
    pState->field_3_mud_count = static_cast<u8>(numMudsForShrykull - field_82_num_muds_for_shrykull);

    return sizeof(BirdPortal_State);
}

void BirdPortal::VRender(PrimHeader** /*ppOt*/)
{
    // Null
}

void BirdPortal::VScreenChanged()
{
    vScreenChanged_499B50();
}

void BirdPortal::VStopAudio()
{
    vStopAudio_499260();
}

s32 BirdPortal::VGetSaveState(u8* pSaveBuffer)
{
    return vGetSaveState_499F50(pSaveBuffer);
}

s16 BirdPortal::VPortalClipper_499430(s16 bUnknown)
{
    return vPortalClipper_499430(bUnknown);
}

void BirdPortal::VKillPortalClipper_499610()
{
    vKillPortalClipper_499610();
}

void BirdPortal::VMudSaved_499A50()
{
    vMudSaved_499A50();
}

BOOL BirdPortal::VActivePortal_499830()
{
    return vActivePortal_499830();
}

void BirdPortal::VGiveShrukull_499680(s16 bPlaySound)
{
    return vGiveShryukull_499680(bPlaySound);
}

BOOL BirdPortal::VAbeInsidePortal_499850()
{
    return vAbeInsidePortal_499850();
}

void BirdPortal::VExitPortal_499870()
{
    vExitPortal_499870();
}

BOOL BirdPortal::VPortalExit_AbeExitting_499A00()
{
    return vPortalExit_AbeExitting_499A00();
}

void BirdPortal::VKillPortalClipper_499A20()
{
    vIncreaseTimerAndKillPortalClipper_499A20();
}

void BirdPortal::VGetMapChange_499AE0(LevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId)
{
    vGetMapChange_499AE0(level, path, camera, screenChangeEffect, movieId);
}

s32 CC BirdPortal::CreateFromSaveState_499C90(const u8* pBuffer)
{
    auto pSaveState = reinterpret_cast<const BirdPortal_State*>(pBuffer);
    auto pTlv = static_cast<Path_BirdPortal*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(pSaveState->field_4_tlvInfo));
    if (!pTlv)
    {
        return sizeof(BirdPortal_State);
    }

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kPortliteResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("PORTAL.BND", nullptr);
    }

    if (pTlv->field_1C_portal_type == PortalType::eShrykull_2)
    {
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kSplineResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("SPLINE.BAN", nullptr);
        }
        if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kAbemorphResID, FALSE, FALSE))
        {
            ResourceManager::LoadResourceFile_49C170("SHRYPORT.BND", nullptr);
        }
    }

    auto pPortal = ae_new<BirdPortal>();
    pPortal->ctor_497E00(pTlv, pSaveState->field_4_tlvInfo);
    pPortal->field_1C_update_delay = 1;
    pPortal->field_82_num_muds_for_shrykull -= pSaveState->field_3_mud_count;

    const auto savedState = static_cast<PortalStates>(pSaveState->field_2_state);
    switch (savedState)
    {
        case PortalStates::JoinBirdsInCenter_2:
        case PortalStates::KillBirds_3:
        case PortalStates::CreateTerminators_4:
        case PortalStates::ExpandTerminators_5:
        case PortalStates::ActivePortal_6:
        {
            pPortal->field_28_state = PortalStates::ActivePortal_6;
            pPortal->CreateTerminators_497D10();
            auto pTerminator1 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_6C_terminator_id));
            auto pTerminator2 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_70_terminator_id));
            pTerminator1->field_BC_ypos -= (FP_FromInteger(45) * pPortal->field_60_scale);
            pTerminator2->field_BC_ypos += (FP_FromInteger(45) * pPortal->field_60_scale);
            break;
        }

        case PortalStates::Unused_8:
        {
            ALIVE_FATAL("PortalStates::Unused_8 never expected to be used");
            break;
        }

        case PortalStates::ShrykullGetDoves_7:
        case PortalStates::GetShrykull_9:
        {
            pPortal->CreateTerminators_497D10();
            auto pTerminator1 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_6C_terminator_id));
            auto pTerminator2 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_70_terminator_id));
            pTerminator1->field_BC_ypos -= (FP_FromInteger(45) * pPortal->field_60_scale);
            pTerminator2->field_BC_ypos += (FP_FromInteger(45) * pPortal->field_60_scale);
            pPortal->field_28_state = PortalStates::GetShrykull_9;
            pPortal->field_5C_timer = sGnFrame_5C1B84 + 20;
            break;
        }
        default:
            break;
    }
    return sizeof(BirdPortal_State);
}

s16 BirdPortal::vPortalClipper_499430(s16 bIgnoreClipping)
{
    if (bIgnoreClipping && field_28_state != PortalStates::ActivePortal_6)
    {
        return 0;
    }

    if (field_74_screen_clipper_id != -1)
    {
        return 1;
    }

    PSX_Point xy = {};
    PSX_Point wh = {};
    if (field_26_side == PortalSide::eLeft_1)
    {
        xy.field_0_x = 0;
        xy.field_2_y = 0;

        wh.field_0_x = PsxToPCX(FP_GetExponent(field_2C_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x), 11);
        wh.field_2_y = 240;
    }
    else
    {
        xy.field_0_x = PsxToPCX(FP_GetExponent(field_2C_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x), 11);
        xy.field_2_y = 0;

        wh.field_0_x = 640;
        wh.field_2_y = 240;
    }

    // Clip objects entering portal?
    auto pClipper1 = ae_new<ScreenClipper>();
    if (pClipper1)
    {
        pClipper1->ctor_416D60(xy, wh, Layer::eLayer_0);
        field_74_screen_clipper_id = pClipper1->field_8_object_id;
        if (field_60_scale == FP_FromInteger(1))
        {
            pClipper1->field_48_ot_layer = Layer::eLayer_BirdPortal_29;
        }
        else
        {
            pClipper1->field_48_ot_layer = Layer::eLayer_BirdPortal_Half_10;
        }
    }

    // Clip whole screen when "in" the portal?
    auto pClipper2 = ae_new<ScreenClipper>();
    if (pClipper2)
    {
        pClipper2->ctor_416D60(PSX_Point{0, 0}, PSX_Point{640, 240}, Layer::eLayer_0);
        field_78_screen_clipper_id = pClipper2->field_8_object_id;
        if (field_60_scale == FP_FromInteger(1))
        {
            pClipper2->field_48_ot_layer = Layer::eLayer_FallingItemPortalClip_31;
        }
        else
        {
            pClipper2->field_48_ot_layer = Layer::eLayer_FallingItemPortalClip_Half_12;
        }
    }

    return 1;
}

void BirdPortal::vKillPortalClipper_499610()
{
    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);

    field_74_screen_clipper_id = -1;
    field_78_screen_clipper_id = -1;

    if (pClipper1)
    {
        pClipper1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        if (pClipper2)
        {
            pClipper2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void BirdPortal::vMudSaved_499A50()
{
    field_82_num_muds_for_shrykull--;
}

BOOL BirdPortal::vActivePortal_499830()
{
    return field_28_state == PortalStates::ActivePortal_6;
}

void BirdPortal::vGiveShryukull_499680(s16 bPlaySound)
{
    if (sActiveHero_5C1B68 != spAbe_554D5C)
    {
        if (field_24_portal_type == PortalType::eShrykull_2 && field_82_num_muds_for_shrykull <= 0)
        {
            SND_SEQ_Play_4CAB10(SeqId::SecretMusic_32, 1, 127, 127);
            field_28_state = PortalStates::ShrykullGetDoves_7;
            field_5C_timer = sGnFrame_5C1B84 + 12;
            field_84_received_doves = 0;

            field_88_pWhirlWind = ae_new<OrbWhirlWind>();
            if (field_88_pWhirlWind)
            {
                field_88_pWhirlWind->ctor_4E3C90(
                    sActiveHero_5C1B68->field_B8_xpos,
                    sActiveHero_5C1B68->field_BC_ypos - (sActiveHero_5C1B68->field_CC_sprite_scale * FP_FromInteger(38)),
                    sActiveHero_5C1B68->field_CC_sprite_scale,
                    0);
            }

            if (sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_112_Chant_45B1C0)
            {
                sActiveHero_5C1B68->ChangeChantState_45BB90(1);
            }
        }
        else
        {
            field_28_state = PortalStates::CollapseTerminators_10;
            if ((field_24_portal_type == PortalType::eWorker_1 || field_24_portal_type == PortalType::eShrykull_2) && sActiveHero_5C1B68->field_106_current_motion == eAbeMotions::Motion_112_Chant_45B1C0)
            {
                sActiveHero_5C1B68->ChangeChantState_45BB90(0);
            }
        }

        if (bPlaySound)
        {
            SFX_Play_46FBA0(SoundEffect::MenuNavigation_52, 70, -1600, field_60_scale);
        }
    }
}

BOOL BirdPortal::vAbeInsidePortal_499850()
{
    return field_28_state == PortalStates::AbeInsidePortal_16;
}

void BirdPortal::vExitPortal_499870()
{
    field_8E_path = gMap_5C3030.field_2_current_path;
    field_8C_level = gMap_5C3030.field_0_current_level;

    auto pPortalExitTlv = static_cast<Path_BirdPortalExit*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::BirdPortalExit_29, 0));
    if (pPortalExitTlv)
    {
        PathLine* pLine = nullptr;
        sCollisions_DArray_5C1128->Raycast_417A60(
            FP_FromInteger(pPortalExitTlv->field_8_top_left.field_0_x),
            FP_FromInteger(pPortalExitTlv->field_8_top_left.field_2_y),
            FP_FromInteger(pPortalExitTlv->field_C_bottom_right.field_0_x),
            FP_FromInteger(pPortalExitTlv->field_C_bottom_right.field_2_y),
            &pLine,
            &field_34_exit_x,
            &field_38_exit_y,
            0xFFFFFFFF); // -1 ??

        field_34_exit_x = FP_FromInteger((pPortalExitTlv->field_8_top_left.field_0_x + pPortalExitTlv->field_C_bottom_right.field_0_x) / 2);
        field_2C_xpos = field_34_exit_x;
        field_30_ypos = field_38_exit_y - FP_FromInteger(55);
        field_26_side = pPortalExitTlv->field_10_side;

        if (pPortalExitTlv->field_12_scale == Scale_short::eHalf_1)
        {
            field_60_scale = FP_FromDouble(0.5);
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = Layer::eLayer_InBirdPortal_Half_11;
            sActiveHero_5C1B68->field_D6_scale = 0;
        }
        else
        {
            field_60_scale = FP_FromInteger(1);
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = Layer::eLayer_InBirdPortal_30;
            sActiveHero_5C1B68->field_D6_scale = 1;
        }

        sActiveHero_5C1B68->field_CC_sprite_scale = field_60_scale;
        sActiveHero_5C1B68->field_C2_lvl_number = gMap_5C3030.field_0_current_level;
        sActiveHero_5C1B68->field_C0_path_number = gMap_5C3030.field_2_current_path;

        field_28_state = PortalStates::PortalExit_SetPosition_17;
    }
}

BOOL BirdPortal::vPortalExit_AbeExitting_499A00()
{
    return field_28_state == PortalStates::PortalExit_AbeExitting_20;
}

void BirdPortal::vIncreaseTimerAndKillPortalClipper_499A20()
{
    field_28_state = PortalStates::KillPortalClipper_21;
    field_5C_timer = sGnFrame_5C1B84 + 30;
}

void BirdPortal::vGetMapChange_499AE0(LevelIds* level, u16* path, u16* camera, CameraSwapEffects* screenChangeEffect, u16* movieId)
{
    *level = field_7C_dest_level;
    *path = field_7E_dest_path;
    *camera = field_80_dest_camera;

    if (field_64_movie_id <= 0)
    {
        *screenChangeEffect = CameraSwapEffects::eEffect0_InstantChange;
    }
    else
    {
        *movieId = field_64_movie_id;
        *screenChangeEffect = CameraSwapEffects::eEffect5_1_FMV;
    }
}

void BirdPortal::dtor_4980A0()
{
    SetVTable(this, 0x546970);
    BaseGameObject* pTerminator1 = sObjectIds_5C1B70.Find_449CF0(field_6C_terminator_id);
    if (pTerminator1)
    {
        pTerminator1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    BaseGameObject* pTerminator2 = sObjectIds_5C1B70.Find_449CF0(field_70_terminator_id);
    if (pTerminator2)
    {
        pTerminator2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    if (pClipper1)
    {
        pClipper1->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);
    if (pClipper2)
    {
        pClipper2->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    BaseGameObject* pDoves = sObjectIds_5C1B70.Find_449CF0(field_44_dove_ids[0]);
    if (pDoves)
    {
        if (field_68_doves_exist)
        {
            for (auto doveId : field_44_dove_ids)
            {
                BaseGameObject* pDove = sObjectIds_5C1B70.Find_449CF0(doveId);
                if (pDove)
                {
                    pDove->field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
            }
        }
    }

    BaseGameObject* pThrowableIndicator = sObjectIds_5C1B70.Find_449CF0(field_40_throwable_indicator_id);
    if (pThrowableIndicator)
    {
        pThrowableIndicator->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_90_sfx_ret)
    {
        SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
        field_90_sfx_ret = 0;
    }

    if (SwitchStates_Get_466020(field_66_delete_id))
    {
        // Never come back
        Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 1);
    }
    else
    {
        // Always come back
        Path::TLV_Reset_4DB8E0(field_20_tlvInfo, -1, 0, 0);
    }

    BaseGameObject_dtor_4DBEC0();
}

s16 BirdPortal::IsScaredAway_4992A0()
{
    for (s32 i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            return FALSE;
        }

        switch (pObj->field_4_typeId)
        {
            case AETypes::eNeverSet_40:
            case AETypes::eAbe_69:
            case AETypes::eMudokon2_81:
            case AETypes::eParamite_96:
            case AETypes::eScrab_112:
            case AETypes::eSlig_125:
                if (pObj->field_C0_path_number != field_8E_path)
                {
                    continue;
                }

                if (FP_Abs(pObj->field_B8_xpos - field_2C_xpos) >= FP_NoFractional((field_60_scale * FP_FromInteger(75))))
                {
                    continue;
                }

                if (FP_Abs(pObj->field_BC_ypos - field_3C_YPos) >= FP_FromInteger(30) || pObj->field_CC_sprite_scale != field_60_scale)
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

void BirdPortal::CreateDovesAndShrykullNumber_497B50()
{
    for (u8 i = 0; i < ALIVE_COUNTOF(field_44_dove_ids); i++)
    {
        auto pDove = ae_new<Dove>();
        if (pDove)
        {
            pDove->ctor_41F660(5516, 41, 20u, 60, field_2C_xpos, field_30_ypos, field_60_scale);
        }
        field_44_dove_ids[i] = pDove->field_8_object_id;

        field_68_doves_exist = 1;

        if (field_24_portal_type == PortalType::eAbe_0)
        {
            pDove->AsAlmostACircle_41FA20(field_2C_xpos, (field_60_scale * FP_FromInteger(30)) + field_30_ypos, 42 * i);
        }
        else
        {
            pDove->AsACircle_41F980(field_2C_xpos, (field_60_scale * FP_FromInteger(30)) + field_30_ypos, 42 * i);
        }
        pDove->field_CC_sprite_scale = field_60_scale;
    }

    if (field_24_portal_type == PortalType::eShrykull_2)
    {
        auto pIndicator = ae_new<ThrowableTotalIndicator>();
        if (pIndicator)
        {
            Layer indicatorLayer = field_60_scale != FP_FromDouble(0.5) ? Layer::eLayer_27 : Layer::eLayer_8;

            pIndicator->ctor_431CB0(
                field_2C_xpos,
                field_30_ypos + FP_FromInteger(10),
                indicatorLayer,
                field_60_scale,
                field_82_num_muds_for_shrykull,
                0);
            field_40_throwable_indicator_id = pIndicator->field_8_object_id;
        }
    }
}

void BirdPortal::KillTerminators_499220()
{
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == AETypes::eBirdPortalTerminator_100)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void BirdPortal::CreateTerminators_497D10()
{
    auto pTerminator1 = ae_new<BirdPortalTerminator>();
    if (pTerminator1)
    {
        pTerminator1->ctor_497960(field_2C_xpos, field_30_ypos, field_60_scale, field_24_portal_type);
        field_6C_terminator_id = pTerminator1->field_8_object_id;
    }

    auto pTerminator2 = ae_new<BirdPortalTerminator>();
    if (pTerminator2)
    {
        pTerminator2->ctor_497960(field_2C_xpos, field_30_ypos, field_60_scale, field_24_portal_type);
        field_70_terminator_id = pTerminator2->field_8_object_id;
    }
}

s16 BirdPortal::GetEvent_499A70()
{
    for (s32 idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == AETypes::eBirdPortal_99)
        {
            if (pObj == this)
            {
                return kEventPortalOpen;
            }
            else
            {
                return kEventUnknown20;
            }
        }
    }
    return kEventPortalOpen;
}

BaseAnimatedWithPhysicsGameObject* BirdPortalTerminator::ctor_497960(FP xpos, FP ypos, FP scale, PortalType /*portalType*/)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x546928);

    field_4_typeId = AETypes::eEyeOrbPart_74;

    const AnimRecord& rec = AnimRec(AnimId::Bird_Portal_Orb);
    u8** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_20_animation.field_B_render_mode = TPageAbr::eBlend_1;
    field_CC_sprite_scale = scale;
    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_Above_FG1_39;
    }
    else
    {
        field_20_animation.field_C_render_layer = Layer::eLayer_Above_FG1_Half_20;
    }

    field_DC_bApplyShadows &= ~1u;

    field_BC_ypos = ypos;
    field_B8_xpos = xpos;

    field_D0_r = 255;
    field_D2_g = 128;
    field_D4_b = 64;

    return this;
}

void BirdPortalTerminator::VScreenChanged()
{
    // Staying alive
}

BaseGameObject* BirdPortalTerminator::VDestructor(s32 flags)
{
    return vdtor_497A70(flags);
}

BaseAnimatedWithPhysicsGameObject* BirdPortalTerminator::vdtor_497A70(s32 flags)
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void BirdPortalTerminator::Fadeout_497AC0()
{
    const s16 r = field_D0_r;
    field_D0_r = (r >> 1) + (r >> 2);

    const s16 g = field_D2_g;
    field_D2_g = (g >> 1) + (g >> 2);

    const s16 b = field_D4_b;
    field_D4_b = (b >> 1) + (b >> 2);
}
