#include "stdafx.h"
#include "Map.hpp"
#include "PathData.hpp"
#include "Function.hpp"
#include "ScreenManager.hpp"
#include "ResourceManager.hpp"

void Map_ForceLink() { }

ALIVE_VAR(1, 0x5c311c, __int16, sMap_word_5C311C, 0);
ALIVE_VAR(1, 0x5c3118, Camera*, sCameraBeingLoaded_5C3118, nullptr);
ALIVE_VAR(1, 0xbb47c0, Path*, sPath_dword_BB47C0, nullptr);

void Map::sub_480B80()
{
    NOT_IMPLEMENTED();
}

int Map::sub_4803F0(__int16 /*a2*/, __int16 /*a3*/, __int16 /*a4*/, __int16 /*a5*/, __int16 /*a6*/, __int16 /*a7*/)
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
            pCamera->field_C_pCamRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Bits, pCamera->field_10_camera, 1, 0);

            sCameraBeingLoaded_5C3118 = pCamera;
            sPath_dword_BB47C0->Loader_4DB800(pCamera->field_14_xpos, pCamera->field_16_ypos, 2, -1);
        }
        sCameraBeingLoaded_5C3118 = nullptr;
    }
}

void Path::Loader_4DB800(__int16 xpos, __int16 ypos, int loadMode, __int16 typeToLoad)
{
    NOT_IMPLEMENTED();

    // Get a pointer to the array of index table offsets
    int* indexTable = reinterpret_cast<int*>(*field_10_ppRes + field_C_pPathData->field_16_object_indextable_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    const int objectTableIdx = indexTable[xpos + (ypos * field_6_cams_on_x)];
    if (objectTableIdx == -1)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    // With this offset we get a pointer to the first TLV item for this camera
    BYTE* ptr = &(*field_10_ppRes)[field_C_pPathData->field_12_object_offset + objectTableIdx];
    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    int tlvOffset = objectTableIdx;
    while (pPathTLV)
    {
        if ((typeToLoad == -1 || typeToLoad == pPathTLV->field_4_type) && ((WORD)loadMode || !(pPathTLV->field_0_flags & 3)))
        {
            void(__cdecl *pPathFnTable)(Path_TLV*, Path*, DWORD, __int16);
            pPathFnTable = (decltype(pPathFnTable))field_C_pPathData->field_1E_object_funcs[pPathTLV->field_4_type];

            if (!(WORD)loadMode)
            {
                pPathTLV->field_0_flags |= 3;
            }

            const DWORD gen = tlvOffset | ((field_0_levelId | (field_2_pathId << 8)) << 16);
            pPathFnTable(pPathTLV, this, gen, static_cast<short>(loadMode));
        }

        // End of TLV list for current camera
        if (pPathTLV->field_0_flags & 4)
        {
            break;
        }

        pPathTLV = Path::Next_TLV_4DB6A0(pPathTLV);
        tlvOffset += pPathTLV->field_2_length;
    }
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

ALIVE_VAR(1, 0x5C3030, Map, gMap_5C3030, {});
