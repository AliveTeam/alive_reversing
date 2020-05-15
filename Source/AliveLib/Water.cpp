#include "stdafx.h"
#include "Water.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "Game.hpp"
#include "Sound/Sound.hpp"
#include "Sound/Midi.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Particle.hpp"
#include "PsxDisplay.hpp"
#include "stdlib.hpp"

Water* Water::ctor_4E02C0(Path_Water* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x547F10); // vTbl_Water_547F10
 
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kWdropResID);
    if (ppRes)
    {
        Animation_Init_424E10(104, 11, 7, ppRes, 1, 1);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit25_bDecompressDone);
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);

        Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSplashResID);

        field_114_tlvInfo = tlvInfo;
        field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
        field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

        field_104_top_left = pTlv->field_8_top_left;
        field_108_bottom_right = pTlv->field_C_bottom_right;

        field_104_top_left.field_0_x += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        field_104_top_left.field_2_y += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);
        
        field_108_bottom_right.field_0_x += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
        field_108_bottom_right.field_2_y += -FP_GetExponent(pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

        field_124_tlv_data = pTlv->field_10_data;

        // Limit upper bound
        if (field_124_tlv_data.field_10_max_drops > 128)
        {
            field_124_tlv_data.field_10_max_drops = 128;
        }

        field_130_splash_x_vel = FP_FromRaw(field_124_tlv_data.field_16_splash_x_velocity << 8);
        field_134_emit_x_vel = FP_FromRaw(field_124_tlv_data.field_16_splash_x_velocity << 8);
        field_118_radius = FP_FromInteger((field_108_bottom_right.field_0_x - field_104_top_left.field_0_x) / 2);
        field_11C_centre = FP_FromInteger(field_104_top_left.field_0_x) + field_118_radius;

        field_138_splash_time = 0;

        field_F4_ppWaterRes = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_Water, 0, field_124_tlv_data.field_10_max_drops * sizeof(Water_Res));
        if (field_F4_ppWaterRes)
        {
            field_F8_pWaterRes = reinterpret_cast<Water_Res*>(*field_F4_ppWaterRes);
            field_FC_state = static_cast<State>(pTlv->field_1_unknown);

            if (field_FC_state == State::State_2)
            {
                field_140_time_on = sGnFrame_5C1B84 + field_124_tlv_data.field_1A_timeout;
            }

            field_148_bHitTimeout &= ~1u;
            field_10C_particle_count = 0;
            field_10E_current_particle_idx = 0;

            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit13_Is8Bit))
            {
                field_FE_texture_mode = 1;
            }
            else if (field_20_animation.field_4_flags.Get(AnimFlags::eBit14_Is16Bit))
            {
                field_FE_texture_mode = 2;
            }
            else
            {
                field_FE_texture_mode = 0;
            }

            BYTE u0 = field_20_animation.field_84_vram_rect.x & 63;
            if (field_FE_texture_mode == 1)
            {
                u0 = 2 * u0;
            }
            else if (field_FE_texture_mode == 0)
            {
                u0 = 4 * u0;
            }

            const BYTE v0 = field_20_animation.field_84_vram_rect.y & 0xFF;

            const FrameHeader* pFrameHeader = reinterpret_cast<const FrameHeader*>(&(*field_20_animation.field_20_ppBlock)[field_20_animation.Get_FrameHeader_40B730(-1)->field_0_frame_header_offset]);
            field_120_frame_width = pFrameHeader->field_4_width;
            field_122_frame_height = pFrameHeader->field_5_height;

            const BYTE u1 = pFrameHeader->field_4_width + u0 - 1;
            const BYTE v1 = pFrameHeader->field_5_height + v0 - 1;

            const int tPage = PSX_getTPage_4F60E0(
                static_cast<char>(field_FE_texture_mode),
                3,
                field_20_animation.field_84_vram_rect.x,
                field_20_animation.field_84_vram_rect.y);

            for (int i = 0; i < field_124_tlv_data.field_10_max_drops; i++)
            {
                field_F8_pWaterRes[i].field_18_enabled = 0;
                // HACK/OG BUG: PC only uses first poly ??
                Poly_FT4* pPoly = field_F8_pWaterRes[i].field_20_polys;

                PolyFT4_Init_4F8870(pPoly);
                Poly_Set_SemiTrans_4F8A60(&pPoly->mBase.header, TRUE);
                Poly_Set_Blending_4F8A20(&pPoly->mBase.header, TRUE);

                const int clut = PSX_getClut_4F6350(
                    field_20_animation.field_8C_pal_vram_xy.field_0_x,
                    field_20_animation.field_8C_pal_vram_xy.field_2_y);

                SetClut(pPoly, static_cast<short>(clut));
                SetTPage(pPoly, static_cast<short>(tPage));

                SetUV0(pPoly, u0, v0);
                SetUV1(pPoly, u1, v0);
                SetUV2(pPoly, u0, v1);
                SetUV3(pPoly, u1, v1);
            }

            field_100_screen_x = FP_GetExponent(field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x);
            field_102_screen_y = FP_GetExponent(field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y);

            PSX_RECT rect = {};
            rect.y = field_20_animation.field_8C_pal_vram_xy.field_2_y;
            rect.x = field_20_animation.field_8C_pal_vram_xy.field_0_x + 1;
            rect.w = 1;
            rect.h = 1;

            // Some sort of hack to set the first 2 pixels to black/transparent?
            const BYTE zeroedData[4] = {};
            PSX_LoadImage_4F5FB0(&rect, zeroedData);

            field_144_sound_channels = 0;
        }
        else
        {
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
    else
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    return this;
}

BaseGameObject* Water::VDestructor(signed int flags)
{
    return vdtor_4E0850(flags);
}

void Water::VUpdate()
{
    vUpdate_4E0B50();
}

void Water::VRender(int** pOrderingTable)
{
    vRender_4E1440(pOrderingTable);
}

void Water::VScreenChanged()
{
    vScreenChanged_4E1780();
}

void Water::VStopAudio()
{
    vStopAudio_4E1800();
}

void Water::dtor_4E0880()
{
    SetVTable(this, 0x547F10); // vTbl_Water_547F10

    if (field_F4_ppWaterRes)
    {
        ResourceManager::FreeResource_49C330(field_F4_ppWaterRes);
    }

    if (field_144_sound_channels)
    {
        SND_Stop_Channels_Mask_4CA810(field_144_sound_channels);
        field_144_sound_channels = 0;
    }

    if (field_FC_state == State::State_1)
    {
        field_FC_state = State::State_2;
    }
    else if (field_FC_state == State::State_3)
    {
        field_FC_state = State::State_4;
    }

    if (field_148_bHitTimeout & 1)
    {
        Path::TLV_Reset_4DB8E0(field_114_tlvInfo, static_cast<short>(field_FC_state), 0, 1);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_114_tlvInfo, static_cast<short>(field_FC_state), 0, 0);
    }

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

Water* Water::vdtor_4E0850(signed int flags)
{
    dtor_4E0880();
    if (flags & 1)
    {
        alive_delete_free(this);
    }
    return this;
}

void Water::vScreenChanged_4E1780()
{
    if (field_144_sound_channels)
    {
        SND_Stop_Channels_Mask_4CA810(field_144_sound_channels);
        field_144_sound_channels = 0;
    }

    if (gMap_5C3030.sCurrentLevelId_5C3030 != gMap_5C3030.field_A_5C303A_levelId ||
        gMap_5C3030.sCurrentPathId_5C3032 != gMap_5C3030.field_C_5C303C_pathId ||
        gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

void Water::vStopAudio_4E1800()
{
    if (field_144_sound_channels)
    {
        SND_Stop_Channels_Mask_4CA810(field_144_sound_channels);
        field_144_sound_channels = 0;
    }
}

void Water::Disable_Water_Particle_4E0B10(__int16 idx)
{
    field_F8_pWaterRes[idx].field_18_enabled = 0;
    field_10C_particle_count--;
    field_10E_current_particle_idx = idx;
}

void Water::Add_Water_Particle_4E09A0()
{
    if (field_10C_particle_count != field_124_tlv_data.field_10_max_drops)
    {
        // Find an unused particle
        Water_Res* pWaterRes = nullptr;
        for (pWaterRes = &field_F8_pWaterRes[field_10E_current_particle_idx];
            field_F8_pWaterRes[field_10E_current_particle_idx].field_18_enabled;
            pWaterRes = &field_F8_pWaterRes[field_10E_current_particle_idx])
        {
            field_10E_current_particle_idx--;
            if (field_10E_current_particle_idx < 0)
            {
                // Loop back to the end
                field_10E_current_particle_idx = field_124_tlv_data.field_10_max_drops - 1;
            }
        }
        
        const BYTE rand1 = Math_NextRandom();
        const FP rand2 = FP_FromRaw(Math_NextRandom() << 8);
        const FP xRand = (rand2 * field_118_radius) + FP_FromInteger(1);
        
        pWaterRes->field_0_xpos = (Math_Sine_496DD0(rand1) * xRand) + field_11C_centre;
        pWaterRes->field_8_zpos = (Math_Cosine_496CD0(rand1) * rand2) * field_118_radius;
        pWaterRes->field_4_ypos = FP_FromInteger(field_104_top_left.field_2_y);

        pWaterRes->field_C_delta_x = field_134_emit_x_vel;
        pWaterRes->field_10_delta_y = FP_FromInteger(0);
        pWaterRes->field_14_delta_z = FP_FromInteger(0);

        pWaterRes->field_18_enabled = 1;
        pWaterRes->field_1C_state = 0;

        field_10C_particle_count++;
    }
}

void Water::vUpdate_4E0B50()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        field_13C_not_in_camera_count = 0;
    }
    else
    {
        field_13C_not_in_camera_count++;
    }

    if (field_13C_not_in_camera_count <= 90)
    {
        const CameraPos soundDir = gMap_5C3030.GetDirection_4811A0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos);

        switch (field_FC_state)
        {
        case State::WaitForEnable_0:
            field_138_splash_time = 0;
            if (!SwitchStates_Get_466020(field_124_tlv_data.field_12_id))
            {
                field_FC_state = State::State_4;
            }
            else
            {
                field_FC_state = State::State_2;
                field_140_time_on = sGnFrame_5C1B84 + field_124_tlv_data.field_1A_timeout;
                field_144_sound_channels = SFX_Play_46FC20(95u, 40, soundDir);
            }
            break;

        case State::State_1:
            if (!(sGnFrame_5C1B84 % 4))
            {
                field_110_current_drops++;
            }

            if (field_110_current_drops > 3 && !field_144_sound_channels)
            {
                field_144_sound_channels = SFX_Play_46FC20(95u, 40, soundDir);
            }

            if (field_110_current_drops < (signed __int16)(field_124_tlv_data.field_10_max_drops >> 5))
            {
                for (int i = 0; i < field_110_current_drops; i++)
                {
                    if (field_10C_particle_count == field_124_tlv_data.field_10_max_drops)
                    {
                        break;
                    }
                    Add_Water_Particle_4E09A0();
                }
            }
            else
            {
                field_FC_state = State::State_2;
                field_140_time_on = sGnFrame_5C1B84 + field_124_tlv_data.field_1A_timeout;
            }
            break;

        case State::State_2:
            field_110_current_drops = Math_NextRandom() % (field_124_tlv_data.field_10_max_drops >> 4);
            if (field_110_current_drops > 3 && !field_144_sound_channels)
            {
                field_144_sound_channels = SFX_Play_46FC20(95u, 40, soundDir);
            }

            for (int i = 0; i < field_110_current_drops; i++)
            {
                if (field_10C_particle_count == field_124_tlv_data.field_10_max_drops)
                {
                    break;
                }
                Add_Water_Particle_4E09A0();
            }

            if (!SwitchStates_Get_466020(field_124_tlv_data.field_12_id))
            {
                field_FC_state = State::State_3;
                field_110_current_drops = field_124_tlv_data.field_10_max_drops >> 5;
            }

            if (field_124_tlv_data.field_1A_timeout && static_cast<int>(sGnFrame_5C1B84) >= field_140_time_on)
            {
                field_148_bHitTimeout |= 1u;
                field_110_current_drops = field_124_tlv_data.field_10_max_drops >> 5;
                field_FC_state = State::State_3;
            }
            break;

        case State::State_3:
            if (!(sGnFrame_5C1B84 % 4))
            {
                --field_110_current_drops;
            }
            if (field_110_current_drops > 0)
            {
                for (int i = 0; i < field_110_current_drops; i++)
                {
                    if (field_10C_particle_count == field_124_tlv_data.field_10_max_drops)
                    {
                        break;
                    }
                    Add_Water_Particle_4E09A0();
                }
            }
            else
            {
                SND_Stop_Channels_Mask_4CA810(field_144_sound_channels);
                field_144_sound_channels = 0;
                SFX_Play_46FC20(0x60u, 40, soundDir);
                field_FC_state = State::State_4;
            }
            break;

        case State::State_4:
            if (field_148_bHitTimeout & 1)
            {
                if (field_124_tlv_data.field_10_max_drops <= 0)
                {
                    field_6_flags.Set(BaseGameObject::eDead_Bit3);
                }
                else
                {
                    bool found = false;
                    for (int i = 0; i < field_124_tlv_data.field_10_max_drops; i++)
                    {
                        if (!field_F8_pWaterRes[i].field_18_enabled)
                        {
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        field_6_flags.Set(BaseGameObject::eDead_Bit3);
                    }
                }
            }
            else if (SwitchStates_Get_466020(field_124_tlv_data.field_12_id))
            {
                field_110_current_drops = 0;
                field_FC_state = State::State_1;
                SFX_Play_46FC20(94u, 40, soundDir);
            }
            break;

        default:
            break;
        }

        for (short i = 0; i < field_124_tlv_data.field_10_max_drops; i++)
        {
            Water_Res* pWaterRes = &field_F8_pWaterRes[i];
            if (pWaterRes->field_18_enabled)
            {
                pWaterRes->field_0_xpos += pWaterRes->field_C_delta_x;
                pWaterRes->field_4_ypos += pWaterRes->field_10_delta_y;
                pWaterRes->field_8_zpos += pWaterRes->field_14_delta_z;
                pWaterRes->field_10_delta_y += FP_FromDouble(0.5);
                pWaterRes->field_1A_splash_time--;
                if (pWaterRes->field_1C_state)
                {
                    if (pWaterRes->field_1C_state > 0 && pWaterRes->field_1C_state <= 2)
                    {
                        // TODO refactor
                        pWaterRes->field_14_delta_z.fpValue -= pWaterRes->field_14_delta_z.fpValue >> 3;
                        pWaterRes->field_C_delta_x.fpValue -= pWaterRes->field_C_delta_x.fpValue >> 3;
                        if (!pWaterRes->field_1A_splash_time)
                        {
                            Disable_Water_Particle_4E0B10(i);
                        }
                    }
                }
                else
                {
                    pWaterRes->field_C_delta_x.fpValue -= pWaterRes->field_C_delta_x.fpValue >> 3;
                    if (FP_GetExponent(pWaterRes->field_4_ypos) > field_108_bottom_right.field_2_y)
                    {
                        // TODO: Refactor
                        if (Math_NextRandom() % 32)
                        {
                            const FP randX = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_C_delta_x = (FP_FromInteger(2) * randX) + field_130_splash_x_vel;
                            pWaterRes->field_10_delta_y = -pWaterRes->field_10_delta_y * FP_FromDouble(0.25);
                            const FP dz = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_14_delta_z = FP_FromInteger(2) * dz;
                            pWaterRes->field_4_ypos = FP_FromInteger(field_108_bottom_right.field_2_y);
                            pWaterRes->field_1C_state = 1;
                            pWaterRes->field_1A_splash_time = field_124_tlv_data.field_14_splash_time;
                        }
                        else
                        {
                            const FP randX = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_C_delta_x = (FP_FromInteger(4) * randX) + field_130_splash_x_vel;
                            pWaterRes->field_10_delta_y = -pWaterRes->field_10_delta_y * FP_FromDouble(0.5);
                            const FP dz = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_14_delta_z = FP_FromInteger(4) * dz;
                            pWaterRes->field_4_ypos = FP_FromInteger(field_108_bottom_right.field_2_y);
                            pWaterRes->field_1C_state = 2;
                            pWaterRes->field_1A_splash_time = 15;
                        }

                        short splash_time = field_138_splash_time;
                        field_138_splash_time = splash_time + 1;

                        if (!(splash_time % 4) && !field_13C_not_in_camera_count)
                        {
                            auto pParticle = alive_new<Particle>();
                            if (pParticle)
                            {
                                pParticle->ctor_4CC4C0(
                                    FP_NoFractional(pWaterRes->field_0_xpos) + pScreenManager_5BB5F4->field_20_pCamPos->field_0_x,
                                    FP_NoFractional(pWaterRes->field_4_ypos) + pScreenManager_5BB5F4->field_20_pCamPos->field_4_y + 
                                        FP_FromInteger(Math_NextRandom() % 4) - FP_FromInteger(2),
                                    332,
                                    17,
                                    7,
                                    field_10_resources_array.ItemAt(1));
                            }
                        }
                    }
                }
            }
        }
    }
}

void Water::vRender_4E1440(int** pOt)
{
    if (gMap_5C3030.Is_Point_In_Current_Camera_4810D0(
        field_C2_lvl_number,
        field_C0_path_number,
        field_B8_xpos,
        field_BC_ypos,
        0))
    {
        short xMin = 32767;
        short wMax = -32767;

        short yMin = 32767;
        short hMax = -32767;

        for (int i=0; i < field_124_tlv_data.field_10_max_drops; i++)
        {
            Water_Res* pWaterRes = &field_F8_pWaterRes[i];
            if (pWaterRes->field_18_enabled)
            {
                const short polyX = PsxToPCX(FP_GetExponent(pWaterRes->field_0_xpos));
                const short polyY = FP_GetExponent((FP_FromRaw(pWaterRes->field_8_zpos.fpValue / 2)) + pWaterRes->field_4_ypos);
                const short width = field_120_frame_width - 1;

                short height;
                if (pWaterRes->field_1C_state == 2)
                {
                    height = field_122_frame_height;
                }
                else
                {
                    int frame_height;
                    FP dy;
                    if (pWaterRes->field_10_delta_y <= FP_FromInteger(0))
                    {
                        dy = -pWaterRes->field_10_delta_y;
                        frame_height = field_122_frame_height;
                    }
                    else
                    {
                        frame_height = field_122_frame_height;
                        dy = pWaterRes->field_10_delta_y;
                    }

                    height = FP_GetExponent((FP_FromInteger(frame_height) * dy) * FP_FromDouble(0.75)) - 1;
                    if (height <= field_122_frame_height)
                    {
                        height = field_122_frame_height;
                    }
                }

                if (pWaterRes->field_1C_state == 0)
                {
                    if ((height + polyY) > field_108_bottom_right.field_2_y)
                    {
                        height = field_108_bottom_right.field_2_y - polyY;
                    }
                }

                Poly_FT4* pPoly = &pWaterRes->field_20_polys[gPsxDisplay_5C1130.field_C_buffer_index];

                // Clear out the data pointer hack
                pPoly->mVerts[1].mUv.tpage_clut_pad = 0;
                pPoly->mVerts[2].mUv.tpage_clut_pad = 0;

                SetXYWH(pPoly, polyX, polyY, width, height);
                OrderingTable_Add_4F8AA0(&pOt[39], &pPoly->mBase.header);

                if (polyX < xMin)
                {
                    xMin = polyX;
                }
                if (polyX > wMax)
                {
                    wMax = polyX;
                }
                if (polyY < yMin)
                {
                    yMin = polyY;
                }
                if (polyY > hMax)
                {
                    hMax = polyY + height;
                }
            }
        }

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            xMin - 6,
            yMin - 6,
            wMax + 6,
            hMax + 6,
            pScreenManager_5BB5F4->field_3A_idx);
    }

}
