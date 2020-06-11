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

START_NS_AO

void Map_ForceLink() {}

ALIVE_VAR(1, 0x507678, BaseAliveGameObject*, gAbe_507678, nullptr);
ALIVE_VAR(1, 0x507680, BaseAliveGameObject*, gElum_507680, nullptr);
ALIVE_VAR(1, 0x50767C, BaseAliveGameObject*, sControlledCharacter_50767C, nullptr);

struct SwitchStates
{
    char mData[256];
};

ALIVE_VAR(1, 0x505568, SwitchStates, sSwitchStates_505568, {});


class BackgroundMusic : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return Vdtor_476A80(flags);
    }

    EXPORT BackgroundMusic* Vdtor_476A80(signed int /*flags*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    EXPORT BackgroundMusic* ctor_476370(__int16 /*musicId*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    __int16 field_10_music_id;
    //__int16 field_12_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x14);


struct OverlayRecord
{
    const char* field_0_fileName;
    DWORD field_4_size;
};
ALIVE_ASSERT_SIZEOF(OverlayRecord, 0x8);

struct OverlayRecords
{
    OverlayRecord records[54];
};

const OverlayRecords kOverlays =
{
  {
      { "\\S1.OVL;1", 5205u },
      { "\\S1.LVL;1", 5207u },
      { "\\S1.MOV;1", 6228u },
      { "\\R1.OVL;1", 46992u },
      { "\\R1.LVL;1", 47042u },
      { "\\R1.MOV;1", 50938u },
      { "\\E1.OVL;1", 109292u },
      { "\\E1.LVL;1", 109358u },
      { "\\E3.OVL;1", 109292u },
      { "\\E4.OVL;1", 109292u },
      { "\\E1.MOV;1", 112254u },
      { "\\L1.OVL;1", 113848u },
      { "\\L1.LVL;1", 113913u },
      { "\\L2.OVL;1", 113848u },
      { "\\L1.MOV;1", 116902u },
      { "\\F1.OVL;1", 123206u },
      { "\\F1.LVL;1", 123267u },
      { "\\F1.MOV;1", 126488u },
      { "\\F2.OVL;1", 140523u },
      { "\\F2.LVL;1", 140605u },
      { "\\F3.OVL;1", 140523u },
      { "\\F2.MOV;1", 143702u },
      { "\\F4.OVL;1", 140523u },
      { "\\F4.LVL;1", 140523u },
      { "\\D1.OVL;1", 149982u },
      { "\\D1.LVL;1", 150044u },
      { "\\D3.OVL;1", 149982u },
      { "\\D5.OVL;1", 149982u },
      { "\\D6.OVL;1", 149982u },
      { "\\D1.MOV;1", 153189u },
      { "\\D2.OVL;1", 164493u },
      { "\\D2.LVL;1", 164554u },
      { "\\D4.OVL;1", 164493u },
      { "\\D2.MOV;1", 167451u },
      { "\\D7.OVL;1", 164493u },
      { "\\D7.LVL;1", 164493u },
      { "\\E2.OVL;1", 174640u },
      { "\\E2.LVL;1", 174685u },
      { "\\E2.MOV;1", 177186u },
      { "\\R2.OVL;1", 178780u },
      { "\\R2.LVL;1", 178850u },
      { "\\R3.OVL;1", 178780u },
      { "\\R4.OVL;1", 178780u },
      { "\\R5.OVL;1", 178780u },
      { "\\R7.OVL;1", 178780u },
      { "\\R8.OVL;1", 178780u },
      { "\\R9.OVL;1", 178780u },
      { "\\RA.OVL;1", 178780u },
      { "\\R2.MOV;1", 182667u },
      { "\\R6.OVL;1", 178780u },
      { "\\R6.LVL;1", 178780u },
      { "\\C1.OVL;1", 178780u },
      { "\\C1.LVL;1", 178850u },
      { "\\C1.MOV;1", 182667u }
  }
};

ALIVE_VAR(1, 0x4C5AA8, OverlayRecords, sOverlayTable_4C5AA8, kOverlays);


struct OpenSeqHandle
{
    const char* field_0_mBsqName;
    int field_4_generated_res_id;
    __int16 field_8_sound_block_idx;
    __int16 field_9_volume;
    int field_C_ppSeq_Data;
    __int16 field_A_id_seqOpenId;
    __int16 field_12_pad;
};
ALIVE_ASSERT_SIZEOF(OpenSeqHandle, 0x14);

OpenSeqHandle g_SeqTable_4C9E70[165] =
{
  { "D1AMB.SEQ", 0, 0, 100, 0, -1, 0 },
  { "D2AMB.SEQ", 0, 0, 100, 0, -1, 0 },
  { "E1AMB.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E2AMB.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E2AMB2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "F1AMB.SEQ", 0, 0, 60, 0, -1, 0 },
  { "F2AMB.SEQ", 0, 0, 100, 0, -1, 0 },
  { "MLAMB.SEQ", 0, 0, 60, 0, -1, 0 },
  { "RFAMB.SEQ", 0, 0, 70, 0, -1, 0 },
  { "OPTAMB.SEQ", 0, 0, 100, 0, -1, 0 },
  { "GUN.SEQ", 0, 0, 40, 0, -1, 0 },
  { "OHM.SEQ", 0, 0, 80, 0, -1, 0 },
  { "MUDOHM.SEQ", 0, 0, 80, 0, -1, 0 },
  { "EBELL2.SEQ", 0, 0, 127, 0, -1, 0 },
  { "ABEMOUNT.SEQ", 0, 0, 85, 0, -1, 0 },
  { "ESCRATCH.SEQ", 0, 0, 45, 0, -1, 0 },
  { "SSCRATCH.SEQ", 0, 0, 55, 0, -1, 0 },
  { "PANTING.SEQ", 0, 0, 45, 0, -1, 0 },
  { "BATSQUEK.SEQ", 0, 0, 55, 0, -1, 0 },
  { "WHISTLE1.SEQ", 0, 0, 127, 0, -1, 0 },
  { "WHISTLE2.SEQ", 0, 0, 127, 0, -1, 0 },
  { "SLIGBOMB.SEQ", 0, 0, 127, 0, -1, 0 },
  { "SLIGBOM2.SEQ", 0, 0, 127, 0, -1, 0 },
  { "OOPS.SEQ", 0, 0, 40, 0, -1, 0 },
  { "PIGEONS.SEQ", 0, 0, 40, 0, -1, 0 },
  { "CHIPPER.SEQ", 0, 0, 90, 0, -1, 0 },
  { "PATROL.SEQ", 0, 0, 60, 0, -1, 0 },
  { "SLEEPING.SEQ", 0, 0, 60, 0, -1, 0 },
  { "ONCHAIN.SEQ", 0, 0, 60, 0, -1, 0 },
  { "SLOSLEEP.SEQ", 0, 0, 60, 0, -1, 0 },
  { "PARAPANT.SEQ", 0, 0, 35, 0, -1, 0 },
  { "GRINDER.SEQ", 0, 0, 35, 0, -1, 0 },
  { "BASICTRK.SEQ", 0, 0, 90, 0, -1, 0 },
  { "LE_LO_1.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_LO_2.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_LO_3.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_LO_4.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_SH_1.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_SH_2.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_SH_3.SEQ", 0, 0, 127, 0, -1, 0 },
  { "LE_SH_4.SEQ", 0, 0, 127, 0, -1, 0 },
  { "MYSTERY1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "MYSTERY2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "NEGATIV1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "NEGATIV3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "POSITIV1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "POSITIV9.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_0_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_0_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_0_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_1_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_1_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_1_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_1_4.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_1_5.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_2_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_2_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_2_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_2_4.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_2_5.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_3_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_4_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_5_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D1_6_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_0_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_0_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_1_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_1_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_2_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_2_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_4_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_5_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "D2_6_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "DE_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "DE_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "DE_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "E1_0_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_0_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_0_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_0_4.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_0_5.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_1_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_1_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_1_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "E1_1_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "E1_1_5.SEQ", 0, 0, 90, 0, -1, 0 },
  { "E1_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "E1_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "E1_6_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_0_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_0_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_0_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_1_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_1_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_1_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_2_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_2_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_2_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_3_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F1_6_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F2_0_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F2_1_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F2_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F2_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F2_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "F2_6_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "FE_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "FE_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "FE_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_0_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_0_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_0_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_0_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_0_5.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_1_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_1_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_1_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_1_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_1_5.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_2_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_2_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_2_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_2_5.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_3_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ML_6_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_0_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_0_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_0_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_0_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_1_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_1_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_1_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_2_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_2_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_2_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RF_6_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RE_2_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RE_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "RE_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "OPT_0_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_0_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_0_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_0_4.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_0_5.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_1_1.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_1_2.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_1_3.SEQ", 0, 0, 60, 0, -1, 0 },
  { "OPT_1_4.SEQ", 0, 0, 90, 0, -1, 0 },
  { "OPT_1_5.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ALL_4_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ALL_5_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ALL_5_2.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ALL_5_3.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ALL_7_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { "ALL_8_1.SEQ", 0, 0, 90, 0, -1, 0 },
  { nullptr, 0, 0, 0, 0, 0, 0 }
};



class Collisions
{
public:
    EXPORT void dtor_40CFB0()
    {
        ao_delete_free_447540(field_0);
    }

    EXPORT Collisions* ctor_40CF30(const CollisionInfo* /*pCollisionInfo*/, BYTE** /*ppPathData*/)
    {
        NOT_IMPLEMENTED();
        return this;
    }

    void* field_0;
    int field_4;
    int field_8;
    int field_C;
};
ALIVE_ASSERT_SIZEOF(Collisions, 0x10);

ALIVE_VAR(1, 0x504C6C, Collisions*, sCollisions_DArray_504C6C, nullptr);

EXPORT void CC SND_Kill_4774A0(int /*mask*/)
{
    NOT_IMPLEMENTED();
}

EXPORT void CC SsUtAllKeyOff_49EDE0(int /*a1*/)
{
    NOT_IMPLEMENTED();
}

ALIVE_VAR(1, 0x507BA8, Map, gMap_507BA8, {});
ALIVE_VAR(1, 0x507C9C, short, sMap_bDoPurpleLightEffect_507C9C, 0);
ALIVE_VAR(1, 0x507CA0, int, gSndChannels_507CA0, 0);
ALIVE_VAR(1, 0x507668, short, sNumCamSwappers_507668, 0);

void Map::Init_443EE0(LevelIds level, __int16 path, __int16 camera, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange)
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

    field_6_state = 0;
}

class LvlArchive
{
public:
    EXPORT void OpenArchive_41BC60(int /*pos*/)
    {
        NOT_IMPLEMENTED();
    }

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
    field_DC_free_all_anim_and_palts = 0;
    field_E0_save_data = 0;
}

__int16 Map::SetActiveCam_444660(LevelIds level, __int16 path, __int16 cam, CameraSwapEffects screenChangeEffect, __int16 fmvBaseId, __int16 forceChange)
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

void AO::Map::Handle_PathTransition_444DD0()
{
    NOT_IMPLEMENTED();
}

void AO::Map::RemoveObjectsWithPurpleLight_4440D0(__int16 /*bMakeInvisible*/)
{
    NOT_IMPLEMENTED();
}

void AO::Map::ScreenChange_Common()
{
    if (field_6_state == 1)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        Handle_PathTransition_444DD0();
    }
    else if (field_6_state == 2)
    {
        ResourceManager::Reclaim_Memory_455660(0);
        GoTo_Camera_445050();
    }

    field_6_state = 0;
    SND_Kill_4774A0(gSndChannels_507CA0);
    gSndChannels_507CA0 = 0;
}

void AO::Map::ScreenChange_4444D0()
{
    if (field_6_state == 0)
    {
        return;
    }

    if (sMap_bDoPurpleLightEffect_507C9C && field_0_current_level != LevelIds::eBoardRoom_12)
    {
        RemoveObjectsWithPurpleLight_4440D0(1);
    }

    PSX_DrawSync_496750(0);

    for (int i = 0; i < 2; i++) // Not sure why this is done twice?
    {
        for (int j = 0; j < gBaseGameObject_list_9F2DF0->Size(); j++)
        {
            BaseGameObject* pItem = gBaseGameObject_list_9F2DF0->ItemAt(j);
            if (!pItem)
            {
                break;
            }

            pItem->VScreenChanged();
            if (pItem->field_6_flags.Get(BaseGameObject::eDead_Bit3))
            {
                if (pItem->field_C_bCanKill == 0)
                {
                    j = gBaseGameObject_list_9F2DF0->RemoveAt(j);
                    pItem->VDestructor(1);
                }
            }
        }
    }

    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pItem = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pItem)
        {
            break;
        }

        if (pItem->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            if (pItem->field_C_bCanKill == 0)
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
            if ((field_A_level != LevelIds::eRemoved_11 &&
                field_A_level != LevelIds::eRuptureFarmsReturn_13 &&
                field_A_level != LevelIds::eForestChase &&
                field_A_level != LevelIds::eDesertEscape) ||
                (field_A_level == LevelIds::eBoardRoom_12 && field_0_current_level == LevelIds::eBoardRoom_12))
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

__int16 Map::GetOverlayId()
{
    return Path_Get_Bly_Record_434650(field_A_level, field_C_path)->field_C_overlay_id;
}


EXPORT void AO::Map::Load_Path_Items_445DA0(Camera* /*pCamera*/, __int16 /*kZero*/)
{
    NOT_IMPLEMENTED();
}

void AO::Map::RestoreObjectStates_446A90(__int16* /*pSaveData*/)
{
    NOT_IMPLEMENTED();
}

Camera* AO::Map::Create_Camera_445BE0(__int16 /*xpos*/, __int16 /*ypos*/, int /*a4*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void AO::Map::Create_FG1s_4447D0()
{
    NOT_IMPLEMENTED();
}

void Map::GoTo_Camera_445050()
{
    NOT_IMPLEMENTED();

    __int16 bShowLoadingIcon = FALSE;

    //dword_507CA4 = 0; // never read

    if (field_0_current_level != LevelIds::eMenu_0 && field_0_current_level != LevelIds::eCredits_10 && field_0_current_level == LevelIds::eNone)
    {
        bShowLoadingIcon = TRUE;
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eEffect11)
    {
        CameraSwapper* pFmvRet = FMV_Camera_Change_4458D0(nullptr, this, field_0_current_level);
        do
        {
            SYS_EventsPump_44FF90();
            for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
            {
                BaseGameObject* pBaseGameObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
                if (!pBaseGameObj)
                {
                    break;
                }

                if (pBaseGameObj->field_6_flags.Get(BaseGameObject::eUpdatable_Bit2))
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
        } while (!pFmvRet->field_6_flags.Get(BaseGameObject::eDead_Bit3));
    }

    if (field_0_current_level != LevelIds::eMenu_0)
    {
        if ((field_A_level != field_0_current_level || field_C_path != field_2_current_path && field_10_screenChangeEffect == CameraSwapEffects::eEffect5_1_FMV))
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
        for (int i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
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

            // Free all but the first ?
            for (int i = 1; i <= gMapData_4CAB58.paths[static_cast<int>(field_0_current_level)].field_18_num_paths; ++i)
            {
                ResourceManager::FreeResource_455550(field_5C_path_res_array.field_0_pPathRecs[i]);
                field_5C_path_res_array.field_0_pPathRecs[i] = nullptr;
            }

            SND_Reset_476BA0();
            ResourceManager::Reclaim_Memory_455660(0);
        }

        ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);

        const PathRoot& rPathRoot = gMapData_4CAB58.paths[static_cast<int>(field_A_level)];

        // Open Path BND
        auto tmp = sOverlayTable_4C5AA8.records[rPathRoot.field_1C_overlay_idx].field_4_size;
        sLvlArchive_4FFD60.OpenArchive_41BC60(tmp);

        ResourceManager::LoadResourceFile_455270(rPathRoot.field_38_bnd_name, 0, 0);

        // Get pointer to each PATH
        for (int i = 1; i <= rPathRoot.field_18_num_paths; ++i)
        {
            field_5C_path_res_array.field_0_pPathRecs[i] = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Path, i, TRUE, FALSE);
        }

        SND_Load_VABS_477040(rPathRoot.field_8_pMusicInfo, rPathRoot.field_10_reverb);
        SND_Load_Seqs_477AB0(g_SeqTable_4C9E70, rPathRoot.field_C_bsq_file_name);

        auto pBackgroundMusic = ao_new<BackgroundMusic>();
        pBackgroundMusic->ctor_476370(rPathRoot.field_12_bg_music_id);

        // TODO: Re-add function
        for (int i = 0; i < 59; i++)
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

    const auto levelId = this->field_A_level;

    const auto old_current_path = field_2_current_path;
    field_DA = field_C_path != old_current_path || levelId != field_0_current_level;
    const auto old_current_path2 = old_current_path;
    const auto old_current_level = field_0_current_level;
    field_4_current_camera = field_E_camera;
    field_2_current_path = field_C_path;
    field_0_current_level = levelId;
    const PathBlyRec* pPathRecord = Path_Get_Bly_Record_434650(levelId, field_C_path);
    const PathData* pPathData = pPathRecord->field_4_pPathData;
    const auto pPathRecord2 = pPathRecord;
    field_D4_pPathData = pPathData;
    const auto bRight = pPathData->field_6_bRight;
    const auto grid_height = pPathData->field_E_grid_height;
    field_24_max_cams_x = (pPathData->field_8_bTop - pPathData->field_4_bLeft) / pPathData->field_C_grid_width;
    field_26_max_cams_y = (pPathData->field_A_bBottom - bRight) / grid_height;
    
    const int kNameBufferSize = 8;
    char camNameBuffer[20] = {};
    Path_Format_CameraName_4346B0(camNameBuffer, levelId, field_C_path, field_E_camera);
    
    const auto camsX = field_24_max_cams_x;
    const auto camsX2 = camsX;
    const auto totalCams = kNameBufferSize * field_26_max_cams_y * camsX;

    int pCamPathIter = 0;
    if (totalCams <= 0)
    {
        pCamPathIter = 0;
    }
    else
    {
        auto curPathCamName = 0;
        auto ppPathRes = field_5C_path_res_array.field_0_pPathRecs[field_C_path];
        pCamPathIter = 0;
        auto pPathRes = *ppPathRes;
        do
        {
            if (!strncmp((const char*)&pPathRes[curPathCamName], camNameBuffer, kNameBufferSize))
            {
                break;
            }
            pCamPathIter += kNameBufferSize;
            curPathCamName = pCamPathIter;
        } while (pCamPathIter < totalCams);
    }

    const auto camX_idx = static_cast<short>((pCamPathIter / kNameBufferSize) % camsX2);
    const auto camY_idx = static_cast<short>((pCamPathIter / kNameBufferSize) / camsX2);
  
    field_20_camX_idx = camX_idx;
    field_22_camY_idx = camY_idx;

    field_2C_camera_offset.field_0_x = FP_FromInteger(camX_idx * pPathData->field_C_grid_width + 440);
    field_2C_camera_offset.field_4_y = FP_FromInteger(camY_idx * pPathData->field_E_grid_height + 240);

    if (old_current_path2 != field_2_current_path || old_current_level != field_0_current_level)
    {
        if (sCollisions_DArray_504C6C)
        {
            sCollisions_DArray_504C6C->dtor_40CFB0();
            ao_delete_free_447540(sCollisions_DArray_504C6C);
        }

        sCollisions_DArray_504C6C = ao_new<Collisions>();
        sCollisions_DArray_504C6C->ctor_40CF30(pPathRecord2->field_8_pCollisionData, field_5C_path_res_array.field_0_pPathRecs[field_2_current_path]);
    }

    if (field_E0_save_data)
    {
        RestoreObjectStates_446A90(field_E0_save_data);
        field_E0_save_data = 0;
    }

    // Copy camera array and blank out the source
    for (int i = 0; i < ALIVE_COUNTOF(field_34_camera_array); i++)
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
    for (int i = 0; i < ALIVE_COUNTOF(field_48_stru_5); i++)
    {
        if (field_48_stru_5[i])
        {
            ResourceManager::Free_Resources_For_Camera_447170(field_48_stru_5[i]);
        }
    }

    ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);

    // Free each camera itself
    for (int i = 0; i < ALIVE_COUNTOF(field_48_stru_5); i++)
    {
        if (field_48_stru_5[i])
        {
            field_48_stru_5[i]->dtor_444700();
            ao_delete_free_447540(field_48_stru_5[i]);
            field_48_stru_5[i] = nullptr;
        }
    }

    Load_Path_Items_445DA0(field_34_camera_array[0], 0);
    ResourceManager::LoadingLoop_41EAD0(bShowLoadingIcon);
    Load_Path_Items_445DA0(field_34_camera_array[3], 0);
    Load_Path_Items_445DA0(field_34_camera_array[4], 0);
    Load_Path_Items_445DA0(field_34_camera_array[1], 0);
    Load_Path_Items_445DA0(field_34_camera_array[2], 0);

    if (!pScreenManager_4FF7C8)
    {
        pScreenManager_4FF7C8 = ao_new<ScreenManager>();
        pScreenManager_4FF7C8->ctor_406830(field_34_camera_array[0]->field_C_ppBits, &field_2C_camera_offset);
    }
    
    Loader_446590(field_20_camX_idx, field_22_camY_idx, 0, -1);

    if (old_current_path2 != field_2_current_path || old_current_level != field_0_current_level)
    {
        if (gAbe_507678 && field_2_current_path == gAbe_507678->field_B0_path)
        {
            gAbe_507678->VCheckCollisionLineStillValid(10);
        }

        if (gElum_507680 && sControlledCharacter_50767C != gElum_507680 && field_2_current_path == gElum_507680->field_B0_path)
        {
            gElum_507680->VCheckCollisionLineStillValid(10);
        }
    }
    
    Create_FG1s_4447D0();

    if (field_10_screenChangeEffect == CameraSwapEffects::eEffect5_1_FMV)
    {
        FMV_Camera_Change_4458D0(field_34_camera_array[0]->field_C_ppBits, this, field_A_level);
    }

    if (field_10_screenChangeEffect == CameraSwapEffects::eEffect11)
    {
        pScreenManager_4FF7C8->DecompressToVRam_407110(reinterpret_cast<unsigned short**>(field_34_camera_array[0]->field_C_ppBits));
        pScreenManager_4FF7C8->InvalidateRect_406CC0(0, 0, 640, 240);
        pScreenManager_4FF7C8->MoveImage_406C40();
        pScreenManager_4FF7C8->field_36_flags = pScreenManager_4FF7C8->field_36_flags & ~1 ^ 1;
    }

    if (field_10_screenChangeEffect != CameraSwapEffects::eEffect5_1_FMV && field_10_screenChangeEffect != CameraSwapEffects::eEffect11)
    {
        if (field_1E_door)
        {
            /*
            for (pTlvIter = TLV_First_Of_Type_In_Camera_4464A0(6u, 0);
                pTlvIter->field_20_door_number != gAbe_507678->field_196_door_id;
                pTlvIter = TLV_Next_Of_Type_446500(&pTlvIter, 6u))
            {
                ;
            }
            auto pCameraSwapperMem = ao_new<CameraSwapper>();
            pCameraSwapperMem->ctor_48C7A0(
                field_34_camera_array[0]->field_C_ppBits,
                field_10_screenChangeEffect,
                pScreenManager_4FF7C8->field_14
                + (pTlvIter->field_10 + pTlvIter->field_14) / 
                - (pScreenManager_4FF7C8->field_10_pCamPos->field_0_x, 6),
                pScreenManager_4FF7C8->field_16
                + pTlvIter->field_12
                - FP_FromInteger(pScreenManager_4FF7C8->field_10_pCamPos->field_4_y));
            */
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

EXPORT BYTE* Map::TLV_Reset_446870(unsigned int /*a2*/, __int16 /*a3*/, unsigned __int8 /*a4*/, char /*a5*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}


CameraSwapper* CC AO::Map::FMV_Camera_Change_4458D0(BYTE** /*ppBits*/, Map* /*pMap*/, LevelIds /*levelId*/)
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void AO::Map::GetCurrentCamCoords_444890(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_pPathData->field_C_grid_width * field_20_camX_idx;
    pPoint->field_2_y = field_D4_pPathData->field_E_grid_height * field_22_camY_idx;
}

void AO::Map::Get_map_size_444870(PSX_Point* pPoint)
{
    pPoint->field_0_x = field_D4_pPathData->field_8_bTop;
    pPoint->field_2_y = field_D4_pPathData->field_A_bBottom;
}


AO::BaseGameObject* AO::CameraSwapper::VDestructor(signed int flags)
{
    return Vdtor_48D380(flags);
}

EXPORT CameraSwapper* AO::CameraSwapper::Vdtor_48D380(signed int /*flags*/)
{
    NOT_IMPLEMENTED();
    return this;
}

EXPORT CameraSwapper* CameraSwapper::ctor_48C7A0(BYTE** /*ppBits*/, CameraSwapEffects /*changeEffect*/, __int16 /*xpos*/, __int16 /*ypos*/)
{
    NOT_IMPLEMENTED();
    return this;
}

END_NS_AO
