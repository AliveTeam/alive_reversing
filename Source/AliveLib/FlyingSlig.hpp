#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_FlyingSlig_Data
{
    __int16 field_0_scale;
    __int16 field_2_state;
    __int16 field_4_hi_pause_time;
    __int16 field_6_patrol_pause_min;
    __int16 field_8_patrol_pause_max;
    __int16 field_A_direction;
    __int16 field_C_panic_delay;
    __int16 field_E_give_up_chase_delay;
    __int16 field_10_prechase_delay;
    __int16 field_12_slig_id;
    __int16 field_14_listen_time;
    __int16 field_16_trigger_id;
    __int16 field_18_grenade_delay;
    __int16 field_1A_max_velocity;
    __int16 field_1C_launch_id;
    __int16 field_1E_persistant;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig_Data, 0x20);

struct Path_FlyingSlig : public Path_TLV
{
    Path_FlyingSlig_Data field_10_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig, 0x30);

struct Path_FlyingSligSpawner : public Path_TLV
{
    Path_FlyingSlig_Data field_10;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FlyingSlig, 0x30);

struct FlyingSligSpawner_State
{
    Types field_0_type;
    //__int16 field_2_pad;
    int field_4_tlvInfo;
    int field_8_bSpawned;
    int field_C_spawned_slig_obj_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FlyingSligSpawner_State, 0x10);

class FlyingSligSpawner : public BaseGameObject
{
public:
    EXPORT static signed int CC CreateFromSaveState_43B690(const BYTE* pBuffer);

    EXPORT FlyingSligSpawner* ctor_433D50(Path_FlyingSligSpawner* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

private:
    EXPORT void dtor_434030();
    EXPORT FlyingSligSpawner* vdtor_433DE0(signed int flags);
    EXPORT void vUpdate_433E10();
    EXPORT signed int vGetSaveState_43B730(FlyingSligSpawner_State* pSaveState);

private:
    int field_20_tlvInfo;
    int field_24_spawned_slig_id;
    __int16 field_28_trigger_id;
    __int16 field_2A_pad;
    Path_TLV field_2C_tlv_header;
    int field_3C_bSpawned;
    __int16 field_40_bFirstUpdate;
    __int16 field_42_pad;
};
ALIVE_ASSERT_SIZEOF(FlyingSligSpawner, 0x44);

class FlyingSlig : public BaseAliveGameObject
{
public:
    EXPORT FlyingSlig* ctor_4342B0(Path_FlyingSlig* pTlv, int tlvInfo);



private:

    EXPORT void vUpdate_434AD0();
    EXPORT __int16 sub_436A90();
    EXPORT void sub_4348A0();
    EXPORT void sub_4396E0();
private:
    __int16 field_116_pad;
    Path_FlyingSlig field_118_data;
    int field_148_tlvInfo;
    int field_14C_hi_pause_timer;
    int field_150;
    int field_154;
    int field_158_obj_id;
    __int16 field_15C;
    __int16 field_15E;
    __int16 field_160;
    __int16 field_162;
    int field_164;
    __int16 field_168;
    __int16 field_16A;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    __int16 field_174;
    __int16 field_176;
    __int16 field_178;
    __int16 field_17A;
    __int16 field_17C;
    __int16 field_17E_flags;
    __int16 field_180;
    __int16 field_182;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    int field_198;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    int field_1A4;
    int field_1A8;
    int field_1AC;
    int field_1B0;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8;
    __int16 field_1BA;
    int field_1BC;
    int field_1C0;
    int field_1C4;
    int field_1C8;
    int field_1CC;
    __int16 field_1D0;
    __int16 field_1D2;
    __int16 field_1D4;
    __int16 field_1D6;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
    __int16 field_1E8;
    __int16 field_1EA;
    int field_1EC_pLine;
    int field_1F0_pLine;
    __int16 field_1F4_pal_data[64];
    PSX_RECT field_274_pal_rect;
    __int16 field_27C_old_g;
    __int16 field_27E_old_b;
    __int16 field_280_old_r;
    __int16 field_282;
    int field_284;
    int field_288;
    __int16 field_28C;
    __int16 field_28E;
    int field_290;
    FP field_294_nextXPos;
    FP field_298_nextYPos;
    int field_29C_pTable1;
    __int16 field_2A0_abe_level;
    __int16 field_2A2_abe_path;
    __int16 field_2A4_abe_camera;
    __int16 field_2A6;
    FP field_2A8;
    FP field_2AC;
    FP field_2B0;
    FP field_2B4;
    FP field_2B8;
};
ALIVE_ASSERT_SIZEOF(FlyingSlig, 0x2BC);
