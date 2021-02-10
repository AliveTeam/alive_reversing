#include "stdafx.h"
#include "LevelLoader.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "DeathFadeOut.hpp"
#include "ObjectIds.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

BaseGameObject* LevelLoader::VDestructor(signed int flags)
{
    return vdtor_4DD3B0(flags);
}

void LevelLoader::VUpdate()
{
    vUpdate_4DD400();
}

LevelLoader* LevelLoader::ctor_4DD330(Path_LevelLoader* pTlv, DWORD tlvInfo)
{
    BaseGameObject_ctor_4DBFA0(TRUE, 0);
    SetVTable(this, 0x547B5C); // vTbl_LevelLoader_547B5C
    field_C_objectId = tlvInfo;
    field_20_id = pTlv->field_10_id;
    field_24_level = pTlv->field_12_dest_level;
    field_26_path = pTlv->field_14_dest_path;
    field_28_camera = pTlv->field_16_dest_camera;
    field_2A_movie = pTlv->field_18_movie;
    field_2C_tlvInfo = tlvInfo;
    field_22_state = States::eIdle_0;
    return this;
}

void LevelLoader::vUpdate_4DD400()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    else
    {
        if (field_22_state == States::eIdle_0)
        {
            if (SwitchStates_Get_466020(field_20_id))
            {
                auto pFade = ae_new<DeathFadeOut>();
                if (pFade)
                {
                    pFade->ctor_427030(Layer::eLayer_40, 1, 1, 32, TPageAbr::eBlend_2);
                    field_22_state = States::eFadingOut_1;
                    field_30_fade_id = pFade->field_8_object_id;
                }
            }
        }
        else if (field_22_state == States::eFadingOut_1)
        {
            DeathFadeOut* pFade = static_cast<DeathFadeOut*>(sObjectIds_5C1B70.Find_449CF0(field_30_fade_id));
            if (!pFade || pFade->field_7E_bDone)
            {
                if (field_2A_movie)
                {
                    gMap_5C3030.SetActiveCam_480D30(
                        field_24_level,
                        field_26_path,
                        field_28_camera,
                        CameraSwapEffects::eEffect5_1_FMV,
                        field_2A_movie,
                        0);
                }
                else
                {
                    gMap_5C3030.SetActiveCam_480D30(
                        field_24_level,
                        field_26_path,
                        field_28_camera,
                        CameraSwapEffects::eEffect0_InstantChange,
                        0,
                        0);
                }
                field_22_state = States::eDone_2;
                sMap_bDoPurpleLightEffect_5C311C = 0;
            }
        }
    }
}

LevelLoader* LevelLoader::vdtor_4DD3B0(signed int flags)
{
    dtor_4DD3E0();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

void LevelLoader::dtor_4DD3E0()
{
    BaseGameObject_dtor_4DBEC0();
}
