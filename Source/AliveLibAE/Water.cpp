#include "stdafx.h"
#include "Water.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Game.hpp"
#include "../relive_lib/Sound/Midi.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Path.hpp"

Water::Water(relive::Path_Water* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::WaterDrop));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::WaterSplash));

    Animation_Init(GetAnimRes(AnimId::WaterDrop));
    // mAnim.mFlags.Set(AnimFlags::eBit25_bDecompressDone);
    GetAnimation().SetSemiTrans(false);

    field_114_tlvInfo = tlvId;
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    field_104_top_left.x = pTlv->mTopLeftX;
    field_104_top_left.y = pTlv->mTopLeftY;
    field_108_bottom_right.x = pTlv->mBottomRightX;
    field_108_bottom_right.y = pTlv->mBottomRightY;

    field_104_top_left.x += -FP_GetExponent(gScreenManager->CamXPos());
    field_104_top_left.y += -FP_GetExponent(gScreenManager->CamYPos());

    field_108_bottom_right.x += -FP_GetExponent(gScreenManager->CamXPos());
    field_108_bottom_right.y += -FP_GetExponent(gScreenManager->CamYPos());

    field_124_tlv_data = *pTlv;

    // Limit upper bound.
    if (field_124_tlv_data.mMaxDrops > 128)
    {
        field_124_tlv_data.mMaxDrops = 128;
    }

    field_130_splash_x_vel = FP_FromRaw(field_124_tlv_data.mSplashVelX << 8);
    field_134_emit_x_vel = FP_FromRaw(field_124_tlv_data.mSplashVelX << 8);
    field_118_radius = FP_FromInteger((field_108_bottom_right.x - field_104_top_left.x) / 2);
    field_11C_centre = FP_FromInteger(field_104_top_left.x) + field_118_radius;

    field_138_splash_time = 0;

    field_F8_pWaterRes = relive_new Water_Res[field_124_tlv_data.mMaxDrops];
    if (field_F8_pWaterRes)
    {
        field_FC_state = static_cast<WaterState>(pTlv->mTlvSpecificMeaning);

        if (field_FC_state == WaterState::eFlowing_2)
        {
            field_140_water_duration = sGnFrame + field_124_tlv_data.mWaterDuration;
        }

        field_148_bHitTimeout &= ~1u;
        field_10C_particle_count = 0;
        field_10E_current_particle_idx = 0;

      

        u8 u0 = 0;// mAnim.mVramRect.x & 63;
        const u8 v0 = 0; // mAnim.mVramRect.y & 0xFF;

        const PerFrameInfo* pFrameHeader = GetAnimation().Get_FrameHeader(-1);
        field_120_frame_width = static_cast<s16>(pFrameHeader->mWidth);
        field_122_frame_height = static_cast<s16>(pFrameHeader->mHeight);

        const u8 u1 = static_cast<u8>(pFrameHeader->mWidth + u0 - 1);
        const u8 v1 = static_cast<u8>(pFrameHeader->mHeight + v0 - 1);

        const s32 tPage = PSX_getTPage(TPageAbr::eBlend_3);

        for (s32 i = 0; i < field_124_tlv_data.mMaxDrops; i++)
        {
            field_F8_pWaterRes[i].field_18_enabled = 0;
            // HACK/OG BUG: PC only uses first poly ??
            Poly_FT4* pPoly = &field_F8_pWaterRes[i].field_20_polys[0];

            PolyFT4_Init(pPoly);
            Poly_Set_SemiTrans(&pPoly->mBase.header, true);
            Poly_Set_Blending(&pPoly->mBase.header, true);

            SetTPage(pPoly, static_cast<s16>(tPage));

            pPoly->mAnim = &GetAnimation();

            SetUV0(pPoly, u0, v0);
            SetUV1(pPoly, u1, v0);
            SetUV2(pPoly, u0, v1);
            SetUV3(pPoly, u1, v1);
        }

        field_100_screen_x = FP_GetExponent(mXPos - gScreenManager->CamXPos());
        field_102_screen_y = FP_GetExponent(mYPos - gScreenManager->CamYPos());

        /*
        // TODO: Move to data conversion
        PSX_RECT rect = {};
        rect.y = 0;
        rect.x = 1;
        rect.w = 1;
        rect.h = 1;

        // Some sort of hack to set the first 2 pixels to black/transparent?
        const u8 zeroedData[4] = {};
        PSX_LoadImage_4F5FB0(&rect, zeroedData); // TODO: FIX ME - won't work with other renderers
        */

        field_144_sound_channels = 0;
    }
    else
    {
        SetDead(true);
    }
}

Water::~Water()
{
    relive_delete[] field_F8_pWaterRes;

    if (field_144_sound_channels)
    {
        SND_Stop_Channels_Mask(field_144_sound_channels);
        field_144_sound_channels = 0;
    }

    if (field_FC_state == WaterState::eStarting_1)
    {
        field_FC_state = WaterState::eFlowing_2;
    }
    else if (field_FC_state == WaterState::eStopping_3)
    {
        field_FC_state = WaterState::eInactive_4;
    }

    if (field_148_bHitTimeout & 1)
    {
        Path::TLV_Reset(field_114_tlvInfo, static_cast<s16>(field_FC_state), 0, 1);
    }
    else
    {
        Path::TLV_Reset(field_114_tlvInfo, static_cast<s16>(field_FC_state), 0, 0);
    }
}

void Water::VScreenChanged()
{
    if (field_144_sound_channels)
    {
        SND_Stop_Channels_Mask(field_144_sound_channels);
        field_144_sound_channels = 0;
    }

    if (gMap.LevelChanged() || gMap.PathChanged())
    {
        SetDead(true);
    }
}

void Water::VStopAudio()
{
    if (field_144_sound_channels)
    {
        SND_Stop_Channels_Mask(field_144_sound_channels);
        field_144_sound_channels = 0;
    }
}

void Water::Disable_Water_Particle(s16 idx)
{
    field_F8_pWaterRes[idx].field_18_enabled = 0;
    field_10C_particle_count--;
    field_10E_current_particle_idx = idx;
}

void Water::Add_Water_Particle()
{
    if (field_10C_particle_count != field_124_tlv_data.mMaxDrops)
    {
        // Find an unused particle.
        Water_Res* pWaterRes = nullptr;
        for (pWaterRes = &field_F8_pWaterRes[field_10E_current_particle_idx];
             field_F8_pWaterRes[field_10E_current_particle_idx].field_18_enabled;
             pWaterRes = &field_F8_pWaterRes[field_10E_current_particle_idx])
        {
            field_10E_current_particle_idx--;
            if (field_10E_current_particle_idx < 0)
            {
                // Loop back to the end.
                field_10E_current_particle_idx = field_124_tlv_data.mMaxDrops - 1;
            }
        }

        const u8 rand1 = Math_NextRandom();
        const FP rand2 = FP_FromRaw(Math_NextRandom() << 8);
        const FP xRand = (rand2 * field_118_radius) + FP_FromInteger(1);

        pWaterRes->field_0_xpos = (Math_Sine(rand1) * xRand) + field_11C_centre;
        pWaterRes->field_8_zpos = (Math_Cosine(rand1) * rand2) * field_118_radius;
        pWaterRes->field_4_ypos = FP_FromInteger(field_104_top_left.y);

        pWaterRes->field_C_delta_x = field_134_emit_x_vel;
        pWaterRes->field_10_delta_y = FP_FromInteger(0);
        pWaterRes->field_14_delta_z = FP_FromInteger(0);

        pWaterRes->field_18_enabled = 1;
        pWaterRes->field_1C_state = 0;

        field_10C_particle_count++;
    }
}

void Water::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
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
        const CameraPos soundDir = gMap.GetDirection(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos);

        switch (field_FC_state)
        {
            case WaterState::eInit_0:
                field_138_splash_time = 0;
                if (!SwitchStates_Get(field_124_tlv_data.mSwitchId))
                {
                    field_FC_state = WaterState::eInactive_4;
                }
                else
                {
                    field_FC_state = WaterState::eFlowing_2;
                    field_140_water_duration = sGnFrame + field_124_tlv_data.mWaterDuration;
                    field_144_sound_channels = SFX_Play_Camera(relive::SoundEffects::WaterFall, 40, soundDir);
                }
                break;

            case WaterState::eStarting_1:
                if (!(sGnFrame % 4))
                {
                    field_110_current_drops++;
                }

                if (field_110_current_drops > 3 && !field_144_sound_channels)
                {
                    field_144_sound_channels = SFX_Play_Camera(relive::SoundEffects::WaterFall, 40, soundDir);
                }

                if (field_110_current_drops < (s16)(field_124_tlv_data.mMaxDrops >> 5))
                {
                    for (s32 i = 0; i < field_110_current_drops; i++)
                    {
                        if (field_10C_particle_count == field_124_tlv_data.mMaxDrops)
                        {
                            break;
                        }
                        Add_Water_Particle();
                    }
                }
                else
                {
                    field_FC_state = WaterState::eFlowing_2;
                    field_140_water_duration = sGnFrame + field_124_tlv_data.mWaterDuration;
                }
                break;

            case WaterState::eFlowing_2:
                field_110_current_drops = Math_NextRandom() % (field_124_tlv_data.mMaxDrops >> 4);
                if (field_110_current_drops > 3 && !field_144_sound_channels)
                {
                    field_144_sound_channels = SFX_Play_Camera(relive::SoundEffects::WaterFall, 40, soundDir);
                }

                for (s32 i = 0; i < field_110_current_drops; i++)
                {
                    if (field_10C_particle_count == field_124_tlv_data.mMaxDrops)
                    {
                        break;
                    }
                    Add_Water_Particle();
                }

                if (!SwitchStates_Get(field_124_tlv_data.mSwitchId))
                {
                    field_FC_state = WaterState::eStopping_3;
                    field_110_current_drops = field_124_tlv_data.mMaxDrops >> 5;
                }

                if (field_124_tlv_data.mWaterDuration && static_cast<s32>(sGnFrame) >= field_140_water_duration)
                {
                    field_148_bHitTimeout |= 1u;
                    field_110_current_drops = field_124_tlv_data.mMaxDrops >> 5;
                    field_FC_state = WaterState::eStopping_3;
                }
                break;

            case WaterState::eStopping_3:
                if (!(sGnFrame % 4))
                {
                    --field_110_current_drops;
                }
                if (field_110_current_drops > 0)
                {
                    for (s32 i = 0; i < field_110_current_drops; i++)
                    {
                        if (field_10C_particle_count == field_124_tlv_data.mMaxDrops)
                        {
                            break;
                        }
                        Add_Water_Particle();
                    }
                }
                else
                {
                    SND_Stop_Channels_Mask(field_144_sound_channels);
                    field_144_sound_channels = 0;
                    SFX_Play_Camera(relive::SoundEffects::WaterEnd, 40, soundDir);
                    field_FC_state = WaterState::eInactive_4;
                }
                break;

            case WaterState::eInactive_4:
                if (field_148_bHitTimeout & 1)
                {
                    if (field_124_tlv_data.mMaxDrops <= 0) // Someone created a water object in the map with no particles.
                    {
                        SetDead(true);
                    }
                    else
                    {
                        bool allParticlesDead = true;
                        for (s32 i = 0; i < field_124_tlv_data.mMaxDrops; i++)
                        {
                            if (field_F8_pWaterRes[i].field_18_enabled)
                            {
                                allParticlesDead = false;
                                break;
                            }
                        }

                        if (allParticlesDead)
                        {
                            SetDead(true);
                        }
                    }
                }
                else if (SwitchStates_Get(field_124_tlv_data.mSwitchId))
                {
                    field_110_current_drops = 0;
                    field_FC_state = WaterState::eStarting_1;
                    SFX_Play_Camera(relive::SoundEffects::WaterStart, 40, soundDir);
                }
                break;

            default:
                break;
        }

        for (s16 i = 0; i < field_124_tlv_data.mMaxDrops; i++)
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
                        if (pWaterRes->field_1A_splash_time == 0)
                        {
                            Disable_Water_Particle(i);
                        }
                    }
                }
                else
                {
                    pWaterRes->field_C_delta_x.fpValue -= pWaterRes->field_C_delta_x.fpValue >> 3;
                    if (FP_GetExponent(pWaterRes->field_4_ypos) > field_108_bottom_right.y)
                    {
                        // TODO: Refactor
                        if (Math_NextRandom() % 32)
                        {
                            const FP randX = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_C_delta_x = (FP_FromInteger(2) * randX) + field_130_splash_x_vel;
                            pWaterRes->field_10_delta_y = -pWaterRes->field_10_delta_y * FP_FromDouble(0.25);
                            const FP dz = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_14_delta_z = FP_FromInteger(2) * dz;
                            pWaterRes->field_4_ypos = FP_FromInteger(field_108_bottom_right.y);
                            pWaterRes->field_1C_state = 1;
                            pWaterRes->field_1A_splash_time = field_124_tlv_data.mSplashTime;
                        }
                        else
                        {
                            const FP randX = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_C_delta_x = (FP_FromInteger(4) * randX) + field_130_splash_x_vel;
                            pWaterRes->field_10_delta_y = -pWaterRes->field_10_delta_y * FP_FromDouble(0.5);
                            const FP dz = FP_FromRaw(Math_NextRandom() << 9) - FP_FromInteger(1);
                            pWaterRes->field_14_delta_z = FP_FromInteger(4) * dz;
                            pWaterRes->field_4_ypos = FP_FromInteger(field_108_bottom_right.y);
                            pWaterRes->field_1C_state = 2;
                            pWaterRes->field_1A_splash_time = 15;
                        }

                        s16 old_splash_time = field_138_splash_time;
                        field_138_splash_time = old_splash_time + 1;

                        if (!(old_splash_time % 4) && !field_13C_not_in_camera_count)
                        {
                            relive_new Particle(FP_NoFractional(pWaterRes->field_0_xpos) + gScreenManager->CamXPos(),
                                                              FP_NoFractional(pWaterRes->field_4_ypos) + gScreenManager->CamYPos() + FP_FromInteger(Math_NextRandom() % 4) - FP_FromInteger(2),
                                                               GetAnimRes(AnimId::WaterSplash));
                        }
                    }
                }
            }
        }
    }
}

void Water::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        s16 xMin = 32767;
        s16 wMax = -32767;

        s16 yMin = 32767;
        s16 hMax = -32767;

        for (s32 i = 0; i < field_124_tlv_data.mMaxDrops; i++)
        {
            Water_Res* pWaterRes = &field_F8_pWaterRes[i];
            if (pWaterRes->field_18_enabled)
            {
                const s16 polyX = PsxToPCX(FP_GetExponent(pWaterRes->field_0_xpos));
                const s16 polyY = FP_GetExponent((FP_FromRaw(pWaterRes->field_8_zpos.fpValue / 2)) + pWaterRes->field_4_ypos);
                const s16 width = field_120_frame_width - 1;

                s16 height;
                if (pWaterRes->field_1C_state == 2)
                {
                    height = field_122_frame_height;
                }
                else
                {
                    s32 frame_height;
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
                    if ((height + polyY) > field_108_bottom_right.y)
                    {
                        height = field_108_bottom_right.y - polyY;
                    }
                }

                Poly_FT4* pPoly = &pWaterRes->field_20_polys[gPsxDisplay.mBufferIndex];

                // Clear out the data pointer hack.
                pPoly->mVerts[1].mUv.tpage_clut_pad = 0;
                pPoly->mVerts[2].mUv.tpage_clut_pad = 0;

                SetXYWH(pPoly, polyX, polyY, width, height);
                OrderingTable_Add(OtLayer(ppOt, Layer::eLayer_Above_FG1_39), &pPoly->mBase.header);

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
    }
}
