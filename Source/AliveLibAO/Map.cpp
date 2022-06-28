#include "stdafx_ao.h"
#include "Map.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "ScreenManager.hpp"
#include "PathData.hpp"
#include "Psx.hpp"
#include "Game.hpp"
#include "Midi.hpp"
#include "BaseAliveGameObject.hpp"
#include "Door.hpp"
#include "Abe.hpp"
#include "PsxDisplay.hpp"
#include "FixedPoint.hpp"
#include "AmbientSound.hpp"
#include "FG1.hpp"
#include "Movie.hpp"
#include "BackgroundMusic.hpp"
#include "MusicController.hpp"
#include "CameraSwapper.hpp"
#include "Particle.hpp"
#include "LvlArchive.hpp"
#include "Collisions.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "Sfx.hpp"
#include "Elum.hpp"
#include "Sys.hpp"
#include "PlatformBase.hpp"
#include "Camera.hpp"

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

    mCamera = cam;
    field_12_fmv_base_id = fmvBaseId;
    mPath = path;
    mLevel = level;
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
    Path_Change* pTlv = nullptr;
    if (field_18_pAliveObj)
    {
        pTlv = static_cast<Path_Change*>(TLV_Get_At_446260(
            FP_GetExponent(field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::PathTransition_1));
    }

    if (field_18_pAliveObj && pTlv)
    {
        mLevel = MapWrapper::FromAO(pTlv->field_18_level);
        mPath = pTlv->field_1A_path;
        mCamera = pTlv->field_1C_camera;
        field_12_fmv_base_id = pTlv->field_1E_movie;

        field_10_screenChangeEffect = kPathChangeEffectToInternalScreenChangeEffect_4CDC78[pTlv->field_20_wipe];

        field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_LvlNumber = MapWrapper::FromAO(pTlv->field_18_level);
        field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_PathNumber = pTlv->field_1A_path;

        // TODO: Probably OG bug, when changing camera/path the TLV pointer can become invalid
        // resulting in a corrupted next_path_scale value ?
        // Pointer points to the Path res which is invalid after ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, i, TRUE, FALSE);
        // is called. Happens even if calling real func below.
        const auto next_path_scale = pTlv->field_22_next_path_scale;

        GoTo_Camera();

        switch (next_path_scale)
        {
            case Scale_short::eFull_0:
                sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromInteger(1);
                sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_32;
                if (gElum_507680)
                {
                    gElum_507680->mBaseAnimatedWithPhysicsGameObject_SpriteScale = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    gElum_507680->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_ZapLinesElum_28;
                }
                break;

            case Scale_short::eHalf_1:
                sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale = FP_FromDouble(0.5);
                sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_AbeMenu_Half_13;
                if (gElum_507680)
                {
                    gElum_507680->mBaseAnimatedWithPhysicsGameObject_SpriteScale = sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_SpriteScale;
                    gElum_507680->mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_ZapLinesElum_Half_9;
                }
                break;

            default:
                LOG_ERROR("Invalid scale " << static_cast<s16>(pTlv->field_22_next_path_scale));
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
                        MaxGridBlocks_41FA10(field_18_pAliveObj->mBaseAnimatedWithPhysicsGameObject_SpriteScale) - 1);
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

        const u32 pCamNameOffset = (sizeof(CameraName) * (field_20_camX_idx + field_22_camY_idx * field_24_max_cams_x));
        const u8* pPathRes = *GetPathResourceBlockPtr(mCurrentPath);
        auto pCameraName = reinterpret_cast<const CameraName*>(pPathRes + pCamNameOffset);

        // Convert the 2 digit camera number string to an integer
        mCamera = 1 * (pCameraName->name[7] - '0') + 10 * (pCameraName->name[6] - '0');

        GoTo_Camera();
    }
}

void Map::RemoveObjectsWithPurpleLight(s16 bMakeInvisible)
{
    auto pObjectsWithLightsArray = relive_new DynamicArrayT<BaseAnimatedWithPhysicsGameObject>(16);

    auto pPurpleLightArray = relive_new DynamicArrayT<Particle>(16);

    bool bAddedALight = false;
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        auto pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4)
            && pObjIter->mBaseAliveGameObjectFlags.Get(Flags_10A::e10A_Bit6)
            && pObjIter->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit3_Render)
            && !pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eDead)
            && pObjIter != sControlledCharacter_50767C)
        {
            bool bAdd = false;
            if (pObjIter->mBaseAnimatedWithPhysicsGameObject_LvlNumber == mCurrentLevel
                && pObjIter->mBaseAnimatedWithPhysicsGameObject_PathNumber == mCurrentPath)
            {
                PSX_RECT rect = {};
                rect.x = FP_GetExponent(pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos);
                rect.w = FP_GetExponent(pObjIter->mBaseAnimatedWithPhysicsGameObject_XPos);
                rect.y = FP_GetExponent(pObjIter->mBaseAnimatedWithPhysicsGameObject_YPos);
                rect.h = FP_GetExponent(pObjIter->mBaseAnimatedWithPhysicsGameObject_YPos);
                bAdd = Rect_Location_Relative_To_Active_Camera(&rect, 0) == CameraPos::eCamCurrent_0;
            }

            if (bAdd)
            {
                pObjectsWithLightsArray->Push_Back(pObjIter);

                const PSX_RECT objRect = pObjIter->VGetBoundingRect();

                const FP k60Scaled = pObjIter->mBaseAnimatedWithPhysicsGameObject_SpriteScale * FP_FromInteger(60);
                auto pPurpleLight = New_DestroyOrCreateObject_Particle_419D00(
                    FP_FromInteger((objRect.x + objRect.w) / 2),
                    FP_FromInteger((objRect.y + objRect.h) / 2) + k60Scaled,
                    pObjIter->mBaseAnimatedWithPhysicsGameObject_SpriteScale);

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
        SFX_Play_Pitch(SoundEffect::PossessEffect_21, 40, 2400, 0);

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
                    pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit3_Render);
                }
            }

            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
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
                    pLight->mBaseAnimatedWithPhysicsGameObject_Anim.VDecode();
                }
            }

            for (s32 i = 0; i < gObjListDrawables->Size(); i++)
            {
                BaseGameObject* pDrawable = gObjListDrawables->ItemAt(i);
                if (!pDrawable)
                {
                    break;
                }

                if (!pDrawable->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
                {
                    // TODO: Seems strange to check this flag, how did it get in the drawable list if its not a drawable ??
                    if (pDrawable->mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4))
                    {
                        pDrawable->VRender(gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer);
                    }
                }
            }

            PSX_DrawSync_496750(0);
            pScreenManager->VRender(gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer);
            SYS_EventsPump_44FF90();
            gPsxDisplay_504C78.PSX_Display_Render_OT_40DD20();
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
                pObj->mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit3_Render);
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
    SND_Stop_Channels_Mask_4774A0(gSndChannels_507CA0);
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
            BaseGameObject* pItem = gBaseGameObjects->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();
            if (pItem->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
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
        BaseGameObject* pItem = gBaseGameObjects->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            if (pItem->mBaseGameObjectRefCount == 0)
            {
                i = gBaseGameObjects->RemoveAt(i);
                delete pItem;
            }
        }
    }

    if (sMap_bDoPurpleLightEffect_507C9C || mLevel != mCurrentLevel)
    {
        if (mLevel != mCurrentLevel)
        {
            SsUtAllKeyOff_49EDE0(0);
        }

        // TODO: Re-check this logic
        if (mLevel != EReliveLevelIds::eMenu)
        {
            if ((mLevel != EReliveLevelIds::eRuptureFarmsReturn && mLevel != EReliveLevelIds::eForestChase && mLevel != EReliveLevelIds::eDesertEscape) || (mLevel == EReliveLevelIds::eBoardRoom && mCurrentLevel == EReliveLevelIds::eBoardRoom))
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
    return Path_Get_Bly_Record_434650(mLevel, mPath)->field_C_overlay_id;
}

Path_TLV* Map::Get_First_TLV_For_Offsetted_Camera(s16 cam_x_idx, s16 cam_y_idx)
{
    const auto camX = cam_x_idx + field_20_camX_idx;
    const auto camY = cam_y_idx + field_22_camY_idx;

    if (camX >= field_24_max_cams_x || camX < 0 || camY >= field_26_max_cams_y || camY < 0)
    {
        return nullptr;
    }

    u8* pPathData = *GetPathResourceBlockPtr(mCurrentPath);
    const s32* indexTable = reinterpret_cast<const s32*>(pPathData + field_D4_pPathData->field_18_object_index_table_offset);
    const s32 indexTableEntry = indexTable[(camX + (camY * field_24_max_cams_x))];
    if (indexTableEntry == -1 || indexTableEntry >= 0x100000)
    {
        return nullptr;
    }

    Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(pPathData + indexTableEntry + field_D4_pPathData->field_14_object_offset);
    pTlv->RangeCheck();
    return pTlv;
}

void Map::SaveBlyData(u8* pSaveBuffer)
{
    memcpy(pSaveBuffer, sSwitchStates_505568.mData, sizeof(sSwitchStates_505568.mData));

    u8* pAfterSwitchStates = pSaveBuffer + sizeof(sSwitchStates_505568.mData);
    for (s16 i = 1; i < Path_Get_Num_Paths(mCurrentLevel); i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_434650(mCurrentLevel, i);
        if (pPathRec->field_0_blyName)
        {
            auto ppPathRes = GetPathResourceBlockPtr(i);
            const PathData* pPathData = pPathRec->field_4_pPathData;

            const s32 widthCount = (pPathData->field_8_bTop - pPathData->field_4_bLeft) / pPathData->field_C_grid_width;
            const s32 heightCount = (pPathData->field_A_bBottom - pPathData->field_6_bRight) / pPathData->field_E_grid_height;
            const s32 totalCameraCount = widthCount * heightCount;
            const s32* pIndexTable = reinterpret_cast<const s32*>(&(*ppPathRes)[pPathData->field_18_object_index_table_offset]);

            for (s32 j = 0; j < totalCameraCount; j++)
            {
                const s32 tlvOffset = pIndexTable[j];
                if (tlvOffset != -1 && tlvOffset < 0x100000)
                {
                    u8* ptr = &(*ppPathRes)[pPathData->field_14_object_offset + tlvOffset];
                    Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(ptr);
                    pTlv->RangeCheck();

                    for (;;)
                    {
                        BitField8<TLV_Flags> flags = pTlv->field_0_flags;
                        if (flags.Get(eBit1_Created))
                        {
                            flags.Clear(eBit1_Created);
                            flags.Clear(eBit2_Destroyed);
                        }

                        // Save the flags
                        *pAfterSwitchStates = flags.Raw().all;
                        pAfterSwitchStates++;
                        *pAfterSwitchStates = pTlv->field_1_unknown;
                        pAfterSwitchStates++;

                        if (pTlv->field_0_flags.Get(eBit3_End_TLV_List))
                        {
                            break;
                        }

                        pTlv = Path_TLV::Next_446460(pTlv);
                        pTlv->RangeCheck();
                    }
                }
            }
        }
    }
}

void Map::RestoreBlyData(const u8* pSaveData)
{
    memcpy(sSwitchStates_505568.mData, pSaveData, sizeof(sSwitchStates_505568.mData));
    const u8* pAfterSwitchStates = pSaveData + sizeof(sSwitchStates_505568.mData);

    for (s16 i = 1; i < Path_Get_Num_Paths(mCurrentLevel); i++)
    {
        auto ppPathRes = GetPathResourceBlockPtr(i);
        if (ppPathRes)
        {
            const PathBlyRec* pPathRec = Path_Get_Bly_Record_434650(mCurrentLevel, i);
            if (pPathRec->field_0_blyName)
            {
                const PathData* pPathData = pPathRec->field_4_pPathData;
                const s32* pIndexTable = reinterpret_cast<const s32*>(&(*ppPathRes)[pPathData->field_18_object_index_table_offset]);
                const s32 totalCameraCount = (pPathData->field_8_bTop - pPathData->field_4_bLeft) / pPathData->field_C_grid_width * ((pPathData->field_A_bBottom - pPathData->field_6_bRight) / pPathData->field_E_grid_height);

                for (s32 j = 0; j < totalCameraCount; j++)
                {
                    const s32 index_table_value = pIndexTable[j];
                    if (index_table_value != -1 && index_table_value < 0x100000)
                    {
                        auto pTlv = reinterpret_cast<Path_TLV*>(&(*ppPathRes)[pPathData->field_14_object_offset + index_table_value]);
                        for (;;)
                        {
                            pTlv->RangeCheck();
                            pTlv->field_0_flags.Raw().all = *pAfterSwitchStates;
                            pAfterSwitchStates++;

                            pTlv->field_1_unknown = *pAfterSwitchStates;
                            pAfterSwitchStates++;
                            if (pTlv->field_0_flags.Get(eBit3_End_TLV_List))
                            {
                                break;
                            }

                            pTlv = Path_TLV::Next_NoCheck(pTlv);
                            pTlv->RangeCheck();
                            if (pTlv->field_2_length == 0)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Map::Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    u8* pPathData = *GetPathResourceBlockPtr(mCurrentPath);
    u32* pIndexTableStart = reinterpret_cast<u32*>(pPathData + field_D4_pPathData->field_18_object_index_table_offset);

    const s32 totalCams = field_26_max_cams_y * field_24_max_cams_x;

    const s32 cam_global_left = field_D4_pPathData->field_C_grid_width * cam_x_idx;
    const s32 cam_global_right = cam_global_left + field_D4_pPathData->field_C_grid_width;

    const s32 cam_y_grid_top = field_D4_pPathData->field_E_grid_height * cam_y_idx;
    const s32 cam_y_grid_bottom = cam_y_grid_top + field_D4_pPathData->field_E_grid_height;

    for (s32 camNum = 0; camNum < totalCams; camNum++)
    {
        const s32 index_table_value = pIndexTableStart[camNum];
        if (index_table_value != -1 && index_table_value < 0x100000)
        {
            // Get the first TLV at this index table entry
            Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(&pPathData[index_table_value + field_D4_pPathData->field_14_object_offset]);
            pTlv->RangeCheck();

            // Enumerate the TLVs
            for (;;)
            {
                if (pTlv->field_10_top_left.field_0_x >= cam_global_left && pTlv->field_10_top_left.field_0_x <= cam_global_right)
                {
                    if (pTlv->field_10_top_left.field_2_y >= cam_y_grid_top && pTlv->field_10_top_left.field_2_y <= cam_y_grid_bottom && (!pTlv->field_0_flags.Get(eBit1_Created) && !pTlv->field_0_flags.Get(eBit2_Destroyed)))
                    {
                        Start_Sounds_for_TLV(direction, pTlv);
                    }
                }

                if (pTlv->field_0_flags.Get(eBit3_End_TLV_List))
                {
                    break;
                }

                pTlv = Path_TLV::Next_NoCheck(pTlv);
                pTlv->RangeCheck();
            }
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
    Path_Change* pPathChangeTLV = nullptr;
    CameraSwapEffects convertedSwapEffect = CameraSwapEffects::eInstantChange_0;
    if (pObj)
    {
        pPathChangeTLV = static_cast<Path_Change*>(TLV_Get_At_446260(
            FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos),
            FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_XPos),
            FP_GetExponent(pObj->mBaseAnimatedWithPhysicsGameObject_YPos),
            TlvTypes::PathTransition_1));
    }

    if (pObj && pPathChangeTLV)
    {
        mLevel = MapWrapper::FromAO(pPathChangeTLV->field_18_level);
        mPath = pPathChangeTLV->field_1A_path;
        mCamera = pPathChangeTLV->field_1C_camera;
        if (swapEffect < 0)
        {
            // Map the TLV/editor value of screen change to the internal screen change
            convertedSwapEffect = kPathChangeEffectToInternalScreenChangeEffect_4CDC78[pPathChangeTLV->field_20_wipe];
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

        mPath = mCurrentPath;
        mLevel = mCurrentLevel;
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
    if (Event_Get(kEventDeathReset))
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

    if (pRect->x > FP_GetExponent(field_2C_camera_offset.field_0_x + xTweak))
    {
        return CameraPos::eCamRight_4;
    }

    if (pRect->y > FP_GetExponent(field_2C_camera_offset.field_4_y + yTweak))
    {
        return CameraPos::eCamBottom_2;
    }

    if (pRect->w >= FP_GetExponent(field_2C_camera_offset.field_0_x - xTweak))
    {
        if (pRect->h < FP_GetExponent(field_2C_camera_offset.field_4_y - yTweak))
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

Path_TLV* Map::TLV_Get_At_446260(s16 xpos, s16 ypos, s16 width, s16 height, TlvTypes typeToFind)
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
    const s32* indexTable = reinterpret_cast<const s32*>(*GetPathResourceBlockPtr(mCurrentPath) + field_D4_pPathData->field_18_object_index_table_offset);

    s32 indexTableEntry = indexTable[(grid_cell_x + (grid_cell_y * field_24_max_cams_x))];
    if (indexTableEntry == -1 || indexTableEntry >= 0x100000)
    {
        return nullptr;
    }

    Path_TLV* pTlvIter = reinterpret_cast<Path_TLV*>(*GetPathResourceBlockPtr(mCurrentPath) + indexTableEntry + field_D4_pPathData->field_14_object_offset);
    pTlvIter->RangeCheck();

    while (right > pTlvIter->field_14_bottom_right.field_0_x
           || left < pTlvIter->field_10_top_left.field_0_x
           || bottom < pTlvIter->field_10_top_left.field_2_y
           || top > pTlvIter->field_14_bottom_right.field_2_y
           || pTlvIter->field_4_type.mType != typeToFind)
    {
        if (pTlvIter->field_0_flags.Get(eBit3_End_TLV_List))
        {
            return nullptr;
        }

        pTlvIter = Path_TLV::Next_446460(pTlvIter);
        pTlvIter->RangeCheck();
    }
    return pTlvIter;
}

Path_TLV* Map::TLV_Get_At_446060(Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height)
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

        u8* ppPathRes = *GetPathResourceBlockPtr(mCurrentPath);
        const s32* pIndexTable = reinterpret_cast<const s32*>(ppPathRes + pPathData->field_18_object_index_table_offset);
        const s32 indexTableEntry = pIndexTable[camX + (field_24_max_cams_x * camY)];

        if (indexTableEntry == -1 || indexTableEntry >= 0x100000)
        {
            return nullptr;
        }

        pTlv = reinterpret_cast<Path_TLV*>(&ppPathRes[pPathData->field_14_object_offset + indexTableEntry]);
        pTlv->RangeCheck();

        if (!bContinue || (xpos_converted <= pTlv->field_14_bottom_right.field_0_x && width_converted >= pTlv->field_10_top_left.field_0_x && height_converted >= pTlv->field_10_top_left.field_2_y && ypos_converted <= pTlv->field_14_bottom_right.field_2_y))
        {
            return pTlv;
        }
    }

    if (pTlv->field_0_flags.Get(eBit3_End_TLV_List))
    {
        return nullptr;
    }

    while (1)
    {
        pTlv = Path_TLV::Next_446460(pTlv);
        pTlv->RangeCheck();

        if (!bContinue || (xpos_converted <= pTlv->field_14_bottom_right.field_0_x && width_converted >= pTlv->field_10_top_left.field_0_x && height_converted >= pTlv->field_10_top_left.field_2_y && ypos_converted <= pTlv->field_14_bottom_right.field_2_y))
        {
            break;
        }

        if (pTlv->field_0_flags.Get(eBit3_End_TLV_List))
        {
            return 0;
        }
    }
    return pTlv;
}

void Map::sub_447430(u16 pathNum)
{
    const auto pPathData = Path_Get_Bly_Record_434650(mCurrentLevel, pathNum)->field_4_pPathData;
    const auto pPathRes = *GetPathResourceBlockPtr(pathNum);

    const auto counterInit = (pPathData->field_A_bBottom - pPathData->field_6_bRight)
                           / pPathData->field_E_grid_height
                           * ((pPathData->field_8_bTop - pPathData->field_4_bLeft) / pPathData->field_C_grid_width);

    if (counterInit > 0)
    {
        u32* pObjectTable = reinterpret_cast<u32*>(&pPathRes[pPathData->field_18_object_index_table_offset]);

        for (auto counter = 0; counter < counterInit; counter++)
        {
            if (pObjectTable[counter] != -1)
            {
                auto pTlv = reinterpret_cast<Path_TLV*>(&pPathRes[pPathData->field_14_object_offset + pObjectTable[counter]]);

                pTlv->RangeCheck();

                pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Destroyed);
                while (!pTlv->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
                {
                    pTlv = Path_TLV::Next_NoCheck(pTlv);

                    pTlv->RangeCheck();
                    if (pTlv->field_2_length == 0)
                    {
                        break;
                    }

                    pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
                    pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Destroyed);
                }
            }
        }
    }
}

Path_TLV* Map::TLV_First_Of_Type_In_Camera(TlvTypes type, s32 camX)
{
    Path_TLV* pTlvIter = Get_First_TLV_For_Offsetted_Camera(static_cast<s16>(camX), 0);
    if (!pTlvIter)
    {
        return nullptr;
    }

    while (pTlvIter->field_4_type.mType != type)
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
            Loader(pCamera->field_14_cam_x, pCamera->field_16_cam_y, LoadMode::LoadResourceFromList_1, TlvTypes::None_m1); // none = load all
        }
        else
        {
            // Blocking camera load
            ResourceManager::LoadResourceFile_455270(pCamera->field_1E_fileName, pCamera);
            pCamera->field_30_flags |= 1u;
            pCamera->field_C_ppBits = ResourceManager::GetLoadedResource(ResourceManager::Resource_Bits, pCamera->field_10_resId, 1, 0);
            sCameraBeingLoaded_507C98 = pCamera;
            Loader(pCamera->field_14_cam_x, pCamera->field_16_cam_y, LoadMode::LoadResource_2, TlvTypes::None_m1); // none = load all
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
            if (sActiveHero_507678 && sActiveHero_507678->mCurrentMotion == eAbeMotions::Motion_61_Respawn_42CD20)
            {
                pTemp->field_30_flags |= 2u;
            }
            return pTemp;
        }
    }

    // Get a pointer to the camera name from the Path resource
    const u8* pPathData = *GetPathResourceBlockPtr(mCurrentPath);
    auto pCamName = reinterpret_cast<const CameraName*>(&pPathData[(sizeof(CameraName) * (xpos + field_24_max_cams_x * ypos))]);

    // Empty/blank camera in the map array
    if (!pCamName->name[0])
    {
        return nullptr;
    }

    auto newCamera = relive_new Camera();

    // Copy in the camera name from the Path resource and append .CAM
    memset(newCamera->field_1E_fileName, 0, sizeof(newCamera->field_1E_fileName));
    strncpy(newCamera->field_1E_fileName, pCamName->name, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->field_1E_fileName, ".CAM");

    newCamera->field_14_cam_x = xpos;
    newCamera->field_16_cam_y = ypos;

    newCamera->field_30_flags &= ~1u;

    newCamera->field_1A_level = mCurrentLevel;
    newCamera->field_18_path = mCurrentPath;

    // Calculate hash/resource ID of the camera
    newCamera->field_10_resId = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0') + 100 * (pCamName->name[4] - '0') + 1000 * (pCamName->name[3] - '0');

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
            if (pHeader->field_8_type == ResourceManager::Resource_FG1)
            {
                relive_new FG1(ppRes);
            }
        }
    }
}

// OG only allows for 30 paths, given the editor allows for the full 99 we have to use a bigger array in a non ABI breaking way
// Note that currently there is only ever 1 Map object instance thus this global workaround is OK for now.
struct Map_PathsArrayExtended final
{
    u8** field_0_pPathRecs[99];
};
static Map_PathsArrayExtended sPathsArrayExtended = {};

void Map::FreePathResourceBlocks()
{
    for (s32 i = 0; i < Path_Get_Num_Paths(mCurrentLevel); ++i)
    {
        if (sPathsArrayExtended.field_0_pPathRecs[i])
        {
            ResourceManager::FreeResource_455550(sPathsArrayExtended.field_0_pPathRecs[i]);
            sPathsArrayExtended.field_0_pPathRecs[i] = nullptr;
        }

        if (i < ALIVE_COUNTOF(field_5C_path_res_array.field_0_pPathRecs))
        {
            if (field_5C_path_res_array.field_0_pPathRecs[i])
            {
                field_5C_path_res_array.field_0_pPathRecs[i] = nullptr;
            }
        }
    }
}

void Map::GetPathResourceBlockPtrs()
{
    // Get pointer to each PATH
    for (s32 i = 1; i < Path_Get_Num_Paths(mLevel); ++i)
    {
        sPathsArrayExtended.field_0_pPathRecs[i] = ResourceManager::GetLoadedResource(ResourceManager::Resource_Path, i, TRUE, FALSE);

        if (i < ALIVE_COUNTOF(field_5C_path_res_array.field_0_pPathRecs))
        {
            field_5C_path_res_array.field_0_pPathRecs[i] = sPathsArrayExtended.field_0_pPathRecs[i];
        }
    }
}

u8** Map::GetPathResourceBlockPtr(u32 pathId)
{
    if (pathId < ALIVE_COUNTOF(field_5C_path_res_array.field_0_pPathRecs))
    {
        return field_5C_path_res_array.field_0_pPathRecs[pathId];
    }
    return sPathsArrayExtended.field_0_pPathRecs[pathId];
}

void Map::ClearPathResourceBlocks()
{
    for (s32 i = 0; i < ALIVE_COUNTOF(field_5C_path_res_array.field_0_pPathRecs); i++)
    {
        field_5C_path_res_array.field_0_pPathRecs[i] = nullptr;
    }

    for (s32 i = 0; i < ALIVE_COUNTOF(sPathsArrayExtended.field_0_pPathRecs); i++)
    {
         sPathsArrayExtended.field_0_pPathRecs[i] = nullptr;
    }
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
                BaseGameObject* pBaseGameObj = gBaseGameObjects->ItemAt(i);
                if (!pBaseGameObj)
                {
                    break;
                }

                if (pBaseGameObj->mBaseGameObjectFlags.Get(BaseGameObject::eUpdatable_Bit2))
                {
                    if (!pBaseGameObj->mBaseGameObjectFlags.Get(BaseGameObject::eDead) && (!sNumCamSwappers_507668 || pBaseGameObj->mBaseGameObjectFlags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
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
        while (!pFmvRet->mBaseGameObjectFlags.Get(BaseGameObject::eDead));
    }

    if (mCurrentLevel != EReliveLevelIds::eMenu)
    {
        if (mLevel != mCurrentLevel || (mPath != mCurrentPath && field_10_screenChangeEffect == CameraSwapEffects::ePlay1FMV_5))
        {
            Game_ShowLoadingIcon_445EB0();
        }
    }

    if (mLevel != mCurrentLevel || mPath != mCurrentPath)
    {
        mOverlayId = GetOverlayId();
    }

    if (mLevel != mCurrentLevel)
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
        auto tmp = sOverlayTable_4C5AA8.records[Path_Get_OverlayIdx(mLevel)].field_4_pos;
        sLvlArchive_4FFD60.OpenArchive(CdLvlName(mLevel), tmp);

        ResourceManager::LoadResourceFile_455270(Path_Get_BndName(mLevel), nullptr);

        GetPathResourceBlockPtrs();

        SND_Load_VABS_477040(Path_Get_MusicInfo(mLevel), Path_Get_Reverb(mLevel));

        // Struct is using AE format so pass address of seq table in the exe to avoid a crash
        //SND_Load_Seqs_477AB0(reinterpret_cast<OpenSeqHandleAE*>(0x4C9E70), rPathRoot.field_C_bsq_file_name);

        SND_Load_Seqs_477AB0(g_SeqTable_4C9E70, Path_Get_BsqFileName(mLevel));
        relive_new BackgroundMusic(Path_Get_BackGroundMusicId(mLevel));

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

    if (!mPath)
    {
        mPath = 1;
    }


    const auto old_current_path = mCurrentPath;
    const auto old_current_level = mCurrentLevel;

    field_DA_bMapChanged = mPath != old_current_path || mLevel != mCurrentLevel;

    mCurrentCamera = mCamera;
    mCurrentPath = mPath;
    mCurrentLevel = mLevel;

    const PathBlyRec* pPathRecord = Path_Get_Bly_Record_434650(mLevel, mPath);
    field_D4_pPathData = pPathRecord->field_4_pPathData;
    field_24_max_cams_x = (field_D4_pPathData->field_8_bTop - field_D4_pPathData->field_4_bLeft) / field_D4_pPathData->field_C_grid_width;
    field_26_max_cams_y = (field_D4_pPathData->field_A_bBottom - field_D4_pPathData->field_6_bRight) / field_D4_pPathData->field_E_grid_height;

    char_type camNameBuffer[20] = {};
    Path_Format_CameraName_4346B0(camNameBuffer, mLevel, mPath, mCamera);

    const auto totalCams = field_26_max_cams_y * field_24_max_cams_x;

    s32 camIdx = 0;
    if (totalCams > 0)
    {
        auto ppPathRes = GetPathResourceBlockPtr(mPath);
        auto pName = reinterpret_cast<CameraName*>(&(*ppPathRes)[0]);
        for (camIdx = 0; camIdx < totalCams; camIdx++)
        {
            if (!strncmp(pName->name, camNameBuffer, sizeof(CameraName)))
            {
                break;
            }
            pName++;
        }
    }

    const auto camX_idx = static_cast<s16>(camIdx % field_24_max_cams_x);
    const auto camY_idx = static_cast<s16>(camIdx / field_24_max_cams_x);

    field_20_camX_idx = camX_idx;
    field_22_camY_idx = camY_idx;

    field_2C_camera_offset.field_0_x = FP_FromInteger(camX_idx * field_D4_pPathData->field_C_grid_width + 440);
    field_2C_camera_offset.field_4_y = FP_FromInteger(camY_idx * field_D4_pPathData->field_E_grid_height + 240);

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (sCollisions)
        {
            // OG FIX: Remove any pointers to the line objects that we are about to delete
            for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
            {
                BaseGameObject* pObjIter = gBaseGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
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

        sCollisions = relive_new Collisions(pPathRecord->field_8_pCollisionData, *GetPathResourceBlockPtr(mCurrentPath));
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

    Loader(field_20_camX_idx, field_22_camY_idx, LoadMode::ConstructObject_0, TlvTypes::None_m1); // none = load all

    if (old_current_path != mCurrentPath || old_current_level != mCurrentLevel)
    {
        if (sActiveHero_507678 && mCurrentPath == sActiveHero_507678->mBaseAnimatedWithPhysicsGameObject_PathNumber)
        {
            sActiveHero_507678->VCheckCollisionLineStillValid(10);
        }

        if (gElum_507680 && sControlledCharacter_50767C != gElum_507680 && mCurrentPath == gElum_507680->mBaseAnimatedWithPhysicsGameObject_PathNumber)
        {
            gElum_507680->VCheckCollisionLineStillValid(10);
        }
    }

    Create_FG1s();

    if (field_10_screenChangeEffect == CameraSwapEffects::ePlay1FMV_5)
    {
        FMV_Camera_Change(field_34_camera_array[0]->field_C_ppBits, this, mLevel);
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        pScreenManager->DecompressCameraToVRam(reinterpret_cast<u16**>(field_34_camera_array[0]->field_C_ppBits));
        pScreenManager->InvalidateRectCurrentIdx(0, 0, 640, 240);
        pScreenManager->MoveImage();
        pScreenManager->mFlags = (pScreenManager->mFlags & ~1) ^ 1;
    }

    if (field_10_screenChangeEffect != CameraSwapEffects::ePlay1FMV_5 && field_10_screenChangeEffect != CameraSwapEffects::eUnknown_11)
    {
        if (field_1E_door)
        {
            Path_Door* pTlvIter = static_cast<Path_Door*>(TLV_First_Of_Type_In_Camera(TlvTypes::Door_6, 0));
            while (pTlvIter->field_20_door_number != sActiveHero_507678->field_196_door_id)
            {
                pTlvIter = static_cast<Path_Door*>(Path_TLV::TLV_Next_Of_Type_446500(pTlvIter, TlvTypes::Door_6));
            }

            const auto pCamPos = pScreenManager->mCamPos;
            const auto xpos = pScreenManager->mCamXOff + ((pTlvIter->field_10_top_left.field_0_x + pTlvIter->field_14_bottom_right.field_0_x) / 2) - FP_GetExponent(pCamPos->field_0_x);
            const auto ypos = pScreenManager->mCamYOff + pTlvIter->field_10_top_left.field_2_y - FP_GetExponent(pCamPos->field_4_y);
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

void Map::Loader(s16 camX, s16 camY, LoadMode loadMode, TlvTypes typeToLoad)
{
    // Get a pointer to the array of index table offsets
    u8* pPathRes = *GetPathResourceBlockPtr(mCurrentPath);
    const s32* indexTable = reinterpret_cast<const s32*>(pPathRes + field_D4_pPathData->field_18_object_index_table_offset);

    // Calculate the index of the offset we want for the given camera at x/y
    s32 objectTableIdx = indexTable[camX + (camY * field_24_max_cams_x)];
    if (objectTableIdx == -1 || objectTableIdx >= 0x100000)
    {
        // -1 means there are no objects for the given camera
        return;
    }

    u8* ptr = pPathRes + objectTableIdx + field_D4_pPathData->field_14_object_offset;
    Path_TLV* pPathTLV = reinterpret_cast<Path_TLV*>(ptr);
    pPathTLV->RangeCheck();

    if (static_cast<s32>(pPathTLV->field_4_type.mType) <= 0x100000 && pPathTLV->field_2_length <= 0x2000u && pPathTLV->field_8 <= 0x1000000)
    {
        while (1)
        {
            if (typeToLoad == TlvTypes::None_m1 || typeToLoad == pPathTLV->field_4_type.mType)
            {
                if (loadMode != LoadMode::ConstructObject_0 || !(pPathTLV->field_0_flags.Get(TLV_Flags::eBit1_Created) || pPathTLV->field_0_flags.Get(TLV_Flags::eBit2_Destroyed)))
                {
                    TlvItemInfoUnion data;
                    data.parts.tlvOffset = static_cast<u16>(objectTableIdx);
                    data.parts.levelId = static_cast<u8>(MapWrapper::ToAO(mCurrentLevel));
                    data.parts.pathId = static_cast<u8>(mCurrentPath);

                    // Call the factory to construct the item
                    field_D4_pPathData->field_1C_object_funcs.object_funcs[static_cast<s16>(pPathTLV->field_4_type.mType)](pPathTLV, this, data, loadMode);

                    if (loadMode == LoadMode::ConstructObject_0)
                    {
                        pPathTLV->field_0_flags.Set(TLV_Flags::eBit1_Created);
                        pPathTLV->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);
                    }
                }
            }

            // End of TLV list for current camera
            if (pPathTLV->field_0_flags.Get(TLV_Flags::eBit3_End_TLV_List))
            {
                break;
            }

            objectTableIdx += pPathTLV->field_2_length;
            pPathTLV = Path_TLV::Next_446460(pPathTLV);
            pPathTLV->RangeCheck();
        }
    }
}

void Map::TLV_Reset(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(MapWrapper::ToAO(mCurrentLevel)))
    {
        const auto pBlyRec = Path_Get_Bly_Record_434650(MapWrapper::FromAO(static_cast<LevelIds>(data.parts.levelId)), data.parts.pathId);

        Path_TLV* pTlv = reinterpret_cast<Path_TLV*>(*GetPathResourceBlockPtr(data.parts.pathId) + pBlyRec->field_4_pPathData->field_14_object_offset + data.parts.tlvOffset);

        if (bSetDestroyed & 1)
        {
            pTlv->field_0_flags.Set(TLV_Flags::eBit2_Destroyed);
        }
        else
        {
            pTlv->field_0_flags.Clear(TLV_Flags::eBit2_Destroyed);
        }

        if (bSetCreated & 1)
        {
            pTlv->field_0_flags.Set(TLV_Flags::eBit1_Created);
        }
        else
        {
            pTlv->field_0_flags.Clear(TLV_Flags::eBit1_Created);
        }

        if (hiFlags != -1)
        {
            // Seems to be a blob per TLV specific bits
            pTlv->field_1_unknown = static_cast<u8>(hiFlags);
        }
    }
}

CameraSwapper* Map::FMV_Camera_Change(u8** ppBits, Map* pMap, EReliveLevelIds levelId)
{
    if (pMap->field_12_fmv_base_id > 10000u)
    {
        // TODO: Check division is correct
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_434680(levelId, pMap->field_12_fmv_base_id / 10000);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_434680(levelId, pMap->field_12_fmv_base_id / 100 % 100);
        FmvInfo* pFmvRec3 = Path_Get_FMV_Record_434680(levelId, pMap->field_12_fmv_base_id % 100);

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
    else if (pMap->field_12_fmv_base_id > 100u)
    {
        // Double FMV
        FmvInfo* pFmvRec1 = Path_Get_FMV_Record_434680(levelId, pMap->field_12_fmv_base_id / 100);
        FmvInfo* pFmvRec2 = Path_Get_FMV_Record_434680(levelId, pMap->field_12_fmv_base_id % 100);
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
        FmvInfo* pFmvRecord = Path_Get_FMV_Record_434680(levelId, pMap->field_12_fmv_base_id);
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
    pPoint->field_0_x = field_D4_pPathData->field_C_grid_width * field_20_camX_idx;
    pPoint->field_2_y = field_D4_pPathData->field_E_grid_height * field_22_camY_idx;
}

void Map::Get_map_size(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_pPathData->field_8_bTop;
    pPoint->field_2_y = field_D4_pPathData->field_A_bBottom;
}

Path_TLV* Path_TLV::Next_446460(Path_TLV* pTlv)
{
    return Next(pTlv);
}

Path_TLV* Path_TLV::TLV_Next_Of_Type_446500(Path_TLV* pTlv, TlvTypes type)
{
    pTlv->RangeCheck();

    pTlv = Path_TLV::Next_446460(pTlv);
    pTlv->RangeCheck();
    if (!pTlv)
    {
        return nullptr;
    }

    while (pTlv->field_4_type.mType != type)
    {
        pTlv = Path_TLV::Next_446460(pTlv);
        pTlv->RangeCheck();
        if (!pTlv)
        {
            return nullptr;
        }
    }

    return pTlv;
}

} // namespace AO
