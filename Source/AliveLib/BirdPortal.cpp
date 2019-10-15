#include "stdafx.h"
#include "BirdPortal.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"
#include "ObjectIds.hpp"
#include "SwitchStates.hpp"
#include "Midi.hpp"
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

BaseGameObject* BirdPortal::ctor_497E00(Path_BirdPortal* pTlv, int tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x546970);
    field_68_doves_exist = 0;
    field_4_typeId = Types::eBirdPortal_99;
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
    field_64_movie_id = pTlv->field_1A_moive_id;
    field_24_portal_type = pTlv->field_1C_portal_type;
    field_82_num_muds_for_shrykul = pTlv->field_1E_num_muds_for_shrykul;
    field_66_delete_id = pTlv->field_22_delete_id;
    
    if (pTlv->field_18_scale == 1)
    {
        field_60_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_60_scale = FP_FromInteger(1);
    }

    field_8E_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_8C_level = gMap_5C3030.sCurrentLevelId_5C3030;
    field_28_state = States::State_0;
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

    field_2C_xpos = FP_FromInteger((pTlv->field_C_bottom_right.field_0_x + pTlv->field_8_top_left.field_0_x ) / 2);
    field_90_sfx_ret = 0;
    field_30_ypos = field_3C_YPos - (FP_FromInteger(55) * field_60_scale);
    return this;
}

BaseGameObject* BirdPortal::VDestructor(signed int flags)
{
    return vdtor_498050(flags);
}

void BirdPortal::VUpdate()
{
    vUpdate_498280();
}

BirdPortal* BirdPortal::vdtor_498050(signed int flags)
{
    dtor_4980A0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
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
        if (field_28_state > States::State_4 && field_28_state < States::State_11)
        {
            field_90_sfx_ret = SFX_Play_46FC20(58, 45, direction);
        }
    }

    switch (field_28_state)
    {
    case States::State_0:
        CreateDovesAndShrykullNumber_497B50();
        field_28_state = States::State_1;
        break;

    case States::State_1:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_5C_timer)
        {
            SFX_Play_46FA90(0xDu, 35, field_60_scale);
            field_5C_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(24, 40);
        }

        if (Event_Get_422C00(kEventAbeOhm))
        {
            BaseGameObject* pShrykullNumMuds = sObjectIds_5C1B70.Find_449CF0(field_40_throwable_indicator_id);
            if (pShrykullNumMuds)
            {
                pShrykullNumMuds->field_6_flags.Set(BaseGameObject::eDead);
            }

            for (auto& id : field_44_dove_ids)
            {
                auto pDove = static_cast<Dove*>(sObjectIds_5C1B70.Find_449CF0(id));
                if (pDove)
                {
                    pDove->AsJoin_41F940(field_2C_xpos, field_30_ypos + (field_60_scale * FP_FromInteger(20)));
                }
            }

            field_28_state = States::State_2;
            field_5C_timer = sGnFrame_5C1B84 + 15;
            Event_Broadcast_422BC0(GetEvent_499A70(), this);
            SFX_Play_46FA90(0xDu, 70, field_60_scale);
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
                pThrowableIndicator->field_6_flags.Set(BaseGameObject::eDead);
            }
            SFX_Play_46FA90(13u, 70, field_60_scale);
            field_6_flags.Set(BaseGameObject::eDead);
        }
        break;
    case States::State_2:
        Event_Broadcast_422BC0(GetEvent_499A70(), this);
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            CreateTerminators_497D10();
            field_28_state = States::State_3;
            field_5C_timer = sGnFrame_5C1B84 + 5;
        }
        break;

    case States::State_3:
        Event_Broadcast_422BC0(GetEvent_499A70(), this);
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            for (auto& id : field_44_dove_ids)
            {
                BaseGameObject* pDove = sObjectIds_5C1B70.Find_449CF0(id);
                if (pDove)
                {
                    pDove->field_6_flags.Set(BaseGameObject::eDead);
                }
            }
            field_28_state = States::State_4;
        }
        break;

    case States::State_4:
        Event_Broadcast_422BC0(GetEvent_499A70(), this);
        if (pTerminator1->field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            pTerminator1->field_20_animation.Set_Animation_Data_409C80(4068, 0);
            pTerminator2->field_20_animation.Set_Animation_Data_409C80(4068, 0);
            field_5C_timer = sGnFrame_5C1B84 + 12;
            field_28_state = States::State_5;
            field_90_sfx_ret = SFX_Play_46FA90(0x3Au, 0, field_60_scale);
            SFX_Play_46FBA0(0x54u, 115, 300, field_60_scale);
        }
        break;

    case States::State_5:
        Event_Broadcast_422BC0(GetEvent_499A70(), this);
        pTerminator1->field_BC_ypos -= (FP_FromDouble(3.5) * field_60_scale);
        pTerminator2->field_BC_ypos += (FP_FromDouble(3.5) * field_60_scale);
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            field_28_state = States::State_6;
        }
        break;

    case States::State_6:
        Event_Broadcast_422BC0(GetEvent_499A70(), this);
        if (field_24_portal_type != PortalType::eWorker_1 && field_24_portal_type != PortalType::eShrykull_2 || Event_Get_422C00(kEventAbeOhm))
        {
            if ((Math_NextRandom() % 8) == 0)
            {
                BYTE** ppLightRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, ResourceID::kPortliteResID, TRUE, FALSE);
                if (ppLightRes)
                {
                    auto pParticle = alive_new<Particle>();
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
                    pParticle->field_20_animation.field_B_render_mode = 1;
                    pParticle->field_4_typeId = Types::eType_100;
                    pParticle->field_CC_sprite_scale = field_60_scale;

                    if (static_cast<int>(sGnFrame_5C1B84) % 2)
                    {
                        pParticle->field_20_animation.field_4_flags.Set(AnimFlags::eBit19_LoopBackwards);
                        pParticle->field_20_animation.SetFrame_409D50(pParticle->field_20_animation.Get_Frame_Count_40AC70());
                    }

                    if (direction == CameraPos::eCamCurrent_0)
                    {
                        SFX_Play_46FBA0(0x29u, 50, 2400, field_60_scale);
                    }
                }
                ResourceManager::FreeResource_49C330(ppLightRes);
            }
        }
        else
        {
            VGiveShrukul_499680(0); // TODO: Check correct VCall
            if (field_28_state != States::State_7)
            {
                field_28_state = States::State_22;
                field_5C_timer = sGnFrame_5C1B84 + 30;
            }
        }
        break;

    case States::State_7:
        BirdPortal::GoAwayIfType100_499220();
        if (field_84 >= 7)
        {
            field_28_state = States::State_9;
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
            const short xOffExp = FP_GetExponent(xOff);
            const short scale32 = FP_GetExponent(field_60_scale * FP_FromInteger(32));

            if (static_cast<int>(sGnFrame_5C1B84) >= field_5C_timer)
            {
                field_5C_timer = sGnFrame_5C1B84 + Math_RandomRange_496AB0(4, 12);
                auto pDove_1 = alive_new<Dove>();
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

                SFX_Play_46FA90(0xDu, 70, field_60_scale);
                pDove_1->field_CC_sprite_scale = field_60_scale;
                pDove_1->AsJoin_41F940(sActiveHero_5C1B68->field_B8_xpos, FP_FromInteger(Math_RandomRange_496AB0(-36, 4)) + sActiveHero_5C1B68->field_BC_ypos);

                if (++field_84 == 6)
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

    case States::State_9:
        if (static_cast<int>(sGnFrame_5C1B84) >= field_5C_timer)
        {
            sActiveHero_5C1B68->Get_Shrykull_Resources_45AA20();
            sActiveHero_5C1B68->field_168_ring_pulse_timer = sGnFrame_5C1B84 + 32000;
            sActiveHero_5C1B68->field_16C_bHaveShrykull = 1;
            sActiveHero_5C1B68->field_16E_bHaveInvisiblity = 0;
            field_28_state = States::State_10;
            if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_112_Chant_45B1C0)
            {
                sActiveHero_5C1B68->sub_45BB90(0);
            }
        }
        break;

    case States::State_10:
        BirdPortal::GoAwayIfType100_499220();
        pTerminator1->field_BC_ypos += (FP_FromDouble(3.5) * field_60_scale);
        pTerminator2->field_BC_ypos -= (FP_FromDouble(3.5) * field_60_scale);
        if (FP_GetExponent(pTerminator1->field_BC_ypos) >= FP_GetExponent(pTerminator2->field_BC_ypos))
        {
            BYTE** ppLightRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPortlitResID);
            if (ppLightRes)
            {
                auto pParticle = alive_new<Particle>();
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
                pParticle->field_20_animation.field_B_render_mode = 1;
                pParticle->field_DC_bApplyShadows &= ~1u;
                pParticle->field_CC_sprite_scale = field_60_scale;
            }

            field_28_state = States::State_11;
            field_5C_timer = sGnFrame_5C1B84 + 5;
            pTerminator1->field_6_flags.Set(BaseGameObject::eDead);
            pTerminator2->field_6_flags.Set(BaseGameObject::eDead);
            field_6C_terminator_id = -1;
            field_70_terminator_id = -1;
            SFX_Play_46FBA0(0x34u, 100, -1800, field_60_scale);
        }
        break;

    case States::State_11:
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            if (field_90_sfx_ret)
            {
                SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
                field_90_sfx_ret = 0;
            }
            field_28_state = States::State_12;
        }
        break;

    case States::State_12:
    {
        auto pFlash_1 = alive_new<Flash>();
        if (pFlash_1)
        {
            pFlash_1->ctor_428570(40, 255u, 255u, 255u, 1, 3u, 1);
        }
        field_28_state = States::State_13;
    }
        break;

    case States::State_13:
    {
        auto pFlash = alive_new<Flash>();
        if (pFlash)
        {
            pFlash->ctor_428570(40, 255u, 255u, 255u, 1, 0, 1);
        }
        field_28_state = States::State_14;
    }
        break;

    case States::State_14:
    {
        auto pFlash_2 = alive_new<Flash>();
        if (pFlash_2)
        {
            pFlash_2->ctor_428570(40, 255u, 255u, 255u, 0, 0, 1);
        }
        field_28_state = States::State_15;
        field_5C_timer = sGnFrame_5C1B84 + 5;
    }
        break;

    case States::State_15:
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            if (field_24_portal_type == PortalType::eWorker_1 || field_24_portal_type == PortalType::eShrykull_2)
            {
                field_6_flags.Set(BaseGameObject::eDead);
            }
            else
            {
                field_28_state = States::State_16;
            }
        }
        break;

    case States::State_17:
    {
        pScreenManager_5BB5F4->field_40_flags |= 0x10000;
        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            0,
            0,
            gPsxDisplay_5C1130.field_0_width,
            gPsxDisplay_5C1130.field_2_height,
            pScreenManager_5BB5F4->field_3A_idx);

        auto pNewTerminator1 = alive_new<BirdPortalTerminator>();
        if (pNewTerminator1)
        {
            pNewTerminator1->ctor_497960(
                field_2C_xpos,
                field_30_ypos,
                field_60_scale,
                field_24_portal_type);
        }

        auto pNewTerminator2 = alive_new<BirdPortalTerminator>();
        if (pNewTerminator2)
        {
            pNewTerminator2->ctor_497960(
                field_2C_xpos,
                field_30_ypos,
                field_60_scale,
                field_24_portal_type);
        }

        field_6C_terminator_id = pNewTerminator1->field_8_object_id;
        field_70_terminator_id = pNewTerminator2->field_8_object_id;
        field_28_state = States::State_18;
        field_90_sfx_ret = SFX_Play_46FA90(58u, 0, field_60_scale);
    }
        break;

    case States::State_18:
        if (pTerminator1->field_20_animation.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            pTerminator1->field_20_animation.Set_Animation_Data_409C80(4068, 0);
            pTerminator2->field_20_animation.Set_Animation_Data_409C80(4068, 0);
            field_28_state = States::State_19;
            field_5C_timer = sGnFrame_5C1B84 + 12;
        }
        break;

    case States::State_19:
        pTerminator1->field_BC_ypos -= (FP_FromDouble(3.5) * field_60_scale);
        pTerminator2->field_BC_ypos += (FP_FromDouble(3.5) * field_60_scale);
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            field_28_state = States::State_20;
        }
        break;

    case States::State_21:
        if (static_cast<int>(sGnFrame_5C1B84) > field_5C_timer)
        {
            pTerminator1->field_20_animation.Set_Animation_Data_409C80(4168, 0);
            pTerminator2->field_20_animation.Set_Animation_Data_409C80(4168, 0);
            field_28_state = States::State_22;
            field_5C_timer = sGnFrame_5C1B84 + 30;
            
            if (pClipper1)
            {
                pClipper1->field_6_flags.Set(BaseGameObject::eDead);
            }

            if (pClipper2)
            {
                pClipper2->field_6_flags.Set(BaseGameObject::eDead);
            }
            field_74_screen_clipper_id = -1;
            field_78_screen_clipper_id = -1;

            if (field_60_scale == FP_FromInteger(1))
            {
                sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 32;
            }
            else
            {
                sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 13;
            }
            if (field_90_sfx_ret)
            {
                SND_Stop_Channels_Mask_4CA810(field_90_sfx_ret);
                field_90_sfx_ret = 0;
            }
        }
        break;

    case States::State_22:
        if (static_cast<int>(sGnFrame_5C1B84) <= field_5C_timer)
        {
            pTerminator1->sub_497AC0();
            pTerminator2->sub_497AC0();
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
        break;

    default:
        break;
    }

    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
}


void BirdPortal::vScreenChanged_499B50()
{
    if (field_28_state <= States::State_1 || field_28_state >= States::State_21 || 
        (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
         gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId) && 

        (field_28_state != States::State_16 || field_24_portal_type != PortalType::eAbe_0 || 
            gMap_5C3030.field_A_5C303A_levelId != field_7C_dest_level || 
            gMap_5C3030.field_C_5C303C_pathId != field_7E_dest_path))
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }
    BaseGameObject* pTerminator1 = sObjectIds_5C1B70.Find_449CF0(field_6C_terminator_id);
    BaseGameObject* pTerminator2 = sObjectIds_5C1B70.Find_449CF0(field_70_terminator_id);
    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);

    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        if (pTerminator1)
        {
            pTerminator1->field_6_flags.Set(BaseGameObject::eDead);
        }

        if (pTerminator2)
        {
            pTerminator2->field_6_flags.Set(BaseGameObject::eDead);
        }

        if (pClipper1)
        {
            pClipper1->field_6_flags.Set(BaseGameObject::eDead);
        }

        if (pClipper2)
        {
            pClipper2->field_6_flags.Set(BaseGameObject::eDead);
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

signed int BirdPortal::vGetSaveState_499F50(BYTE* pBuffer)
{
    auto pState = reinterpret_cast<BirdPortal_State*>(pBuffer);
    auto pTlv = static_cast<Path_BirdPortal*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam_4DB770(field_20_tlvInfo));
    
    __int16 numMudsForShrykull = 0;
    if (pTlv)
    {
        numMudsForShrykull = pTlv->field_1E_num_muds_for_shrykul;
    }

    pState->field_0_type = Types::eBirdPortal_99;
    pState->field_4_tlvInfo = field_20_tlvInfo;
    pState->field_2_state = static_cast<BYTE>(field_28_state);
    pState->field_3_mud_count = static_cast<BYTE>(numMudsForShrykull - field_82_num_muds_for_shrykul);

    return sizeof(BirdPortal_State);
}

void BirdPortal::VRender(int** /*pOrderingTable*/)
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

signed int BirdPortal::VGetSaveState(BYTE* pSaveBuffer)
{
    return vGetSaveState_499F50(pSaveBuffer);
}

signed __int16 BirdPortal::Vsub_499430(__int16 bUnknown)
{
    return vsub_499430(bUnknown);
}

void BirdPortal::Vsub_499610()
{
    vsub_499610();
}

void BirdPortal::VMudSaved_499A50()
{
    vMudSaved_499A50();
}

BOOL BirdPortal::VStateIs6_499830()
{
    return vStateIs6_499830();
}

void BirdPortal::VGiveShrukul_499680(__int16 bPlaySound)
{
    return vGiveShrukul_499680(bPlaySound);
}

BOOL BirdPortal::VStateIs16_499850()
{
    return vStateIs16_499850();
}

void BirdPortal::VExitPortal_499870()
{
    vExitPortal_499870();
}

BOOL BirdPortal::VIsState20_499A00()
{
    return vIsState20_499A00();
}

void BirdPortal::Vsub_499A20()
{
    vsub_499A20();
}

void BirdPortal::VGetMapChange_499AE0(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId)
{
    vGetMapChange_499AE0(level, path, camera, screenChangeEffect, movieId);
}

int CC BirdPortal::CreateFromSaveState_499C90(const BYTE* pBuffer)
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

    auto pPortal = alive_new<BirdPortal>();
    pPortal->ctor_497E00(pTlv, pSaveState->field_4_tlvInfo);
    pPortal->field_1C_update_delay = 1;
    pPortal->field_82_num_muds_for_shrykul -= pSaveState->field_3_mud_count;

    const auto savedState = static_cast<States>(pSaveState->field_2_state);
    switch (savedState)
    {
    case States::State_2:
    case States::State_3:
    case States::State_4:
    case States::State_5:
    case States::State_6:
    {
        pPortal->field_28_state = States::State_6;
        pPortal->CreateTerminators_497D10();
        auto pTerminator1 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_6C_terminator_id));
        auto pTerminator2 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_70_terminator_id));
        pTerminator1->field_BC_ypos -= (FP_FromInteger(45) * pPortal->field_60_scale);
        pTerminator2->field_BC_ypos += (FP_FromInteger(45) * pPortal->field_60_scale);
        break;
    }

    case States::State_7:
    case States::State_8:
    case States::State_9:
    {
        pPortal->CreateTerminators_497D10();
        auto pTerminator1 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_6C_terminator_id));
        auto pTerminator2 = static_cast<BaseAliveGameObject*>(sObjectIds_5C1B70.Find_449CF0(pPortal->field_70_terminator_id));
        pTerminator1->field_BC_ypos -= (FP_FromInteger(45) * pPortal->field_60_scale);
        pTerminator2->field_BC_ypos += (FP_FromInteger(45) * pPortal->field_60_scale);
        pPortal->field_28_state = States::State_9;
        pPortal->field_5C_timer = sGnFrame_5C1B84 + 20;
        break;
    }
    default:
        break;
    }
    return sizeof(BirdPortal_State);
}

signed __int16 BirdPortal::vsub_499430(__int16 bUnknown)
{
    if (bUnknown && field_28_state != States::State_6)
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
    auto pClipper1 = alive_new<ScreenClipper>();
    if (pClipper1)
    {
        pClipper1->ctor_416D60(xy, wh, 0);
        field_74_screen_clipper_id = pClipper1->field_8_object_id;
        if (field_60_scale == FP_FromInteger(1))
        {
            pClipper1->field_48_ot_layer = 29;
        }
        else
        {
            pClipper1->field_48_ot_layer = 10;
        }
    }

    // Clip whole screen when "in" the portal?
    auto pClipper2 = alive_new<ScreenClipper>();
    if (pClipper2)
    {
        pClipper2->ctor_416D60(PSX_Point{ 0,0 }, PSX_Point{ 640 , 240 }, 0);
        field_78_screen_clipper_id = pClipper2->field_8_object_id;
        if (field_60_scale == FP_FromInteger(1))
        {
            pClipper2->field_48_ot_layer = 31;
        }
        else
        {
            pClipper2->field_48_ot_layer = 12;
        }
    }

    return 1;
}

void BirdPortal::vsub_499610()
{
    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);
    
    field_74_screen_clipper_id = -1;
    field_78_screen_clipper_id = -1;

    if (pClipper1)
    {
        pClipper1->field_6_flags.Set(BaseGameObject::eDead);
        if (pClipper2)
        {
            pClipper2->field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void BirdPortal::vMudSaved_499A50()
{
    field_82_num_muds_for_shrykul--;
}

BOOL BirdPortal::vStateIs6_499830()
{
    return field_28_state == States::State_6;
}

void BirdPortal::vGiveShrukul_499680(__int16 bPlaySound)
{
    if (sActiveHero_5C1B68 != spAbe_554D5C)
    {
        if (field_24_portal_type == PortalType::eShrykull_2 && field_82_num_muds_for_shrykul <= 0)
        {
            SND_SEQ_Play_4CAB10(SeqId::SecretMusic_32, 1, 127, 127);
            field_28_state = States::State_7;
            field_5C_timer = sGnFrame_5C1B84 + 12;
            field_84 = 0;

            field_88_pWhirlWind = alive_new<OrbWhirlWind>();
            if (field_88_pWhirlWind)
            {
                field_88_pWhirlWind->ctor_4E3C90(
                    sActiveHero_5C1B68->field_B8_xpos,
                    sActiveHero_5C1B68->field_BC_ypos - (sActiveHero_5C1B68->field_CC_sprite_scale * FP_FromInteger(38)),
                    sActiveHero_5C1B68->field_CC_sprite_scale,
                    0);
            }

            if (sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_112_Chant_45B1C0)
            {
                sActiveHero_5C1B68->sub_45BB90(1);
            }
        }
        else
        {
            field_28_state = States::State_10;
            if ((field_24_portal_type == PortalType::eWorker_1 || field_24_portal_type == PortalType::eShrykull_2) && sActiveHero_5C1B68->field_106_current_motion == eAbeStates::State_112_Chant_45B1C0)
            {
                sActiveHero_5C1B68->sub_45BB90(0);
            }
        }

        if (bPlaySound)
        {
            SFX_Play_46FBA0(52u, 70, -1600, field_60_scale);
        }
    }
}

BOOL BirdPortal::vStateIs16_499850()
{
    return field_28_state == States::State_16;
}

void BirdPortal::vExitPortal_499870()
{
    field_8E_path = gMap_5C3030.sCurrentPathId_5C3032;
    field_8C_level = gMap_5C3030.sCurrentLevelId_5C3030;

    auto pPortalExitTlv = static_cast<Path_BirdPortal_Exit*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::PortalExit_29, 0));
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

        if (pPortalExitTlv->field_12_scale == 1)
        {
            field_60_scale = FP_FromDouble(0.5);
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 11;
            sActiveHero_5C1B68->field_D6_scale = 0;
        }
        else
        {
            field_60_scale = FP_FromInteger(1);
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 30;
            sActiveHero_5C1B68->field_D6_scale = 1;
        }

        sActiveHero_5C1B68->field_CC_sprite_scale = field_60_scale;
        sActiveHero_5C1B68->field_C2_lvl_number = gMap_5C3030.sCurrentLevelId_5C3030;
        sActiveHero_5C1B68->field_C0_path_number = gMap_5C3030.sCurrentPathId_5C3032;

        field_28_state = States::State_17;
    }
}

BOOL BirdPortal::vIsState20_499A00()
{
    return field_28_state == States::State_20;
}

void BirdPortal::vsub_499A20()
{
    field_28_state = States::State_21;
    field_5C_timer = sGnFrame_5C1B84 + 30;
}

void BirdPortal::vGetMapChange_499AE0(LevelIds* level, WORD* path, WORD* camera, CameraSwapEffects* screenChangeEffect, WORD* movieId)
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
        pTerminator1->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pTerminator2 = sObjectIds_5C1B70.Find_449CF0(field_70_terminator_id);
    if (pTerminator2)
    {
        pTerminator2->field_6_flags.Set(BaseGameObject::eDead);;
    }

    BaseGameObject* pClipper1 = sObjectIds_5C1B70.Find_449CF0(field_74_screen_clipper_id);
    if (pClipper1)
    {
        pClipper1->field_6_flags.Set(BaseGameObject::eDead);
    }

    BaseGameObject* pClipper2 = sObjectIds_5C1B70.Find_449CF0(field_78_screen_clipper_id);
    if (pClipper2)
    {
        pClipper2->field_6_flags.Set(BaseGameObject::eDead);
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
                    pDove->field_6_flags.Set(BaseGameObject::eDead);
                }
            }
        }
    }

    BaseGameObject* pThrowableIndicator = sObjectIds_5C1B70.Find_449CF0(field_40_throwable_indicator_id);
    if (pThrowableIndicator)
    {
        pThrowableIndicator->field_6_flags.Set(BaseGameObject::eDead);
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

signed __int16 BirdPortal::IsScaredAway_4992A0()
{
    for (int i = 0; i < gBaseAliveGameObjects_5C1B7C->Size(); i++)
    {
        BaseAliveGameObject* pObj = gBaseAliveGameObjects_5C1B7C->ItemAt(i);
        if (!pObj)
        {
            return FALSE;
        }

        switch (pObj->field_4_typeId)
        {
        case Types::eType_40:
        case Types::eAbe_69:
        case Types::eMudokon2_81:
        case Types::eParamite_96:
        case Types::eScrab_112:
        case Types::eSlig_125:
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

        default: return FALSE;
        }
    }
    return FALSE;
}

void BirdPortal::CreateDovesAndShrykullNumber_497B50()
{
    for (BYTE i = 0; i < ALIVE_COUNTOF(field_44_dove_ids); i++)
    {
        auto pDove = alive_new<Dove>();
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
        auto pIndicator = alive_new<ThrowableTotalIndicator>();
        if (pIndicator)
        {
            pIndicator->ctor_431CB0(
                field_2C_xpos,
                field_30_ypos + FP_FromInteger(10),
                field_60_scale != FP_FromDouble(0.5) ? 27 : 8,
                field_60_scale,
                field_82_num_muds_for_shrykul,
                0);
            field_40_throwable_indicator_id = pIndicator->field_8_object_id;
        }
    }
}

void BirdPortal::GoAwayIfType100_499220()
{
    for (int i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eType_100)
        {
            pObj->field_6_flags.Set(BaseGameObject::eDead);
        }
    }
}

void BirdPortal::CreateTerminators_497D10()
{
    auto pTerminator1 = alive_new<BirdPortalTerminator>();
    if (pTerminator1)
    {
        pTerminator1->ctor_497960(field_2C_xpos, field_30_ypos, field_60_scale, field_24_portal_type);
        field_6C_terminator_id = pTerminator1->field_8_object_id;
    }

    auto pTerminator2 = alive_new<BirdPortalTerminator>();
    if (pTerminator2)
    {
        pTerminator2->ctor_497960(field_2C_xpos, field_30_ypos, field_60_scale, field_24_portal_type);
        field_70_terminator_id = pTerminator2->field_8_object_id;
    }
}

signed __int16 BirdPortal::GetEvent_499A70()
{
    for (int idx = 0; idx < gBaseGameObject_list_BB47C4->Size(); idx++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(idx);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_4_typeId == Types::eBirdPortal_99)
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

    field_4_typeId = Types::eEyeOrbPart_74;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kPortalTerminatorID);
    Animation_Init_424E10(4144, 32, 18, ppRes, 1, 1);

    field_20_animation.field_B_render_mode = 1;
    field_CC_sprite_scale = scale;
    if (field_CC_sprite_scale == FP_FromInteger(1))
    {
        field_20_animation.field_C_render_layer = 39;
    }
    else
    {
        field_20_animation.field_C_render_layer = 20;
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

BaseGameObject* BirdPortalTerminator::VDestructor(signed int flags)
{
    return vdtor_497A70(flags);
}

BaseAnimatedWithPhysicsGameObject* BirdPortalTerminator::vdtor_497A70(signed int flags)
{
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void BirdPortalTerminator::sub_497AC0()
{
    const __int16 r = field_D0_r;
    field_D0_r = (r >> 1) + (r >> 2);

    const __int16 g = field_D2_g;
    field_D2_g = (g >> 1) + (g >> 2);

    const __int16 b = field_D4_b;
    field_D4_b = (b >> 1) + (b >> 2);
}
