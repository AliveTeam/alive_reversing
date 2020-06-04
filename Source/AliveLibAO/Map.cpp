#include "stdafx_ao.h"
#include "Map.hpp"
#include "Function.hpp"

START_NS_AO

void Map_ForceLink() {}


ALIVE_VAR(1, 0x507BA8, Map, gMap_507BA8, {});
ALIVE_VAR(1, 0x507C9C, short, sMap_bDoPurpleLightEffect_507C9C, 0);

void Map::Init_443EE0(__int16 level, __int16 path, __int16 camera, __int16 screenChangeEffect, __int16 fmvBaseId, __int16 forceChange)
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

    if (forceChange || camera != -1 || level != -1 || path != -1)
    {
        field_E_camera = camera;
        field_C_path = path;
        field_A_level = level;
        field_12_fmv_base_id = fmvBaseId;
        field_10_screenChangeEffect = screenChangeEffect;
        field_6_state = 2;
        
        sMap_bDoPurpleLightEffect_507C9C = 0;

        if (screenChangeEffect == 5 || screenChangeEffect == 11)
        {
            sMap_bDoPurpleLightEffect_507C9C = 1;
        }
    }

    GoTo_Camera_445050();
    field_6_state = 0;
}

void Map::ScreenChange_4444D0()
{
    NOT_IMPLEMENTED();
}

void Map::GoTo_Camera_445050()
{
    NOT_IMPLEMENTED();
}

void Map::Shutdown_443F90()
{
    NOT_IMPLEMENTED();
}

END_NS_AO
