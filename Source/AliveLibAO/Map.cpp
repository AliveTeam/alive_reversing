#include "stdafx_ao.h"
#include "Map.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
#include "PathData.hpp"

START_NS_AO

void Map_ForceLink() {}


ALIVE_VAR(1, 0x507BA8, Map, gMap_507BA8, {});
ALIVE_VAR(1, 0x507C9C, short, sMap_bDoPurpleLightEffect_507C9C, 0);

void Map::Init_443EE0(__int16 level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange)
{
    field_34_camera_array[0] = nullptr;
    field_34_camera_array[1] = nullptr;
    field_34_camera_array[2] = nullptr;
    field_34_camera_array[3] = nullptr;
    field_34_camera_array[4] = nullptr;

    field_28_cd_or_overlay_num = -1;

    field_4_current_camera = -1;
    field_2_current_path = -1;
    field_0_current_level = -1;

    SetActiveCam_444660(level, path, camera, screenChangeEffect, fmvBaseId, forceChange);
    GoTo_Camera_445050();

    field_6_state = 0;
}

class LvlArchive
{
public:
    EXPORT __int16 Free_41BEB0()
    {
        NOT_IMPLEMENTED();
        return 0;
    }

private:
    BYTE** field_0_0x2800_res;
    int field_4_cd_pos;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR(1, 0x4FFD60, LvlArchive, sLvlArchive_4FFD60, {});
ALIVE_VAR(1, 0x507C90, LvlArchive, stru_507C90, {});

void Map::Shutdown_443F90()
{
    sLvlArchive_4FFD60.Free_41BEB0();
    stru_507C90.Free_41BEB0();

    // Free Path resources
    for (int i = 0; i < ALIVE_COUNTOF(field_5C_path_res_array.field_0_pPathRecs); i++)
    {
        if (field_5C_path_res_array.field_0_pPathRecs[i])
        {
            ResourceManager::FreeResource_455550(field_5C_path_res_array.field_0_pPathRecs[i]);
            field_5C_path_res_array.field_0_pPathRecs[i] = nullptr;
        }
    }

    // Free cameras
    for (int i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        if (field_34_camera_array[i])
        {
            field_34_camera_array[i]->dtor_444700();
            ao_delete_free_447540(field_34_camera_array[i]);
            field_34_camera_array[i] = nullptr;
        }
    }

    pScreenManager_4FF7C8 = nullptr;

    ResourceManager::Reclaim_Memory_455660(0);
    Reset();
}

void Map::Reset()
{
    for (int i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        field_34_camera_array[i] = nullptr;
    }

    for (int i = 0; i < ALIVE_COUNTOF(field_5C_path_res_array.field_0_pPathRecs); i++)
    {
        field_5C_path_res_array.field_0_pPathRecs[i] = nullptr;
    }

    field_D8 = 1;
    field_DC = 0;
    field_E0_save_data = 0;
}

__int16 Map::SetActiveCam_444660(__int16 level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange)
{
    if (!forceChange && cam == field_4_current_camera && level == field_0_current_level && path == field_2_current_path)
    {
        return 0;
    }

    field_E_camera = cam;
    field_12_fmv_base_id = fmvBaseId;
    field_C_path = path;
    field_A_level = level;
    field_10_screenChangeEffect = screenChangeEffect;
    field_6_state = 2;


    if (screenChangeEffect == CameraSwapEffects::eEffect5_1_FMV || screenChangeEffect == CameraSwapEffects::eEffect11)
    {
        sMap_bDoPurpleLightEffect_507C9C = 1;
    }
    else
    {
        sMap_bDoPurpleLightEffect_507C9C = 0;
    }

    return 1;
}

__int16 Map::GetOverlayId_4440B0()
{
    return Path_Get_Bly_Record_434650(field_A_level, field_C_path)->field_C_overlay_id;
}

void Map::ScreenChange_4444D0()
{
    NOT_IMPLEMENTED();
}

void Map::GoTo_Camera_445050()
{
    NOT_IMPLEMENTED();
}

void Map::Loader_446590(__int16 camX, __int16 camY, int loadMode, __int16 typeToLoad)
{
    // Get a pointer to the array of index table offsets
    const int* indexTable = reinterpret_cast<const int*>(*field_5C_path_res_array.field_0_pPathRecs[field_2_current_path] + field_D4_pPathData->field_18_object_index_table_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    int objectTableIdx = indexTable[camX + (camY * field_24_max_cams_x)];
    if (objectTableIdx == -1 || objectTableIdx >= 0x100000)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    DWORD* pPathData = (DWORD*)*field_5C_path_res_array.field_0_pPathRecs[field_2_current_path];
    Path_TLV* pTlvIter = (Path_TLV*)((char*)pPathData + objectTableIdx + field_D4_pPathData->field_14_offset);
    if (pTlvIter->field_2_length < 24u || pTlvIter->field_2_length > 480u)
    {
        pTlvIter->field_0_flags |= 4u;
    }

    if (pTlvIter->field_4_type <= 0x100000 && pTlvIter->field_2_length <= 0x2000u && *(DWORD*)&pTlvIter->field_8_top_left <= 0x1000000)
    {
        while (1)
        {
            if ((typeToLoad == -1 || typeToLoad == pTlvIter->field_4_type) && (loadMode || !(pTlvIter->field_0_flags & 3)))
            {
                TlvItemInfoUnion data;
                data.parts.tlvOffset = static_cast<WORD>(objectTableIdx);
                data.parts.levelId = static_cast<BYTE>(field_0_current_level);
                data.parts.pathId = static_cast<BYTE>(field_2_current_path);
                
                // Call the factory to construct the item
                field_D4_pPathData->field_1C_object_funcs.object_funcs[pTlvIter->field_4_type](pTlvIter, this, data, static_cast<short>(loadMode));

                if (!loadMode)
                {
                    pTlvIter->field_0_flags |= 3u;
                }
            }

            if (((unsigned __int8)pTlvIter->field_0_flags >> 2) & 1)
            {
                break;
            }

            const auto tlv_len = pTlvIter->field_2_length;
            pTlvIter = (Path_TLV*)((char*)pTlvIter + tlv_len);
            objectTableIdx += tlv_len;

            if (pTlvIter->field_2_length < 24u || pTlvIter->field_2_length > 480u)
            {
                pTlvIter->field_0_flags |= 4u;
            }
        }
    }
}

END_NS_AO
