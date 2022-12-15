#include "stdafx.h"
#include "LevelLoader.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Fade.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

LevelLoader::LevelLoader(relive::Path_LevelLoader* pTlv, const Guid& tlvId)
    : BaseGameObject(true, 0)
{
    mBaseGameObjectTlvInfo = tlvId;
    mSwitchId = pTlv->mSwitchId;
    mDestLevel = pTlv->mDestLevel;
    mDestPath = pTlv->mDestPath;
    mDestCamera = pTlv->mDestCamera;
    mMovieId = pTlv->mMovieId;
    mTlvId = tlvId;
    mState = States::eIdle_0;
}

void LevelLoader::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
    else
    {
        if (mState == States::eIdle_0)
        {
            if (SwitchStates_Get(mSwitchId))
            {
                auto pFade = relive_new Fade(Layer::eLayer_FadeFlash_40, FadeOptions::eFadeIn, true, 32, TPageAbr::eBlend_2);
                if (pFade)
                {
                    mState = States::eFadingOut_1;
                    mFadeId = pFade->mBaseGameObjectId;
                }
            }
        }
        else if (mState == States::eFadingOut_1)
        {
            Fade* pFade = static_cast<Fade*>(sObjectIds.Find_Impl(mFadeId));
            if (!pFade || pFade->mDone)
            {
                if (mMovieId)
                {
                    gMap.SetActiveCam(
                        mDestLevel,
                        mDestPath,
                        mDestCamera,
                        CameraSwapEffects::ePlay1FMV_5,
                        mMovieId,
                        0);
                }
                else
                {
                    gMap.SetActiveCam(
                        mDestLevel,
                        mDestPath,
                        mDestCamera,
                        CameraSwapEffects::eInstantChange_0,
                        0,
                        0);
                }
                mState = States::eDone_2;
                gMap_bDoPurpleLightEffect = false;
            }
        }
    }
}
