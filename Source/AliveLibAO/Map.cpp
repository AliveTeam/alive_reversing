#include "stdafx_ao.h"
#include "Map.hpp"
#include "../relive_lib/Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/ResourceManager.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "PathData.hpp"
#include "Game.hpp"
#include "Midi.hpp"
#include "BaseAliveGameObject.hpp"
#include "Abe.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "AmbientSound.hpp"
#include "../relive_lib/GameObjects/BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "CameraSwapper.hpp"
#include "../AliveLibAE/Game.hpp"
#include "../relive_lib/GameObjects/Particle.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "Sfx.hpp"
#include "Elum.hpp"
#include "../relive_lib/Sys.hpp"
#include "../relive_lib/Camera.hpp"

#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FatalError.hpp"
#include "../relive_lib/BinaryPath.hpp"
#include "../AliveLibAE/PathData.hpp"
#include "Path.hpp"
#include "Factory.hpp"

class BaseGameObject;

namespace AO {

Map gMap = {};
s32 sSoundChannelsMask = 0;

OpenSeqHandle g_SeqTable_4C9E70[165] = {
    {"D1AMB.SEQ", 0, 0, 100, -1, {}},
    {"D2AMB.SEQ", 0, 0, 100, -1, {}},
    {"E1AMB.SEQ", 0, 0, 60, -1, {}},
    {"E2AMB.SEQ", 0, 0, 60, -1, {}},
    {"E2AMB2.SEQ", 0, 0, 60, -1, {}},
    {"F1AMB.SEQ", 0, 0, 60, -1, {}},
    {"F2AMB.SEQ", 0, 0, 100, -1, {}},
    {"MLAMB.SEQ", 0, 0, 60, -1, {}},
    {"RFAMB.SEQ", 0, 0, 70, -1, {}},
    {"OPTAMB.SEQ", 0, 0, 100, -1, {}},
    {"GUN.SEQ", 0, 0, 40, -1, {}},
    {"OHM.SEQ", 0, 0, 80, -1, {}},
    {"MUDOHM.SEQ", 0, 0, 80, -1, {}},
    {"EBELL2.SEQ", 0, 0, 127, -1, {}},
    {"ABEMOUNT.SEQ", 0, 0, 85, -1, {}},
    {"ESCRATCH.SEQ", 0, 0, 45, -1, {}},
    {"SSCRATCH.SEQ", 0, 0, 55, -1, {}},
    {"PANTING.SEQ", 0, 0, 45, -1, {}},
    {"BATSQUEK.SEQ", 0, 0, 55, -1, {}},
    {"WHISTLE1.SEQ", 0, 0, 127, -1, {}},
    {"WHISTLE2.SEQ", 0, 0, 127, -1, {}},
    {"SLIGBOMB.SEQ", 0, 0, 127, -1, {}},
    {"SLIGBOM2.SEQ", 0, 0, 127, -1, {}},
    {"OOPS.SEQ", 0, 0, 40, -1, {}},
    {"PIGEONS.SEQ", 0, 0, 40, -1, {}},
    {"CHIPPER.SEQ", 0, 0, 90, -1, {}},
    {"PATROL.SEQ", 0, 0, 60, -1, {}},
    {"SLEEPING.SEQ", 0, 0, 60, -1, {}},
    {"ONCHAIN.SEQ", 0, 0, 60, -1, {}},
    {"SLOSLEEP.SEQ", 0, 0, 60, -1, {}},
    {"PARAPANT.SEQ", 0, 0, 35, -1, {}},
    {"GRINDER.SEQ", 0, 0, 35, -1, {}},
    {"BASICTRK.SEQ", 0, 0, 90, -1, {}},
    {"LE_LO_1.SEQ", 0, 0, 127, -1, {}},
    {"LE_LO_2.SEQ", 0, 0, 127, -1, {}},
    {"LE_LO_3.SEQ", 0, 0, 127, -1, {}},
    {"LE_LO_4.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_1.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_2.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_3.SEQ", 0, 0, 127, -1, {}},
    {"LE_SH_4.SEQ", 0, 0, 127, -1, {}},
    {"MYSTERY1.SEQ", 0, 0, 60, -1, {}},
    {"MYSTERY2.SEQ", 0, 0, 60, -1, {}},
    {"NEGATIV1.SEQ", 0, 0, 60, -1, {}},
    {"NEGATIV3.SEQ", 0, 0, 60, -1, {}},
    {"POSITIV1.SEQ", 0, 0, 60, -1, {}},
    {"POSITIV9.SEQ", 0, 0, 60, -1, {}},
    {"D1_0_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_0_2.SEQ", 0, 0, 60, -1, {}},
    {"D1_0_3.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_2.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_3.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_4.SEQ", 0, 0, 60, -1, {}},
    {"D1_1_5.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_2.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_3.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_4.SEQ", 0, 0, 60, -1, {}},
    {"D1_2_5.SEQ", 0, 0, 60, -1, {}},
    {"D1_3_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_4_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_5_1.SEQ", 0, 0, 60, -1, {}},
    {"D1_6_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_0_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_0_2.SEQ", 0, 0, 60, -1, {}},
    {"D2_1_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_1_2.SEQ", 0, 0, 60, -1, {}},
    {"D2_2_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_2_2.SEQ", 0, 0, 60, -1, {}},
    {"D2_4_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_5_1.SEQ", 0, 0, 60, -1, {}},
    {"D2_6_1.SEQ", 0, 0, 60, -1, {}},
    {"DE_2_1.SEQ", 0, 0, 90, -1, {}},
    {"DE_4_1.SEQ", 0, 0, 90, -1, {}},
    {"DE_5_1.SEQ", 0, 0, 90, -1, {}},
    {"E1_0_1.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_2.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_3.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_4.SEQ", 0, 0, 60, -1, {}},
    {"E1_0_5.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_1.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_2.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_3.SEQ", 0, 0, 60, -1, {}},
    {"E1_1_4.SEQ", 0, 0, 90, -1, {}},
    {"E1_1_5.SEQ", 0, 0, 90, -1, {}},
    {"E1_4_1.SEQ", 0, 0, 90, -1, {}},
    {"E1_5_1.SEQ", 0, 0, 90, -1, {}},
    {"E1_6_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_0_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_0_2.SEQ", 0, 0, 90, -1, {}},
    {"F1_0_3.SEQ", 0, 0, 90, -1, {}},
    {"F1_1_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_1_2.SEQ", 0, 0, 90, -1, {}},
    {"F1_1_3.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_2.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_3.SEQ", 0, 0, 90, -1, {}},
    {"F1_2_4.SEQ", 0, 0, 90, -1, {}},
    {"F1_3_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_4_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_5_1.SEQ", 0, 0, 90, -1, {}},
    {"F1_6_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_0_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_1_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_2_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_4_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_5_1.SEQ", 0, 0, 90, -1, {}},
    {"F2_6_1.SEQ", 0, 0, 90, -1, {}},
    {"FE_2_1.SEQ", 0, 0, 90, -1, {}},
    {"FE_4_1.SEQ", 0, 0, 90, -1, {}},
    {"FE_5_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_2.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_3.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_4.SEQ", 0, 0, 90, -1, {}},
    {"ML_0_5.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_2.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_3.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_4.SEQ", 0, 0, 90, -1, {}},
    {"ML_1_5.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_2.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_3.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_4.SEQ", 0, 0, 90, -1, {}},
    {"ML_2_5.SEQ", 0, 0, 90, -1, {}},
    {"ML_3_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_4_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_5_1.SEQ", 0, 0, 90, -1, {}},
    {"ML_6_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_2.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_3.SEQ", 0, 0, 90, -1, {}},
    {"RF_0_4.SEQ", 0, 0, 90, -1, {}},
    {"RF_1_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_1_2.SEQ", 0, 0, 90, -1, {}},
    {"RF_1_3.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_2.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_3.SEQ", 0, 0, 90, -1, {}},
    {"RF_2_4.SEQ", 0, 0, 90, -1, {}},
    {"RF_4_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_5_1.SEQ", 0, 0, 90, -1, {}},
    {"RF_6_1.SEQ", 0, 0, 90, -1, {}},
    {"RE_2_1.SEQ", 0, 0, 90, -1, {}},
    {"RE_4_1.SEQ", 0, 0, 90, -1, {}},
    {"RE_5_1.SEQ", 0, 0, 90, -1, {}},
    {"OPT_0_1.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_2.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_3.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_4.SEQ", 0, 0, 60, -1, {}},
    {"OPT_0_5.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_1.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_2.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_3.SEQ", 0, 0, 60, -1, {}},
    {"OPT_1_4.SEQ", 0, 0, 90, -1, {}},
    {"OPT_1_5.SEQ", 0, 0, 90, -1, {}},
    {"ALL_4_1.SEQ", 0, 0, 90, -1, {}},
    {"ALL_5_1.SEQ", 0, 0, 90, -1, {}},
    {"ALL_5_2.SEQ", 0, 0, 90, -1, {}},
    {"ALL_5_3.SEQ", 0, 0, 90, -1, {}},
    {"ALL_7_1.SEQ", 0, 0, 90, -1, {}},
    {"ALL_8_1.SEQ", 0, 0, 90, -1, {}},
    {nullptr, 0, 0, 0, 0, {}}};

s32 MaxGridBlocks(FP scale)
{
    if (scale == FP_FromDouble(0.5))
    {
        return 30; // (29+1) * 13 (grid block size) for 377/390
    }
    else if (scale == FP_FromInteger(1))
    {
        return 16; // (15+1) * 25 (grid block size) for 375/400
    }
    else
    {
        LOG_WARNING("Scale should be 0.5 or 1 but got %f. This usually occurs when you die with DDCheat on.", FP_GetDouble(scale));
        return 0;
    }
}

void Map::ScreenChange_Common()
{
    if (mCamState == CamChangeStates::eSliceCam_1)
    {
        Handle_PathTransition();
    }
    else if (mCamState == CamChangeStates::eInstantChange_2)
    {
        GoTo_Camera();
    }

    mCamState = CamChangeStates::eInactive_0;

    SND_Stop_Channels_Mask(sSoundChannelsMask);
    sSoundChannelsMask = 0;
}

Map::Map()
{
    Reset();
}

void Map::Reset()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        field_2C_camera_array[i] = nullptr;
    }

    ClearPathResourceBlocks();

    mFreeAllAnimAndPalts = false;
    mSaveData = 0;
}

void Map::Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    field_2C_camera_array[0] = nullptr;
    field_2C_camera_array[1] = nullptr;
    field_2C_camera_array[2] = nullptr;
    field_2C_camera_array[3] = nullptr;
    field_2C_camera_array[4] = nullptr;

    mOverlayId = -1;

    mCurrentCamera = -1;
    mCurrentPath = -1;
    mCurrentLevel = EReliveLevelIds::eNone;

    SetActiveCam(level, path, camera, screenChangeEffect, fmvBaseId, forceChange);
    GoTo_Camera();

    mCamState = CamChangeStates::eInactive_0;
}

void Map::ScreenChange()
{
    if (mCamState == CamChangeStates::eInactive_0)
    {
        return;
    }

    if (gMap_bDoPurpleLightEffect && mCurrentLevel != EReliveLevelIds::eBoardRoom)
    {
        RemoveObjectsWithPurpleLight(1);
    }

    for (s32 i = 0; i < 2; i++) // Not sure why this is done twice?
    {
        for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
        {
            BaseGameObject* pItem = gBaseGameObjects->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();

            // Did the screen change kill the object?
            if (pItem->GetDead())
            {
                if (pItem->mBaseGameObjectRefCount == 0)
                {
                    j = gBaseGameObjects->RemoveAt(j);
                    delete pItem;
                }
            }
        }
    }

    for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
    {
        ::BaseGameObject* pItem = gBaseGameObjects->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->GetDead())
        {
            if (pItem->mBaseGameObjectRefCount == 0)
            {
                i = gBaseGameObjects->RemoveAt(i);
                delete pItem;
            }
        }
    }

    if (gMap_bDoPurpleLightEffect || LevelChanged())
    {
        if (LevelChanged())
        {
            SsUtAllKeyOff(0);
        }

        // TODO: Re-check this logic
        if (mNextLevel != EReliveLevelIds::eMenu)
        {
            if ((mNextLevel != EReliveLevelIds::eRuptureFarmsReturn && mNextLevel != EReliveLevelIds::eForestChase && mNextLevel != EReliveLevelIds::eDesertEscape) || (mNextLevel == EReliveLevelIds::eBoardRoom && mCurrentLevel == EReliveLevelIds::eBoardRoom))
            {
                sSoundChannelsMask = 0;
            }
        }
        else
        {
            sSoundChannelsMask = 0;
        }
    }

    ScreenChange_Common();
}


void Map::Shutdown()
{
    // Free Path resources
    FreePathResourceBlocks();

    // Free cameras
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        if (field_2C_camera_array[i])
        {
            relive_delete field_2C_camera_array[i];
            field_2C_camera_array[i] = nullptr;
        }
    }

    gScreenManager = nullptr;

    Reset();
}


s16 Map::SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    if (!forceChange && cam == mCurrentCamera && level == mCurrentLevel && path == mCurrentPath)
    {
        return 0;
    }

    mNextCamera = cam;
    mFmvBaseId = fmvBaseId;
    mNextPath = path;
    mNextLevel = level;
    mCameraSwapEffect = screenChangeEffect;
    mCamState = CamChangeStates::eInstantChange_2;

    if (screenChangeEffect == CameraSwapEffects::ePlay1FMV_5 || screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        gMap_bDoPurpleLightEffect = true;
    }
    else
    {
        gMap_bDoPurpleLightEffect = false;
    }

    return 1;
}

void Map::RemoveObjectsWithPurpleLight(s16 bMakeInvisible)
{
    auto pObjectsWithLightsArray = relive_new DynamicArrayT<BaseAnimatedWithPhysicsGameObject>(16);

    auto pPurpleLightArray = relive_new DynamicArrayT<Particle>(16);

    bool bAddedALight = false;
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObj = gBaseAliveGameObjects->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->GetDrawable())
        {
            auto pBaseObj = static_cast<BaseAnimatedWithPhysicsGameObject*>(pObj);
            if (pBaseObj->GetDoPurpleLightEffect())
            {
                if (pBaseObj->GetAnimation().GetRender())
                {
                    if (!pBaseObj->GetDead() && pObj != sControlledCharacter)
                    {
                        bool bAdd = false;
                        if (pBaseObj->mCurrentLevel == mCurrentLevel
                            && pBaseObj->mCurrentPath == mCurrentPath)
                        {
                            PSX_RECT rect = {};
                            rect.x = FP_GetExponent(pBaseObj->mXPos);
                            rect.w = FP_GetExponent(pBaseObj->mXPos);
                            rect.y = FP_GetExponent(pBaseObj->mYPos);
                            rect.h = FP_GetExponent(pBaseObj->mYPos);
                            bAdd = Rect_Location_Relative_To_Active_Camera(&rect, 0) == CameraPos::eCamCurrent_0;
                        }

                        if (bAdd)
                        {
                            pObjectsWithLightsArray->Push_Back(pBaseObj);

                            const PSX_RECT objRect = pBaseObj->VGetBoundingRect();

                            const FP k60Scaled = pBaseObj->GetSpriteScale() * FP_FromInteger(60);
                            Particle* pPurpleLight = New_DestroyOrCreateObject_Particle(
                                FP_FromInteger((objRect.x + objRect.w) / 2),
                                FP_FromInteger((objRect.y + objRect.h) / 2) + k60Scaled,
                                pBaseObj->GetSpriteScale());

                            if (pPurpleLight)
                            {
                                pPurpleLightArray->Push_Back(pPurpleLight);
                                bAddedALight = true;
                            }
                        }
                    }
                }
            }
        }
    }

    if (bAddedALight)
    {
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400);

        const auto kTotal = bMakeInvisible != 0 ? 12 : 4;
        for (s32 counter = 0; counter < kTotal; counter++)
        {
            if (bMakeInvisible && counter == 4)
            {
                // Make all the objects that have lights invisible now that the lights have been rendered for a few frames
                for (s32 i = 0; i < pObjectsWithLightsArray->Size(); i++)
                {
                    BaseAnimatedWithPhysicsGameObject* pObj = pObjectsWithLightsArray->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }
                    pObj->GetAnimation().SetRender(false);
                }
            }

            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->GetDead())
                {
                    pLight->VUpdate();
                }
            }

            // TODO/HACK what is the point of the f64 loop? Why not do both in 1 iteration ??
            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->GetDead())
                {
                    pLight->GetAnimation().VDecode();
                }
            }

            for (s32 i = 0; i < gObjListDrawables->Size(); i++)
            {
                BaseGameObject* pDrawable = gObjListDrawables->ItemAt(i);
                if (!pDrawable)
                {
                    break;
                }

                if (!pDrawable->GetDead())
                {
                    // TODO: Seems strange to check this flag, how did it get in the drawable list if its not a drawable ??
                    if (pDrawable->GetDrawable())
                    {
                        pDrawable->VRender(gPsxDisplay.mDrawEnv.mOrderingTable);
                    }
                }
            }

            gScreenManager->VRender(gPsxDisplay.mDrawEnv.mOrderingTable);
            SYS_EventsPump();
            gPsxDisplay.RenderOrderingTable();
        }

        if (bMakeInvisible)
        {
            // Make all the objects that had lights visible again
            for (s32 i = 0; i < pObjectsWithLightsArray->Size(); i++)
            {
                BaseAnimatedWithPhysicsGameObject* pObj = pObjectsWithLightsArray->ItemAt(i);
                if (!pObj)
                {
                    break;
                }
                pObj->GetAnimation().SetRender(true);
            }
        }
    }

    pObjectsWithLightsArray->mUsedSize = 0;
    pPurpleLightArray->mUsedSize = 0;

    relive_delete pObjectsWithLightsArray;
    relive_delete pPurpleLightArray;
}

void Map::Handle_PathTransition()
{
    relive::Path_PathTransition* pTlv = nullptr;
    if (mAliveObj)
    {
        pTlv = static_cast<relive::Path_PathTransition*>(VTLV_Get_At_Of_Type(
            FP_GetExponent(mAliveObj->mXPos),
            FP_GetExponent(mAliveObj->mYPos),
            FP_GetExponent(mAliveObj->mXPos),
            FP_GetExponent(mAliveObj->mYPos),
            ReliveTypes::ePathTransition));
    }

    if (mAliveObj && pTlv)
    {
        mNextLevel = pTlv->mNextLevel;
        mNextPath = pTlv->mNextPath;
        mNextCamera = pTlv->mNextCamera;
        mFmvBaseId = pTlv->mMovieId;

        mCameraSwapEffect = kPathChangeEffectToInternalScreenChangeEffect[pTlv->mWipeEffect];

        mAliveObj->mCurrentLevel = pTlv->mNextLevel;
        mAliveObj->mCurrentPath = pTlv->mNextPath;

        // TODO: Probably OG bug, when changing camera/path the TLV pointer can become invalid
        // resulting in a corrupted next_path_scale value ?
        // Pointer points to the Path res which is invalid after ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, i, true, false);
        // is called. Happens even if calling real func below.
        const auto next_path_scale = pTlv->mNextPathScale;

        GoTo_Camera();

        switch (next_path_scale)
        {
            case relive::reliveScale::eFull:
                gAbe->SetSpriteScale(FP_FromInteger(1));
                gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_32);
                if (gElum)
                {
                    gElum->SetSpriteScale(gAbe->GetSpriteScale());
                    gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesElumMuds_28);
                }
                break;

            case relive::reliveScale::eHalf:
                gAbe->SetSpriteScale(FP_FromDouble(0.5));
                gAbe->GetAnimation().SetRenderLayer(Layer::eLayer_AbeMenu_Half_13);
                if (gElum)
                {
                    gElum->SetSpriteScale(gAbe->GetSpriteScale());
                    gElum->GetAnimation().SetRenderLayer(Layer::eLayer_ZapLinesMudsElum_Half_9);
                }
                break;

            default:
                LOG_ERROR("Invalid scale %d", static_cast<s16>(next_path_scale));
                break;
        }

        CameraPos remapped = CameraPos::eCamInvalid_m1;
        switch (mMapDirection)
        {
            case MapDirections::eMapLeft_0:
                remapped = CameraPos::eCamLeft_3;
                break;
            case MapDirections::eMapRight_1:
                remapped = CameraPos::eCamRight_4;
                break;
            case MapDirections::eMapTop_2:
                remapped = CameraPos::eCamTop_1;
                break;
            case MapDirections::eMapBottom_3:
                remapped = CameraPos::eCamBottom_2;
                break;
        }

        mAliveObj->VOnPathTransition(
            mPathData->field_C_grid_width * mCamIdxOnX,
            mPathData->field_E_grid_height * mCamIdxOnY,
            remapped);
    }
    else
    {
        switch (mMapDirection)
        {
            case MapDirections::eMapLeft_0:
                mCamIdxOnX--;
                if (mAliveObj)
                {
                    mAliveObj->VSetXSpawn(
                        mCamIdxOnX * mPathData->field_C_grid_width,
                        MaxGridBlocks(mAliveObj->GetSpriteScale()) - 1);
                }
                mCameraSwapEffect = CameraSwapEffects::eRightToLeft_2;
                break;
            case MapDirections::eMapRight_1:
                mCamIdxOnX++;
                if (mAliveObj)
                {
                    mAliveObj->VSetXSpawn(mCamIdxOnX * mPathData->field_C_grid_width,
                                                   1);
                }
                mCameraSwapEffect = CameraSwapEffects::eLeftToRight_1;
                break;
            case MapDirections::eMapTop_2:
                mCamIdxOnY--;
                if (mAliveObj)
                {
                    mAliveObj->VSetYSpawn(mCamIdxOnY * mPathData->field_E_grid_height,
                                                   1);
                }
                mCameraSwapEffect = CameraSwapEffects::eBottomToTop_4;
                break;
            case MapDirections::eMapBottom_3:
                mCamIdxOnY++;
                if (mAliveObj)
                {
                    mAliveObj->VSetYSpawn(mCamIdxOnY * mPathData->field_E_grid_height,
                                                   2);
                }
                mCameraSwapEffect = CameraSwapEffects::eTopToBottom_3;
                break;
            default:
                break;
        }

        const BinaryPath* pPathRes = GetPathResourceBlockPtr(mCurrentPath);
        auto pCameraName = pPathRes->CameraName(mCamIdxOnX, mCamIdxOnY);

        // Convert the 2 digit camera number string to an integer
        mNextCamera = 1 * (pCameraName[7] - '0') + 10 * (pCameraName[6] - '0');

        GoTo_Camera();
    }
}

CameraPos Map::GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos)
{
    if (level != mCurrentLevel)
    {
        return CameraPos::eCamInvalid_m1;
    }

    if (path != mCurrentPath)
    {
        return CameraPos::eCamInvalid_m1;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);

    CameraPos ret = Rect_Location_Relative_To_Active_Camera(&rect, 0);

    PSX_RECT camWorldRect = {};
    if (!Get_Camera_World_Rect(ret, &camWorldRect))
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
            if (ypos < y || xpos < x || xpos > w)
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



void Map::GoTo_Camera()
{
    s16 bShowLoadingIcon = false;

    // NOTE: None check changed to match AE
    if (mCurrentLevel != EReliveLevelIds::eMenu && mCurrentLevel != EReliveLevelIds::eCredits && mCurrentLevel != EReliveLevelIds::eNone)
    {
        bShowLoadingIcon = true;
    }

    if (mCameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        CamResource nullRes;
        BaseGameObject* pFmvRet = FMV_Camera_Change(nullRes, this, mCurrentLevel);
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            SYS_EventsPump();

            BaseGameObject* pBaseGameObj = gBaseGameObjects->ItemAt(i);
            if (!pBaseGameObj)
            {
                break;
            }

            if (pBaseGameObj->GetDead() && pBaseGameObj->mBaseGameObjectRefCount == 0)
            {
                i = gBaseGameObjects->RemoveAt(i);
                relive_delete pBaseGameObj;
                if (pBaseGameObj == pFmvRet)
                {
                    // FMV trans done
                    break;
                }
            }
            else if (pBaseGameObj->GetUpdatable())
            {
                if (!pBaseGameObj->GetDead() && (!gNumCamSwappers || pBaseGameObj->GetUpdateDuringCamSwap()))
                {
                    if (pBaseGameObj->UpdateDelay() > 0)
                    {
                        pBaseGameObj->SetUpdateDelay(pBaseGameObj->UpdateDelay() - 1);
                    }
                    else
                    {
                        pBaseGameObj->VUpdate();
                    }
                }
            }
        }
    }

    // NOTE: None check changed to match AE
    if (mCurrentLevel != EReliveLevelIds::eMenu && mCurrentLevel != EReliveLevelIds::eNone)
    {
        if (LevelChanged() || (PathChanged() && mCameraSwapEffect == CameraSwapEffects::ePlay1FMV_5))
        {
            ResourceManagerWrapper::ShowLoadingIcon();
        }
    }

    if (LevelChanged() || PathChanged())
    {
        mOverlayId = GetOverlayId();
    }

    if (LevelChanged())
    {
        ResourceManagerWrapper::LoadingLoop(bShowLoadingIcon);

        // Free all cameras
        for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
        {
            if (field_2C_camera_array[i])
            {
                relive_delete field_2C_camera_array[i];
                field_2C_camera_array[i] = nullptr;
            }
        }

        if (mCurrentLevel != EReliveLevelIds::eNone)
        {
            if (LevelChanged())
            {
                SND_Reset_476BA0();
                FreePathResourceBlocks();
            }

        }

        ResourceManagerWrapper::LoadingLoop(bShowLoadingIcon);

        if (LevelChanged())
        {
            mLoadedPaths = ResourceManagerWrapper::LoadPaths(mNextLevel);
        }

        SND_Load_VABS(mLoadedPaths[0]->GetSoundInfo(), AO::Path_Get_Reverb(mNextLevel)); // TODO: Remove hard coded data
        SND_Load_Seqs_477AB0(g_SeqTable_4C9E70, mLoadedPaths[0]->GetSoundInfo());

        relive_new BackgroundMusic(AO::Path_Get_BackGroundMusicId(mNextLevel)); // TODO: Remove hard coded data

        // TODO: Re-add function
        for (s32 i = 0; i < 236; i++)
        {
            gSwitchStates.mData[i] = 0;
        }

        if (mFreeAllAnimAndPalts)
        {
            mFreeAllAnimAndPalts = false;
        }
    }

    if (!mNextPath)
    {
        mNextPath = 1;
    }


    const auto old_current_path = mCurrentPath;
    const auto old_current_level = mCurrentLevel;

    mMapChanged = mNextPath != old_current_path || LevelChanged();

    mCurrentCamera = mNextCamera;
    mCurrentPath = mNextPath;
    mCurrentLevel = mNextLevel;

    const PathBlyRec* pPathRecord = AO::Path_Get_Bly_Record(mNextLevel, mNextPath);
    mPathData = pPathRecord->field_4_pPathData;
    mMaxCamsX = (mPathData->field_8_bTop - mPathData->field_4_bLeft) / mPathData->field_C_grid_width;
    mMaxCamsY = (mPathData->field_A_bBottom - mPathData->field_6_bRight) / mPathData->field_E_grid_height;

    char_type camNameBuffer[20] = {};
    AO::Path_Format_CameraName(camNameBuffer, mNextLevel, mNextPath, mNextCamera);

    mCamIdxOnX = 0;
    mCamIdxOnY = 0;

    BinaryPath* pNextPath = GetPathResourceBlockPtr(mNextPath);
    for (auto& cam : pNextPath->GetCameras())
    {
        if (!strncmp(cam->mName.c_str(), camNameBuffer, sizeof(CameraName)))
        {
            mCamIdxOnX = static_cast<s16>(cam->mX);
            mCamIdxOnY = static_cast<s16>(cam->mY);
            break;
        }
    }


    mCameraOffset.x = FP_FromInteger(mCamIdxOnX * mPathData->field_C_grid_width + 440);
    mCameraOffset.y = FP_FromInteger(mCamIdxOnY * mPathData->field_E_grid_height + 240);

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (gCollisions)
        {
            // OG FIX: Remove any pointers to the line objects that we are about to delete
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                ::BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->GetIsBaseAliveGameObject())
                {
                    auto pBaseAliveGameObj = static_cast<BaseAliveGameObject*>(pObjIter);
                    pBaseAliveGameObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }

            if (gPlatformsArray->Size() > 0)
            {
                ALIVE_FATAL("%d Platforms have been leaked!", gPlatformsArray->Size());
            }

            relive_delete gCollisions;
        }

        gCollisions = relive_new Collisions(GetPathResourceBlockPtr(mCurrentPath)->GetCollisions());
    }

    if (mSaveData)
    {
        RestoreBlyData(mSaveData);
        mSaveData = nullptr;
    }

    // Copy camera array and blank out the source
    for (s32 i = 0; i < ALIVE_COUNTOF(field_2C_camera_array); i++)
    {
        field_40_stru_5[i] = field_2C_camera_array[i];
        field_2C_camera_array[i] = nullptr;
    }

    field_2C_camera_array[0] = Create_Camera(mCamIdxOnX, mCamIdxOnY, 1);
    field_2C_camera_array[3] = Create_Camera(mCamIdxOnX - 1, mCamIdxOnY, 0);
    field_2C_camera_array[4] = Create_Camera(mCamIdxOnX + 1, mCamIdxOnY, 0);
    field_2C_camera_array[1] = Create_Camera(mCamIdxOnX, mCamIdxOnY - 1, 0);
    field_2C_camera_array[2] = Create_Camera(mCamIdxOnX, mCamIdxOnY + 1, 0);

    // Free resources for each camera
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i])
        {
            //ResourceManager::Free_Resources_For_Camera_447170(field_40_stru_5[i]);
        }
    }

    ResourceManagerWrapper::LoadingLoop(bShowLoadingIcon);

    // Free each camera itself
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i])
        {
            relive_delete field_40_stru_5[i];
            field_40_stru_5[i] = nullptr;
        }
    }

    Load_Path_Items(field_2C_camera_array[0], LoadMode::ConstructObject_0);
    ResourceManagerWrapper::LoadingLoop(bShowLoadingIcon);
    Load_Path_Items(field_2C_camera_array[3], LoadMode::ConstructObject_0);
    Load_Path_Items(field_2C_camera_array[4], LoadMode::ConstructObject_0);
    Load_Path_Items(field_2C_camera_array[1], LoadMode::ConstructObject_0);
    Load_Path_Items(field_2C_camera_array[2], LoadMode::ConstructObject_0);

    if (!gScreenManager)
    {
        gScreenManager = relive_new ScreenManager(field_2C_camera_array[0]->mCamRes, &mCameraOffset);
    }

    Loader(mCamIdxOnX, mCamIdxOnY, LoadMode::ConstructObject_0, ReliveTypes::eNone); // none = load all

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (gAbe && mCurrentPath == gAbe->mCurrentPath)
        {
            gAbe->VCheckCollisionLineStillValid(10);
        }

        if (gElum && sControlledCharacter != gElum && mCurrentPath == gElum->mCurrentPath)
        {
            gElum->VCheckCollisionLineStillValid(10);
        }
    }

    Create_FG1s();

    if (mCameraSwapEffect == CameraSwapEffects::ePlay1FMV_5)
    {
        FMV_Camera_Change(field_2C_camera_array[0]->mCamRes, this, mNextLevel);
    }

    if (mCameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        gScreenManager->DecompressCameraToVRam(field_2C_camera_array[0]->mCamRes);
        gScreenManager->EnableRendering();
    }

    if (mCameraSwapEffect != CameraSwapEffects::ePlay1FMV_5 && mCameraSwapEffect != CameraSwapEffects::eUnknown_11)
    {
        if (field_1E_door)
        {
            relive::Path_Door* pTlvIter = static_cast<relive::Path_Door*>(TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0));
            while (pTlvIter->mDoorId != gAbe->field_196_door_id)
            {
                pTlvIter = static_cast<relive::Path_Door*>(Path_TLV::TLV_Next_Of_Type_446500(pTlvIter, ReliveTypes::eDoor));
            }

            const auto pCamPos = gScreenManager->mCamPos;
            const auto xpos = gScreenManager->mCamXOff + ((pTlvIter->mTopLeftX + pTlvIter->mBottomRightX) / 2) - FP_GetExponent(pCamPos->x);
            const auto ypos = gScreenManager->mCamYOff + pTlvIter->mTopLeftY - FP_GetExponent(pCamPos->y);
            relive_new CameraSwapper(
                field_2C_camera_array[0]->mCamRes,
                mCameraSwapEffect,
                static_cast<s16>(xpos),
                static_cast<s16>(ypos));
        }
        else
        {
            relive_new CameraSwapper(field_2C_camera_array[0]->mCamRes, mCameraSwapEffect, 184, 120);
        }
    }
    ResourceManagerWrapper::bHideLoadingIcon = 0;
    ResourceManagerWrapper::loading_ticks = 0;
}

s16 Map::GetOverlayId()
{
    return AO::Path_Get_Bly_Record(mNextLevel, mNextPath)->mOverlayId;
}

relive::Path_TLV* Map::Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx)
{
    const auto camX = cam_x_idx + mCamIdxOnX;
    const auto camY = cam_y_idx + mCamIdxOnY;

    if (camX >= mMaxCamsX || camX < 0 || camY >= mMaxCamsY || camY < 0)
    {
        return nullptr;
    }

    BinaryPath* pPathData = GetPathResourceBlockPtr(mCurrentPath);
    return pPathData->TlvsForCamera(camX, camY);
}

void Map::Create_FG1s()
{
    Camera* pCamera = field_2C_camera_array[0];
    pCamera->CreateFG1();
}

void Map::SaveBlyData(u8* pSaveBuffer)
{
    memcpy(pSaveBuffer, gSwitchStates.mData, sizeof(gSwitchStates.mData));

    u8* pAfterSwitchStates = pSaveBuffer + sizeof(gSwitchStates.mData);
    for (s16 i = 1; i < AO::Path_Get_Num_Paths(mCurrentLevel); i++)
    {
        const PathBlyRec* pPathRec = AO::Path_Get_Bly_Record(mCurrentLevel, i);
        if (pPathRec->field_0_blyName)
        {
            BinaryPath* ppPathRes = GetPathResourceBlockPtr(i); // TODO: Is this actually the id ??

            for (auto& cam : ppPathRes->GetCameras())
            {
                relive::Path_TLV* pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
                while (pTlv)
                {
                    BitField8<relive::TlvFlags> flags = pTlv->mTlvFlags;
                    if (flags.Get(relive::eBit1_Created))
                    {
                        flags.Clear(relive::eBit1_Created);
                        flags.Clear(relive::eBit2_Destroyed);
                    }

                    // Save the flags
                    *pAfterSwitchStates = flags.Raw().all;
                    pAfterSwitchStates++;
                    *pAfterSwitchStates = pTlv->mTlvSpecificMeaning;
                    pAfterSwitchStates++;

                    if (pTlv->mTlvFlags.Get(relive::eBit3_End_TLV_List))
                    {
                        break;
                    }

                    pTlv = Path_TLV::Next_446460(pTlv);
                }
            }
        }
    }
}

void Map::TLV_Reset(const Guid& tlvId, s16 hiFlags)
{
    Path::TLV_Reset(tlvId, hiFlags);
}

void Map::TLV_Persist(const Guid& tlvId, s16 hiFlags)
{
    Path::TLV_Persist(tlvId, hiFlags);
}

void Map::TLV_Delete(const Guid& tlvId, s16 hiFlags)
{
    Path::TLV_Delete(tlvId, hiFlags);
}

void Map::Set_TLVData(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    Path::Set_TLVData(tlvId, hiFlags, bSetCreated, bSetDestroyed);
}

void Map::RestoreBlyData(const u8* pSaveData)
{
    memcpy(gSwitchStates.mData, pSaveData, sizeof(gSwitchStates.mData));
    const u8* pAfterSwitchStates = pSaveData + sizeof(gSwitchStates.mData);

    for (s16 i = 1; i < AO::Path_Get_Num_Paths(mCurrentLevel); i++)
    {
        BinaryPath* ppPathRes = GetPathResourceBlockPtr(i);
        if (ppPathRes)
        {
            const PathBlyRec* pPathRec = AO::Path_Get_Bly_Record(mCurrentLevel, i);
            if (pPathRec->field_0_blyName)
            {
                for (auto& cam : ppPathRes->GetCameras())
                {
                    relive::Path_TLV* pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
                    while (pTlv)
                    {
                        pTlv->mTlvFlags.Raw().all = *pAfterSwitchStates;
                        pAfterSwitchStates++;

                        pTlv->mTlvSpecificMeaning = *pAfterSwitchStates;
                        pAfterSwitchStates++;
                        if (pTlv->mTlvFlags.Get(relive::eBit3_End_TLV_List))
                        {
                            break;
                        }

                        pTlv = Path_TLV::Next_NoCheck(pTlv);
                        if (pTlv->mLength == 0)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

void Map::Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    BinaryPath* pPathData = GetPathResourceBlockPtr(mCurrentPath);

    // TODO: Shouldn't really need to depend on these
    const s32 cam_global_left = mPathData->field_C_grid_width * cam_x_idx;
    const s32 cam_global_right = cam_global_left + mPathData->field_C_grid_width;

    const s32 cam_y_grid_top = mPathData->field_E_grid_height * cam_y_idx;
    const s32 cam_y_grid_bottom = cam_y_grid_top + mPathData->field_E_grid_height;

    for (auto& cam : pPathData->GetCameras())
    {
        relive::Path_TLV* pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
        // Enumerate the TLVs
        while (pTlv)
        {
            if (pTlv->mTopLeftX >= cam_global_left && pTlv->mTopLeftX <= cam_global_right)
            {
                if (pTlv->mTopLeftY >= cam_y_grid_top && pTlv->mTopLeftY <= cam_y_grid_bottom && (!pTlv->mTlvFlags.Get(relive::eBit1_Created) && !pTlv->mTlvFlags.Get(relive::eBit2_Destroyed)))
                {
                    Start_Sounds_for_TLV(direction, pTlv);
                }
            }

            if (pTlv->mTlvFlags.Get(relive::eBit3_End_TLV_List))
            {
                break;
            }

            pTlv = Path_TLV::Next_NoCheck(pTlv);
        }
    }
}

void Map::Start_Sounds_For_Objects_In_Near_Cameras()
{
    SND_Reset_Ambiance();

    if (Get_Camera_World_Rect(CameraPos::eCamLeft_3, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamLeft_3, mCamIdxOnX - 1, mCamIdxOnY);
    }

    if (Get_Camera_World_Rect(CameraPos::eCamRight_4, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamRight_4, mCamIdxOnX + 1, mCamIdxOnY);
    }

    if (Get_Camera_World_Rect(CameraPos::eCamTop_1, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamTop_1, mCamIdxOnX, mCamIdxOnY - 1);
    }

    if (Get_Camera_World_Rect(CameraPos::eCamBottom_2, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamBottom_2, mCamIdxOnX, mCamIdxOnY + 1);
    }
}

s16 Map::Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width)
{
    if (level != mCurrentLevel || path != mCurrentPath) // TODO: Remove when 100%
    {
        return false;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);
    return Rect_Location_Relative_To_Active_Camera(&rect, width) == CameraPos::eCamCurrent_0;
}

s16 Map::Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect)
{
    if (camIdx < CameraPos::eCamCurrent_0 || camIdx > CameraPos::eCamRight_4)
    {
        return 0;
    }

    Camera* pCamera = field_2C_camera_array[static_cast<s32>(camIdx)];
    if (!pCamera)
    {
        return 0;
    }

    if (!pRect)
    {
        return 1;
    }

    s16 cam_x_pos = mPathData->field_C_grid_width * pCamera->mCamXOff;
    cam_x_pos += 120;

    const s16 cam_y_pos = mPathData->field_E_grid_height * pCamera->mCamYOff;

    pRect->x = cam_x_pos;
    pRect->y = cam_y_pos + 120;
    pRect->w = cam_x_pos + 640;
    pRect->h = cam_y_pos + 360;
    return 1;
}

CameraPos Map::Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width)
{
    if (EventGet(kEventDeathReset))
    {
        return CameraPos::eCamNone_5;
    }

    FP xTweak = {};
    FP yTweak = {};
    if (width)
    {
        xTweak = FP_FromInteger(234);
        yTweak = FP_FromInteger(150);
    }
    else
    {
        xTweak = FP_FromInteger(184);
        yTweak = FP_FromInteger(120);
    }

    if (pRect->x > FP_GetExponent(mCameraOffset.x + xTweak))
    {
        return CameraPos::eCamRight_4;
    }

    if (pRect->y > FP_GetExponent(mCameraOffset.y + yTweak))
    {
        return CameraPos::eCamBottom_2;
    }

    if (pRect->w >= FP_GetExponent(mCameraOffset.x - xTweak))
    {
        if (pRect->h < FP_GetExponent(mCameraOffset.y - yTweak))
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

relive::Path_TLV* Map::VTLV_Get_At_Of_Type(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes typeToFind)
{
    s32 right = 0;
    s32 left = 0;
    if (xpos >= width)
    {
        right = width;
        left = xpos;
    }
    else
    {
        right = xpos;
        left = width;
    }

    s32 top = 0;
    s32 bottom = 0;
    if (ypos >= height)
    {
        top = height;
        bottom = ypos;
    }
    else
    {
        top = ypos;
        bottom = height;
    }

    const s32 grid_cell_y = top / mPathData->field_E_grid_height;
    const s32 grid_cell_x = (right / mPathData->field_C_grid_width);

    // Check within map bounds
    if (grid_cell_x >= mMaxCamsX)
    {
        return nullptr;
    }

    if (grid_cell_y >= mMaxCamsY)
    {
        return nullptr;
    }

    // Get the offset to where the TLV list starts for this camera cell
    BinaryPath* pBinPath = GetPathResourceBlockPtr(mCurrentPath);
    relive::Path_TLV* pTlvIter = pBinPath->TlvsForCamera(grid_cell_x, grid_cell_y);
    if (!pTlvIter)
    {
        return nullptr;
    }

    while (right > pTlvIter->mBottomRightX
           || left < pTlvIter->mTopLeftX
           || bottom < pTlvIter->mTopLeftY
           || top > pTlvIter->mBottomRightY
           || pTlvIter->mTlvType != typeToFind)
    {
        if (pTlvIter->mTlvFlags.Get(relive::eBit3_End_TLV_List))
        {
            return nullptr;
        }

        pTlvIter = Path_TLV::Next_446460(pTlvIter);
    }
    return pTlvIter;
}

relive::Path_TLV* Map::TLV_Get_At(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height)
{
    bool bContinue = true;

    const auto xpos_converted = FP_GetExponent(xpos);
    const auto ypos_converted = FP_GetExponent(ypos);
    auto width_converted = FP_GetExponent(width);
    auto height_converted = FP_GetExponent(height);

    if (xpos_converted < 0 || ypos_converted < 0)
    {
        bContinue = false;
    }

    if (width_converted < 0 || height_converted < 0)
    {
        width_converted = FP_GetExponent(xpos);
        height_converted = FP_GetExponent(ypos);
    }

    if (!pTlv)
    {
        const PathData* pPathData = mPathData;

        const auto camX = xpos_converted / pPathData->field_C_grid_width;
        const auto camY = ypos_converted / pPathData->field_E_grid_height;

        if (camX >= mMaxCamsX || camY >= mMaxCamsY)
        {
            return nullptr;
        }

        if (camX < 0 || camY < 0)
        {
            return nullptr;
        }

        BinaryPath* pBinPath = GetPathResourceBlockPtr(mCurrentPath);
        pTlv = pBinPath->TlvsForCamera(camX, camY);
        if (!pTlv)
        {
            return nullptr;
        }

        if (!bContinue || (xpos_converted <= pTlv->mBottomRightX && width_converted >= pTlv->mTopLeftX && height_converted >= pTlv->mTopLeftY && ypos_converted <= pTlv->mBottomRightY))
        {
            return pTlv;
        }
    }

    if (pTlv->mTlvFlags.Get(relive::eBit3_End_TLV_List))
    {
        return nullptr;
    }

    while (1)
    {
        pTlv = Path_TLV::Next_446460(pTlv);

        if (!bContinue || (xpos_converted <= pTlv->mBottomRightX && width_converted >= pTlv->mTopLeftX && height_converted >= pTlv->mTopLeftY && ypos_converted <= pTlv->mBottomRightY))
        {
            break;
        }

        if (pTlv->mTlvFlags.Get(relive::eBit3_End_TLV_List))
        {
            return 0;
        }
    }
    return pTlv;
}

void Map::ResetPathObjects(u16 pathNum)
{
    BinaryPath* pPathRes = GetPathResourceBlockPtr(pathNum);
    for (auto& cam : pPathRes->GetCameras())
    {
        auto pTlv = reinterpret_cast<relive::Path_TLV*>(cam->mBuffer.data());
        if (pTlv)
        {
            pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
            pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
            while (!pTlv->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
            {
                pTlv = Path_TLV::Next_NoCheck(pTlv);

                if (pTlv->mLength == 0)
                {
                    break;
                }

                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit1_Created);
                pTlv->mTlvFlags.Clear(relive::TlvFlags::eBit2_Destroyed);
            }
        }
    }
}

relive::Path_TLV* Map::TLV_First_Of_Type_In_Camera(ReliveTypes type, s32 camX)
{
    relive::Path_TLV* pTlvIter = Get_First_TLV_For_Offsetted_Camera(static_cast<s16>(camX), 0);
    if (!pTlvIter)
    {
        return nullptr;
    }

    while (pTlvIter->mTlvType != type)
    {
        pTlvIter = Path_TLV::Next_446460(pTlvIter);
        if (!pTlvIter)
        {
            return nullptr;
        }
    }

    return pTlvIter;
}

void Map::Load_Path_Items(Camera* pCamera, LoadMode loadMode)
{
    if (!pCamera)
    {
        return;
    }

    // Is camera resource loaded check
    if (!pCamera->mCamResLoaded)
    {
        if (loadMode == LoadMode::ConstructObject_0)
        {
            // Async camera load
            /*
            ResourceManager::LoadResourceFile(
                pCamera->field_1E_fileName,
                Camera::On_Loaded,
                pCamera,
                pCamera);*/

            pCamera->mCamRes = ResourceManagerWrapper::LoadCam(pCamera->mLevel, pCamera->mPath, pCamera->mCamera);
            Loader(pCamera->mCamXOff, pCamera->mCamYOff, LoadMode::LoadResourceFromList_1, ReliveTypes::eNone); // none = load all
        }
        else
        {
            // Blocking camera load
            /*
            ResourceManager::LoadResourceFile_455270(pCamera->field_1E_fileName, pCamera);
            pCamera->field_C_ppBits = ResourceManager::GetLoadedResource(ResourceManager::Resource_Bits, pCamera->field_10_resId, 1, 0);
            */

            pCamera->mCamResLoaded = true;

            Loader(pCamera->mCamXOff, pCamera->mCamYOff, LoadMode::LoadResource_2, ReliveTypes::eNone); // none = load all
        }
    }
}

Camera* Map::Create_Camera(s16 xpos, s16 ypos, s32 /*a4*/)
{
    // Check min bound
    if (xpos < 0 || ypos < 0)
    {
        return nullptr;
    }

    // Check max bounds
    if (xpos >= mMaxCamsX || ypos >= mMaxCamsY)
    {
        return nullptr;
    }

    // Return existing camera if we already have one
    for (s32 i = 0; i < ALIVE_COUNTOF(field_40_stru_5); i++)
    {
        if (field_40_stru_5[i]
            && field_40_stru_5[i]->mLevel == mCurrentLevel
            && field_40_stru_5[i]->mPath == mCurrentPath
            && field_40_stru_5[i]->mCamXOff == xpos
            && field_40_stru_5[i]->mCamYOff == ypos)
        {
            auto pTemp = field_40_stru_5[i];
            field_40_stru_5[i] = nullptr;
            return pTemp;
        }
    }

    // Get a pointer to the camera name from the Path resource
    BinaryPath* pPathData = GetPathResourceBlockPtr(mCurrentPath);
    auto pCamName = pPathData->CameraName(xpos, ypos);

    // Empty/blank camera in the map array
    if (!pCamName || !pCamName[0])
    {
        return nullptr;
    }

    auto newCamera = relive_new Camera();

    // Copy in the camera name from the Path resource and append .CAM
    memset(newCamera->mCamName, 0, sizeof(newCamera->mCamName));
    strncpy(newCamera->mCamName, pCamName, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->mCamName, ".CAM");

    newCamera->mCamXOff = xpos;
    newCamera->mCamYOff = ypos;

    newCamera->mCamResLoaded = false;

    newCamera->mLevel = mCurrentLevel;
    newCamera->mPath = mCurrentPath;

    // Calculate hash/resource ID of the camera
    //newCamera->field_10_resId = 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0') + 100 * (pCamName[4] - '0') + 1000 * (pCamName[3] - '0');

    //newCamera->field_1C = mCurrentCamera;

    // Convert the 2 digit camera number string to an integer
    newCamera->mCamera = 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0');

    return newCamera;
}


void Map::FreePathResourceBlocks()
{
    mLoadedPaths.clear();
}

BinaryPath* Map::GetPathResourceBlockPtr(u32 pathId)
{
    for (auto& loadedPath : mLoadedPaths)
    {
        if (loadedPath->GetPathId() == pathId)
        {
            return loadedPath.get();
        }
    }
    return nullptr;
}

void Map::ClearPathResourceBlocks()
{
    mLoadedPaths.clear();
}


void Map::Loader(s16 camX, s16 camY, LoadMode loadMode, ReliveTypes typeToLoad)
{
    // Get TLVs for this cam
    BinaryPath* pPathRes = GetPathResourceBlockPtr(mCurrentPath);
    relive::Path_TLV* pPathTLV = pPathRes->TlvsForCamera(camX, camY);
    if (!pPathTLV)
    {
        return;
    }

    s32 tlvOffset = 0;
    while (1)
    {
        if (typeToLoad == ReliveTypes::eNone || typeToLoad == pPathTLV->mTlvType)
        {
            if (loadMode != LoadMode::ConstructObject_0 || !(pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit1_Created) || pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit2_Destroyed)))
            {
                // Call the factory to construct the item
                ConstructTLVObject(pPathTLV, this, pPathTLV->mId, loadMode);

                if (loadMode == LoadMode::ConstructObject_0)
                {
                    pPathTLV->mTlvFlags.Set(relive::TlvFlags::eBit1_Created);
                    pPathTLV->mTlvFlags.Set(relive::TlvFlags::eBit2_Destroyed);
                }
            }
        }

        // End of TLV list for current camera
        if (pPathTLV->mTlvFlags.Get(relive::TlvFlags::eBit3_End_TLV_List))
        {
            break;
        }

        tlvOffset += pPathTLV->mLength;
        pPathTLV = Path_TLV::Next_446460(pPathTLV);
    }
}

CameraSwapper* Map::FMV_Camera_Change(CamResource& ppBits, Map* pMap, EReliveLevelIds levelId)
{
    // This is required to make the movies work when abe completes scrabania and paramonia.
    // u16 41617 -> s16 -23919 -> u16 41617
    // TODO: should probably use the s32 type for every function parameter with a fmv base id
    u16 fmvBaseId = pMap->mFmvBaseId;

    if (fmvBaseId > 10000u)
    {
        FmvInfo* pFmvRec1 = AO::Path_Get_FMV_Record(levelId, fmvBaseId / 10000);
        FmvInfo* pFmvRec2 = AO::Path_Get_FMV_Record(levelId, fmvBaseId % 100);
        FmvInfo* pFmvRec3 = AO::Path_Get_FMV_Record(levelId, fmvBaseId / 100 % 100);

        if (pFmvRec1->field_8_stop_music || pFmvRec2->field_8_stop_music || pFmvRec3->field_8_stop_music)
        {
            BackgroundMusic::Stop();
            MusicController::EnableMusic(0);
        }

        return relive_new CameraSwapper(
            ppBits,
            pFmvRec1->field_A == 1,
            pFmvRec1->field_0_pName,
            pFmvRec2->field_A == 1,
            pFmvRec2->field_0_pName,
            pFmvRec3->field_A == 1,
            pFmvRec3->field_0_pName);
    }
    else if (fmvBaseId > 100u)
    {
        // Double FMV
        FmvInfo* pFmvRec1 = AO::Path_Get_FMV_Record(levelId, fmvBaseId / 100);
        FmvInfo* pFmvRec2 = AO::Path_Get_FMV_Record(levelId, fmvBaseId % 100);
        if (pFmvRec1->field_8_stop_music || pFmvRec2->field_8_stop_music)
        {
            BackgroundMusic::Stop();
            MusicController::EnableMusic(0);
        }

        return relive_new CameraSwapper(
            ppBits,
            pFmvRec1->field_A == 1,
            pFmvRec1->field_0_pName,
            pFmvRec2->field_A == 1,
            pFmvRec2->field_0_pName);
    }
    else // < 100
    {
        // Single FMV
        FmvInfo* pFmvRecord = AO::Path_Get_FMV_Record(levelId, fmvBaseId);
        if (pFmvRecord->field_8_stop_music)
        {
            BackgroundMusic::Stop();
            MusicController::EnableMusic(0);
        }

        return relive_new CameraSwapper(
            ppBits,
            pFmvRecord->field_A == 1,
            pFmvRecord->field_0_pName);
    }
}

void Map::GetCurrentCamCoords(PSX_Point* pPoint)
{
    pPoint->x = mPathData->field_C_grid_width * mCamIdxOnX;
    pPoint->y = mPathData->field_E_grid_height * mCamIdxOnY;
}

void Map::Get_map_size(PSX_Point* pPoint)
{
    pPoint->x = mPathData->field_8_bTop;
    pPoint->y = mPathData->field_A_bBottom;
}

relive::Path_TLV* Path_TLV::Next_446460(relive::Path_TLV* pTlv)
{
    return Next(pTlv);
}

relive::Path_TLV* Path_TLV::TLV_Next_Of_Type_446500(relive::Path_TLV* pTlv, ReliveTypes type)
{
    pTlv = Path_TLV::Next_446460(pTlv);
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->mTlvType != type)
    {
        pTlv = Path_TLV::Next_446460(pTlv);
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}

} // namespace AO
