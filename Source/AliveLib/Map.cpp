#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "LvlArchive.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "stdlib.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"
#include "Sfx.hpp"
#include "FG1.hpp"
#include "CameraSwapper.hpp"
#include "MainMenu.hpp"
#include "Events.hpp"
#include "Movie.hpp"
#include <assert.h>

void Map_ForceLink() { }

ALIVE_VAR(1, 0x5c311c, __int16, sMap_word_5C311C, 0);
ALIVE_VAR(1, 0x5c3118, Camera*, sCameraBeingLoaded_5C3118, nullptr);
ALIVE_VAR(1, 0x5c3120, DWORD, sSoundChannelsMask_5C3120, 0);

ALIVE_VAR(1, 0xbb234c, WORD, word_BB234C, 0);
ALIVE_VAR(1, 0x5c3154, DWORD, dword_5C3154, 0);

struct Path_ChangeTLV : public Path_TLV
{
    const static unsigned short kType;
    LevelIds field_10_level;
    __int16 field_12_path;
    __int16 field_14_camera;
    __int16 field_16_movie;
    __int16 field_18_wipe;
    __int16 field_1A_scale;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ChangeTLV, 0x1C);

const unsigned short Path_ChangeTLV::kType = 1;

// Map Path_ChangeTLV::field_18_wipe to CameraSwapEffects
const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_55D55C[12] =
{ 
    CameraSwapEffects::eEffect5_1_FMV,
    CameraSwapEffects::eEffect2_RightToLeft,
    CameraSwapEffects::eEffect1_LeftToRight,
    CameraSwapEffects::eEffect4_BottomToTop,
    CameraSwapEffects::eEffect3_TopToBottom,
    CameraSwapEffects::eEffect8_BoxOut,
    CameraSwapEffects::eEffect6_VerticalSplit,
    CameraSwapEffects::eEffect7_HorizontalSplit,
    CameraSwapEffects::eEffect11,
    CameraSwapEffects::eEffect0_InstantChange,
    CameraSwapEffects::eEffect0_InstantChange,
    CameraSwapEffects::eEffect0_InstantChange
};

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

void Map::sub_480B80_Common()
{
    if (field_6_state == 1)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        sub_481610();
    }
    else if (field_6_state == 2)
    {
        ResourceManager::Reclaim_Memory_49C470(0);
        GoTo_Camera_481890();
    }

    field_6_state = 0;

    SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
    sSoundChannelsMask_5C3120 = 0;
}

void Map::sub_480B80()
{
    if (field_6_state)
    {
        if (sMap_word_5C311C)
        {
            sub_480740(1);
        }

        PSX_DrawSync_4F6280(0);

        for (int i = 0; i < 2; i++) // Not sure why this is done twice?
        {
            DynamicArrayIter iter = {};
            iter.field_4_idx = 0;
            iter.field_0_pDynamicArray = gBaseGameObject_list_BB47C4;

            while (iter.field_4_idx < iter.field_0_pDynamicArray->field_4_used_size)
            {
                BaseGameObject* pItem = gBaseGameObject_list_BB47C4->ItemAt(iter.field_4_idx);
                ++iter.field_4_idx;
                if (!pItem)
                {
                    break;
                }

                pItem->VScreenChanged();

                // Did the screen change kill the object?
                if (pItem->field_6_flags.Get(BaseGameObject::eDead))
                {
                    iter.Remove_At_Iter_40CCA0();
                    pItem->VDestructor(1);
                }
            }
        }

        ResourceManager::NoEffect_49C700();

        //dword_5CA4A8 = 0; // TODO: Never used?

        // TODO: Refactor this logic
        if (!sMap_word_5C311C && field_A_5C303A_levelId == sCurrentLevelId_5C3030)
        {
            sub_480B80_Common();
            return;
        }

        if (field_A_5C303A_levelId != sCurrentLevelId_5C3030)
        {
            MIDI_Stop_All_Channels_4FDFE0();
        }

        if (field_A_5C303A_levelId != LevelIds::eNone)
        {
            if (field_A_5C303A_levelId == LevelIds::eCredits_16)
            {
                sSoundChannelsMask_5C3120 = 0;
                sub_480B80_Common();
                return;
            }
        }
        else if (sCurrentLevelId_5C3030 == LevelIds::eMenu_0)
        {
            sSoundChannelsMask_5C3120 = 0;
            sub_480B80_Common();
            return;
        }
        sSoundChannelsMask_5C3120 = SND_4CA5D0(0, 0, 36, 70, 0, 0);
        sub_480B80_Common();
    }
}

void Map::sub_480740(__int16 /*a2*/)
{
    NOT_IMPLEMENTED();
}

__int16 word_5463F8[] = { 3, 0, 4, 0, 1, 0, 2, 0, 0, 0 };

void Map::sub_481610()
{
    Path_ChangeTLV* pPathChangeTLV = nullptr;
    if (field_18_pAliveObj)
    {
        pPathChangeTLV = reinterpret_cast<Path_ChangeTLV*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(field_18_pAliveObj->field_B8_xpos),
            FP_GetExponent(field_18_pAliveObj->field_BC_ypos),
            FP_GetExponent(field_18_pAliveObj->field_B8_xpos),
            FP_GetExponent(field_18_pAliveObj->field_BC_ypos),
            Path_ChangeTLV::kType));
    }

    if (field_18_pAliveObj && pPathChangeTLV)
    {
        field_A_5C303A_levelId = pPathChangeTLV->field_10_level;
        field_C_5C303C_pathId = pPathChangeTLV->field_12_path;
        field_E_cameraId = pPathChangeTLV->field_14_camera;
        field_12_fmv_base_id = pPathChangeTLV->field_16_movie;
        field_10_screen_change_effect = kPathChangeEffectToInternalScreenChangeEffect_55D55C[pPathChangeTLV->field_18_wipe];
        field_18_pAliveObj->field_C2_lvl_number = field_A_5C303A_levelId;
        field_18_pAliveObj->field_C0_path_number = field_C_5C303C_pathId;
        GoTo_Camera_481890();

        // Translate TLV scale to internal scale?
        if (pPathChangeTLV->field_1A_scale > 0)
        {
            if (pPathChangeTLV->field_1A_scale -1 == 0)
            {
                sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(0.5);
                sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 13;
            }
        }
        else
        {
            sActiveHero_5C1B68->field_CC_sprite_scale = FP_FromDouble(1.0);
            sActiveHero_5C1B68->field_20_animation.field_C_render_layer = 32;
        }
        field_18_pAliveObj->Vsub_408320(
            field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width,
            field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height,
            word_5463F8[2 * (unsigned __int16)field_14_direction]); // remap direction ??
    }
    else
    {
        switch (field_14_direction)
        {
        case MapDirections::eMapLeft_0:
            field_D0_cam_x_idx--;
            field_10_screen_change_effect = CameraSwapEffects::eEffect2_RightToLeft;
            break;
        case MapDirections::eMapRight_1:
            field_D0_cam_x_idx++;
            field_10_screen_change_effect = CameraSwapEffects::eEffect1_LeftToRight;
            break;
        case MapDirections::eMapTop_2:
            field_D2_cam_y_idx--;
            field_10_screen_change_effect = CameraSwapEffects::eEffect4_BottomToTop;
            break;
        case MapDirections::eMapBottom_3:
            field_D2_cam_y_idx++;
            field_10_screen_change_effect = CameraSwapEffects::eEffect3_TopToBottom;
            break;
        default:
            break;
        }

        DWORD pCamNameOffset = sizeof(CameraName) * (field_D0_cam_x_idx + (field_D2_cam_y_idx * sPath_dword_BB47C0->field_6_cams_on_x));
        BYTE* pPathRes = *field_54_path_res_array.field_0_pPathRecs[sCurrentPathId_5C3032];
        CameraName* pCameraName = reinterpret_cast<CameraName*>(pPathRes + pCamNameOffset);

        // Convert the 2 digit camera number string to an integer
        field_E_cameraId =
            1 * (pCameraName->name[7] - '0') +
            10 * (pCameraName->name[6] - '0');

        GoTo_Camera_481890();
    }
}

CameraPos Map::GetDirection_4811A0(int level, int path, FP xpos, FP ypos)
{
    if (level != static_cast<int>(sCurrentLevelId_5C3030))
    {
        return CameraPos::eCamInvalid_m1;
    }

    if (path != sCurrentPathId_5C3032)
    {
        return CameraPos::eCamInvalid_m1;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);

    CameraPos ret = Is_Rect_In_Current_Camera_480FE0(&rect);

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
        if (ypos < y  || xpos < x || xpos > w)
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

void Map::Init_4803F0(LevelIds level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 a6, __int16 forceChange)
{
    sPath_dword_BB47C0 = alive_new<Path>();
    sPath_dword_BB47C0->ctor_4DB170();

    field_2C_5C305C_camera_array[0] = 0;
    field_2C_5C305C_camera_array[1] = 0;
    field_2C_5C305C_camera_array[2] = 0;
    field_2C_5C305C_camera_array[3] = 0;
    field_2C_5C305C_camera_array[4] = 0;

    field_22 = -1;

    sCurrentCamId_5C3034 = static_cast<short>(-1);
    sCurrentPathId_5C3032 = static_cast<short>(-1);
    sCurrentLevelId_5C3030 = LevelIds::eNone;

    field_8 = 0;

    SetActiveCam_480D30(level, path, camera, screenChangeEffect, a6, forceChange);
    GoTo_Camera_481890();
    
    field_6_state = 0;
}

void Map::Shutdown_4804E0()
{
    sLvlArchive_5BC520.Free_433130();
    stru_5C3110.Free_433130();

    // Free Path resources
    for (int i = 0; i < 30; i++)
    {
        if (field_54_path_res_array.field_0_pPathRecs[i])
        {
            ResourceManager::FreeResource_49C330(field_54_path_res_array.field_0_pPathRecs[i]);
            field_54_path_res_array.field_0_pPathRecs[i] = nullptr;
        }
    }

    // Free cameras
    for (int i = 0; i < 5; i++)
    {
        if (field_2C_5C305C_camera_array[i])
        {
            field_2C_5C305C_camera_array[i]->dtor_480E00();
            Mem_Free_495540(field_2C_5C305C_camera_array[i]);
            field_2C_5C305C_camera_array[i] = nullptr;
        }
    }

    pScreenManager_5BB5F4 = nullptr;

    // Free path
    if (sPath_dword_BB47C0)
    {
        sPath_dword_BB47C0->dtor_4DB1A0();
        Mem_Free_495540(sPath_dword_BB47C0);
    }
    sPath_dword_BB47C0 = nullptr;
    
    ResourceManager::Reclaim_Memory_49C470(0);
    Reset_4805D0();
}

void Map::Reset_4805D0()
{
    field_2C_5C305C_camera_array[0] = 0;
    field_2C_5C305C_camera_array[1] = 0;
    field_2C_5C305C_camera_array[2] = 0;
    field_2C_5C305C_camera_array[3] = 0;
    field_2C_5C305C_camera_array[4] = 0;
    memset(&field_54_path_res_array, 0, sizeof(field_54_path_res_array));
    field_CC = 1;
    field_CE = 0;
    field_D8_restore_quick_save = 0;
}

EXPORT void CCSTD sub_465800(int)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC sub_4C9A30()
{
    NOT_IMPLEMENTED();
}

void Map::GoTo_Camera_481890()
{
    dword_5C3154 = 0; // TODO: Not used ?
    
    __int16 bShowLoadingIcon = FALSE;
    if (sCurrentLevelId_5C3030 != LevelIds::eMenu_0 && sCurrentLevelId_5C3030 != LevelIds::eCredits_16 && sCurrentLevelId_5C3030 != LevelIds::eNone)
    {
        bShowLoadingIcon = TRUE;
    }


    if (field_10_screen_change_effect == CameraSwapEffects::eEffect11)
    {
        BaseGameObject* pFmvRet = FMV_Camera_Change_482650(nullptr, this, sCurrentLevelId_5C3030);
        do
        {
            SYS_EventsPump_494580();

            for (int i=0; i < gBaseGameObject_list_BB47C4->Size(); i++)
            {
                BaseGameObject* pBaseGameObj = gBaseGameObject_list_BB47C4->ItemAt(i);
                if (!pBaseGameObj)
                {
                    break;
                }

                if (pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdatable))
                {
                    if (!(pBaseGameObj->field_6_flags.Get(BaseGameObject::eDead)) && (!sNum_CamSwappers_5C1B66 || pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap)))
                    {
                        if (pBaseGameObj->field_1C_update_delay > 0)
                        {
                            pBaseGameObj->field_1C_update_delay--;
                        }
                        else
                        {
                            pBaseGameObj->VUpdate();
                        }
                    }
                }
            }
        } while (!(pFmvRet->field_6_flags.Get(BaseGameObject::eDead)));

        if (sSoundChannelsMask_5C3120)
        {
            SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
        }
        sSoundChannelsMask_5C3120 = SND_4CA5D0(0, 0, 36, 70, 0, 0);
    }

    if (sCurrentLevelId_5C3030 != LevelIds::eMenu_0 && sCurrentLevelId_5C3030 != LevelIds::eNone)
    {
        if (field_A_5C303A_levelId != sCurrentLevelId_5C3030
            || field_8
            || field_C_5C303C_pathId != sCurrentPathId_5C3032 
            && field_10_screen_change_effect == CameraSwapEffects::eEffect5_1_FMV)
        {
            Game_ShowLoadingIcon_482D80();
        }
    }

    if (field_A_5C303A_levelId != sCurrentLevelId_5C3030
        || field_C_5C303C_pathId != sCurrentPathId_5C3032
        || field_8)
    {
        field_22 = Get_Path_Unknown_480710();
    }

    if (field_A_5C303A_levelId != sCurrentLevelId_5C3030 || field_8)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);
        
        // Free all cameras
        for (int i = 0; i < 5; i++)
        {
            if (field_2C_5C305C_camera_array[i])
            {
                field_2C_5C305C_camera_array[i]->dtor_480E00();
                Mem_Free_495540(field_2C_5C305C_camera_array[i]);
                field_2C_5C305C_camera_array[i] = nullptr;
            }
        }

        if (sCurrentLevelId_5C3030 != LevelIds::eNone)
        {
            // Close LVL archives
            sLvlArchive_5BC520.Free_433130();
            stru_5C3110.Free_433130();

            // Free all but the first ?
            for (int i = 1; i <= sPathData_559660.paths[static_cast<int>(sCurrentLevelId_5C3030)].field_1A_num_paths; ++i)
            {
                ResourceManager::FreeResource_49C330(field_54_path_res_array.field_0_pPathRecs[i]);
                field_54_path_res_array.field_0_pPathRecs[i] = nullptr;
            }

            sPath_dword_BB47C0->Free_4DB1C0();

            if (field_A_5C303A_levelId != sCurrentLevelId_5C3030)
            {
                SND_Reset_4C9FB0();
            }

            ResourceManager::Reclaim_Memory_49C470(0);
        }

        pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);

        const PathRoot& pathData = sPathData_559660.paths[static_cast<int>(field_A_5C303A_levelId)];

        // Open LVL
        while (!sLvlArchive_5BC520.Open_Archive_432E80(pathData.field_22_lvl_name_cd))
        {
            if (word_5C1BA0)
            {
                // NOTE: Dead branch? Given no attract directory exists
                char fileName[256] = {};
                strcpy(fileName, "ATTRACT");
                strcat(fileName, pathData.field_22_lvl_name_cd);
                if (sLvlArchive_5BC520.Open_Archive_432E80(fileName))
                {
                    break;
                }
            }
            sub_465800(pathData.field_1C_unused);
        }

        // Open Path BND
        ResourceManager::LoadResourceFile_49C170(pathData.field_3A_bnd_name, 0);

        // Get pointer to each PATH
        for (int i = 1; i <= pathData.field_1A_num_paths; ++i)
        {
            field_54_path_res_array.field_0_pPathRecs[i] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, TRUE, FALSE);
        }

        if (field_A_5C303A_levelId == sCurrentLevelId_5C3030)
        {
            MusicController::sub_47FD60(0, sActiveHero_5C1B68, 0, 0);
        }
        else
        {
            SND_Load_VABS_4CA350(pathData.field_8_pMusicInfo, pathData.field_10_reverb);
            SND_Load_Seqs_4CAED0(sSeqData_558D50.mData, pathData.field_C_bsq_file_name);

            auto pBackgroundMusic = alive_new<BackgroundMusic>();
            if (pBackgroundMusic)
            {
                pBackgroundMusic->ctor_4CB110(pathData.field_14_bg_music_id);
            }
        }

        if (!field_8)
        {
            SwitchStates_SetRange_465FA0(2, 255);
        }

        if (field_CE)
        {
            ResourceManager::Free_Resource_Of_Type_49C6B0(ResourceManager::Resource_Animation);
            ResourceManager::Free_Resource_Of_Type_49C6B0(ResourceManager::Resource_Palt);
            field_CE = 0;
        }
    }

    if (!field_C_5C303C_pathId)
    {
        field_C_5C303C_pathId = 1;
    }

    const __int16 prevPathId = sCurrentPathId_5C3032;
    const LevelIds prevLevelId = sCurrentLevelId_5C3030;

    sCurrentPathId_5C3032 = field_C_5C303C_pathId;
    sCurrentLevelId_5C3030 = field_A_5C303A_levelId;
    sCurrentCamId_5C3034 = field_E_cameraId;

    const PathBlyRec* pPathRec_1 = Path_Get_Bly_Record_460F30(field_A_5C303A_levelId, field_C_5C303C_pathId);
    field_D4_ptr = pPathRec_1->field_4_pPathData;
    
    sPath_dword_BB47C0->Init_4DB200(
        field_D4_ptr,
        field_A_5C303A_levelId,
        field_C_5C303C_pathId,
        field_E_cameraId,
        field_54_path_res_array.field_0_pPathRecs[field_C_5C303C_pathId]);
    
    if (word_BB234C)
    {
        sub_4C9A30();
    }

    char pStrBuffer[13] = {};
    Path_Format_CameraName_460FB0(pStrBuffer, field_A_5C303A_levelId, field_C_5C303C_pathId, field_E_cameraId);

    DWORD pCamNameOffset = 0;
    if (sizeof(CameraName) * sPath_dword_BB47C0->field_6_cams_on_x * sPath_dword_BB47C0->field_8_cams_on_y > 0)
    {
        for (;;)
        {
            BYTE* pPathRes = *field_54_path_res_array.field_0_pPathRecs[field_C_5C303C_pathId];
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

    field_D0_cam_x_idx = static_cast<short>((pCamNameOffset / sizeof(CameraName)) % sPath_dword_BB47C0->field_6_cams_on_x);
    field_D2_cam_y_idx = static_cast<short>((pCamNameOffset / sizeof(CameraName)) / sPath_dword_BB47C0->field_6_cams_on_x);
    field_24_camera_offset.field_0_x = FP_FromInteger(field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width);
    field_24_camera_offset.field_4_y = FP_FromInteger(field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height);

    // If map has changed then load new collision info
    if (prevPathId != sCurrentPathId_5C3032 || prevLevelId != sCurrentLevelId_5C3030)
    {
        if (sCollisions_DArray_5C1128)
        {
            sCollisions_DArray_5C1128->dtor_4189F0();
            Mem_Free_495540(sCollisions_DArray_5C1128);
        }

        sCollisions_DArray_5C1128 = alive_new<Collisions>();
        if (sCollisions_DArray_5C1128)
        {
            sCollisions_DArray_5C1128->ctor_418930(pPathRec_1->field_8_pCollisionData, *field_54_path_res_array.field_0_pPathRecs[sCurrentPathId_5C3032]);
        }
    }

    if (field_D8_restore_quick_save)
    {
        QuikSave_RestoreObjectStates_D481890_4C9BE0(field_D8_restore_quick_save);
        field_D8_restore_quick_save = nullptr;
    }

    // Copy camera array and blank out the source
    for (int i = 0; i < 5; i++)
    {
        field_40_stru_5[i] = field_2C_5C305C_camera_array[i];
        field_2C_5C305C_camera_array[i] = nullptr;
    }

    field_2C_5C305C_camera_array[0] = Create_Camera_4829E0(field_D0_cam_x_idx,      field_D2_cam_y_idx,         1);
    field_2C_5C305C_camera_array[3] = Create_Camera_4829E0(field_D0_cam_x_idx - 1,  field_D2_cam_y_idx,         0);
    field_2C_5C305C_camera_array[4] = Create_Camera_4829E0(field_D0_cam_x_idx + 1,  field_D2_cam_y_idx,         0);
    field_2C_5C305C_camera_array[1] = Create_Camera_4829E0(field_D0_cam_x_idx,      field_D2_cam_y_idx - 1,     0);
    field_2C_5C305C_camera_array[2] = Create_Camera_4829E0(field_D0_cam_x_idx,      field_D2_cam_y_idx + 1,     0);
 
    // Free resources for each camera
    for (int i = 0; i < 5; i++)
    {
        if (field_40_stru_5[i])
        {
            pResourceManager_5C1BB0->Free_Resources_For_Camera_4656F0(field_40_stru_5[i]);
        }
    }

    pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);

    // Free each camera itself
    for (int i = 0; i < 5; i++)
    {
        if (field_40_stru_5[i])
        {
            field_40_stru_5[i]->dtor_480E00();
            Mem_Free_495540(field_40_stru_5[i]);
            field_40_stru_5[i] = nullptr;
        }
    }

    Map::Load_Path_Items_482C10(field_2C_5C305C_camera_array[0], 0);
    pResourceManager_5C1BB0->LoadingLoop_465590(bShowLoadingIcon);
    Map::Load_Path_Items_482C10(field_2C_5C305C_camera_array[3], 0);
    Map::Load_Path_Items_482C10(field_2C_5C305C_camera_array[4], 0);
    Map::Load_Path_Items_482C10(field_2C_5C305C_camera_array[1], 0);
    Map::Load_Path_Items_482C10(field_2C_5C305C_camera_array[2], 0);

    // Create the screen manager if it hasn't already been done (probably should have always been done by this point though?)
    if (!pScreenManager_5BB5F4)
    {
        pScreenManager_5BB5F4 = alive_new<ScreenManager>();
        if (pScreenManager_5BB5F4)
        {
            pScreenManager_5BB5F4->ctor_40E3E0(field_2C_5C305C_camera_array[0]->field_C_pCamRes, &field_24_camera_offset);
        }
    }

    sPath_dword_BB47C0->Loader_4DB800(field_D0_cam_x_idx, field_D2_cam_y_idx, 0, -1);
    if (prevPathId != sCurrentPathId_5C3032 || prevLevelId != sCurrentLevelId_5C3030)
    {
        if (sActiveHero_5C1B68)
        {
            if (sCurrentPathId_5C3032 == sActiveHero_5C1B68->field_C0_path_number)
            {
                sActiveHero_5C1B68->VCheckCollisionLineStillValid_408A40(10);
            }
        }
    }

    Create_FG1s_480F10();

    if (field_10_screen_change_effect == CameraSwapEffects::eEffect5_1_FMV)
    {
        Map::FMV_Camera_Change_482650(field_2C_5C305C_camera_array[0]->field_C_pCamRes, this, field_A_5C303A_levelId);
    }

    if (field_10_screen_change_effect == CameraSwapEffects::eEffect11)
    {
        pScreenManager_5BB5F4->DecompressToVRam_40EF60(reinterpret_cast<WORD**>(field_2C_5C305C_camera_array[0]->field_C_pCamRes));
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
        pScreenManager_5BB5F4->MoveImage_40EB70();
        pScreenManager_5BB5F4->field_40_flags |= 0x10000;
    }

    if (prevLevelId != sCurrentLevelId_5C3030)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    if (field_10_screen_change_effect != CameraSwapEffects::eEffect5_1_FMV && field_10_screen_change_effect != CameraSwapEffects::eEffect11)
    {
        if (field_1E_door)
        {
            // TODO: Add template helpers

            // Door transition
            Path_Door* pDoorTlv = reinterpret_cast<Path_Door*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::Door_5, 0));
            while (pDoorTlv->field_18_door_number != sActiveHero_5C1B68->field_1A0_door_id)
            {
                pDoorTlv = reinterpret_cast<Path_Door*>(Path::TLV_Next_Of_Type_4DB720(pDoorTlv, TlvTypes::Door_5));
            }

            CreateScreenTransistionForTLV(pDoorTlv);
        }
        else
        {
            if (!field_20)
            {
                auto obj = alive_new<CameraSwapper>();
                if (obj)
                {
                    obj->ctor_4E5000(field_2C_5C305C_camera_array[0]->field_C_pCamRes, field_10_screen_change_effect, 368 / 2, 240 / 2);
                }
            }
            else
            {
                // TODO: Add template helpers
                
                // Teleporter transition
                Path_Teleporter* pTeleporterTlv = reinterpret_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(TlvTypes::Teleporter_88, 0));
                Path_Teleporter_Data teleporterData = {};
                memcpy(&teleporterData, &pTeleporterTlv->field_10_data, sizeof(Path_Teleporter_Data));
                while (teleporterData.field_10_id != sActiveHero_5C1B68->field_1A0_door_id)
                {
                    pTeleporterTlv = reinterpret_cast<Path_Teleporter*>(Path::TLV_Next_Of_Type_4DB720(pTeleporterTlv, TlvTypes::Teleporter_88));
                    memcpy(&teleporterData, &pTeleporterTlv->field_10_data, sizeof(Path_Teleporter_Data));
                }

                CreateScreenTransistionForTLV(pTeleporterTlv);
            }
        }
    }

    word_5C1BAA = 0;
    dword_5C1BAC = 0;

    field_8 = 0;

    if (sSoundChannelsMask_5C3120)
    {
        SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
        sSoundChannelsMask_5C3120 = 0;
    }
}

Camera* Map::GetCamera(CameraPos pos)
{
    return field_2C_5C305C_camera_array[static_cast<int>(pos)];
}

void Map::CreateScreenTransistionForTLV(Path_TLV* pTlv)
{
    auto obj = alive_new<CameraSwapper>();
    if (obj)
    {
        // TODO: Refactor
        const FP_Point* pCamPos2 = pScreenManager_5BB5F4->field_20_pCamPos;
        const short doorYDiff = static_cast<short>(pTlv->field_8_top_left.field_2_y - FP_GetExponent(pCamPos2->field_4_y));
        FP camX = pCamPos2->field_0_x;
        const short midX = (pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2;

        const short rightPos = static_cast<short>(midX - FP_GetExponent(camX));
        const short xpos2 = rightPos;
        obj->ctor_4E5000(field_2C_5C305C_camera_array[0]->field_C_pCamRes, field_10_screen_change_effect, xpos2, doorYDiff);
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

__int16 Map::Get_Path_Unknown_480710()
{
    // TODO: Probably need to redo field_C data as 1 bytes instead of a word
    return Path_Get_Bly_Record_460F30(field_A_5C303A_levelId, field_C_5C303C_pathId)->field_C & 0xFF;
}

void Map::Create_FG1s_480F10()
{
    pScreenManager_5BB5F4->UnsetDirtyBits_FG1_40ED70();

    Camera* pCamera = field_2C_5C305C_camera_array[0];
    for (int i = 0; i < pCamera->field_0.Size(); i++)
    {
        BYTE** ppRes = pCamera->field_0.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        if (*ppRes)
        {
            ResourceManager::Header* pHeader = ResourceManager::Get_Header_49C410(ppRes);
            if (pHeader->field_8_type == ResourceManager::Resource_FG1)
            {
                FG1* pFG1 = alive_new<FG1>();
                pFG1->ctor_499FC0(ppRes);
            }
        }
    }
}

signed __int16 Map::Get_Camera_World_Rect_481410(CameraPos camIdx, PSX_RECT* pRect)
{
    if (camIdx < CameraPos::eCamCurrent_0)
    {
        return 0;
    }

    if (camIdx > CameraPos::eCamRight_4)
    {
        return 0;
    }

    Camera* pCamera = field_2C_5C305C_camera_array[static_cast<int>(camIdx)];
    if (!pCamera)
    {
        return 0;
    }

    if (!pRect)
    {
        return 1;
    }

    const __int16 xpos = pCamera->field_14_xpos * field_D4_ptr->field_A_grid_width;
    const __int16 ypos = pCamera->field_16_ypos * field_D4_ptr->field_C_grid_height;

    pRect->x = xpos;
    pRect->y = ypos;
    pRect->w = xpos + 368;
    pRect->h = ypos + 240;
    return 1;
}

__int16 Map::Is_Point_In_Current_Camera_4810D0(int level, int path, FP xpos, FP ypos, __int16 width)
{
    const FP calculated_width = (width != 0) ? FP_FromInteger(6) : FP_FromInteger(0);
    if (static_cast<LevelIds>(level) != sCurrentLevelId_5C3030 || path != sCurrentPathId_5C3032) // TODO: Remove when 100%
    {
        return 0;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos - calculated_width);
    rect.w = FP_GetExponent(calculated_width + xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);
    return Is_Rect_In_Current_Camera_480FE0(&rect) == CameraPos::eCamCurrent_0;
}


EXPORT CameraPos Map::Is_Rect_In_Current_Camera_480FE0(PSX_RECT* pRect)
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        return CameraPos::eCamNone_5;
    }

    const int camX = FP_GetExponent(field_24_camera_offset.field_0_x);
    const int camY = FP_GetExponent(field_24_camera_offset.field_4_y);

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
        if (pRect->h < camY)     // return 1 or 0
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

signed __int16 Map::SetActiveCam_480D30(LevelIds level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange)
{
    if (!forceChange && cam == sCurrentCamId_5C3034 && level == sCurrentLevelId_5C3030 && path == sCurrentPathId_5C3032)
    {
        return 0;
    }

    field_E_cameraId = cam;
    field_12_fmv_base_id = fmvBaseId;
    field_C_5C303C_pathId = path;
    field_A_5C303A_levelId = level;
    field_10_screen_change_effect = screenChangeEffect;

    field_6_state = 2;

    if (screenChangeEffect == CameraSwapEffects::eEffect5_1_FMV || screenChangeEffect == CameraSwapEffects::eEffect11)
    {
        sMap_word_5C311C = 1;
    }
    else
    {
        sMap_word_5C311C = 0;
    }

    return 1;
}

BaseGameObject* CC Map::FMV_Camera_Change_482650(BYTE** ppBits, Map* pMap, LevelIds lvlId)
{
    if (pMap->field_12_fmv_base_id > 10000u)
    {
        // Trippe FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id / 10000);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id / 100 % 100);
        FmvInfo* pFmvRec3 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id % 100);
        sLevelId_dword_5CA408 = static_cast<int>(lvlId); // TODO: Strongly type this, but it hasn't got the same underlaying type as the enum grr..
        DWORD pos1 = 0;
        DWORD pos2 = 0;
        DWORD pos3 = 0;
        Get_fmvs_sectors_494460(pFmvRec1->field_0_pName, pFmvRec2->field_0_pName, pFmvRec3->field_0_pName, &pos1, &pos2, &pos3);
        CameraSwapper* pSwapperMem3 = alive_new<CameraSwapper>();
        if (pSwapperMem3)
        {
            return pSwapperMem3->ctor_4E4ED0(
                ppBits,
                pos1,
                pFmvRec1->field_4_id,
                pos2,
                pFmvRec2->field_4_id,
                pos3,
                pFmvRec3->field_4_id,

                pFmvRec1->field_6_flags & 1,
                pFmvRec1->field_8,
                pFmvRec1->field_A_volume,

                pFmvRec2->field_6_flags & 1,
                pFmvRec2->field_8,
                pFmvRec2->field_A_volume,

                pFmvRec3->field_6_flags & 1,
                pFmvRec3->field_8,
                pFmvRec3->field_A_volume);
        }
    }
    if (pMap->field_12_fmv_base_id >= 100u)
    {
        // Double FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id / 100);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id % 100);
        DWORD cdPos1 = 0;
        DWORD cdPos2 = 0;
        Get_fmvs_sectors_494460(pFmvRec1->field_0_pName, pFmvRec2->field_0_pName, 0, &cdPos1, &cdPos2, 0);
        sLevelId_dword_5CA408 = static_cast<int>(lvlId); // HACK
        CameraSwapper* pSwapperMem2 = alive_new<CameraSwapper>();
        if (pSwapperMem2)
        {
            return pSwapperMem2->ctor_4E4DC0(
                ppBits,
                cdPos1,
                pFmvRec1->field_4_id,
                cdPos2,
                pFmvRec2->field_4_id,
                pFmvRec1->field_6_flags & 1,
                pFmvRec1->field_8,
                pFmvRec1->field_A_volume,
                pFmvRec2->field_6_flags & 1,
                pFmvRec2->field_8,
                pFmvRec2->field_A_volume);
        }
    }
    else // < 100
    {
        // Single FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_460F70(lvlId, pMap->field_12_fmv_base_id);
        DWORD cdPos = 0;
        Get_fmvs_sectors_494460(pFmvRec1->field_0_pName, 0, 0, &cdPos, 0, 0);
        sLevelId_dword_5CA408 = static_cast<int>(lvlId); // HACK
        CameraSwapper* pSwapperMem = alive_new<CameraSwapper>();
        if (pSwapperMem)
        {
            return pSwapperMem->ctor_4E4CA0(
                ppBits,
                cdPos,
                pFmvRec1->field_4_id,
                pFmvRec1->field_6_flags & 1,
                pFmvRec1->field_8,
                pFmvRec1->field_A_volume);
        }
    }
    return nullptr;
}

Camera* Map::Create_Camera_4829E0(__int16 xpos, __int16 ypos, int /*a4*/)
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
    for (int i = 0; i < 5; i++)
    {
        if (field_40_stru_5[i]
            && field_40_stru_5[i]->field_1A_level == sCurrentLevelId_5C3030
            && field_40_stru_5[i]->field_18_path == sCurrentPathId_5C3032
            && field_40_stru_5[i]->field_14_xpos == xpos
            && field_40_stru_5[i]->field_16_ypos == ypos)
        {
            Camera* pTemp = field_40_stru_5[i];
            field_40_stru_5[i] = nullptr;
            return pTemp;
        }
    }

    // Get a pointer to the camera name from the Path resource
    BYTE* pPathData = *field_54_path_res_array.field_0_pPathRecs[sCurrentPathId_5C3032];
    CameraName* pCamName = reinterpret_cast<CameraName*>(&pPathData[(xpos + (ypos * sPath_dword_BB47C0->field_6_cams_on_x)) * sizeof(CameraName)]);

    // Empty/blank camera in the map array
    if (!pCamName->name[0])
    {
        return nullptr;
    }

    Camera* newCamera = alive_new<Camera>();
    newCamera->ctor_480DD0();

    // Copy in the camera name from the Path resource and append .CAM
    memset(newCamera->field_1E_cam_name, 0, sizeof(newCamera->field_1E_cam_name));
    strncpy(newCamera->field_1E_cam_name, pCamName->name, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->field_1E_cam_name, ".CAM");

    newCamera->field_14_xpos = xpos;
    newCamera->field_16_ypos = ypos;

    newCamera->field_30_flags &= -1;

    newCamera->field_1A_level = sCurrentLevelId_5C3030;
    newCamera->field_18_path = sCurrentPathId_5C3032;;

    // Calculate hash/resource ID of the camera
    newCamera->field_10_camera_resource_id =
        1 * (pCamName->name[7] - '0') +
        10 * (pCamName->name[6] - '0') +
        100 * (pCamName->name[4] - '0') +
        1000 * (pCamName->name[3] - '0');

    // Convert the 2 digit camera number string to an integer
    newCamera->field_1C_camera_number = 
        1* (pCamName->name[7] - '0') + 
        10 *(pCamName->name[6] - '0');
    
    return newCamera;
}

void CCSTD Map::Load_Path_Items_482C10(Camera* pCamera, __int16 loadMode)
{
    if (!pCamera)
    {
        return;
    }

    // Is camera resource loaded check
    if (!(pCamera->field_30_flags & 1))
    {
        if (loadMode == 0)
        {
            // Async camera load
            ResourceManager::LoadResourceFile_49C130(pCamera->field_1E_cam_name, Camera::On_Loaded_480ED0, pCamera, pCamera);

            sCameraBeingLoaded_5C3118 = pCamera;
            sPath_dword_BB47C0->Loader_4DB800(pCamera->field_14_xpos, pCamera->field_16_ypos, 1, -1);
        }
        else
        {
            // Blocking camera load
            ResourceManager::LoadResourceFile_49C170(pCamera->field_1E_cam_name, pCamera);
            pCamera->field_30_flags |= 1;
            pCamera->field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, pCamera->field_10_camera_resource_id, 1, 0);

            sCameraBeingLoaded_5C3118 = pCamera;
            sPath_dword_BB47C0->Loader_4DB800(pCamera->field_14_xpos, pCamera->field_16_ypos, 2, -1);
        }
        sCameraBeingLoaded_5C3118 = nullptr;
    }
}

void CC Map::LoadResource_4DBE00(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad)
{
    if (!bDontLoad)
    {
        pResourceManager_5C1BB0->LoadResource_464EE0(pFileName, type, resourceId, sCameraBeingLoaded_5C3118, sCameraBeingLoaded_5C3118, 0, 1);
        if (loadMode == 2)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
    }
}

void CC Map::LoadResourcesFromList_4DBE70(const char* pFileName, ResourceManager::ResourcesToLoadList* pList, __int16 loadMode, __int16 bDontLoad)
{
    if (!bDontLoad)
    {
        pResourceManager_5C1BB0->LoadResourcesFromList_465150(pFileName, pList, sCameraBeingLoaded_5C3118, sCameraBeingLoaded_5C3118, 0, 1);
        if (loadMode == 2)
        {
            pResourceManager_5C1BB0->LoadingLoop_465590(0);
        }
    }
}

signed __int16 Map::SetActiveCameraDelayed_4814A0(MapDirections direction, BaseAliveGameObject* pObj, __int16 kMinus1)
{
    Path_ChangeTLV* pPathChangeTLV = nullptr;
    CameraSwapEffects unknown = CameraSwapEffects::eEffect0_InstantChange;
    if (pObj)
    {
        pPathChangeTLV = reinterpret_cast<Path_ChangeTLV*>(sPath_dword_BB47C0->TLV_Get_At_4DB4B0(
            FP_GetExponent(pObj->field_B8_xpos),
            FP_GetExponent(pObj->field_BC_ypos),
            FP_GetExponent(pObj->field_B8_xpos),
            FP_GetExponent(pObj->field_BC_ypos),
            Path_ChangeTLV::kType));
    }

    if (pObj && pPathChangeTLV)
    {
        field_A_5C303A_levelId = pPathChangeTLV->field_10_level;
        field_C_5C303C_pathId = pPathChangeTLV->field_12_path;
        field_E_cameraId = pPathChangeTLV->field_14_camera;
        if (kMinus1 < 0)
        {
            // Map the TLV/editor value of screen change to the internal screen change
            unknown = kPathChangeEffectToInternalScreenChangeEffect_55D55C[pPathChangeTLV->field_18_wipe];
        }
        else
        {
            unknown = static_cast<CameraSwapEffects>(kMinus1); // TODO: Correct ??
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

        field_A_5C303A_levelId = sCurrentLevelId_5C3030;
        field_C_5C303C_pathId = sCurrentPathId_5C3032;
        unknown = static_cast<CameraSwapEffects>(kMinus1); // TODO: Correct ??
    }

    field_14_direction = direction;
    field_18_pAliveObj = pObj;
    field_1C = unknown;
    field_6_state = 1;
    sMap_word_5C311C = 0;
    
    if (unknown == CameraSwapEffects::eEffect5_1_FMV || unknown == CameraSwapEffects::eEffect11)
    {
        sMap_word_5C311C = 1;
    }

    return 1;
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});
