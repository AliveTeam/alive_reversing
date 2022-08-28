#include "stdafx.h"
#include "LevelLoader.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "DeathFadeOut.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

LevelLoader::LevelLoader(relive::Path_LevelLoader* pTlv, const TLVUniqueId& tlvId)
    : BaseGameObject(TRUE, 0)
{
    mBaseGameObjectTlvInfo = tlvId;
    field_20_switch_id = pTlv->mSwitchId;
    field_24_level = pTlv->mDestLevel;
    field_26_path = pTlv->mDestPath;
    field_28_camera = pTlv->mDestCamera;
    field_2A_movie_id = pTlv->mMovieId;
    field_2C_tlvInfo = tlvId;
    field_22_state = States::eIdle_0;
}

void LevelLoader::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        if (field_22_state == States::eIdle_0)
        {
            if (SwitchStates_Get(field_20_switch_id))
            {
                auto pFade = relive_new DeathFadeOut(Layer::eLayer_FadeFlash_40, 1, 1, 32, TPageAbr::eBlend_2);
                if (pFade)
                {
                    field_22_state = States::eFadingOut_1;
                    field_30_fade_id = pFade->mBaseGameObjectId;
                }
            }
        }
        else if (field_22_state == States::eFadingOut_1)
        {
            DeathFadeOut* pFade = static_cast<DeathFadeOut*>(sObjectIds.Find_Impl(field_30_fade_id));
            if (!pFade || pFade->field_7E_bDone)
            {
                if (field_2A_movie_id)
                {
                    gMap.SetActiveCam(
                        field_24_level,
                        field_26_path,
                        field_28_camera,
                        CameraSwapEffects::ePlay1FMV_5,
                        field_2A_movie_id,
                        0);
                }
                else
                {
                    gMap.SetActiveCam(
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
