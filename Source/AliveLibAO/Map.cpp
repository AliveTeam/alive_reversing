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

namespace AO {

class BaseGameObject;

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
    s16 field_12_pad;
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

EXPORT s32 CC MaxGridBlocks_41FA10(FP scale)
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

ALIVE_VAR(1, 0x507BA8, Map, gMap_507BA8, {});
ALIVE_VAR(1, 0x507C9C, s16, sMap_bDoPurpleLightEffect_507C9C, 0);
ALIVE_VAR(1, 0x507CA0, s32, gSndChannels_507CA0, 0);

void Map::ctor_static_443E10()
{
    gMap_507BA8.ctor();
    atexit(Map::dtor_static_443E60);
}


void Map::dtor_static_443E60()
{
    gMap_507BA8.Shutdown_443F90();
}

void Map::ctor()
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

void Map::Init_443EE0(LevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
{
    field_34_camera_array[0] = nullptr;
    field_34_camera_array[1] = nullptr;
    field_34_camera_array[2] = nullptr;
    field_34_camera_array[3] = nullptr;
    field_34_camera_array[4] = nullptr;

    field_28_cd_or_overlay_num = -1;

    field_4_current_camera = -1;
    field_2_current_path = -1;
    field_0_current_level = LevelIds::eNone;

    SetActiveCam_444660(level, path, camera, screenChangeEffect, fmvBaseId, forceChange);
    GoTo_Camera_445050();

    field_6_state = CamChangeStates::eInactive_0;
}

void Map::Shutdown_443F90()
{
    sLvlArchive_4FFD60.Free_41BEB0();
    stru_507C90.Free_41BEB0();

    FreePathResourceBlocks();

    // Free cameras
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
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
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        field_34_camera_array[i] = nullptr;
    }

    ClearPathResourceBlocks();

    field_D8 = 1;
    field_DC_free_all_anim_and_palts = 0;
    field_E0_save_data = 0;
}

s16 Map::SetActiveCam_444660(LevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange)
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

s16 Map::GetOverlayId_4440B0()
{
    return Path_Get_Bly_Record_434650(field_A_level, field_C_path)->field_C_overlay_id;
}

void Map::Handle_PathTransition_444DD0()
{
    Path_Change* pTlv = nullptr;
    if (field_18_pAliveObj)
    {
        pTlv = static_cast<Path_Change*>(TLV_Get_At_446260(
            FP_GetExponent(field_18_pAliveObj->field_A8_xpos),
            FP_GetExponent(field_18_pAliveObj->field_AC_ypos),
            FP_GetExponent(field_18_pAliveObj->field_A8_xpos),
            FP_GetExponent(field_18_pAliveObj->field_AC_ypos),
            TlvTypes::PathTransition_1));
    }

    if (field_18_pAliveObj && pTlv)
    {
        field_A_level = pTlv->field_18_level;
        field_C_path = pTlv->field_1A_path;
        field_E_camera = pTlv->field_1C_camera;
        field_12_fmv_base_id = pTlv->field_1E_movie;

        field_10_screenChangeEffect = kPathChangeEffectToInternalScreenChangeEffect_4CDC78[pTlv->field_20_wipe];

        field_18_pAliveObj->field_B2_lvl_number = pTlv->field_18_level;
        field_18_pAliveObj->field_B0_path_number = pTlv->field_1A_path;

        // TODO: Probably OG bug, when changing camera/path the TLV pointer can become invalid
        // resulting in a corrupted next_path_scale value ?
        // Pointer points to the Path res which is invalid after ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Path, i, TRUE, FALSE);
        // is called. Happens even if calling real func below.
        const auto next_path_scale = pTlv->field_22_next_path_scale;

        GoTo_Camera_445050();

        switch (next_path_scale)
        {
            case Scale_short::eFull_0:
                sActiveHero_507678->field_BC_sprite_scale = FP_FromInteger(1);
                sActiveHero_507678->field_10_anim.field_C_layer = Layer::eLayer_AbeMenu_32;
                if (gElum_507680)
                {
                    gElum_507680->field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
                    gElum_507680->field_10_anim.field_C_layer = Layer::eLayer_ZapLinesElum_28;
                }
                break;

            case Scale_short::eHalf_1:
                sActiveHero_507678->field_BC_sprite_scale = FP_FromDouble(0.5);
                sActiveHero_507678->field_10_anim.field_C_layer = Layer::eLayer_AbeMenu_Half_13;
                if (gElum_507680)
                {
                    gElum_507680->field_BC_sprite_scale = sActiveHero_507678->field_BC_sprite_scale;
                    gElum_507680->field_10_anim.field_C_layer = Layer::eLayer_ZapLinesElum_Half_9;
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
                        MaxGridBlocks_41FA10(field_18_pAliveObj->field_BC_sprite_scale) - 1);
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
        const u8* pPathRes = *GetPathResourceBlockPtr(field_2_current_path);
        auto pCameraName = reinterpret_cast<const CameraName*>(pPathRes + pCamNameOffset);

        // Convert the 2 digit camera number string to an integer
        field_E_camera = 1 * (pCameraName->name[7] - '0') + 10 * (pCameraName->name[6] - '0');

        GoTo_Camera_445050();
    }
}

void Map::RemoveObjectsWithPurpleLight_4440D0(s16 bMakeInvisible)
{
    auto pObjectsWithLightsArray = ao_new<DynamicArrayT<BaseAnimatedWithPhysicsGameObject>>();
    pObjectsWithLightsArray->ctor_4043E0(16);

    auto pPurpleLightArray = ao_new<DynamicArrayT<Particle>>();
    pPurpleLightArray->ctor_4043E0(16);

    bool bAddedALight = false;
    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
    {
        auto pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
        if (!pObjIter)
        {
            break;
        }

        if (pObjIter->field_6_flags.Get(BaseGameObject::eDrawable_Bit4)
            && pObjIter->field_10A_flags.Get(Flags_10A::e10A_Bit6)
            && pObjIter->field_10_anim.field_4_flags.Get(AnimFlags::eBit3_Render)
            && !pObjIter->field_6_flags.Get(BaseGameObject::eDead_Bit3)
            && pObjIter != sControlledCharacter_50767C)
        {
            bool bAdd = false;
            if (pObjIter->field_B2_lvl_number == field_0_current_level
                && pObjIter->field_B0_path_number == field_2_current_path)
            {
                PSX_RECT rect = {};
                rect.x = FP_GetExponent(pObjIter->field_A8_xpos);
                rect.w = FP_GetExponent(pObjIter->field_A8_xpos);
                rect.y = FP_GetExponent(pObjIter->field_AC_ypos);
                rect.h = FP_GetExponent(pObjIter->field_AC_ypos);
                bAdd = Rect_Location_Relative_To_Active_Camera_4448C0(&rect, 0) == CameraPos::eCamCurrent_0;
            }

            if (bAdd)
            {
                pObjectsWithLightsArray->Push_Back(pObjIter);

                PSX_RECT objRect = {};
                pObjIter->VGetBoundingRect(&objRect, 1);

                const FP k60Scaled = pObjIter->field_BC_sprite_scale * FP_FromInteger(60);
                auto pPurpleLight = New_DestroyOrCreateObject_Particle_419D00(
                    FP_FromInteger((objRect.x + objRect.w) / 2),
                    FP_FromInteger((objRect.y + objRect.h) / 2) + k60Scaled,
                    pObjIter->field_BC_sprite_scale);

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
        SFX_Play_43AE60(SoundEffect::PossessEffect_21, 40, 2400, 0);

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
                    pObj->field_10_anim.field_4_flags.Clear(AnimFlags::eBit3_Render);
                }
            }

            for (s32 i = 0; i < pPurpleLightArray->Size(); i++)
            {
                Particle* pLight = pPurpleLightArray->ItemAt(i);
                if (!pLight)
                {
                    break;
                }

                if (!pLight->field_6_flags.Get(BaseGameObject::eDead_Bit3))
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

                if (!pLight->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    pLight->field_10_anim.vDecode();
                }
            }

            for (s32 i = 0; i < gObjList_drawables_504618->Size(); i++)
            {
                BaseGameObject* pDrawable = gObjList_drawables_504618->ItemAt(i);
                if (!pDrawable)
                {
                    break;
                }

                if (!pDrawable->field_6_flags.Get(BaseGameObject::eDead_Bit3))
                {
                    // TODO: Seems strange to check this flag, how did it get in the drawable list if its not a drawable ??
                    if (pDrawable->field_6_flags.Get(BaseGameObject::eDrawable_Bit4))
                    {
                        pDrawable->VRender(gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer);
                    }
                }
            }

            PSX_DrawSync_496750(0);
            pScreenManager_4FF7C8->VRender(gPsxDisplay_504C78.field_C_drawEnv[gPsxDisplay_504C78.field_A_buffer_index].field_70_ot_buffer);
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
                pObj->field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
            }
        }
    }

    pObjectsWithLightsArray->field_4_used_size = 0;
    pPurpleLightArray->field_4_used_size = 0;

    if (pObjectsWithLightsArray)
    {
        pObjectsWithLightsArray->dtor_404440();
        ao_delete_free_447540(pObjectsWithLightsArray);
    }

    if (pPurpleLightArray)
    {
        pPurpleLightArray->dtor_404440();
        ao_delete_free_447540(pPurpleLightArray);
    }
}

void Map::ScreenChange_Common()
{
    if (field_6_state == CamChangeStates::eSliceCam_1)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        Handle_PathTransition_444DD0();
    }
    else if (field_6_state == CamChangeStates::eInstantChange_2)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        GoTo_Camera_445050();
    }

    field_6_state = CamChangeStates::eInactive_0;
    SND_Stop_Channels_Mask_4774A0(gSndChannels_507CA0);
    gSndChannels_507CA0 = 0;
}

void Map::ScreenChange_4444D0()
{
    if (field_6_state == CamChangeStates::eInactive_0)
    {
        return;
    }

    if (sMap_bDoPurpleLightEffect_507C9C && field_0_current_level != LevelIds::eBoardRoom_12)
    {
        RemoveObjectsWithPurpleLight_4440D0(1);
    }

    PSX_DrawSync_496750(0);

    for (s32 i = 0; i < 2; i++) // Not sure why this is done twice?
    {
        for (s32 j = 0; j < gBaseGameObject_list_9F2DF0->Size(); j++)
        {
            BaseGameObject* pItem = gBaseGameObject_list_9F2DF0->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();
            if (pItem->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                if (pItem->field_C_refCount == 0)
                {
                    j = gBaseGameObject_list_9F2DF0->RemoveAt(j);
                    pItem->VDestructor(1);
                }
            }
        }
    }

    for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pItem = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            if (pItem->field_C_refCount == 0)
            {
                i = gBaseGameObject_list_9F2DF0->RemoveAt(i);
                pItem->VDestructor(1);
            }
        }
    }

    if (sMap_bDoPurpleLightEffect_507C9C || field_A_level != field_0_current_level)
    {
        if (field_A_level != field_0_current_level)
        {
            SsUtAllKeyOff_49EDE0(0);
        }

        // TODO: Re-check this logic
        if (field_A_level != LevelIds::eMenu_0)
        {
            if ((field_A_level != LevelIds::eRemoved_11 && field_A_level != LevelIds::eRuptureFarmsReturn_13 && field_A_level != LevelIds::eForestChase_14 && field_A_level != LevelIds::eDesertEscape_15) || (field_A_level == LevelIds::eBoardRoom_12 && field_0_current_level == LevelIds::eBoardRoom_12))
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
    return Path_Get_Bly_Record_434650(field_A_level, field_C_path)->field_C_overlay_id;
}

Path_TLV* Map::Get_First_TLV_For_Offsetted_Camera_4463B0(s16 cam_x_idx, s16 cam_y_idx)
{
    const auto camX = cam_x_idx + field_20_camX_idx;
    const auto camY = cam_y_idx + field_22_camY_idx;

    if (camX >= field_24_max_cams_x || camX < 0 || camY >= field_26_max_cams_y || camY < 0)
    {
        return nullptr;
    }

    u8* pPathData = *GetPathResourceBlockPtr(field_2_current_path);
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

void Map::SaveBlyData_446900(u8* pSaveBuffer)
{
    memcpy(pSaveBuffer, sSwitchStates_505568.mData, sizeof(sSwitchStates_505568.mData));

    u8* pAfterSwitchStates = pSaveBuffer + sizeof(sSwitchStates_505568.mData);
    for (s16 i = 1; i < Path_Get_Num_Paths(field_0_current_level); i++)
    {
        const PathBlyRec* pPathRec = Path_Get_Bly_Record_434650(field_0_current_level, i);
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

void Map::RestoreBlyData_446A90(const u8* pSaveData)
{
    memcpy(sSwitchStates_505568.mData, pSaveData, sizeof(sSwitchStates_505568.mData));
    const u8* pAfterSwitchStates = pSaveData + sizeof(sSwitchStates_505568.mData);

    for (s16 i = 1; i < Path_Get_Num_Paths(field_0_current_level); i++)
    {
        auto ppPathRes = GetPathResourceBlockPtr(i);
        if (ppPathRes)
        {
            const PathBlyRec* pPathRec = Path_Get_Bly_Record_434650(field_0_current_level, i);
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

void Map::Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx)
{
    u8* pPathData = *GetPathResourceBlockPtr(field_2_current_path);
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
                        Start_Sounds_for_TLV_476640(direction, pTlv);
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

void Map::Start_Sounds_For_Objects_In_Near_Cameras_4467D0()
{
    SND_Reset_Ambiance_4765E0();

    if (Get_Camera_World_Rect_444C30(CameraPos::eCamLeft_3, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos::eCamLeft_3, field_20_camX_idx - 1, field_22_camY_idx);
    }

    if (Get_Camera_World_Rect_444C30(CameraPos::eCamRight_4, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos::eCamRight_4, field_20_camX_idx + 1, field_22_camY_idx);
    }

    if (Get_Camera_World_Rect_444C30(CameraPos::eCamTop_1, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos::eCamTop_1, field_20_camX_idx, field_22_camY_idx - 1);
    }

    if (Get_Camera_World_Rect_444C30(CameraPos::eCamBottom_2, nullptr))
    {
        Start_Sounds_For_Objects_In_Camera_4466A0(CameraPos::eCamBottom_2, field_20_camX_idx, field_22_camY_idx + 1);
    }
}

Camera* Map::GetCamera(CameraPos pos)
{
    return field_34_camera_array[static_cast<s32>(pos)];
}

s16 Map::SetActiveCameraDelayed_444CA0(MapDirections direction, BaseAliveGameObject* pObj, s16 swapEffect)
{
    Path_Change* pPathChangeTLV = nullptr;
    CameraSwapEffects convertedSwapEffect = CameraSwapEffects::eInstantChange_0;
    if (pObj)
    {
        pPathChangeTLV = static_cast<Path_Change*>(TLV_Get_At_446260(
            FP_GetExponent(pObj->field_A8_xpos),
            FP_GetExponent(pObj->field_AC_ypos),
            FP_GetExponent(pObj->field_A8_xpos),
            FP_GetExponent(pObj->field_AC_ypos),
            TlvTypes::PathTransition_1));
    }

    if (pObj && pPathChangeTLV)
    {
        field_A_level = pPathChangeTLV->field_18_level;
        field_C_path = pPathChangeTLV->field_1A_path;
        field_E_camera = pPathChangeTLV->field_1C_camera;
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

        field_C_path = field_2_current_path;
        field_A_level = field_0_current_level;
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

s16 Map::Is_Point_In_Current_Camera_4449C0(s32 level, s32 path, FP xpos, FP ypos, s16 width)
{
    if (static_cast<LevelIds>(level) != field_0_current_level || path != field_2_current_path) // TODO: Remove when 100%
    {
        return FALSE;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);
    return Rect_Location_Relative_To_Active_Camera_4448C0(&rect, width) == CameraPos::eCamCurrent_0;
}

s16 Map::Get_Camera_World_Rect_444C30(CameraPos camIdx, PSX_RECT* pRect)
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

CameraPos Map::Rect_Location_Relative_To_Active_Camera_4448C0(PSX_RECT* pRect, s16 width)
{
    if (Event_Get_417250(kEventDeathReset_4))
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

CameraPos Map::GetDirection_444A40(s32 level, s32 path, FP xpos, FP ypos)
{
    if (level != static_cast<s32>(field_0_current_level))
    {
        return CameraPos::eCamInvalid_m1;
    }

    if (path != field_2_current_path)
    {
        return CameraPos::eCamInvalid_m1;
    }

    PSX_RECT rect = {};
    rect.x = FP_GetExponent(xpos);
    rect.w = FP_GetExponent(xpos);
    rect.y = FP_GetExponent(ypos);
    rect.h = FP_GetExponent(ypos);

    CameraPos ret = Rect_Location_Relative_To_Active_Camera_4448C0(&rect, 0);

    PSX_RECT camWorldRect = {};
    if (!Get_Camera_World_Rect_444C30(ret, &camWorldRect))
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

EXPORT Path_TLV* Map::TLV_Get_At_446260(s16 xpos, s16 ypos, s16 width, s16 height, TlvTypes typeToFind)
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
    const s32* indexTable = reinterpret_cast<const s32*>(*GetPathResourceBlockPtr(field_2_current_path) + field_D4_pPathData->field_18_object_index_table_offset);

    s32 indexTableEntry = indexTable[(grid_cell_x + (grid_cell_y * field_24_max_cams_x))];
    if (indexTableEntry == -1 || indexTableEntry >= 0x100000)
    {
        return nullptr;
    }

    Path_TLV* pTlvIter = reinterpret_cast<Path_TLV*>(*GetPathResourceBlockPtr(field_2_current_path) + indexTableEntry + field_D4_pPathData->field_14_object_offset);
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

        u8* ppPathRes = *GetPathResourceBlockPtr(field_2_current_path);
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
    const auto pPathData = Path_Get_Bly_Record_434650(field_0_current_level, pathNum)->field_4_pPathData;
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

Path_TLV* Map::TLV_First_Of_Type_In_Camera_4464A0(TlvTypes type, s32 camX)
{
    Path_TLV* pTlvIter = Get_First_TLV_For_Offsetted_Camera_4463B0(static_cast<s16>(camX), 0);
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

void Map::Load_Path_Items_445DA0(Camera* pCamera, LoadMode loadMode)
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
            Loader_446590(pCamera->field_14_cam_x, pCamera->field_16_cam_y, LoadMode::LoadResourceFromList_1, TlvTypes::None_m1); // none = load all
        }
        else
        {
            // Blocking camera load
            ResourceManager::LoadResourceFile_455270(pCamera->field_1E_fileName, pCamera);
            pCamera->field_30_flags |= 1u;
            pCamera->field_C_ppBits = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Bits, pCamera->field_10_resId, 1, 0);
            sCameraBeingLoaded_507C98 = pCamera;
            Loader_446590(pCamera->field_14_cam_x, pCamera->field_16_cam_y, LoadMode::LoadResource_2, TlvTypes::None_m1); // none = load all
        }
        sCameraBeingLoaded_507C98 = nullptr;
    }
}

Camera* Map::Create_Camera_445BE0(s16 xpos, s16 ypos, s32 /*a4*/)
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
            && field_48_stru_5[i]->field_1A_level == field_0_current_level
            && field_48_stru_5[i]->field_18_path == field_2_current_path
            && field_48_stru_5[i]->field_14_cam_x == xpos
            && field_48_stru_5[i]->field_16_cam_y == ypos)
        {
            auto pTemp = field_48_stru_5[i];
            field_48_stru_5[i] = nullptr;
            if (sActiveHero_507678 && sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_61_Respawn_42CD20)
            {
                pTemp->field_30_flags |= 2u;
            }
            return pTemp;
        }
    }

    // Get a pointer to the camera name from the Path resource
    const u8* pPathData = *GetPathResourceBlockPtr(field_2_current_path);
    auto pCamName = reinterpret_cast<const CameraName*>(&pPathData[(sizeof(CameraName) * (xpos + field_24_max_cams_x * ypos))]);

    // Empty/blank camera in the map array
    if (!pCamName->name[0])
    {
        return nullptr;
    }

    auto newCamera = ao_new<Camera>();
    newCamera->ctor_4446E0();

    // Copy in the camera name from the Path resource and append .CAM
    memset(newCamera->field_1E_fileName, 0, sizeof(newCamera->field_1E_fileName));
    strncpy(newCamera->field_1E_fileName, pCamName->name, ALIVE_COUNTOF(CameraName::name));
    strcat(newCamera->field_1E_fileName, ".CAM");

    newCamera->field_14_cam_x = xpos;
    newCamera->field_16_cam_y = ypos;

    newCamera->field_30_flags &= ~1u;

    newCamera->field_1A_level = field_0_current_level;
    newCamera->field_18_path = field_2_current_path;

    // Calculate hash/resource ID of the camera
    newCamera->field_10_resId = 1 * (pCamName->name[7] - '0') + 10 * (pCamName->name[6] - '0') + 100 * (pCamName->name[4] - '0') + 1000 * (pCamName->name[3] - '0');

    newCamera->field_1C = field_4_current_camera;

    return newCamera;
}

void Map::Create_FG1s_4447D0()
{
    pScreenManager_4FF7C8->UnsetDirtyBits_FG1_406EF0();

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
                FG1* pFG1 = ao_new<FG1>();
                pFG1->ctor_4539C0(ppRes);
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
    for (s32 i = 0; i < Path_Get_Num_Paths(field_0_current_level); ++i)
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
    for (s32 i = 1; i < Path_Get_Num_Paths(field_A_level); ++i)
    {
        sPathsArrayExtended.field_0_pPathRecs[i] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Path, i, TRUE, FALSE);

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

void Map::GoTo_Camera_445050()
{
    s16 bShowLoadingIcon = FALSE;

    //dword_507CA4 = 0; // never read

    if (field_0_current_level != LevelIds::eMenu_0 && field_0_current_level != LevelIds::eCredits_10 && field_0_current_level == LevelIds::eNone)
    {
        bShowLoadingIcon = TRUE;
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        CameraSwapper* pFmvRet = FMV_Camera_Change_4458D0(nullptr, this, field_0_current_level);
        for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
        {
            SYS_EventsPump_44FF90();

            BaseGameObject* pBaseGameObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
            if (!pBaseGameObj)
            {
                break;
            }

            if (pBaseGameObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) && pBaseGameObj->field_C_refCount == 0)
            {
                i = gBaseGameObject_list_9F2DF0->RemoveAt(i);
                pBaseGameObj->VDestructor(1);
                if (pBaseGameObj == pFmvRet)
                {
                    // FMV trans done
                    break;
                }
            }
            else if (pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
            {
                if (!pBaseGameObj->field_6_flags.Get(BaseGameObject::eDead_Bit3) && (!sNumCamSwappers_507668 || pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdateDuringCamSwap_Bit10)))
                {
                    if (pBaseGameObj->field_8_update_delay > 0)
                    {
                        pBaseGameObj->field_8_update_delay--;
                    }
                    else
                    {
                        pBaseGameObj->VUpdate();
                    }
                }
            }
        }
    }

    if (field_0_current_level != LevelIds::eMenu_0)
    {
        if (field_A_level != field_0_current_level || (field_C_path != field_2_current_path && field_10_screenChangeEffect == CameraSwapEffects::ePlay1FMV_5))
        {
            Game_ShowLoadingIcon_445EB0();
        }
    }

    if (field_A_level != field_0_current_level || field_C_path != field_2_current_path)
    {
        field_28_cd_or_overlay_num = GetOverlayId();
    }

    if (field_A_level != field_0_current_level)
    {
        ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);

        // Free all cameras
        for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
        {
            if (field_34_camera_array[i])
            {
                field_34_camera_array[i]->dtor_444700();
                ao_delete_free_447540(field_34_camera_array[i]);
                field_34_camera_array[i] = nullptr;
            }
        }

        if (field_0_current_level != LevelIds::eNone)
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
        auto tmp = sOverlayTable_4C5AA8.records[Path_Get_OverlayIdx(field_A_level)].field_4_pos;
        sLvlArchive_4FFD60.OpenArchive(CdLvlName(field_A_level), tmp);

        ResourceManager::LoadResourceFile_455270(Path_Get_BndName(field_A_level), nullptr);

        GetPathResourceBlockPtrs();

        SND_Load_VABS_477040(Path_Get_MusicInfo(field_A_level), Path_Get_Reverb(field_A_level));

        // Struct is using AE format so pass address of seq table in the exe to avoid a crash
        //SND_Load_Seqs_477AB0(reinterpret_cast<OpenSeqHandleAE*>(0x4C9E70), rPathRoot.field_C_bsq_file_name);

        SND_Load_Seqs_477AB0(g_SeqTable_4C9E70, Path_Get_BsqFileName(field_A_level));
        auto pBackgroundMusic = ao_new<BackgroundMusic>();
        pBackgroundMusic->ctor_476370(Path_Get_BackGroundMusicId(field_A_level));

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

    if (!field_C_path)
    {
        field_C_path = 1;
    }


    const auto old_current_path = field_2_current_path;
    const auto old_current_level = field_0_current_level;

    field_DA_bMapChanged = field_C_path != old_current_path || field_A_level != field_0_current_level;

    field_4_current_camera = field_E_camera;
    field_2_current_path = field_C_path;
    field_0_current_level = field_A_level;

    const PathBlyRec* pPathRecord = Path_Get_Bly_Record_434650(field_A_level, field_C_path);
    field_D4_pPathData = pPathRecord->field_4_pPathData;
    field_24_max_cams_x = (field_D4_pPathData->field_8_bTop - field_D4_pPathData->field_4_bLeft) / field_D4_pPathData->field_C_grid_width;
    field_26_max_cams_y = (field_D4_pPathData->field_A_bBottom - field_D4_pPathData->field_6_bRight) / field_D4_pPathData->field_E_grid_height;

    char_type camNameBuffer[20] = {};
    Path_Format_CameraName_4346B0(camNameBuffer, field_A_level, field_C_path, field_E_camera);

    const auto totalCams = field_26_max_cams_y * field_24_max_cams_x;

    s32 camIdx = 0;
    if (totalCams > 0)
    {
        auto ppPathRes = GetPathResourceBlockPtr(field_C_path);
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

    if (old_current_path != field_2_current_path || old_current_level != field_0_current_level)
    {
        if (sCollisions_DArray_504C6C)
        {
            // OG FIX: Remove any pointers to the line objects that we are about to delete
            for (s32 i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
            {
                BaseGameObject* pObjIter = gBaseGameObject_list_9F2DF0->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->field_4_typeId == Types::eScrab_77)
                {
                    auto pScrab = static_cast<BaseAliveGameObject*>(pObjIter);
                    if (pScrab->field_B0_path_number != field_2_current_path || pScrab->field_B2_lvl_number != field_0_current_level)
                    {
                        ALIVE_FATAL("Scrab(s) leaked!");
                    }
                }

                if (pObjIter->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
                {
                    auto pBaseAliveGameObj = static_cast<BaseAliveGameObject*>(pObjIter);
                    pBaseAliveGameObj->field_F4_pLine = nullptr;
                }
            }

            if (ObjListPlatforms_50766C->Size() > 0)
            {
                LOG_ERROR(ObjListPlatforms_50766C->Size() << " Platforms have been leaked!");
                ALIVE_FATAL("Platforms leaked!");
            }


            sCollisions_DArray_504C6C->dtor_40CFB0();
            ao_delete_free_447540(sCollisions_DArray_504C6C);
        }

        sCollisions_DArray_504C6C = ao_new<Collisions>();
        sCollisions_DArray_504C6C->ctor_40CF30(pPathRecord->field_8_pCollisionData, *GetPathResourceBlockPtr(field_2_current_path));
    }

    if (field_E0_save_data)
    {
        RestoreBlyData_446A90(field_E0_save_data);
        field_E0_save_data = nullptr;
    }

    // Copy camera array and blank out the source
    for (s32 i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
    {
        field_48_stru_5[i] = field_34_camera_array[i];
        field_34_camera_array[i] = nullptr;
    }

    field_34_camera_array[0] = Create_Camera_445BE0(field_20_camX_idx, field_22_camY_idx, 1);
    field_34_camera_array[3] = Create_Camera_445BE0(field_20_camX_idx - 1, field_22_camY_idx, 0);
    field_34_camera_array[4] = Create_Camera_445BE0(field_20_camX_idx + 1, field_22_camY_idx, 0);
    field_34_camera_array[1] = Create_Camera_445BE0(field_20_camX_idx, field_22_camY_idx - 1, 0);
    field_34_camera_array[2] = Create_Camera_445BE0(field_20_camX_idx, field_22_camY_idx + 1, 0);

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
            field_48_stru_5[i]->dtor_444700();
            ao_delete_free_447540(field_48_stru_5[i]);
            field_48_stru_5[i] = nullptr;
        }
    }

    Load_Path_Items_445DA0(field_34_camera_array[0], LoadMode::ConstructObject_0);
    ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);
    Load_Path_Items_445DA0(field_34_camera_array[3], LoadMode::ConstructObject_0);
    Load_Path_Items_445DA0(field_34_camera_array[4], LoadMode::ConstructObject_0);
    Load_Path_Items_445DA0(field_34_camera_array[1], LoadMode::ConstructObject_0);
    Load_Path_Items_445DA0(field_34_camera_array[2], LoadMode::ConstructObject_0);

    if (!pScreenManager_4FF7C8)
    {
        pScreenManager_4FF7C8 = ao_new<ScreenManager>();
        pScreenManager_4FF7C8->ctor_406830(field_34_camera_array[0]->field_C_ppBits, &field_2C_camera_offset);
    }

    Loader_446590(field_20_camX_idx, field_22_camY_idx, LoadMode::ConstructObject_0, TlvTypes::None_m1); // none = load all

    if (old_current_path != field_2_current_path || old_current_level != field_0_current_level)
    {
        if (sActiveHero_507678 && field_2_current_path == sActiveHero_507678->field_B0_path_number)
        {
            sActiveHero_507678->VCheckCollisionLineStillValid(10);
        }

        if (gElum_507680 && sControlledCharacter_50767C != gElum_507680 && field_2_current_path == gElum_507680->field_B0_path_number)
        {
            gElum_507680->VCheckCollisionLineStillValid(10);
        }
    }

    Create_FG1s_4447D0();

    if (field_10_screenChangeEffect == CameraSwapEffects::ePlay1FMV_5)
    {
        FMV_Camera_Change_4458D0(field_34_camera_array[0]->field_C_ppBits, this, field_A_level);
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eUnknown_11)
    {
        pScreenManager_4FF7C8->DecompressCameraToVRam_407110(reinterpret_cast<u16**>(field_34_camera_array[0]->field_C_ppBits));
        pScreenManager_4FF7C8->InvalidateRect_406CC0(0, 0, 640, 240);
        pScreenManager_4FF7C8->MoveImage_406C40();
        pScreenManager_4FF7C8->field_36_flags = (pScreenManager_4FF7C8->field_36_flags & ~1) ^ 1;
    }

    if (field_10_screenChangeEffect != CameraSwapEffects::ePlay1FMV_5 && field_10_screenChangeEffect != CameraSwapEffects::eUnknown_11)
    {
        if (field_1E_door)
        {
            Path_Door* pTlvIter = static_cast<Path_Door*>(TLV_First_Of_Type_In_Camera_4464A0(TlvTypes::Door_6, 0));
            while (pTlvIter->field_20_door_number != sActiveHero_507678->field_196_door_id)
            {
                pTlvIter = static_cast<Path_Door*>(Path_TLV::TLV_Next_Of_Type_446500(pTlvIter, TlvTypes::Door_6));
            }

            const auto pCamPos = pScreenManager_4FF7C8->field_10_pCamPos;
            const auto xpos = pScreenManager_4FF7C8->field_14_xpos + ((pTlvIter->field_10_top_left.field_0_x + pTlvIter->field_14_bottom_right.field_0_x) / 2) - FP_GetExponent(pCamPos->field_0_x);
            const auto ypos = pScreenManager_4FF7C8->field_16_ypos + pTlvIter->field_10_top_left.field_2_y - FP_GetExponent(pCamPos->field_4_y);
            auto pCameraSwapperMem = ao_new<CameraSwapper>();
            pCameraSwapperMem->ctor_48C7A0(
                field_34_camera_array[0]->field_C_ppBits,
                field_10_screenChangeEffect,
                static_cast<s16>(xpos),
                static_cast<s16>(ypos));
        }
        else
        {
            auto pCameraSwapper = ao_new<CameraSwapper>();
            pCameraSwapper->ctor_48C7A0(field_34_camera_array[0]->field_C_ppBits, field_10_screenChangeEffect, 184, 120);
        }
    }
    bHideLoadingIcon_5076A0 = 0;
    loading_ticks_5076A4 = 0;
}

void Map::Loader_446590(s16 camX, s16 camY, LoadMode loadMode, TlvTypes typeToLoad)
{
    // Get a pointer to the array of index table offsets
    u8* pPathRes = *GetPathResourceBlockPtr(field_2_current_path);
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
                    data.parts.levelId = static_cast<u8>(field_0_current_level);
                    data.parts.pathId = static_cast<u8>(field_2_current_path);

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

EXPORT void Map::TLV_Reset_446870(u32 tlvOffset_levelId_PathId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed)
{
    TlvItemInfoUnion data;
    data.all = tlvOffset_levelId_PathId;

    if (data.parts.levelId == static_cast<s32>(field_0_current_level))
    {
        const auto pBlyRec = Path_Get_Bly_Record_434650(static_cast<LevelIds>(data.parts.levelId), data.parts.pathId);

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


CameraSwapper* CC Map::FMV_Camera_Change_4458D0(u8** ppBits, Map* pMap, LevelIds levelId)
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
            BackgroundMusic::Stop_476290();
            MusicController::EnableMusic_443900(0);
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

        auto pCameraSwapper = ao_new<CameraSwapper>();
        return pCameraSwapper->ctor_48C6B0(
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
            BackgroundMusic::Stop_476290();
            MusicController::EnableMusic_443900(0);
        }

        u32 cdPos1 = 0;
        u32 cdPos2 = 0;
        Get_fmvs_sectors_44FEB0(pFmvRec1->field_0_pName, pFmvRec2->field_0_pName, 0, &cdPos1, &cdPos2, 0);

        auto pCameraSwapperMem = ao_new<CameraSwapper>();
        return pCameraSwapperMem->ctor_48C5E0(
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
            BackgroundMusic::Stop_476290();
            MusicController::EnableMusic_443900(0);
        }

        u32 cdPos = 0;
        Get_fmvs_sectors_44FEB0(pFmvRecord->field_0_pName, 0, 0, &cdPos, 0, 0);
        auto pCameraSwapperMem = ao_new<CameraSwapper>();
        return pCameraSwapperMem->ctor_48C540(
            ppBits,
            cdPos,
            pFmvRecord->field_4_id,
            static_cast<s8>(pFmvRecord->field_6),
            pFmvRecord->field_A,
            pFmvRecord->field_C_volume);
    }
}

void Map::GetCurrentCamCoords_444890(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_pPathData->field_C_grid_width * field_20_camX_idx;
    pPoint->field_2_y = field_D4_pPathData->field_E_grid_height * field_22_camY_idx;
}

void Map::Get_map_size_444870(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_pPathData->field_8_bTop;
    pPoint->field_2_y = field_D4_pPathData->field_A_bBottom;
}

Path_TLV* CCSTD Path_TLV::Next_446460(Path_TLV* pTlv)
{
    return Next(pTlv);
}

EXPORT Path_TLV* CCSTD Path_TLV::TLV_Next_Of_Type_446500(Path_TLV* pTlv, TlvTypes type)
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
