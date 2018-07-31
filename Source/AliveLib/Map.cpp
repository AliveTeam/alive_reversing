#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"

void Map_ForceLink() { }

ALIVE_VAR(1, 0x5c311c, __int16, sMap_word_5C311C, 0);
ALIVE_VAR(1, 0x5c3118, Camera*, sCameraBeingLoaded_5C3118, nullptr);
ALIVE_VAR(1, 0xbb47c0, Path*, sPath_dword_BB47C0, nullptr);

void Map::sub_480B80()
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
    
    field_6 = 0;
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

void Map::GoTo_Camera_481890()
{
    NOT_IMPLEMENTED();
}

void Map::sub_4804E0()
{
    NOT_IMPLEMENTED();
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
    field_6 = 2;

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

struct CameraName
{
    char name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

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

void Path::ctor_4DB170()
{
    field_C_pPathData = nullptr;
    field_10_ppRes = 0;
    field_8_cams_on_y = 0;
    field_6_cams_on_x = 0;
    field_4_cameraId = 0;
    field_2_pathId = 0;
    field_0_levelId = 0;
}

void Path::dtor_4DB1A0()
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
}

void Path::Free_4DB1C0()
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
    field_C_pPathData = 0;
    field_10_ppRes = 0;
    field_8_cams_on_y = 0;
    field_6_cams_on_x = 0;
    field_4_cameraId = 0;
    field_2_pathId = 0;
    field_0_levelId = 0;
}

void Path::Init_4DB200(PathData* pPathData, __int16 level, __int16 path, __int16 cameraId, BYTE** ppPathRes)
{
    ResourceManager::FreeResource_49C330(field_10_ppRes);
    field_10_ppRes = ppPathRes;
    ResourceManager::Inc_Ref_Count_49C310(ppPathRes);

    field_4_cameraId = cameraId;
    field_0_levelId = level;
    field_2_pathId = path;

    field_C_pPathData = pPathData;
    field_6_cams_on_x = (field_C_pPathData->field_4_bTop - field_C_pPathData->field_0_bLeft) / field_C_pPathData->field_A_grid_width;
    field_8_cams_on_y = (field_C_pPathData->field_6_bBottom - field_C_pPathData->field_2_bRight) / field_C_pPathData->field_C_grid_height;
}

void Path::Loader_4DB800(__int16 xpos, __int16 ypos, int loadMode, __int16 typeToLoad)
{
    // Get a pointer to the array of index table offsets
    const int* indexTable = reinterpret_cast<const int*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    int objectTableIdx = indexTable[xpos + (ypos * field_6_cams_on_x)];
    if (objectTableIdx == -1)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    BYTE* ptr = &(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + objectTableIdx];
    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    while (pPathTLV)
    {
        if ((typeToLoad == -1 || typeToLoad == pPathTLV->field_4_type) && ((WORD)loadMode || !(pPathTLV->field_0_flags & 3)))
        {
            void(__cdecl *pPathFnTable)(Path_TLV*, Path*, DWORD, __int16);
            pPathFnTable = (decltype(pPathFnTable))field_C_pPathData->field_1E_object_funcs[pPathTLV->field_4_type];

            if (!(WORD)loadMode)
            {
                pPathTLV->field_0_flags |= 3u;
            }

            pPathFnTable(
                pPathTLV,
                this,
                objectTableIdx | ((field_0_levelId | (field_2_pathId << 8)) << 16),
                static_cast<short>(loadMode));
        }

        // End of TLV list for current camera
        if (pPathTLV->field_0_flags & 4)
        {
            break;
        }

        objectTableIdx += pPathTLV->field_2_length;
        pPathTLV = Next_TLV_4DB6A0(pPathTLV);
    }
}

Path_TLV* Path::Get_First_TLV_For_Offsetted_Camera_4DB610(__int16 cam_x_idx, __int16 cam_y_idx)
{
    const int camY = cam_y_idx + gMap_5C3030.field_D2_cam_y_idx;
    const int camX = cam_x_idx + gMap_5C3030.field_D0_cam_x_idx;
    if (camX >= field_6_cams_on_x || camX < 0 || camY >= field_8_cams_on_y || camY < 0)
    {
        return nullptr;
    }

    const int* indexTable = reinterpret_cast<const int*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);
    const int indexTableEntry = indexTable[(camX + (camY * field_6_cams_on_x))];
    if (indexTableEntry == -1)
    {
        return nullptr;
    }

    return reinterpret_cast<Path_TLV*>(&(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + indexTableEntry]);
}

Path_TLV* __stdcall Path::Next_TLV_4DB6A0(Path_TLV* pTlv)
{
    if (pTlv->field_0_flags & 4)
    {
        return nullptr;
    }

    // Skip length bytes to get to the start of the next TLV
    BYTE* ptr = reinterpret_cast<BYTE*>(pTlv);
    BYTE* pNext = ptr + pTlv->field_2_length;
    return reinterpret_cast<Path_TLV*>(pNext);
}

Path_TLV* Path::TLV_First_Of_Type_In_Camera_4DB6D0(unsigned __int16 objectType, __int16 camX)
{
    Path_TLV* pTlv = Path::Get_First_TLV_For_Offsetted_Camera_4DB610(camX, 0);
    if (!pTlv)
    {
        return 0;
    }

    while (pTlv->field_4_type != objectType)
    {
        pTlv = Path::Next_TLV_4DB6A0(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }
    return pTlv;
}

Path_TLV* __stdcall Path::TLV_Next_Of_Type_4DB720(Path_TLV* pTlv, unsigned __int16 type)
{
    pTlv = Path::Next_TLV_4DB6A0(pTlv);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->field_4_type != type)
    {
        pTlv = Path::Next_TLV_4DB6A0(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});
