#include "stdafx_ao.h"
#include "Map.hpp"
#include "Function.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "PathData.hpp"
#include "Psx.hpp"
#include "Game.hpp"
#include "Midi.hpp"
#include "BaseAliveGameObject.hpp"
#include "Door.hpp"
#include "Abe.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "FixedPoint.hpp"
#include "AmbientSound.hpp"
#include "FG1.hpp"
#include "Movie.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "CameraSwapper.hpp"
#include "Particle.hpp"
#include "LvlArchive.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Elum.hpp"
#include "Sys.hpp"
#include "PlatformBase.hpp"
#include "Camera.hpp"

#include "../relive_lib/data_conversion/file_system.hpp"
#include "../relive_lib/data_conversion/data_conversion.hpp" // TODO: don't include this in the engine
#include "../relive_lib/data_conversion/relive_tlvs.hpp"
#include "../relive_lib/data_conversion/relive_tlvs_serialization.hpp"


class BaseGameObject;

namespace AO {


void Map_ForceLink()
{ }

ALIVE_VAR(1, 0x507C98, Camera*, sCameraBeingLoaded_507C98, nullptr);

const OverlayRecords kOverlays = {
    {{"\\S1.OVL;1", 5205u},
     {"\\S1.LVL;1", 5207u},
     {"\\S1.MOV;1", 6228u},
     {"\\R1.OVL;1", 46992u},
     {"\\R1.LVL;1", 47042u},
     {"\\R1.MOV;1", 50938u},
     {"\\E1.OVL;1", 109292u},
     {"\\E1.LVL;1", 109358u},
     {"\\E3.OVL;1", 109292u},
     {"\\E4.OVL;1", 109292u},
     {"\\E1.MOV;1", 112254u},
     {"\\L1.OVL;1", 113848u},
     {"\\L1.LVL;1", 113913u},
     {"\\L2.OVL;1", 113848u},
     {"\\L1.MOV;1", 116902u},
     {"\\F1.OVL;1", 123206u},
     {"\\F1.LVL;1", 123267u},
     {"\\F1.MOV;1", 126488u},
     {"\\F2.OVL;1", 140523u},
     {"\\F2.LVL;1", 140605u},
     {"\\F3.OVL;1", 140523u},
     {"\\F2.MOV;1", 143702u},
     {"\\F4.OVL;1", 140523u},
     {"\\F4.LVL;1", 140523u},
     {"\\D1.OVL;1", 149982u},
     {"\\D1.LVL;1", 150044u},
     {"\\D3.OVL;1", 149982u},
     {"\\D5.OVL;1", 149982u},
     {"\\D6.OVL;1", 149982u},
     {"\\D1.MOV;1", 153189u},
     {"\\D2.OVL;1", 164493u},
     {"\\D2.LVL;1", 164554u},
     {"\\D4.OVL;1", 164493u},
     {"\\D2.MOV;1", 167451u},
     {"\\D7.OVL;1", 164493u},
     {"\\D7.LVL;1", 164493u},
     {"\\E2.OVL;1", 174640u},
     {"\\E2.LVL;1", 174685u},
     {"\\E2.MOV;1", 177186u},
     {"\\R2.OVL;1", 178780u},
     {"\\R2.LVL;1", 178850u},
     {"\\R3.OVL;1", 178780u},
     {"\\R4.OVL;1", 178780u},
     {"\\R5.OVL;1", 178780u},
     {"\\R7.OVL;1", 178780u},
     {"\\R8.OVL;1", 178780u},
     {"\\R9.OVL;1", 178780u},
     {"\\RA.OVL;1", 178780u},
     {"\\R2.MOV;1", 182667u},
     {"\\R6.OVL;1", 178780u},
     {"\\R6.LVL;1", 178780u},
     {"\\C1.OVL;1", 178780u},
     {"\\C1.LVL;1", 178850u},
     {"\\C1.MOV;1", 182667u}}};

ALIVE_VAR(1, 0x4C5AA8, OverlayRecords, sOverlayTable_4C5AA8, kOverlays);


struct OpenSeqHandleAO final
{
    const s8* field_0_mBsqName;
    s32 field_4_generated_res_id;
    s16 field_8_sound_block_idx;
    s16 field_9_volume;
    u8* field_C_ppSeq_Data;
    s16 field_A_id_seqOpenId;
};
ALIVE_ASSERT_SIZEOF(OpenSeqHandleAO, 0x14);

struct OpenSeqHandleAE final // Same as ::OpenSeqHandle
{
    const char_type* field_0_mBsqName;
    s32 field_4_generated_res_id; // A hash of the named which matches the resource Id
    s8 field_8_sound_block_idx;
    s8 field_9_volume;
    s16 field_A_id_seqOpenId;
    u8* field_C_ppSeq_Data;
};
ALIVE_ASSERT_SIZEOF(OpenSeqHandleAE, 0x10);

// WARNING: The AO and AE OpenSeqHandle are not memory layout compatible
// since we use the AE funcs for sound we must use the AE definition here
OpenSeqHandleAE g_SeqTable_4C9E70[165] = {
    {"D1AMB.SEQ", 0, 0, 100, -1, nullptr},
    {"D2AMB.SEQ", 0, 0, 100, -1, nullptr},
    {"E1AMB.SEQ", 0, 0, 60, -1, nullptr},
    {"E2AMB.SEQ", 0, 0, 60, -1, nullptr},
    {"E2AMB2.SEQ", 0, 0, 60, -1, nullptr},
    {"F1AMB.SEQ", 0, 0, 60, -1, nullptr},
    {"F2AMB.SEQ", 0, 0, 100, -1, nullptr},
    {"MLAMB.SEQ", 0, 0, 60, -1, nullptr},
    {"RFAMB.SEQ", 0, 0, 70, -1, nullptr},
    {"OPTAMB.SEQ", 0, 0, 100, -1, nullptr},
    {"GUN.SEQ", 0, 0, 40, -1, nullptr},
    {"OHM.SEQ", 0, 0, 80, -1, nullptr},
    {"MUDOHM.SEQ", 0, 0, 80, -1, nullptr},
    {"EBELL2.SEQ", 0, 0, 127, -1, nullptr},
    {"ABEMOUNT.SEQ", 0, 0, 85, -1, nullptr},
    {"ESCRATCH.SEQ", 0, 0, 45, -1, nullptr},
    {"SSCRATCH.SEQ", 0, 0, 55, -1, nullptr},
    {"PANTING.SEQ", 0, 0, 45, -1, nullptr},
    {"BATSQUEK.SEQ", 0, 0, 55, -1, nullptr},
    {"WHISTLE1.SEQ", 0, 0, 127, -1, nullptr},
    {"WHISTLE2.SEQ", 0, 0, 127, -1, nullptr},
    {"SLIGBOMB.SEQ", 0, 0, 127, -1, nullptr},
    {"SLIGBOM2.SEQ", 0, 0, 127, -1, nullptr},
    {"OOPS.SEQ", 0, 0, 40, -1, nullptr},
    {"PIGEONS.SEQ", 0, 0, 40, -1, nullptr},
    {"CHIPPER.SEQ", 0, 0, 90, -1, nullptr},
    {"PATROL.SEQ", 0, 0, 60, -1, nullptr},
    {"SLEEPING.SEQ", 0, 0, 60, -1, nullptr},
    {"ONCHAIN.SEQ", 0, 0, 60, -1, nullptr},
    {"SLOSLEEP.SEQ", 0, 0, 60, -1, nullptr},
    {"PARAPANT.SEQ", 0, 0, 35, -1, nullptr},
    {"GRINDER.SEQ", 0, 0, 35, -1, nullptr},
    {"BASICTRK.SEQ", 0, 0, 90, -1, nullptr},
    {"LE_LO_1.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_LO_2.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_LO_3.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_LO_4.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_SH_1.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_SH_2.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_SH_3.SEQ", 0, 0, 127, -1, nullptr},
    {"LE_SH_4.SEQ", 0, 0, 127, -1, nullptr},
    {"MYSTERY1.SEQ", 0, 0, 60, -1, nullptr},
    {"MYSTERY2.SEQ", 0, 0, 60, -1, nullptr},
    {"NEGATIV1.SEQ", 0, 0, 60, -1, nullptr},
    {"NEGATIV3.SEQ", 0, 0, 60, -1, nullptr},
    {"POSITIV1.SEQ", 0, 0, 60, -1, nullptr},
    {"POSITIV9.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_0_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_0_2.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_0_3.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_1_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_1_2.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_1_3.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_1_4.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_1_5.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_2_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_2_2.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_2_3.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_2_4.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_2_5.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_3_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_4_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_5_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D1_6_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_0_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_0_2.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_1_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_1_2.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_2_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_2_2.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_4_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_5_1.SEQ", 0, 0, 60, -1, nullptr},
    {"D2_6_1.SEQ", 0, 0, 60, -1, nullptr},
    {"DE_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"DE_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"DE_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"E1_0_1.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_0_2.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_0_3.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_0_4.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_0_5.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_1_1.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_1_2.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_1_3.SEQ", 0, 0, 60, -1, nullptr},
    {"E1_1_4.SEQ", 0, 0, 90, -1, nullptr},
    {"E1_1_5.SEQ", 0, 0, 90, -1, nullptr},
    {"E1_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"E1_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"E1_6_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_0_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_0_2.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_0_3.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_1_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_1_2.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_1_3.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_2_2.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_2_3.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_2_4.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_3_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F1_6_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F2_0_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F2_1_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F2_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F2_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F2_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"F2_6_1.SEQ", 0, 0, 90, -1, nullptr},
    {"FE_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"FE_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"FE_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_0_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_0_2.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_0_3.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_0_4.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_0_5.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_1_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_1_2.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_1_3.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_1_4.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_1_5.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_2_2.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_2_3.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_2_4.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_2_5.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_3_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ML_6_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_0_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_0_2.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_0_3.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_0_4.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_1_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_1_2.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_1_3.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_2_2.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_2_3.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_2_4.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RF_6_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RE_2_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RE_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"RE_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"OPT_0_1.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_0_2.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_0_3.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_0_4.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_0_5.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_1_1.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_1_2.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_1_3.SEQ", 0, 0, 60, -1, nullptr},
    {"OPT_1_4.SEQ", 0, 0, 90, -1, nullptr},
    {"OPT_1_5.SEQ", 0, 0, 90, -1, nullptr},
    {"ALL_4_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ALL_5_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ALL_5_2.SEQ", 0, 0, 90, -1, nullptr},
    {"ALL_5_3.SEQ", 0, 0, 90, -1, nullptr},
    {"ALL_7_1.SEQ", 0, 0, 90, -1, nullptr},
    {"ALL_8_1.SEQ", 0, 0, 90, -1, nullptr},
    {nullptr, 0, 0, 0, 0, nullptr}};

s32 MaxGridBlocks_41FA10(FP scale)
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
        LOG_WARNING("Scale should be 0.5 or 1 but got " << FP_GetDouble(scale) << ". This usually occurs when you die with DDCheat on.");
        return 0;
    }
}


// Map Path_ChangeTLV::field_18_wipe to CameraSwapEffects
const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect_4CDC78[10] = {
    CameraSwapEffects::ePlay1FMV_5,
    CameraSwapEffects::eRightToLeft_2,
    CameraSwapEffects::eLeftToRight_1,
    CameraSwapEffects::eBottomToTop_4,
    CameraSwapEffects::eTopToBottom_3,
    CameraSwapEffects::eBoxOut_8,
    CameraSwapEffects::eVerticalSplit_6,
    CameraSwapEffects::eHorizontalSplit_7,
    CameraSwapEffects::eUnknown_11,
    CameraSwapEffects::eInstantChange_0};

ALIVE_VAR(1, 0x507BA8, Map, gMap, {});
ALIVE_VAR(1, 0x507C9C, s16, sMap_bDoPurpleLightEffect_507C9C, 0);
ALIVE_VAR(1, 0x507CA0, s32, gSndChannels_507CA0, 0);

Map::Map()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        field_34_camera_array[i] = nullptr;
    }

    ClearPathResourceBlocks();
    field_D8 = 1;
    field_DC_free_all_anim_and_palts = 0;
    field_E0_save_data = nullptr;
}

void Map::Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    field_34_camera_array[0] = nullptr;
    field_34_camera_array[1] = nullptr;
    field_34_camera_array[2] = nullptr;
    field_34_camera_array[3] = nullptr;
    field_34_camera_array[4] = nullptr;

    mOverlayId = -1;

    mCurrentCamera = -1;
    mCurrentPath = -1;
    mCurrentLevel = EReliveLevelIds::eNone;

    SetActiveCam(level, path, camera, screenChangeEffect, fmvBaseId, forceChange);
    GoTo_Camera();

    field_6_state = CamChangeStates::eInactive_0;
}

void Map::Shutdown()
{
    sLvlArchive_4FFD60.Free_41BEB0();
    stru_507C90.Free_41BEB0();

    FreePathResourceBlocks();

    // Free cameras
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        if (field_34_camera_array[i])
        {
            relive_delete field_34_camera_array[i];
            field_34_camera_array[i] = nullptr;
        }
    }

    pScreenManager = nullptr;

    ResourceManager::Reclaim_Memory_455660(0);
    Reset();
}

void Map::Reset()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        field_34_camera_array[i] = nullptr;
    }

    ClearPathResourceBlocks();

    field_D8 = 1;
    field_DC_free_all_anim_and_palts = 0;
    field_E0_save_data = 0;
}

s16 Map::SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    if (!forceChange && cam == mCurrentCamera && level == mCurrentLevel && path == mCurrentPath)
    {
        return 0;
    }

    mNextCamera = cam;
    field_12_fmv_base_id = fmvBaseId;
    mNextPath = path;
    mNextLevel = level;
    field_10_screenChangeEffect = screenChangeEffect;
    field_6_state = CamChangeStates::eInstantChange_2;


    if (screenChangeEffect == CameraSwapEffects::ePlay1FMV_5 || screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        sMap_bDoPurpleLightEffect_507C9C = 1;
    }
    else
    {
        sMap_bDoPurpleLightEffect_507C9C = 0;
    }

    return 1;
}

void Map::Handle_PathTransition()
{
    relive::Path_PathTransition* pTlv = nullptr;
    if (field_18_pAliveObj)
    {
        pTlv = static_cast<relive::Path_PathTransition*>(TLV_Get_At_446260(
            FP_GetExponent(field_18_pAliveObj->mXPos),
            FP_GetExponent(field_18_pAliveObj->mYPos),
            FP_GetExponent(field_18_pAliveObj->mXPos),
            FP_GetExponent(field_18_pAliveObj->mYPos),
            ReliveTypes::ePathTransition));
    }

    if (field_18_pAliveObj && pTlv)
    {
        mNextLevel = pTlv->mNextLevel;
        mNextPath = pTlv->mNextPath;
        mNextCamera = pTlv->mNextCamera;
        field_12_fmv_base_id = pTlv->mMovieId;

        field_10_screenChangeEffect = kPathChangeEffectToInternalScreenChangeEffect_4CDC78[pTlv->mWipeEffect];

        field_18_pAliveObj->mCurrentLevel = pTlv->mNextLevel;
        field_18_pAliveObj->mCurrentPath = pTlv->mNextPath;

        // TODO: Probably OG bug, when changing camera/path the TLV pointer can become invalid
        // resulting in a corrupted next_path_scale value ?
        // Pointer points to the Path res which is invalid after ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, i, TRUE, FALSE);
        // is called. Happens even if calling real func below.
        const auto next_path_scale = pTlv->mNextPathScale;

        GoTo_Camera();

        switch (next_path_scale)
        {
            case relive::reliveScale::eFull:
                sActiveHero->mSpriteScale = FP_FromInteger(1);
                sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_32;
                if (gElum)
                {
                    gElum->mSpriteScale = sActiveHero->mSpriteScale;
                    gElum->mAnim.mRenderLayer = Layer::eLayer_ZapLinesElumMuds_28;
                }
                break;

            case relive::reliveScale::eHalf:
                sActiveHero->mSpriteScale = FP_FromDouble(0.5);
                sActiveHero->mAnim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
                if (gElum)
                {
                    gElum->mSpriteScale = sActiveHero->mSpriteScale;
                    gElum->mAnim.mRenderLayer = Layer::eLayer_ZapLinesMudsElum_Half_9;
                }
                break;

            default:
                LOG_ERROR("Invalid scale " << static_cast<s16>(pTlv->mNextPathScale));
                break;
        }

        CameraPos remapped = CameraPos::eCamInvalid_m1;
        switch (field_14_direction)
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

        field_18_pAliveObj->VOnPathTransition(
            field_D4_pPathData->field_C_grid_width * field_20_camX_idx,
            field_D4_pPathData->field_E_grid_height * field_22_camY_idx,
            remapped);
    }
    else
    {
        switch (field_14_direction)
        {
            case Map::MapDirections::eMapLeft_0:
                field_20_camX_idx--;
                if (field_18_pAliveObj)
                {
                    field_18_pAliveObj->VSetXSpawn(
                        field_20_camX_idx * field_D4_pPathData->field_C_grid_width,
                        MaxGridBlocks_41FA10(field_18_pAliveObj->mSpriteScale) - 1);
                }
                field_10_screenChangeEffect = CameraSwapEffects::eRightToLeft_2;
                break;
            case MapDirections::eMapRight_1:
                field_20_camX_idx++;
                if (field_18_pAliveObj)
                {
                    field_18_pAliveObj->VSetXSpawn(field_20_camX_idx * field_D4_pPathData->field_C_grid_width,
                                                   1);
                }
                field_10_screenChangeEffect = CameraSwapEffects::eLeftToRight_1;
                break;
            case MapDirections::eMapTop_2:
                field_22_camY_idx--;
                if (field_18_pAliveObj)
                {
                    field_18_pAliveObj->VSetYSpawn(field_22_camY_idx * field_D4_pPathData->field_E_grid_height,
                                                   1);
                }
                field_10_screenChangeEffect = CameraSwapEffects::eBottomToTop_4;
                break;
            case MapDirections::eMapBottom_3:
                field_22_camY_idx++;
                if (field_18_pAliveObj)
                {
                    field_18_pAliveObj->VSetYSpawn(field_22_camY_idx * field_D4_pPathData->field_E_grid_height,
                                                   2);
                }
                field_10_screenChangeEffect = CameraSwapEffects::eTopToBottom_3;
                break;
            default:
                break;
        }

        const BinaryPath* pPathRes = GetPathResourceBlockPtr(mCurrentPath);
        auto pCameraName = pPathRes->CameraName(field_20_camX_idx, field_22_camY_idx);

        // Convert the 2 digit camera number string to an integer
        mNextCamera = 1 * (pCameraName[7] - '0') + 10 * (pCameraName[6] - '0');

        GoTo_Camera();
    }
}

void Map::RemoveObjectsWithPurpleLight(s16 bMakeInvisible)
{
    auto pObjectsWithLightsArray = relive_new DynamicArrayT<BaseAnimatedWithPhysicsGameObject>(16);

    auto pPurpleLightArray = relive_new DynamicArrayT<Particle>(16);

    bool bAddedALight = false;
    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
    {
        auto pObjIter = gBaseAliveGameObjects->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectFlags.Get(::BaseGameObject::eDrawable_Bit4)
            && pObjIter->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit6)
            && pObjIter->mAnim.mFlags.Get(AnimFlags::eBit3_Render)
            && !pObjIter->mBaseGameObjectFlags.Get(::BaseGameObject::eDead)
            && pObjIter != sControlledCharacter)
        {
            bool bAdd = false;
            if (pObjIter->mCurrentLevel == mCurrentLevel
                && pObjIter->mCurrentPath == mCurrentPath)
            {
                PSX_RECT rect = {};
                rect.x = FP_GetExponent(pObjIter->mXPos);
                rect.w = FP_GetExponent(pObjIter->mXPos);
                rect.y = FP_GetExponent(pObjIter->mYPos);
                rect.h = FP_GetExponent(pObjIter->mYPos);
                bAdd = Rect_Location_Relative_To_Active_Camera(&rect, 0) == CameraPos::eCamCurrent_0;
            }

            if (bAdd)
            {
                pObjectsWithLightsArray->Push_Back(pObjIter);

                const PSX_RECT objRect = pObjIter->VGetBoundingRect();

                const FP k60Scaled = pObjIter->mSpriteScale * FP_FromInteger(60);
                auto pPurpleLight = New_DestroyOrCreateObject_Particle_419D00(
                    FP_FromInteger((objRect.x + objRect.w) / 2),
                    FP_FromInteger((objRect.y + objRect.h) / 2) + k60Scaled,
                    pObjIter->mSpriteScale);

                if (pPurpleLight)
                {
                    pPurpleLightArray->Push_Back(pPurpleLight);
                    bAddedALight = true;
                }
            }
        }
    }

    if (bAddedALight)
    {
        SFX_Play_Pitch(relive::SoundEffects::PossessEffect, 40, 2400, 0);

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
                    pObj->mAnim.mFlags.Clear(AnimFlags::eBit3_Render);
                }
            }

            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->mBaseGameObjectFlags.Get(::BaseGameObject::eDead))
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

                if (!pLight->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    pLight->mAnim.VDecode();
                }
            }

            for (s32 i = 0; i < gObjListDrawables->Size(); i++)
            {
                ::BaseGameObject* pDrawable = gObjListDrawables->ItemAt(i);
                if (!pDrawable)
                {
                    break;
                }

                if (!pDrawable->mBaseGameObjectFlags.Get(::BaseGameObject::eDead))
                {
                    // TODO: Seems strange to check this flag, how did it get in the drawable list if its not a drawable ??
                    if (pDrawable->mBaseGameObjectFlags.Get(::BaseGameObject::eDrawable_Bit4))
                    {
                        pDrawable->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
                    }
                }
            }

            PSX_DrawSync_496750(0);
            pScreenManager->VRender(gPsxDisplay.mDrawEnvs[gPsxDisplay.mBufferIndex].mOrderingTable);
            SYS_EventsPump_44FF90();
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
                pObj->mAnim.mFlags.Set(AnimFlags::eBit3_Render);
            }
        }
    }

    pObjectsWithLightsArray->field_4_used_size = 0;
    pPurpleLightArray->field_4_used_size = 0;

    relive_delete pObjectsWithLightsArray;
    relive_delete pPurpleLightArray;
}

void Map::ScreenChange_Common()
{
    if (field_6_state == CamChangeStates::eSliceCam_1)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        Handle_PathTransition();
    }
    else if (field_6_state == CamChangeStates::eInstantChange_2)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        GoTo_Camera();
    }

    field_6_state = CamChangeStates::eInactive_0;
    SND_Stop_Channels_Mask(gSndChannels_507CA0);
    gSndChannels_507CA0 = 0;
}

void Map::ScreenChange()
{
    if (field_6_state == CamChangeStates::eInactive_0)
    {
        return;
    }

    if (sMap_bDoPurpleLightEffect_507C9C && mCurrentLevel != EReliveLevelIds::eBoardRoom)
    {
        RemoveObjectsWithPurpleLight(1);
    }

    PSX_DrawSync_496750(0);

    for (s32 i = 0; i < 2; i++) // Not sure why this is done twice?
    {
        for (s32 j = 0; j < gBaseGameObjects->Size(); j++)
        {
            ::BaseGameObject* pItem = gBaseGameObjects->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();
            if (pItem->mBaseGameObjectFlags.Get(::BaseGameObject::eDead))
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

        if (pItem->mBaseGameObjectFlags.Get(::BaseGameObject::eDead))
        {
            if (pItem->mBaseGameObjectRefCount == 0)
            {
                i = gBaseGameObjects->RemoveAt(i);
                delete pItem;
            }
        }
    }

    if (sMap_bDoPurpleLightEffect_507C9C || mNextLevel != mCurrentLevel)
    {
        if (mNextLevel != mCurrentLevel)
        {
            SsUtAllKeyOff_49EDE0(0);
        }

        // TODO: Re-check this logic
        if (mNextLevel != EReliveLevelIds::eMenu)
        {
            if ((mNextLevel != EReliveLevelIds::eRuptureFarmsReturn && mNextLevel != EReliveLevelIds::eForestChase && mNextLevel != EReliveLevelIds::eDesertEscape) || (mNextLevel == EReliveLevelIds::eBoardRoom && mCurrentLevel == EReliveLevelIds::eBoardRoom))
            {
                gSndChannels_507CA0 = 0;
            }
        }
        else
        {
            gSndChannels_507CA0 = 0;
        }
    }

    ScreenChange_Common();
}

s16 Map::GetOverlayId()
{
    return Path_Get_Bly_Record_434650(mNextLevel, mNextPath)->field_C_overlay_id;
}

relive::Path_TLV* Map::Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx)
{
    const auto camX = cam_x_idx + field_20_camX_idx;
    const auto camY = cam_y_idx + field_22_camY_idx;

    if (camX >= field_24_max_cams_x || camX < 0 || camY >= field_26_max_cams_y || camY < 0)
    {
        return nullptr;
    }

    BinaryPath* pPathData = GetPathResourceBlockPtr(mCurrentPath);
    return pPathData->TlvsForCamera(camX, camY);
}

void Map::SaveBlyData(u8* pSaveBuffer)
{
    memcpy(pSaveBuffer, sSwitchStates_505568.mData, sizeof(sSwitchStates_505568.mData));

    u8* pAfterSwitchStates = pSaveBuffer + sizeof(sSwitchStates_505568.mData);
    for (s16 i = 1; i < AO::Path_Get_Num_Paths(mCurrentLevel); i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_434650(mCurrentLevel, i);
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

void Map::RestoreBlyData(const u8* pSaveData)
{
    memcpy(sSwitchStates_505568.mData, pSaveData, sizeof(sSwitchStates_505568.mData));
    const u8* pAfterSwitchStates = pSaveData + sizeof(sSwitchStates_505568.mData);

    for (s16 i = 1; i < AO::Path_Get_Num_Paths(mCurrentLevel); i++)
    {
        BinaryPath* ppPathRes = GetPathResourceBlockPtr(i);
        if (ppPathRes)
        {
            const PathBlyRec* pPathRec = Path_Get_Bly_Record_434650(mCurrentLevel, i);
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
    const s32 cam_global_left = field_D4_pPathData->field_C_grid_width * cam_x_idx;
    const s32 cam_global_right = cam_global_left + field_D4_pPathData->field_C_grid_width;

    const s32 cam_y_grid_top = field_D4_pPathData->field_E_grid_height * cam_y_idx;
    const s32 cam_y_grid_bottom = cam_y_grid_top + field_D4_pPathData->field_E_grid_height;

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
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamLeft_3, field_20_camX_idx - 1, field_22_camY_idx);
    }

    if (Get_Camera_World_Rect(CameraPos::eCamRight_4, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamRight_4, field_20_camX_idx + 1, field_22_camY_idx);
    }

    if (Get_Camera_World_Rect(CameraPos::eCamTop_1, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamTop_1, field_20_camX_idx, field_22_camY_idx - 1);
    }

    if (Get_Camera_World_Rect(CameraPos::eCamBottom_2, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera(CameraPos::eCamBottom_2, field_20_camX_idx, field_22_camY_idx + 1);
    }
}

Camera* Map::GetCamera(CameraPos pos)
{
    return field_34_camera_array[static_cast<s32>(pos)];
}

s16 Map::SetActiveCameraDelayed(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect)
{
    relive::Path_PathTransition* pPathChangeTLV = nullptr;
    CameraSwapEffects convertedSwapEffect = CameraSwapEffects::eInstantChange_0;
    if (pObj)
    {
        pPathChangeTLV = static_cast<relive::Path_PathTransition*>(TLV_Get_At_446260(
            FP_GetExponent(pObj->mXPos),
            FP_GetExponent(pObj->mYPos),
            FP_GetExponent(pObj->mXPos),
            FP_GetExponent(pObj->mYPos),
            ReliveTypes::ePathTransition));
    }

    if (pObj && pPathChangeTLV)
    {
        mNextLevel = pPathChangeTLV->mNextLevel;
        mNextPath = pPathChangeTLV->mNextPath;
        mNextCamera = pPathChangeTLV->mNextCamera;
        if (swapEffect < 0)
        {
            // Map the TLV/editor value of screen change to the internal screen change
            convertedSwapEffect = kPathChangeEffectToInternalScreenChangeEffect_4CDC78[pPathChangeTLV->mWipeEffect];
        }
        else
        {
            // If not negative then its an actual swap effect
            convertedSwapEffect = static_cast<CameraSwapEffects>(swapEffect);
        }
    }
    else
    {
        switch (direction)
        {
            case MapDirections::eMapLeft_0:
                if (!GetCamera(CameraPos::eCamLeft_3))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapRight_1:
                if (!GetCamera(CameraPos::eCamRight_4))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapBottom_3:
                if (!GetCamera(CameraPos::eCamBottom_2))
                {
                    return 0;
                }
                break;
            case MapDirections::eMapTop_2:
                if (!GetCamera(CameraPos::eCamTop_1))
                {
                    return 0;
                }
                break;
        }

        mNextPath = mCurrentPath;
        mNextLevel = mCurrentLevel;
    }

    field_14_direction = direction;
    field_18_pAliveObj = pObj;
    field_1C_cameraSwapEffect = convertedSwapEffect;
    field_6_state = CamChangeStates::eSliceCam_1;
    sMap_bDoPurpleLightEffect_507C9C = 0;

    if (field_1C_cameraSwapEffect == CameraSwapEffects::ePlay1FMV_5 || field_1C_cameraSwapEffect == CameraSwapEffects::eUnknown_11)
    {
        sMap_bDoPurpleLightEffect_507C9C = 1;
    }

    return 1;
}

s16 Map::Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width)
{
    if (level != mCurrentLevel || path != mCurrentPath) // TODO: Remove when 100%
    {
        return FALSE;
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

    Camera* pCamera = field_34_camera_array[static_cast<s32>(camIdx)];
    if (!pCamera)
    {
        return 0;
    }

    if (!pRect)
    {
        return 1;
    }

    s16 cam_x_pos = field_D4_pPathData->field_C_grid_width * pCamera->field_14_cam_x;
    cam_x_pos += 120;

    const s16 cam_y_pos = field_D4_pPathData->field_E_grid_height * pCamera->field_16_cam_y;

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

    if (pRect->x > FP_GetExponent(field_2C_camera_offset.x + xTweak))
    {
        return CameraPos::eCamRight_4;
    }

    if (pRect->y > FP_GetExponent(field_2C_camera_offset.y + yTweak))
    {
        return CameraPos::eCamBottom_2;
    }

    if (pRect->w >= FP_GetExponent(field_2C_camera_offset.x - xTweak))
    {
        if (pRect->h < FP_GetExponent(field_2C_camera_offset.y - yTweak))
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

relive::Path_TLV* Map::TLV_Get_At_446260(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes typeToFind)
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

    const s32 grid_cell_y = top / field_D4_pPathData->field_E_grid_height;
    const s32 grid_cell_x = (right / field_D4_pPathData->field_C_grid_width);

    // Check within map bounds
    if (grid_cell_x >= field_24_max_cams_x)
    {
        return nullptr;
    }

    if (grid_cell_y >= field_26_max_cams_y)
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

relive::Path_TLV* Map::TLV_Get_At_446060(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height)
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
        const PathData* pPathData = field_D4_pPathData;

        const auto camX = xpos_converted / pPathData->field_C_grid_width;
        const auto camY = ypos_converted / pPathData->field_E_grid_height;

        if (camX >= field_24_max_cams_x || camY >= field_26_max_cams_y)
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
    if (!(pCamera->field_30_flags & 1))
    {
        if (loadMode == LoadMode::ConstructObject_0)
        {
            // Async camera load
            ResourceManager::LoadResourceFile(
                pCamera->field_1E_fileName,
                Camera::On_Loaded_4447A0,
                pCamera,
                pCamera);
            sCameraBeingLoaded_507C98 = pCamera;
            Loader(pCamera->field_14_cam_x, pCamera->field_16_cam_y, LoadMode::LoadResourceFromList_1, ReliveTypes::eNone); // none = load all
        }
        else
        {
            // Blocking camera load
            ResourceManager::LoadResourceFile_455270(pCamera->field_1E_fileName, pCamera);
            pCamera->field_30_flags |= 1u;
            pCamera->field_C_ppBits = ResourceManager::GetLoadedResource(ResourceManager::Resource_Bits, pCamera->field_10_resId, 1, 0);
            sCameraBeingLoaded_507C98 = pCamera;
            Loader(pCamera->field_14_cam_x, pCamera->field_16_cam_y, LoadMode::LoadResource_2, ReliveTypes::eNone); // none = load all
        }
        sCameraBeingLoaded_507C98 = nullptr;
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
    if (xpos >= field_24_max_cams_x || ypos >= field_26_max_cams_y)
    {
        return nullptr;
    }

    // Return existing camera if we already have one
    for (s32 i = 0; i < ALIVE_COUNTOF(field_48_stru_5); i++)
    {
        if (field_48_stru_5[i]
            && field_48_stru_5[i]->field_1A_level == mCurrentLevel
            && field_48_stru_5[i]->field_18_path == mCurrentPath
            && field_48_stru_5[i]->field_14_cam_x == xpos
            && field_48_stru_5[i]->field_16_cam_y == ypos)
        {
            auto pTemp = field_48_stru_5[i];
            field_48_stru_5[i] = nullptr;
            if (sActiveHero && sActiveHero->mCurrentMotion == eAbeMotions::Motion_61_Respawn_42CD20)
            {
                pTemp->field_30_flags |= 2u;
            }
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
    memset(newCamera->field_1E_fileName, 0, sizeof(newCamera->field_1E_fileName));
    strncpy(newCamera->field_1E_fileName, pCamName, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->field_1E_fileName, ".CAM");

    newCamera->field_14_cam_x = xpos;
    newCamera->field_16_cam_y = ypos;

    newCamera->field_30_flags &= ~1u;

    newCamera->field_1A_level = mCurrentLevel;
    newCamera->field_18_path = mCurrentPath;

    // Calculate hash/resource ID of the camera
    newCamera->field_10_resId = 1 * (pCamName[7] - '0') + 10 * (pCamName[6] - '0') + 100 * (pCamName[4] - '0') + 1000 * (pCamName[3] - '0');

    newCamera->field_1C = mCurrentCamera;

    return newCamera;
}

void Map::Create_FG1s()
{
    pScreenManager->UnsetDirtyBits_FG1();

    Camera* pCamera = field_34_camera_array[0];
    for (s32 i = 0; i < pCamera->field_0_array.Size(); i++)
    {
        u8** ppRes = pCamera->field_0_array.ItemAt(i);
        if (!ppRes)
        {
            break;
        }

        if (*ppRes)
        {
            ResourceManager::Header* pHeader = ResourceManager::Get_Header_455620(ppRes);
            if (pHeader->mLineType == ResourceManager::Resource_FG1)
            {
                relive_new FG1(ppRes);
            }
        }
    }
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

void Map::GoTo_Camera()
{
    s16 bShowLoadingIcon = FALSE;

    //dword_507CA4 = 0; // never read

    if (mCurrentLevel != EReliveLevelIds::eMenu && mCurrentLevel != EReliveLevelIds::eCredits && mCurrentLevel == EReliveLevelIds::eNone)
    {
        bShowLoadingIcon = TRUE;
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        CameraSwapper* pFmvRet = FMV_Camera_Change(nullptr, this, mCurrentLevel);
        do
        {
            SYS_EventsPump_44FF90();
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                ::BaseGameObject* pBaseGameObj = gBaseGameObjects->ItemAt(i);
                if (!pBaseGameObj)
                {
                    break;
                }

                if (pBaseGameObj->mBaseGameObjectFlags.Get(::BaseGameObject::eUpdatable_Bit2))
                {
                    if (!pBaseGameObj->mBaseGameObjectFlags.Get(::BaseGameObject::eDead) && (!sNumCamSwappers_507668 || pBaseGameObj->mBaseGameObjectFlags.Get(::BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                    {
                        if (pBaseGameObj->mBaseGameObjectUpdateDelay > 0)
                        {
                            pBaseGameObj->mBaseGameObjectUpdateDelay--;
                        }
                        else
                        {
                            pBaseGameObj->VUpdate();
                        }
                    }
                }
            }
        }
        while (!pFmvRet->mBaseGameObjectFlags.Get(::BaseGameObject::eDead));
    }

    if (mCurrentLevel != EReliveLevelIds::eMenu)
    {
        if (mNextLevel != mCurrentLevel || (mNextPath != mCurrentPath && field_10_screenChangeEffect == CameraSwapEffects::ePlay1FMV_5))
        {
            Game_ShowLoadingIcon_445EB0();
        }
    }

    if (mNextLevel != mCurrentLevel || mNextPath != mCurrentPath)
    {
        mOverlayId = GetOverlayId();
    }

    if (mNextLevel != mCurrentLevel)
    {
        ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);

        // Free all cameras
        for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
        {
            if (field_34_camera_array[i])
            {
                relive_delete field_34_camera_array[i];
                field_34_camera_array[i] = nullptr;
            }
        }

        if (mCurrentLevel != EReliveLevelIds::eNone)
        {
            // Close LVL archives
            sLvlArchive_4FFD60.Free_41BEB0();
            stru_507C90.Free_41BEB0();

            FreePathResourceBlocks();

            SND_Reset_476BA0();
            ResourceManager::Reclaim_Memory_455660(0);
        }

        ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);

        // Open Path BND
        auto tmp = sOverlayTable_4C5AA8.records[Path_Get_OverlayIdx(mNextLevel)].field_4_pos;
        sLvlArchive_4FFD60.OpenArchive(AO::CdLvlName(mNextLevel), tmp);

        // TODO: Jayson!
        //ResourceManager::LoadResourceFile_455270(Path_Get_BndName(mNextLevel), nullptr);


        // TODO: Load level_info.json so we know which path jsons to load for this level
        FileSystem::Path pathDir;
        pathDir.Append("relive_data").Append("ao").Append(ToString(MapWrapper::ToAO(mNextLevel))).Append("paths");

        FileSystem::Path levelInfo = pathDir;
        levelInfo.Append("level_info.json");

        FileSystem fs;
        const std::string jsonStr = fs.LoadToString(levelInfo);
        nlohmann::json j = nlohmann::json::parse(jsonStr);
        const auto& paths = j["paths"];
        for (const auto& path : paths)
        {
            FileSystem::Path pathJsonFile = pathDir;
            pathJsonFile.Append(path);
            const std::string pathJsonStr = fs.LoadToString(pathJsonFile);

            // TODO: set the res ptrs to the parsed json data
            nlohmann::json pathJson = nlohmann::json::parse(pathJsonStr);
            LOG_INFO("Cam count " << pathJson["cameras"].size());

            auto pathBuffer = std::make_unique<BinaryPath>(pathJson["id"]);
            pathBuffer->CreateFromJson(pathJson);
            mLoadedPaths.emplace_back(std::move(pathBuffer));
        }

        SND_Load_VABS_477040(AO::Path_Get_MusicInfo(mNextLevel), AO::Path_Get_Reverb(mNextLevel));

        // Struct is using AE format so pass address of seq table in the exe to avoid a crash
        //SND_Load_Seqs_477AB0(reinterpret_cast<OpenSeqHandleAE*>(0x4C9E70), rPathRoot.field_C_bsq_file_name);

        SND_Load_Seqs_477AB0(g_SeqTable_4C9E70, AO::Path_Get_BsqFileName(mNextLevel));
        relive_new BackgroundMusic(AO::Path_Get_BackGroundMusicId(mNextLevel));

        // TODO: Re-add function
        for (s32 i = 0; i < 236; i++)
        {
            sSwitchStates_505568.mData[i] = 0;
        }

        if (field_DC_free_all_anim_and_palts)
        {
            ResourceManager::Free_Resource_Of_Type_455810(ResourceManager::Resource_Animation);
            ResourceManager::Free_Resource_Of_Type_455810(ResourceManager::Resource_Palt);
            field_DC_free_all_anim_and_palts = FALSE;
        }
    }

    if (!mNextPath)
    {
        mNextPath = 1;
    }


    const auto old_current_path = mCurrentPath;
    const auto old_current_level = mCurrentLevel;

    field_DA_bMapChanged = mNextPath != old_current_path || mNextLevel != mCurrentLevel;

    mCurrentCamera = mNextCamera;
    mCurrentPath = mNextPath;
    mCurrentLevel = mNextLevel;

    const PathBlyRec* pPathRecord = Path_Get_Bly_Record_434650(mNextLevel, mNextPath);
    field_D4_pPathData = pPathRecord->field_4_pPathData;
    field_24_max_cams_x = (field_D4_pPathData->field_8_bTop - field_D4_pPathData->field_4_bLeft) / field_D4_pPathData->field_C_grid_width;
    field_26_max_cams_y = (field_D4_pPathData->field_A_bBottom - field_D4_pPathData->field_6_bRight) / field_D4_pPathData->field_E_grid_height;

    char_type camNameBuffer[20] = {};
    Path_Format_CameraName_4346B0(camNameBuffer, mNextLevel, mNextPath, mNextCamera);

    field_20_camX_idx = 0;
    field_22_camY_idx = 0;

    BinaryPath* pNextPath = GetPathResourceBlockPtr(mNextPath);
    for (auto& cam : pNextPath->GetCameras())
    {
        if (!strncmp(cam->mName.c_str(), camNameBuffer, sizeof(CameraName)))
        {
            field_20_camX_idx = static_cast<s16>(cam->mX);
            field_22_camY_idx = static_cast<s16>(cam->mY);
            break;
        }
    }


    field_2C_camera_offset.x = FP_FromInteger(field_20_camX_idx * field_D4_pPathData->field_C_grid_width + 440);
    field_2C_camera_offset.y = FP_FromInteger(field_22_camY_idx * field_D4_pPathData->field_E_grid_height + 240);

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (sCollisions)
        {
            // OG FIX: Remove any pointers to the line objects that we are about to delete
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                ::BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->mBaseGameObjectFlags.Get(::BaseGameObject::eIsBaseAliveGameObject_Bit6))
                {
                    auto pBaseAliveGameObj = static_cast<BaseAliveGameObject*>(pObjIter);
                    pBaseAliveGameObj->BaseAliveGameObjectCollisionLine = nullptr;
                }
            }

            if (ObjListPlatforms_50766C->Size() > 0)
            {
                LOG_ERROR(ObjListPlatforms_50766C->Size() << " Platforms have been leaked!");
                ALIVE_FATAL("Platforms leaked!");
            }

            relive_delete sCollisions;
        }

        sCollisions = relive_new Collisions(GetPathResourceBlockPtr(mCurrentPath)->GetCollisions());
    }

    if (field_E0_save_data)
    {
        RestoreBlyData(field_E0_save_data);
        field_E0_save_data = nullptr;
    }

    // Copy camera array and blank out the source
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        field_48_stru_5[i] = field_34_camera_array[i];
        field_34_camera_array[i] = nullptr;
    }

    field_34_camera_array[0] = Create_Camera(field_20_camX_idx, field_22_camY_idx, 1);
    field_34_camera_array[3] = Create_Camera(field_20_camX_idx - 1, field_22_camY_idx, 0);
    field_34_camera_array[4] = Create_Camera(field_20_camX_idx + 1, field_22_camY_idx, 0);
    field_34_camera_array[1] = Create_Camera(field_20_camX_idx, field_22_camY_idx - 1, 0);
    field_34_camera_array[2] = Create_Camera(field_20_camX_idx, field_22_camY_idx + 1, 0);

    // Free resources for each camera
    for (s32 i = 0; i < ALIVE_COUNTOF(field_48_stru_5); i++)
    {
        if (field_48_stru_5[i])
        {
            ResourceManager::Free_Resources_For_Camera_447170(field_48_stru_5[i]);
        }
    }

    ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);

    // Free each camera itself
    for (s32 i = 0; i < ALIVE_COUNTOF(field_48_stru_5); i++)
    {
        if (field_48_stru_5[i])
        {
            relive_delete field_48_stru_5[i];
            field_48_stru_5[i] = nullptr;
        }
    }

    Load_Path_Items(field_34_camera_array[0], LoadMode::ConstructObject_0);
    ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);
    Load_Path_Items(field_34_camera_array[3], LoadMode::ConstructObject_0);
    Load_Path_Items(field_34_camera_array[4], LoadMode::ConstructObject_0);
    Load_Path_Items(field_34_camera_array[1], LoadMode::ConstructObject_0);
    Load_Path_Items(field_34_camera_array[2], LoadMode::ConstructObject_0);

    if (!pScreenManager)
    {
        pScreenManager = relive_new ScreenManager(field_34_camera_array[0]->field_C_ppBits, &field_2C_camera_offset);
    }

    Loader(field_20_camX_idx, field_22_camY_idx, LoadMode::ConstructObject_0, ReliveTypes::eNone); // none = load all

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (sActiveHero && mCurrentPath == sActiveHero->mCurrentPath)
        {
            sActiveHero->VCheckCollisionLineStillValid(10);
        }

        if (gElum && sControlledCharacter != gElum && mCurrentPath == gElum->mCurrentPath)
        {
            gElum->VCheckCollisionLineStillValid(10);
        }
    }

    Create_FG1s();

    if (field_10_screenChangeEffect == CameraSwapEffects::ePlay1FMV_5)
    {
        FMV_Camera_Change(field_34_camera_array[0]->field_C_ppBits, this, mNextLevel);
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        pScreenManager->DecompressCameraToVRam(reinterpret_cast<u16**>(field_34_camera_array[0]->field_C_ppBits));
        pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, 240);
        pScreenManager->MoveImage();
        pScreenManager->EnableRendering();
    }

    if (field_10_screenChangeEffect != CameraSwapEffects::ePlay1FMV_5 && field_10_screenChangeEffect != CameraSwapEffects::eUnknown_11)
    {
        if (field_1E_door)
        {
            relive::Path_Door* pTlvIter = static_cast<relive::Path_Door*>(TLV_First_Of_Type_In_Camera(ReliveTypes::eDoor, 0));
            while (pTlvIter->mDoorId != sActiveHero->field_196_door_id)
            {
                pTlvIter = static_cast<relive::Path_Door*>(Path_TLV::TLV_Next_Of_Type_446500(pTlvIter, ReliveTypes::eDoor));
            }

            const auto pCamPos = pScreenManager->mCamPos;
            const auto xpos = pScreenManager->mCamXOff + ((pTlvIter->mTopLeftX + pTlvIter->mBottomRightX) / 2) - FP_GetExponent(pCamPos->x);
            const auto ypos = pScreenManager->mCamYOff + pTlvIter->mTopLeftY - FP_GetExponent(pCamPos->y);
            relive_new CameraSwapper(
                field_34_camera_array[0]->field_C_ppBits,
                field_10_screenChangeEffect,
                static_cast<s16>(xpos),
                static_cast<s16>(ypos));
        }
        else
        {
            relive_new CameraSwapper(field_34_camera_array[0]->field_C_ppBits, field_10_screenChangeEffect, 184, 120);
        }
    }
    bHideLoadingIcon_5076A0 = 0;
    loading_ticks_5076A4 = 0;
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

CameraSwapper* Map::FMV_Camera_Change(u8** ppBits, Map* pMap, EReliveLevelIds levelId)
{
    // This is required to make the movies work when abe completes scrabania and paramonia.
    // u16 41617 -> s16 -23919 -> u16 41617
    // TODO: should probably use the s32 type for every function parameter with a fmv base id
    u16 fmvBaseId = pMap->field_12_fmv_base_id;

    if (fmvBaseId > 10000u)
    {
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_434680(levelId, fmvBaseId / 10000);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_434680(levelId, fmvBaseId / 100 % 100);
        FmvInfo* pFmvRec3 = Path_Get_FMV_Record_434680(levelId, fmvBaseId % 100);

        if (pFmvRec1->field_8_stop_music || pFmvRec2->field_8_stop_music || pFmvRec3->field_8_stop_music)
        {
            BackgroundMusic::Stop();
            MusicController::EnableMusic(0);
        }

        u32 pos1 = 0;
        u32 pos2 = 0;
        u32 pos3 = 0;
        Get_fmvs_sectors_44FEB0(
            pFmvRec1->field_0_pName,
            pFmvRec2->field_0_pName,
            pFmvRec3->field_0_pName,
            &pos1,
            &pos2,
            &pos3);

        return relive_new CameraSwapper(
            ppBits,
            pos1,
            pFmvRec1->field_4_id,
            pos2,
            pFmvRec2->field_4_id,
            pos3,
            pFmvRec3->field_4_id,

            static_cast<s8>(pFmvRec1->field_6),
            pFmvRec1->field_A,
            pFmvRec1->field_C_volume,

            pFmvRec2->field_6,
            pFmvRec2->field_A,
            pFmvRec2->field_C_volume,

            pFmvRec3->field_6,
            pFmvRec3->field_A,
            pFmvRec3->field_C_volume);
    }
    else if (fmvBaseId > 100u)
    {
        // Double FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_434680(levelId, fmvBaseId / 100);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_434680(levelId, fmvBaseId % 100);
        if (pFmvRec1->field_8_stop_music || pFmvRec2->field_8_stop_music)
        {
            BackgroundMusic::Stop();
            MusicController::EnableMusic(0);
        }

        u32 cdPos1 = 0;
        u32 cdPos2 = 0;
        Get_fmvs_sectors_44FEB0(pFmvRec1->field_0_pName, pFmvRec2->field_0_pName, 0, &cdPos1, &cdPos2, 0);

        return relive_new CameraSwapper(
            ppBits,
            cdPos1,
            pFmvRec1->field_4_id,
            cdPos2,
            pFmvRec2->field_4_id,

            static_cast<s8>(pFmvRec1->field_6),
            pFmvRec1->field_A,
            pFmvRec1->field_C_volume,

            pFmvRec2->field_6,
            pFmvRec2->field_A,
            pFmvRec2->field_C_volume);
    }
    else // < 100
    {
        // Single FMV
        FmvInfo* pFmvRecord = Path_Get_FMV_Record_434680(levelId, fmvBaseId);
        if (pFmvRecord->field_8_stop_music)
        {
            BackgroundMusic::Stop();
            MusicController::EnableMusic(0);
        }

        u32 cdPos = 0;
        Get_fmvs_sectors_44FEB0(pFmvRecord->field_0_pName, 0, 0, &cdPos, 0, 0);
        return relive_new CameraSwapper(
            ppBits,
            cdPos,
            pFmvRecord->field_4_id,
            static_cast<s8>(pFmvRecord->field_6),
            pFmvRecord->field_A,
            pFmvRecord->field_C_volume);
    }
}

void Map::GetCurrentCamCoords(PSX_Point* pPoint)
{
    pPoint->x = field_D4_pPathData->field_C_grid_width * field_20_camX_idx;
    pPoint->y = field_D4_pPathData->field_E_grid_height * field_22_camY_idx;
}

void Map::Get_map_size(PSX_Point* pPoint)
{
    pPoint->x = field_D4_pPathData->field_8_bTop;
    pPoint->y = field_D4_pPathData->field_A_bBottom;
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
