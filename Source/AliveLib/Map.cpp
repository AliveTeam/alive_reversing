#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "LvlArchive.hpp"
#include "PathData.hpp"
#include "Midi.hpp"
#include "SwitchStates.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "MusicController.hpp"
#include "BackgroundMusic.hpp"
#include "stdlib.hpp"
#include "Path.hpp"
#include "QuikSave.hpp"
#include <assert.h>

void Map_ForceLink() { }

ALIVE_VAR(1, 0x5c311c, __int16, sMap_word_5C311C, 0);
ALIVE_VAR(1, 0x5c3118, Camera*, sCameraBeingLoaded_5C3118, nullptr);
ALIVE_VAR(1, 0xbb47c0, Path*, sPath_dword_BB47C0, nullptr);
ALIVE_VAR(1, 0x5c3120, DWORD, sSoundChannelsMask_5C3120, 0);

struct CameraName
{
    char name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

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
                if (pItem->field_6_flags & BaseGameObject::eDead)
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

        if (field_A_5C303A_levelId)
        {
            if (field_A_5C303A_levelId == 16)
            {
                sSoundChannelsMask_5C3120 = 0;
                sub_480B80_Common();
                return;
            }
        }
        else if (!sCurrentLevelId_5C3030)
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

void Map::sub_481610()
{
    NOT_IMPLEMENTED();
}

void Map::Init_4803F0(__int16 level, __int16 path, __int16 camera, __int16 a5, __int16 a6, __int16 a7)
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
    sCurrentLevelId_5C3030 = static_cast<short>(-1);

    field_8 = 0;

    SetActiveCam_480D30(level, path, camera, a5, a6, a7);
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

EXPORT void __stdcall sub_465800(int)
{
    NOT_IMPLEMENTED();
}

class Class_5480E4 : public BaseGameObject
{
public:
    EXPORT void ctor_4E5000(BYTE** a2, int a3, __int16 a4, __int16 a5);
    EXPORT void dtor_4E5790();
    EXPORT void vdtor_4E4D90(signed int flags);
    virtual void VDestructor(signed int flags) override;

    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    int field_44;
    int field_48;
    int field_4C;
    int field_50;
    int field_54;
};
ALIVE_ASSERT_SIZEOF(Class_5480E4, 0x58);

void Class_5480E4::ctor_4E5000(BYTE** /*a2*/, int /*a3*/, __int16 /*a4*/, __int16 /*a5*/)
{
    NOT_IMPLEMENTED();
}

void Class_5480E4::dtor_4E5790()
{
    NOT_IMPLEMENTED();
}

void Class_5480E4::vdtor_4E4D90(signed int flags)
{
    dtor_4E5790();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void Class_5480E4::VDestructor(signed int flags)
{
    vdtor_4E4D90(flags);
}

EXPORT void __cdecl sub_4C9A30()
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0xbb234c, WORD, word_BB234C, 0);
ALIVE_VAR(1, 0x5c3154, DWORD, dword_5C3154, 0);

void Map::GoTo_Camera_481890()
{
    dword_5C3154 = 0; // TODO: Not used ?
    
    __int16 bShowLoadingIcon = FALSE;
    if (sCurrentLevelId_5C3030 != 0 && sCurrentLevelId_5C3030 != 16 && sCurrentLevelId_5C3030 != -1)
    {
        bShowLoadingIcon = TRUE;
    }

    if (field_10_screen_change_effect == 11)
    {
        BaseGameObject* pFmvRet = FMV_482650(nullptr, this, sCurrentLevelId_5C3030);
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

                if (pBaseGameObj->field_6_flags & BaseGameObject::eUpdatable)
                {
                    if (!(pBaseGameObj->field_6_flags & BaseGameObject::eDead) && (!word_5C1B66 || pBaseGameObj->field_6_flags & BaseGameObject::eUpdatableExtra))
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
        } while (!(pFmvRet->field_6_flags & BaseGameObject::eDead));

        if (sSoundChannelsMask_5C3120)
        {
            SND_Stop_Channels_Mask_4CA810(sSoundChannelsMask_5C3120);
        }
        sSoundChannelsMask_5C3120 = SND_4CA5D0(0, 0, 36, 70, 0, 0);
    }

    if (sCurrentLevelId_5C3030)
    {
        if (field_A_5C303A_levelId != sCurrentLevelId_5C3030
            || field_8
            || field_C_5C303C_pathId != sCurrentPathId_5C3032 
            && field_10_screen_change_effect == 5)
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

        if (sCurrentLevelId_5C3030 != -1)
        {
            // Close LVL archives
            sLvlArchive_5BC520.Free_433130();
            stru_5C3110.Free_433130();

            // Free all but the first ?
            for (int i = 1; i <= sPathData_559660.paths[sCurrentLevelId_5C3030].field_1A_num_paths; ++i)
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

        // Open LVL
        while (!sLvlArchive_5BC520.Open_Archive_432E80(sPathData_559660.paths[field_A_5C303A_levelId].field_22_lvl_name_cd))
        {
            if (word_5C1BA0)
            {
                // NOTE: Dead branch? Given no attract directory exists
                char fileName[256] = {};
                strcpy(fileName, "ATTRACT");
                strcat(fileName, sPathData_559660.paths[field_A_5C303A_levelId].field_22_lvl_name_cd);
                if (sLvlArchive_5BC520.Open_Archive_432E80(fileName))
                {
                    break;
                }
            }
            sub_465800(sPathData_559660.paths[field_A_5C303A_levelId].field_1C_unused);
        }

        // Open Path BND
        ResourceManager::LoadResourceFile_49C170(sPathData_559660.paths[field_A_5C303A_levelId].field_3A_bnd_name, 0);

        // Get pointer to each PATH
        for (int i = 1; i <= sPathData_559660.paths[field_A_5C303A_levelId].field_1A_num_paths; ++i)
        {
            field_54_path_res_array.field_0_pPathRecs[i] = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, 1, 0);
        }

        if (field_A_5C303A_levelId == sCurrentLevelId_5C3030)
        {
            MusicController::sub_47FD60(0, sActiveHero_5C1B68, 0, 0);
        }
        else
        {
            SND_Load_VABS_4CA350(sPathData_559660.paths[field_A_5C303A_levelId].field_8_pMusicInfo, sPathData_559660.paths[field_A_5C303A_levelId].field_10_reverb);
            SND_Load_Seqs_4CAED0(sSeqData_558D50.mData, sPathData_559660.paths[field_A_5C303A_levelId].field_C_bsq_file_name);

            auto pBackgroundMusic = alive_new<BackgroundMusic>();
            if (pBackgroundMusic)
            {
                pBackgroundMusic->ctor_4CB110(sPathData_559660.paths[field_A_5C303A_levelId].field_14_bg_music_id);
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
    const __int16 prevLevelId = sCurrentLevelId_5C3030;

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
    field_24_camera_offset.field_0_x = FP(field_D0_cam_x_idx * field_D4_ptr->field_A_grid_width);
    field_24_camera_offset.field_4_y = FP(field_D2_cam_y_idx * field_D4_ptr->field_C_grid_height);

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
            sCollisions_DArray_5C1128->loader_418930(pPathRec_1->field_8_pCollisionData, *field_54_path_res_array.field_0_pPathRecs[sCurrentPathId_5C3032]);
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
                sActiveHero_5C1B68->Vsub_408A40(10);
            }
        }
    }

    Create_FG1s_480F10();

    if (field_10_screen_change_effect == 5)
    {
        Map::FMV_482650(field_2C_5C305C_camera_array[0]->field_C_pCamRes, this, field_A_5C303A_levelId);
    }

    if (field_10_screen_change_effect == 11)
    {
        pScreenManager_5BB5F4->sub_cam_vlc_40EF60(reinterpret_cast<WORD**>(field_2C_5C305C_camera_array[0]->field_C_pCamRes));
        pScreenManager_5BB5F4->InvalidateRect_40EC10(0, 0, 640, 240);
        pScreenManager_5BB5F4->MoveImage_40EB70();
        pScreenManager_5BB5F4->field_40_flags |= 0x10000;
    }

    if (prevLevelId != sCurrentLevelId_5C3030)
    {
        pResourceManager_5C1BB0->LoadingLoop_465590(FALSE);
    }

    if (field_10_screen_change_effect != 5 && field_10_screen_change_effect != 11)
    {
        if (field_1E_door)
        {
            // TODO: Add template helpers

            // Door transition
            Path_Door* pDoorTlv = reinterpret_cast<Path_Door*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(Path_Door::kType, 0));
            while (pDoorTlv->field_18_door_number != sActiveHero_5C1B68->field_1A0_door_id)
            {
                pDoorTlv = reinterpret_cast<Path_Door*>(Path::TLV_Next_Of_Type_4DB720(pDoorTlv, Path_Door::kType));
            }

            CreateScreenTransistionForTLV(pDoorTlv);
        }
        else
        {
            if (!field_20)
            {
                // Normal transition ?
                auto obj = alive_new<Class_5480E4>();
                if (obj)
                {
                    obj->ctor_4E5000(field_2C_5C305C_camera_array[0]->field_C_pCamRes, field_10_screen_change_effect, 368 / 2, 240 / 2);
                }
            }
            else
            {
                // TODO: Add template helpers
                
                // Teleporter transition
                Path_Teleporter* pTeleporterTlv = reinterpret_cast<Path_Teleporter*>(sPath_dword_BB47C0->TLV_First_Of_Type_In_Camera_4DB6D0(Path_Teleporter::kType, 0));
                Path_Teleporter_Data teleporterData = {};
                memcpy(&teleporterData, &pTeleporterTlv->field_10_data, sizeof(Path_Teleporter_Data));
                while (teleporterData.field_10_id != sActiveHero_5C1B68->field_1A0_door_id)
                {
                    pTeleporterTlv = reinterpret_cast<Path_Teleporter*>(Path::TLV_Next_Of_Type_4DB720(pTeleporterTlv, Path_Teleporter::kType));
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

void Map::CreateScreenTransistionForTLV(Path_TLV* pTlv)
{
    auto obj = alive_new<Class_5480E4>();
    if (obj)
    {
        // TODO: Refactor
        const FP_Point* pCamPos2 = pScreenManager_5BB5F4->field_20_pCamPos;
        const short doorYDiff = static_cast<short>(pTlv->field_8_top_left.field_2_y - pCamPos2->field_4_y.GetExponent());
        FP camX = pCamPos2->field_0_x;
        const short midX = (pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2;

        const short rightPos = static_cast<short>(midX - camX.GetExponent());
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
    NOT_IMPLEMENTED();
}

signed __int16 Map::SetActiveCam_480D30(__int16 level, __int16 path, __int16 cam, __int16 screenChangeEffect, __int16 a6, __int16 forceChange)
{
    if (!forceChange && cam == sCurrentCamId_5C3034 && level == sCurrentLevelId_5C3030 && path == sCurrentPathId_5C3032)
    {
        return 0;
    }

    field_E_cameraId = cam;
    field_12_ticks = a6;
    field_C_5C303C_pathId = path;
    field_A_5C303A_levelId = level;
    field_10_screen_change_effect = screenChangeEffect;
    field_6_state = 2;

    if (screenChangeEffect == 5 || screenChangeEffect == 11)
    {
        sMap_word_5C311C = 1;
    }
    else
    {
        sMap_word_5C311C = 0;
    }

    return 1;
}

BaseGameObject* CC Map::FMV_482650(BYTE** /*ppBits*/, Map* /*pMap*/, int /*lvlId*/)
{
    NOT_IMPLEMENTED();
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
    strncpy(newCamera->field_1E_cam_name, pCamName->name, _countof(CameraName::name));
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

void __stdcall Map::Load_Path_Items_482C10(Camera* pCamera, __int16 loadMode)
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

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});
