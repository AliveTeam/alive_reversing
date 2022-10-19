#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "LvlArchive.hpp"
#include "Sound/Midi.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "stdlib.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"
#include "Text.hpp"
#include "Sfx.hpp"
#include "FG1.hpp"
#include "CameraSwapper.hpp"
#include "MainMenu.hpp"
#include "Events.hpp"
#include "Movie.hpp"
#include "Particle.hpp"
#include "Door.hpp"
#include "Sound/PsxSpuApi.hpp"
#include "Sys.hpp"
#include <assert.h>

void Map_ForceLink()
{ }

ALIVE_VAR(1, 0x5c311c, s16, sMap_bDoPurpleLightEffect_5C311C, 0);
ALIVE_VAR(1, 0x5c3118, Camera*, sCameraBeingLoaded_5C3118, nullptr);
ALIVE_VAR(1, 0x5c3120, u32, sSoundChannelsMask_5C3120, 0);


// Map Path_ChangeTLV::field_18_wipe to CameraSwapEffects
const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_55D55C[10] = {
    CameraSwapEffects::ePlay1FMV_5,
    CameraSwapEffects::eRightToLeft_2,
    CameraSwapEffects::eLeftToRight_1,
    CameraSwapEffects::eBottomToTop_4,
    CameraSwapEffects::eTopToBottom_3,
    CameraSwapEffects::eBoxOut_8,
    CameraSwapEffects::eVerticalSplit_6,
    CameraSwapEffects::eHorizontalSplit_7,
    CameraSwapEffects::eUnknown_11,
    CameraSwapEffects::eInstantChange_0};

EXPORT void CC static_map_construct_4802F0()
{
    gMap_5C3030.Reset_4805D0();
}

EXPORT void CC static_map_destruct_480330()
{
    gMap_5C3030.Shutdown_4804E0();
}

EXPORT void CC static_map_init_4802D0()
{
    static_map_construct_4802F0();
    atexit(static_map_destruct_480330);
}

void Map::ScreenChange_Common()
{
    if (field_6_state == CamChangeStates::eSliceCam_1)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        Handle_PathTransition_481610();
    }
    else if (field_6_state == CamChangeStates::eInstantChange_2)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        GoTo_Camera_481890();
    }

    field_6_state = CamChangeStates::eInactive_0;

    SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
    sSoundChannelsMask_5C3120 = 0;
}

void Map::ScreenChange_480B80()
{
    if (field_6_state == CamChangeStates::eInactive_0)
    {
        return;
    }

    if (sMap_bDoPurpleLightEffect_5C311C)
    {
        RemoveObjectsWithPurpleLight_480740(1);
    }

    PSX_DrawSync_4F6280(0);

    for (s32 i = 0; i < 2; i++) // Not sure why this is done twice?
    {
        for (s32 j = 0; j < gBaseGameObject_list_BB47C4->Size(); j++)
        {
            BaseGameObject* pItem = gBaseGameObject_list_BB47C4->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();

            // Did the screen change kill the object?
            if (pItem->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                j = gBaseGameObject_list_BB47C4->RemoveAt(j);
                pItem->VDestructor(1);
            }
        }
    }

    ResourceManager::NoEffect_49C700();

    //dword_5CA4A8 = 0; // TODO: Never used?

    // TODO: Refactor this logic
    if (!sMap_bDoPurpleLightEffect_5C311C && field_A_level == field_0_current_level)
    {
        ScreenChange_Common();
        return;
    }

    if (field_A_level != field_0_current_level)
    {
        SsUtAllKeyOff_4FDFE0(0);
    }

    if (field_A_level != LevelIds::eNone)
    {
        if (field_A_level == LevelIds::eCredits_16)
        {
            sSoundChannelsMask_5C3120 = 0;
            ScreenChange_Common();
            return;
        }
    }
    else if (field_0_current_level == LevelIds::eMenu_0)
    {
        sSoundChannelsMask_5C3120 = 0;
        ScreenChange_Common();
        return;
    }
    sSoundChannelsMask_5C3120 = SND_4CA5D0(0, 0, 36, 70, 0, 0);
    ScreenChange_Common();
}

// OG only allows for 30 paths, given the editor allows for the full 99 we have to use a bigger array in a non ABI breaking way
// Note that currently there is only ever 1 Map object instance thus this global workaround is OK for now.
struct Map_PathsArrayExtended final
{
    u8** field_0_pPathRecs[99];
};
static Map_PathsArrayExtended sPathsArrayExtended = {};

void Map::FreePathResourceBlocks()
{
    for (s32 i = 0; i < Path_Get_Num_Paths(field_0_current_level); ++i)
    {
        if (sPathsArrayExtended.field_0_pPathRecs[i])
        {
            ResourceManager::FreeResource_49C330(sPathsArrayExtended.field_0_pPathRecs[i]);
            sPathsArrayExtended.field_0_pPathRecs[i] = nullptr;
        }

        if (i < ALIVE_COUNTOF(field_54_path_res_array.field_0_pPathRecs))
        {
            if (field_54_path_res_array.field_0_pPathRecs[i])
            {
                field_54_path_res_array.field_0_pPathRecs[i] = nullptr;
            }
        }
    }
}

void Map::GetPathResourceBlockPtrs()
{
    // Get pointer to each PATH
    for (s32 i = 1; i < Path_Get_Num_Paths(field_A_level); ++i)
    {
        sPathsArrayExtended.field_0_pPathRecs[i] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, TRUE, FALSE);

        if (i < ALIVE_COUNTOF(field_54_path_res_array.field_0_pPathRecs))
        {
            field_54_path_res_array.field_0_pPathRecs[i] = sPathsArrayExtended.field_0_pPathRecs[i];
        }
    }
}

u8** Map::GetPathResourceBlockPtr(u32 pathId)
{
    if (pathId < ALIVE_COUNTOF(field_54_path_res_array.field_0_pPathRecs))
    {
        return field_54_path_res_array.field_0_pPathRecs[pathId];
    }
    return sPathsArrayExtended.field_0_pPathRecs[pathId];
}

void Map::ClearPathResourceBlocks()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_54_path_res_array.field_0_pPathRecs); i++)
    {
        field_54_path_res_array.field_0_pPathRecs[i] = nullptr;
    }

    for (s32 i = 0; i < ALIVE_COUNTOF(sPathsArrayExtended.field_0_pPathRecs); i++)
    {
        sPathsArrayExtended.field_0_pPathRecs[i] = nullptr;
    }
}

void Map::RemoveObjectsWithPurpleLight_480740(s16 bMakeInvisible)
{
    auto pObjectsWithLightsArray = ae_new<DynamicArrayT<BaseAnimatedWithPhysicsGameObject>>();
    pObjectsWithLightsArray->ctor_40CA60(16);

    auto pPurpleLightArray = ae_new<DynamicArrayT<Particle>>();
    pPurpleLightArray->ctor_40CA60(16);

    bool bAddedALight = false;
    for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_BB47C4->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAnimatedWithPhysicsObj_Bit5))
        {
            if (pObj->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
            {
                auto pBaseObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);

                PSX_RECT objRect = {};
                pBaseObj->vGetBoundingRect_424FD0(&objRect, 1);

                if (pBaseObj->field_DC_bApplyShadows & 2)
                {
                    if (pBaseObj->field_20_animation.field_4_flags.Get(AnimFlags::eBit3_Render))
                    {
                        if (!pBaseObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pBaseObj != sControlledCharacter_5C1B8C && gMap_5C3030.Rect_Location_Relative_To_Active_Camera_480FE0(&objRect) == CameraPos::eCamCurrent_0)
                        {
                            pObjectsWithLightsArray->Push_Back(pBaseObj);
                            const FP k60Scaled = (pBaseObj->field_CC_sprite_scale * FP_FromInteger(60));
                            Particle* pPurpleLight = New_DestroyOrCreateObject_Particle_426F40(
                                FP_FromInteger((objRect.x + objRect.w) / 2),
                                FP_FromInteger(((objRect.y + objRect.h) / 2)) + k60Scaled,
                                pBaseObj->field_CC_sprite_scale);

                            if (pPurpleLight)
                            {
                                pPurpleLightArray->Push_Back(pPurpleLight);
                                bAddedALight = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (bAddedALight)
    {
        SFX_Play_46FBA0(SoundEffect::PossessEffect_17, 40, 2400);

        for (s32 counter = 0; counter < 12; counter++)
        {
            if (bMakeInvisible && counter == 4)
            {
                // Make all the objects that have lights invisible now that the lights have been rendered for a few frames
                for (s32 i = 0; i < pObjectsWithLightsArray->Size(); i++)
                {
                    BaseAnimatedWithPhysicsGameObject* pObj = pObjectsWithLightsArray->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }
                    pObj->field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
                }
            }

            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    pLight->VUpdate();
                }
            }

            // TODO/HACK what is the point of the f64 loop? Why not do both in 1 iteration ??
            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    pLight->field_20_animation.vDecode_40AC90();
                }
            }

            for (s32 i = 0; i < gObjList_drawables_5C1124->Size(); i++)
            {
                BaseGameObject* pDrawable = gObjList_drawables_5C1124->ItemAt(i);
                if (!pDrawable)
                {
                    break;
                }

                if (!pDrawable->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    // TODO: Seems strange to check this flag, how did it get in the drawable list if its not a drawable ??
                    if (pDrawable->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
                    {
                        pDrawable->VRender(gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);
                    }
                }
            }

            PSX_DrawSync_4F6280(0);
            pScreenManager_5BB5F4->VRender(gPsxDisplay_5C1130.field_10_drawEnv[gPsxDisplay_5C1130.field_C_buffer_index].field_70_ot_buffer);
            SYS_EventsPump_494580();
            gPsxDisplay_5C1130.PSX_Display_Render_OT_41DDF0();
        }

        if (bMakeInvisible)
        {
            // Make all the objects that had lights visible again
            for (s32 i = 0; i < pObjectsWithLightsArray->Size(); i++)
            {
                BaseAnimatedWithPhysicsGameObject* pObj = pObjectsWithLightsArray->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                pObj->field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
        }
    }

    pObjectsWithLightsArray->field_4_used_size = 0;
    pPurpleLightArray->field_4_used_size = 0;

    if (pObjectsWithLightsArray)
    {
        pObjectsWithLightsArray->dtor_40CAD0();
        ae_delete_free_495540(pObjectsWithLightsArray);
    }

    if (pPurpleLightArray)
    {
        pPurpleLightArray->dtor_40CAD0();
        ae_delete_free_495540(pPurpleLightArray);
    }
}

void Map::Handle_PathTransition_481610()
{
    Path_Change* pPathChangeTLV = nullptr;
    if (field_18_pAliveObj)
    {
        pPathChangeTLV = static_cast<Path_Change*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_18_pAliveObj->field_B8_xpos),
            FP_GetExponent(field_18_pAliveObj->field_BC_ypos),
            FP_GetExponent(field_18_pAliveObj->field_B8_xpos),
            FP_GetExponent(field_18_pAliveObj->field_BC_ypos),
            TlvTypes::PathTransition_1));
    }

    if (field_18_pAliveObj && pPathChangeTLV)
    {
        field_A_level = pPathChangeTLV->field_10_level;
        field_C_path = pPathChangeTLV->field_12_path;
        field_E_camera = pPathChangeTLV->field_14_camera;
        field_12_fmv_base_id = pPathChangeTLV->field_16_movie;

        field_10_screen_change_effect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[pPathChangeTLV->field_18_wipe];

        field_18_pAliveObj->field_C2_lvl_number = field_A_level;
        field_18_pAliveObj->field_C0_path_number = field_C_path;
        GoTo_Camera_481890();

        switch (pPathChangeTLV->field_1A_scale)
        {
            case Scale_short::eFull_0:
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(1.0);
                sActiveHero_5C1B68->field_20_animation.field_C_render_layer = Layer::eLayer_AbeMenu_32;
                break;

            case Scale_short::eHalf_1:
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(0.5);
                sActiveHero_5C1B68->field_20_animation.field_C_render_layer = Layer::eLayer_AbeMenu_Half_13;
                break;

            default:
                LOG_ERROR("Invalid scale " << (s32) pPathChangeTLV->field_1A_scale);
                break;
        }

        CameraPos remapped = CameraPos::eCamInvalid_m1;
        switch (field_14_direction)
        {
            case MapDirections::eMapLeft_0:
                remapped = CameraPos::eCamLeft_3;
                break;
            case MapDirections::eMapRight_1:
                remapped = CameraPos::eCamRight_4;
                break;
            case MapDirections::eMapTop_2:
                remapped = CameraPos::eCamTop_1;
                break;
            case MapDirections::eMapBottom_3:
                remapped = CameraPos::eCamBottom_2;
                break;
        }

        field_18_pAliveObj->VOnPathTransition_408320(
            field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width,
            field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height,
            remapped);
    }
    else
    {
        switch (field_14_direction)
        {
            case MapDirections::eMapLeft_0:
                field_D0_cam_x_idx--;
                field_10_screen_change_effect = CameraSwapEffects::eRightToLeft_2;
                break;
            case MapDirections::eMapRight_1:
                field_D0_cam_x_idx++;
                field_10_screen_change_effect = CameraSwapEffects::eLeftToRight_1;
                break;
            case MapDirections::eMapTop_2:
                field_D2_cam_y_idx--;
                field_10_screen_change_effect = CameraSwapEffects::eBottomToTop_4;
                break;
            case MapDirections::eMapBottom_3:
                field_D2_cam_y_idx++;
                field_10_screen_change_effect = CameraSwapEffects::eTopToBottom_3;
                break;
            default:
                break;
        }

        const u32 pCamNameOffset = sizeof(CameraName) * (field_D0_cam_x_idx + (field_D2_cam_y_idx * sPath_dword_BB47C0->field_6_cams_on_x));
        const u8* pPathRes = *GetPathResourceBlockPtr(field_2_current_path);
        auto pCameraName = reinterpret_cast<const CameraName*>(pPathRes + pCamNameOffset);

        // Convert the 2 digit camera number string to an integer
        field_E_camera = 1 * (pCameraName->name[7] - '0') + 10 * (pCameraName->name[6] - '0');

        GoTo_Camera_481890();
    }
}

CameraPos Map::GetDirection_4811A0(s32 level, s32 path, FP xpos, FP ypos)
{
    if (level != static_cast<s32>(field_0_current_level))
    {
        return CameraPos::eCamInvalid_m1;
    }

    if (path != field_2_current_path)
    {
        return CameraPos::eCamInvalid_m1;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);

    CameraPos ret = Rect_Location_Relative_To_Active_Camera_480FE0(&rect);

    PSX_RECT camWorldRect = {};
    if (!Get_Camera_World_Rect_481410(ret, &camWorldRect))
    {
        return CameraPos::eCamInvalid_m1;
    }

    const FP x = FP_FromInteger(camWorldRect.x);
    const FP y = FP_FromInteger(camWorldRect.y);
    const FP w = FP_FromInteger(camWorldRect.w);
    const FP h = FP_FromInteger(camWorldRect.h);

    switch (ret)
    {
        case CameraPos::eCamCurrent_0:
            return ret;

        case CameraPos::eCamTop_1:
            if (ypos < y || xpos < x || xpos > w)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return ypos > h ? CameraPos::eCamCurrent_0 : ret;

        case CameraPos::eCamBottom_2:
            if (ypos > h || xpos < x || xpos > w)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return ypos < y ? CameraPos::eCamCurrent_0 : ret;

        case CameraPos::eCamLeft_3:
            if (xpos < x || ypos < y || ypos > h)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return xpos > w ? CameraPos::eCamCurrent_0 : ret;

        case CameraPos::eCamRight_4:
            if (xpos > w || ypos < y || ypos > h)
            {
                return CameraPos::eCamInvalid_m1;
            }
            return xpos < x ? CameraPos::eCamCurrent_0 : ret;

        default:
            return CameraPos::eCamInvalid_m1;
    }
}

void Map::Init_4803F0(LevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    sPath_dword_BB47C0 = ae_new<Path>();
    sPath_dword_BB47C0->ctor_4DB170();

    field_2C_camera_array[0] = nullptr;
    field_2C_camera_array[1] = nullptr;
    field_2C_camera_array[2] = nullptr;
    field_2C_camera_array[3] = nullptr;
    field_2C_camera_array[4] = nullptr;

    field_22_overlayID = -1;

    field_4_current_camera = static_cast<s16>(-1);
    field_2_current_path = static_cast<s16>(-1);
    field_0_current_level = LevelIds::eNone;

    field_8_force_load = 0;

    SetActiveCam_480D30(level, path, camera, screenChangeEffect, fmvBaseId, forceChange);
    GoTo_Camera_481890();

    field_6_state = CamChangeStates::eInactive_0;
}

void Map::Shutdown_4804E0()
{
    sLvlArchive_5BC520.Free_433130();
    stru_5C3110.Free_433130();

    // Free Path resources
    FreePathResourceBlocks();

    // Free cameras
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        if (field_2C_camera_array[i])
        {
            field_2C_camera_array[i]->dtor_480E00();
            ae_delete_free_495540(field_2C_camera_array[i]);
            field_2C_camera_array[i] = nullptr;
        }
    }

    pScreenManager_5BB5F4 = nullptr;

    // Free path
    if (sPath_dword_BB47C0)
    {
        sPath_dword_BB47C0->dtor_4DB1A0();
        ae_delete_free_495540(sPath_dword_BB47C0);
    }
    sPath_dword_BB47C0 = nullptr;

    ResourceManager::Reclaim_Memory_49C470(0);
    Reset_4805D0();
}

void Map::Reset_4805D0()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        field_2C_camera_array[i] = nullptr;
    }

    ClearPathResourceBlocks();

    field_CC_unused = 1;
    field_CE_free_all_anim_and_palts = 0;
    field_D8_restore_quick_save = 0;
}

void Map::GoTo_Camera_481890()
{
    s16 bShowLoadingIcon = FALSE;
    if (field_0_current_level != LevelIds::eMenu_0 && field_0_current_level != LevelIds::eCredits_16 && field_0_current_level != LevelIds::eNone)
    {
        bShowLoadingIcon = TRUE;
    }

    if (field_10_screen_change_effect == CameraSwapEffects::eUnknown_11)
    {
        BaseGameObject* pFmvRet = FMV_Camera_Change_482650(nullptr, this, field_0_current_level);
        for (s32 i = 0; i < gBaseGameObject_list_BB47C4->Size(); i++)
        {
            SYS_EventsPump_494580();

            BaseGameObject* pBaseGameObj = gBaseGameObject_list_BB47C4->ItemAt(i);
            if (!pBaseGameObj)
            {
                break;
            }

            if (pBaseGameObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pBaseGameObj->field_6_flags.Get(BaseGameObject::eCantKill_Bit11) == false)
            {
                i = gBaseGameObject_list_BB47C4->RemoveAt(i);
                pBaseGameObj->VDestructor(1);
                if (pBaseGameObj == pFmvRet)
                {
                    // FMV trans done
                    break;
                }
            }
            else if (pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
            {
                if (!(pBaseGameObj->field_6_flags.Get(BaseGameObject::eDead_Bit3)) && (!sNum_CamSwappers_5C1B66 || pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                {
                    const s32 updateDelay = pBaseGameObj->UpdateDelay();
                    if (updateDelay > 0)
                    {
                        pBaseGameObj->SetUpdateDelay(updateDelay - 1);
                    }
                    else
                    {
                        pBaseGameObj->VUpdate();
                    }
                }
            }
        }

        if (sSoundChannelsMask_5C3120)
        {
            SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
        }
        sSoundChannelsMask_5C3120 = SND_4CA5D0(0, 0, 36, 70, 0, 0);
    }

    if (field_0_current_level != LevelIds::eMenu_0 && field_0_current_level != LevelIds::eNone)
    {
        if (field_A_level != field_0_current_level
            || field_8_force_load
            || (field_C_path != field_2_current_path && field_10_screen_change_effect == CameraSwapEffects::ePlay1FMV_5))
        {
            Game_ShowLoadingIcon_482D80();
        }
    }

    if (field_A_level != field_0_current_level
        || field_C_path != field_2_current_path
        || field_8_force_load)
    {
        field_22_overlayID = GetOverlayId_480710();
    }

    if (field_A_level != field_0_current_level || field_8_force_load)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);

        // Free all cameras
        for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
        {
            if (field_2C_camera_array[i])
            {
                field_2C_camera_array[i]->dtor_480E00();
                ae_delete_free_495540(field_2C_camera_array[i]);
                field_2C_camera_array[i] = nullptr;
            }
        }

        if (field_0_current_level != LevelIds::eNone)
        {
            // Close LVL archives
            sLvlArchive_5BC520.Free_433130();
            stru_5C3110.Free_433130();

            // Free all but the first ?
            FreePathResourceBlocks();

            sPath_dword_BB47C0->Free_4DB1C0();

            if (field_A_level != field_0_current_level)
            {
                SND_Reset_4C9FB0();
            }

            ResourceManager::Reclaim_Memory_49C470(0);
        }

        pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);

        // Open LVL
        while (!sLvlArchive_5BC520.Open_Archive_432E80(CdLvlName(field_A_level)))
        {
            if (gAttract_5C1BA0)
            {
                // NOTE: Dead branch? Given no attract directory exists
                char_type fileName[256] = {};
                strcpy(fileName, "ATTRACT");
                strcat(fileName, CdLvlName(field_A_level));
                if (sLvlArchive_5BC520.Open_Archive_432E80(fileName))
                {
                    break;
                }
            }
            Display_Full_Screen_Message_Blocking_465820(Path_Get_Unknown(field_A_level), MessageType::eLongTitle_0);
        }

        // Open Path BND
        ResourceManager::LoadResourceFile_49C170(Path_Get_BndName(field_A_level), nullptr);

        // Get pointer to each PATH
        GetPathResourceBlockPtrs();

        if (field_A_level == field_0_current_level)
        {
            MusicController::PlayMusic_47FD60(MusicController::MusicTypes::eNone_0, sActiveHero_5C1B68, 0, 0);
        }
        else
        {
            SND_Load_VABS_4CA350(Path_Get_MusicInfo(field_A_level), Path_Get_Reverb(field_A_level));
            SND_Load_Seqs_4CAED0(sSeqData_558D50.mSeqs, Path_Get_BsqFileName(field_A_level));

            auto pBackgroundMusic = ae_new<BackgroundMusic>();
            if (pBackgroundMusic)
            {
                pBackgroundMusic->ctor_4CB110(Path_Get_BackGroundMusicId(field_A_level));
            }
        }

        if (!field_8_force_load)
        {
            SwitchStates_SetRange_465FA0(2, 255);
        }

        if (field_CE_free_all_anim_and_palts)
        {
            ResourceManager::Free_Resource_Of_Type_49C6B0(ResourceManager::Resource_Animation);
            ResourceManager::Free_Resource_Of_Type_49C6B0(ResourceManager::Resource_Palt);
            field_CE_free_all_anim_and_palts = FALSE;
        }
    }

    if (!field_C_path)
    {
        field_C_path = 1;
    }

    const s16 prevPathId = field_2_current_path;
    const LevelIds prevLevelId = field_0_current_level;

    field_2_current_path = field_C_path;
    field_0_current_level = field_A_level;
    field_4_current_camera = field_E_camera;

    const PathBlyRec* pPathRec_1 = Path_Get_Bly_Record_460F30(field_A_level, field_C_path);
    field_D4_ptr = pPathRec_1->field_4_pPathData;

    sPath_dword_BB47C0->Init_4DB200(
        field_D4_ptr,
        field_A_level,
        field_C_path,
        field_E_camera,
        GetPathResourceBlockPtr(field_C_path));

    if (sQuickSave_saved_switchResetters_count_BB234C > 0)
    {
        Quicksave_RestoreSwitchResetterStates_4C9A30();
    }

    char_type pStrBuffer[13] = {};
    Path_Format_CameraName_460FB0(pStrBuffer, field_A_level, field_C_path, field_E_camera);

    u32 pCamNameOffset = 0;
    if (sizeof(CameraName) * sPath_dword_BB47C0->field_6_cams_on_x * sPath_dword_BB47C0->field_8_cams_on_y > 0)
    {
        for (;;)
        {
            u8* pPathRes = *GetPathResourceBlockPtr(field_C_path);
            CameraName* pCameraNameIter = reinterpret_cast<CameraName*>(pPathRes + pCamNameOffset);

            if (strncmp(pCameraNameIter->name, pStrBuffer, sizeof(CameraName)) == 0)
            {
                // Matched
                break;
            }

            pCamNameOffset += sizeof(CameraName);
            if (pCamNameOffset >= sizeof(CameraName) * sPath_dword_BB47C0->field_6_cams_on_x * sPath_dword_BB47C0->field_8_cams_on_y)
            {
                // Out of bounds
                break;
            }
        }
    }

    field_D0_cam_x_idx = static_cast<s16>((pCamNameOffset / sizeof(CameraName)) % sPath_dword_BB47C0->field_6_cams_on_x);
    field_D2_cam_y_idx = static_cast<s16>((pCamNameOffset / sizeof(CameraName)) / sPath_dword_BB47C0->field_6_cams_on_x);
    field_24_camera_offset.field_0_x = FP_FromInteger(field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width);
    field_24_camera_offset.field_4_y = FP_FromInteger(field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height);

    // If map has changed then load new collision info
    if (prevPathId != field_2_current_path || prevLevelId != field_0_current_level)
    {
        if (sCollisions_DArray_5C1128)
        {
            sCollisions_DArray_5C1128->dtor_4189F0();
            ae_delete_free_495540(sCollisions_DArray_5C1128);
        }

        sCollisions_DArray_5C1128 = ae_new<Collisions>();
        if (sCollisions_DArray_5C1128)
        {
            sCollisions_DArray_5C1128->ctor_418930(pPathRec_1->field_8_pCollisionData, *GetPathResourceBlockPtr(field_2_current_path));
        }
    }

    if (field_D8_restore_quick_save)
    {
        QuikSave_RestoreBlyData_D481890_4C9BE0(field_D8_restore_quick_save);
        field_D8_restore_quick_save = nullptr;
    }

    // Copy camera array and blank out the source
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        field_40_stru_5[i] = field_2C_camera_array[i];
        field_2C_camera_array[i] = nullptr;
    }

    field_2C_camera_array[0] = Create_Camera_4829E0(field_D0_cam_x_idx, field_D2_cam_y_idx, 1);
    field_2C_camera_array[3] = Create_Camera_4829E0(field_D0_cam_x_idx - 1, field_D2_cam_y_idx, 0);
    field_2C_camera_array[4] = Create_Camera_4829E0(field_D0_cam_x_idx + 1, field_D2_cam_y_idx, 0);
    field_2C_camera_array[1] = Create_Camera_4829E0(field_D0_cam_x_idx, field_D2_cam_y_idx - 1, 0);
    field_2C_camera_array[2] = Create_Camera_4829E0(field_D0_cam_x_idx, field_D2_cam_y_idx + 1, 0);

    // Free resources for each camera
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i])
        {
            pResourceManager_5C1BB0->Free_Resources_For_Camera_4656F0(field_40_stru_5[i]);
        }
    }

    pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);

    // Free each camera itself
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i])
        {
            field_40_stru_5[i]->dtor_480E00();
            ae_delete_free_495540(field_40_stru_5[i]);
            field_40_stru_5[i] = nullptr;
        }
    }

    Map::Load_Path_Items_482C10(field_2C_camera_array[0], LoadMode::ConstructObject_0);
    pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);
    Map::Load_Path_Items_482C10(field_2C_camera_array[3], LoadMode::ConstructObject_0);
    Map::Load_Path_Items_482C10(field_2C_camera_array[4], LoadMode::ConstructObject_0);
    Map::Load_Path_Items_482C10(field_2C_camera_array[1], LoadMode::ConstructObject_0);
    Map::Load_Path_Items_482C10(field_2C_camera_array[2], LoadMode::ConstructObject_0);

    // Create the screen manager if it hasn't already been done (probably should have always been done by this point though?)
    if (!pScreenManager_5BB5F4)
    {
        pScreenManager_5BB5F4 = ae_new<ScreenManager>();
        if (pScreenManager_5BB5F4)
        {
            pScreenManager_5BB5F4->ctor_40E3E0(field_2C_camera_array[0]->field_C_pCamRes, &field_24_camera_offset);
        }
    }

    sPath_dword_BB47C0->Loader_4DB800(field_D0_cam_x_idx, field_D2_cam_y_idx, LoadMode::ConstructObject_0, TlvTypes::None_m1); // none = load all
    if (prevPathId != field_2_current_path || prevLevelId != field_0_current_level)
    {
        if (sActiveHero_5C1B68)
        {
            if (field_2_current_path == sActiveHero_5C1B68->field_C0_path_number)
            {
                sActiveHero_5C1B68->VCheckCollisionLineStillValid_408A40(10);
            }
        }
    }

    Create_FG1s_480F10();

    if (field_10_screen_change_effect == CameraSwapEffects::ePlay1FMV_5)
    {
        Map::FMV_Camera_Change_482650(field_2C_camera_array[0]->field_C_pCamRes, this, field_A_level);
    }

    if (field_10_screen_change_effect == CameraSwapEffects::eUnknown_11)
    {
        pScreenManager_5BB5F4->DecompressCameraToVRam_40EF60(reinterpret_cast<u16**>(field_2C_camera_array[0]->field_C_pCamRes));
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
        pScreenManager_5BB5F4->MoveImage_40EB70();
        pScreenManager_5BB5F4->field_40_flags |= 0x10000;
    }

    if (prevLevelId != field_0_current_level)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    if (field_10_screen_change_effect != CameraSwapEffects::ePlay1FMV_5 && field_10_screen_change_effect != CameraSwapEffects::eUnknown_11)
    {
        if (field_1E_door)
        {
            // TODO: Add template helpers

            // Door transition
            Path_Door* pDoorTlv = static_cast<Path_Door*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::Door_5, 0));
            while (pDoorTlv->field_18_door_number != sActiveHero_5C1B68->field_1A0_door_id)
            {
                pDoorTlv = static_cast<Path_Door*>(Path::TLV_Next_Of_Type_4DB720(pDoorTlv, TlvTypes::Door_5));
            }

            CreateScreenTransistionForTLV(pDoorTlv);
        }
        else
        {
            if (!field_20)
            {
                auto obj = ae_new<CameraSwapper>();
                if (obj)
                {
                    obj->ctor_4E5000(field_2C_camera_array[0]->field_C_pCamRes, field_10_screen_change_effect, 368 / 2, 240 / 2);
                }
            }
            else
            {
                // TODO: Add template helpers

                // Teleporter transition
                Path_Teleporter* pTeleporterTlv = static_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::Teleporter_88, 0));
                Path_Teleporter_Data teleporterData = pTeleporterTlv->field_10_data;
                while (teleporterData.field_10_teleporter_switch_id != sActiveHero_5C1B68->field_1A0_door_id)
                {
                    pTeleporterTlv = static_cast<Path_Teleporter*>(Path::TLV_Next_Of_Type_4DB720(pTeleporterTlv, TlvTypes::Teleporter_88));
                    teleporterData = pTeleporterTlv->field_10_data;
                }

                CreateScreenTransistionForTLV(pTeleporterTlv);
            }
        }
    }

    bHideLoadingIcon_5C1BAA = 0;
    loading_ticks_5C1BAC = 0;

    field_8_force_load = 0;

    if (sSoundChannelsMask_5C3120)
    {
        SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
        sSoundChannelsMask_5C3120 = 0;
    }
}

Camera* Map::GetCamera(CameraPos pos)
{
    return field_2C_camera_array[static_cast<s32>(pos)];
}

void Map::CreateScreenTransistionForTLV(Path_TLV* pTlv)
{
    auto obj = ae_new<CameraSwapper>();
    if (obj)
    {
        // TODO: Refactor
        const FP_Point* pCamPos2 = pScreenManager_5BB5F4->field_20_pCamPos;
        const s16 doorYDiff = static_cast<s16>(pTlv->field_8_top_left.field_2_y - FP_GetExponent(pCamPos2->field_4_y));
        FP camX = pCamPos2->field_0_x;
        const s16 midX = (pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2;

        const s16 rightPos = static_cast<s16>(midX - FP_GetExponent(camX));
        const s16 xpos2 = rightPos;
        obj->ctor_4E5000(field_2C_camera_array[0]->field_C_pCamRes, field_10_screen_change_effect, xpos2, doorYDiff);
    }
}

void Map::Get_map_size_480640(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_ptr->field_4_bTop;
    pPoint->field_2_y = field_D4_ptr->field_6_bBottom;
}

void Map::GetCurrentCamCoords_480680(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width;
    pPoint->field_2_y = field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height;
}

void Map::Get_Abe_Spawn_Pos_4806D0(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_ptr->field_1A_abe_start_xpos;
    pPoint->field_2_y = field_D4_ptr->field_1C_abe_start_ypos;
}

s16 Map::GetOverlayId_480710()
{
    // TODO: Probably need to redo field_C data as 1 bytes instead of a word
    return Path_Get_Bly_Record_460F30(field_A_level, field_C_path)->field_C_overlay_id & 0xFF;
}

void Map::Create_FG1s_480F10()
{
    pScreenManager_5BB5F4->UnsetDirtyBits_FG1_40ED70();

    Camera* pCamera = field_2C_camera_array[0];
    for (s32 i = 0; i < pCamera->field_0.Size(); i++)
    {
        u8** ppRes = pCamera->field_0.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        if (*ppRes)
        {
            ResourceManager::Header* pHeader = ResourceManager::Get_Header_49C410(ppRes);
            if (pHeader->field_8_type == ResourceManager::Resource_FG1)
            {
                FG1* pFG1 = ae_new<FG1>();
                pFG1->ctor_499FC0(ppRes);
            }
        }
    }
}

s16 Map::Get_Camera_World_Rect_481410(CameraPos camIdx, PSX_RECT* pRect)
{
    if (camIdx < CameraPos::eCamCurrent_0 || camIdx > CameraPos::eCamRight_4)
    {
        return 0;
    }

    Camera* pCamera = field_2C_camera_array[static_cast<s32>(camIdx)];
    if (!pCamera)
    {
        return 0;
    }

    if (!pRect)
    {
        return 1;
    }

    const s16 xpos = pCamera->field_14_xpos * field_D4_ptr->field_A_grid_width;
    const s16 ypos = pCamera->field_16_ypos * field_D4_ptr->field_C_grid_height;

    pRect->x = xpos;
    pRect->y = ypos;
    pRect->w = xpos + 368;
    pRect->h = ypos + 240;
    return 1;
}

s16 Map::Is_Point_In_Current_Camera_4810D0(s32 level, s32 path, FP xpos, FP ypos, s16 width)
{
    const FP calculated_width = (width != 0) ? FP_FromInteger(6) : FP_FromInteger(0);
    if (static_cast<LevelIds>(level) != field_0_current_level || path != field_2_current_path) // TODO: Remove when 100%
    {
        return FALSE;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos - calculated_width);
    rect.w = FP_GetExponent(calculated_width + xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);
    return Rect_Location_Relative_To_Active_Camera_480FE0(&rect) == CameraPos::eCamCurrent_0;
}


EXPORT CameraPos Map::Rect_Location_Relative_To_Active_Camera_480FE0(PSX_RECT* pRect)
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        return CameraPos::eCamNone_5;
    }

    const s32 camX = FP_GetExponent(field_24_camera_offset.field_0_x);
    const s32 camY = FP_GetExponent(field_24_camera_offset.field_4_y);

    if (pRect->x > (camX + 368))
    {
        return CameraPos::eCamRight_4;
    }

    if (pRect->y > (camY + 240))
    {
        return CameraPos::eCamBottom_2;
    }

    if (pRect->w >= camX)
    {
        if (pRect->h < camY)
        {
            return CameraPos::eCamTop_1;
        }
        else
        {
            return CameraPos::eCamCurrent_0;
        }
    }

    return CameraPos::eCamLeft_3;
}

s16 Map::SetActiveCam_480D30(LevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    if (!forceChange && cam == field_4_current_camera && level == field_0_current_level && path == field_2_current_path)
    {
        return 0;
    }

    field_E_camera = cam;
    field_12_fmv_base_id = fmvBaseId;
    field_C_path = path;
    field_A_level = level;
    field_10_screen_change_effect = screenChangeEffect;

    field_6_state = CamChangeStates::eInstantChange_2;

    if (screenChangeEffect == CameraSwapEffects::ePlay1FMV_5 || screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        sMap_bDoPurpleLightEffect_5C311C = 1;
    }
    else
    {
        sMap_bDoPurpleLightEffect_5C311C = 0;
    }

    return 1;
}

BaseGameObject* CC Map::FMV_Camera_Change_482650(u8** ppBits, Map* pMap, LevelIds lvlId)
{
    if (pMap->field_12_fmv_base_id > 10000u)
    {
        // Trippe FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id / 10000);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id / 100 % 100);
        FmvInfo* pFmvRec3 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id % 100);
        sLevelId_dword_5CA408 = static_cast<s32>(lvlId); // TODO: Strongly type this, but it hasn't got the same underlaying type as the enum grr..
        u32 pos1 = 0;
        u32 pos2 = 0;
        u32 pos3 = 0;
        Get_fmvs_sectors_494460(
            pFmvRec1->field_0_pName,
            pFmvRec2->field_0_pName,
            pFmvRec3->field_0_pName,
            &pos1,
            &pos2,
            &pos3);

        auto pSwapper = ae_new<CameraSwapper>();
        return pSwapper->ctor_4E4ED0(
            ppBits,
            pos1,
            pFmvRec1->field_4_id,
            pos2,
            pFmvRec2->field_4_id,
            pos3,
            pFmvRec3->field_4_id,

            pFmvRec1->field_6_flags & 1,
            pFmvRec1->field_8_flags,
            pFmvRec1->field_A_volume,

            pFmvRec2->field_6_flags & 1,
            pFmvRec2->field_8_flags,
            pFmvRec2->field_A_volume,

            pFmvRec3->field_6_flags & 1,
            pFmvRec3->field_8_flags,
            pFmvRec3->field_A_volume);
    }
    else if (pMap->field_12_fmv_base_id >= 100u)
    {
        // Double FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id / 100);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id % 100);
        u32 cdPos1 = 0;
        u32 cdPos2 = 0;
        Get_fmvs_sectors_494460(pFmvRec1->field_0_pName, pFmvRec2->field_0_pName, 0, &cdPos1, &cdPos2, 0);
        sLevelId_dword_5CA408 = static_cast<s32>(lvlId); // HACK
        auto pSwapper = ae_new<CameraSwapper>();
        return pSwapper->ctor_4E4DC0(
            ppBits,
            cdPos1,
            pFmvRec1->field_4_id,
            cdPos2,
            pFmvRec2->field_4_id,

            pFmvRec1->field_6_flags & 1,
            pFmvRec1->field_8_flags,
            pFmvRec1->field_A_volume,

            pFmvRec2->field_6_flags & 1,
            pFmvRec2->field_8_flags,
            pFmvRec2->field_A_volume);
    }
    else // < 100
    {
        // Single FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id);
        u32 cdPos = 0;
        Get_fmvs_sectors_494460(pFmvRec1->field_0_pName, 0, 0, &cdPos, 0, 0);
        sLevelId_dword_5CA408 = static_cast<s32>(lvlId); // HACK
        auto pSwapper = ae_new<CameraSwapper>();
        return pSwapper->ctor_4E4CA0(
            ppBits,
            cdPos,
            pFmvRec1->field_4_id,
            pFmvRec1->field_6_flags & 1,
            pFmvRec1->field_8_flags,
            pFmvRec1->field_A_volume);
    }
}

Camera* Map::Create_Camera_4829E0(s16 xpos, s16 ypos, s32 /*a4*/)
{
    // Check min bound
    if (xpos < 0 || ypos < 0)
    {
        return nullptr;
    }

    // Check max bounds
    if (xpos >= sPath_dword_BB47C0->field_6_cams_on_x || ypos >= sPath_dword_BB47C0->field_8_cams_on_y)
    {
        return nullptr;
    }

    // Return existing camera if we already have one
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i]
            && field_40_stru_5[i]->field_1A_level == field_0_current_level
            && field_40_stru_5[i]->field_18_path == field_2_current_path
            && field_40_stru_5[i]->field_14_xpos == xpos
            && field_40_stru_5[i]->field_16_ypos == ypos)
        {
            Camera* pTemp = field_40_stru_5[i];
            field_40_stru_5[i] = nullptr;
            return pTemp;
        }
    }

    // Get a pointer to the camera name from the Path resource
    const u8* pPathData = *GetPathResourceBlockPtr(field_2_current_path);
    auto pCamName = reinterpret_cast<const CameraName*>(&pPathData[(xpos + (ypos * sPath_dword_BB47C0->field_6_cams_on_x)) * sizeof(CameraName)]);

    // Empty/blank camera in the map array
    if (!pCamName->name[0])
    {
        return nullptr;
    }

    Camera* newCamera = ae_new<Camera>();
    newCamera->ctor_480DD0();

    // Copy in the camera name from the Path resource and append .CAM
    memset(newCamera->field_1E_cam_name, 0, sizeof(newCamera->field_1E_cam_name));
    strncpy(newCamera->field_1E_cam_name, pCamName->name, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->field_1E_cam_name, ".CAM");

    newCamera->field_14_xpos = xpos;
    newCamera->field_16_ypos = ypos;

    newCamera->field_30_flags &= -1;

    newCamera->field_1A_level = field_0_current_level;
    newCamera->field_18_path = field_2_current_path;

    // Calculate hash/resource ID of the camera
    newCamera->field_10_camera_resource_id = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0') + 100 * (pCamName->name[4] - '0') + 1000 * (pCamName->name[3] - '0');

    // Convert the 2 digit camera number string to an integer
    newCamera->field_1C_camera_number = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0');

    return newCamera;
}

void CCSTD Map::Load_Path_Items_482C10(Camera* pCamera, LoadMode loadMode)
{
    if (!pCamera)
    {
        return;
    }

    // Is camera resource loaded check
    if (!(pCamera->field_30_flags & 1))
    {
        if (loadMode == LoadMode::ConstructObject_0)
        {
            // Async camera load
            ResourceManager::LoadResourceFile_49C130(pCamera->field_1E_cam_name, Camera::On_Loaded_480ED0, pCamera, pCamera);

            sCameraBeingLoaded_5C3118 = pCamera;
            sPath_dword_BB47C0->Loader_4DB800(pCamera->field_14_xpos, pCamera->field_16_ypos, LoadMode::LoadResourceFromList_1, TlvTypes::None_m1); // none = load all
        }
        else
        {
            // Blocking camera load
            ResourceManager::LoadResourceFile_49C170(pCamera->field_1E_cam_name, pCamera);
            pCamera->field_30_flags |= 1;
            pCamera->field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, pCamera->field_10_camera_resource_id, 1, 0);

            sCameraBeingLoaded_5C3118 = pCamera;
            sPath_dword_BB47C0->Loader_4DB800(pCamera->field_14_xpos, pCamera->field_16_ypos, LoadMode::LoadResource_2, TlvTypes::None_m1); // none = load all
        }
        sCameraBeingLoaded_5C3118 = nullptr;
    }
}

void CC Map::LoadResource_4DBE00(const char_type* pFileName, s32 type, s32 resourceId, LoadMode loadMode, s16 bDontLoad)
{
    if (!bDontLoad)
    {
        pResourceManager_5C1BB0->LoadResource_464EE0(pFileName, type, resourceId, sCameraBeingLoaded_5C3118, sCameraBeingLoaded_5C3118, 0, 1);
        if (loadMode == LoadMode::LoadResource_2)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
    }
}

void CC Map::LoadResourcesFromList_4DBE70(const char_type* pFileName, ResourceManager::ResourcesToLoadList* pList, LoadMode loadMode, s16 bDontLoad)
{
    if (!bDontLoad)
    {
        pResourceManager_5C1BB0->LoadResourcesFromList_465150(pFileName, pList, sCameraBeingLoaded_5C3118, sCameraBeingLoaded_5C3118, 0, 1);
        if (loadMode == LoadMode::LoadResource_2)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
    }
}

s16 Map::SetActiveCameraDelayed_4814A0(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect)
{
    Path_Change* pPathChangeTLV = nullptr;
    CameraSwapEffects convertedSwapEffect = CameraSwapEffects::eInstantChange_0;
    if (pObj)
    {
        pPathChangeTLV = reinterpret_cast<Path_Change*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(pObj->field_B8_xpos),
            FP_GetExponent(pObj->field_BC_ypos),
            FP_GetExponent(pObj->field_B8_xpos),
            FP_GetExponent(pObj->field_BC_ypos),
            TlvTypes::PathTransition_1));
    }

    if (pObj && pPathChangeTLV)
    {
        field_A_level = pPathChangeTLV->field_10_level;
        field_C_path = pPathChangeTLV->field_12_path;
        field_E_camera = pPathChangeTLV->field_14_camera;
        if (swapEffect < 0)
        {
            // Map the TLV/editor value of screen change to the internal screen change
            convertedSwapEffect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[pPathChangeTLV->field_18_wipe];
        }
        else
        {
            // If not negative then its an actual swap effect
            convertedSwapEffect = static_cast<CameraSwapEffects>(swapEffect);
        }
    }
    else
    {
        switch (direction)
        {
            case MapDirections::eMapLeft_0:
                if (!GetCamera(CameraPos::eCamLeft_3))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapRight_1:
                if (!GetCamera(CameraPos::eCamRight_4))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapBottom_3:
                if (!GetCamera(CameraPos::eCamBottom_2))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapTop_2:
                if (!GetCamera(CameraPos::eCamTop_1))
                {
                    return 0;
                }
                break;
        }

        field_A_level = field_0_current_level;
        field_C_path = field_2_current_path;
        convertedSwapEffect = static_cast<CameraSwapEffects>(swapEffect); // TODO: Correct ??
    }

    field_14_direction = direction;
    field_18_pAliveObj = pObj;
    field_1C = convertedSwapEffect;
    field_6_state = CamChangeStates::eSliceCam_1;
    sMap_bDoPurpleLightEffect_5C311C = 0;

    if (convertedSwapEffect == CameraSwapEffects::ePlay1FMV_5 || convertedSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        sMap_bDoPurpleLightEffect_5C311C = 1;
    }

    return 1;
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});
