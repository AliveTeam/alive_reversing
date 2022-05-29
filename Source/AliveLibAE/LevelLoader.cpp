#include "stdafx.h"
#include "LevelLoader.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "DeathFadeOut.hpp"
#include "ObjectIds.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

void LevelLoader::VUpdate()
{
    vUpdate_4DD400();
}

LevelLoader::LevelLoader(Path_LevelLoader* pTlv, u32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_C_objectId = tlvInfo;
    field_20_switch_id = pTlv->field_10_switch_id;
    field_24_level = pTlv->field_12_dest_level;
    field_26_path = pTlv->field_14_dest_path;
    field_28_camera = pTlv->field_16_dest_camera;
    field_2A_movie_id = pTlv->field_18_movie_id;
    field_2C_tlvInfo = tlvInfo;
    field_22_state = States::eIdle_0;
}

void LevelLoader::vUpdate_4DD400()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        mFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_22_state == States::eIdle_0)
        {
            if (SwitchStates_Get_466020(field_20_switch_id))
            {
                auto pFade = ae_new<DeathFadeOut>(Layer::eLayer_FadeFlash_40, 1, 1, 32, TPageAbr::eBlend_2);
                if (pFade)
                {
                    field_22_state = States::eFadingOut_1;
                    field_30_fade_id = pFade->field_8_object_id;
                }
            }
        }
        else if (field_22_state == States::eFadingOut_1)
        {
            DeathFadeOut* pFade = static_cast<DeathFadeOut*>(sObjectIds.Find_449CF0(field_30_fade_id));
            if (!pFade || pFade->field_7E_bDone)
            {
                if (field_2A_movie_id)
                {
                    gMap.SetActiveCam_480D30(
                        field_24_level,
                        field_26_path,
                        field_28_camera,
                        CameraSwapEffects::ePlay1FMV_5,
                        field_2A_movie_id,
                        0);
                }
                else
                {
                    gMap.SetActiveCam_480D30(
                        field_24_level,
                        field_26_path,
                        field_28_camera,
                        CameraSwapEffects::eInstantChange_0,
                        0,
                        0);
                }
                field_22_state = States::eDone_2;
                sMap_bDoPurpleLightEffect_5C311C = 0;
            }
        }
    }
}
