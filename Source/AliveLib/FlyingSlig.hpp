#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FlyingSligSpawner.hpp"

enum class LevelIds : __int16;

class FlyingSlig;

using TFlyingSligFn = void(FlyingSlig::*)(void);

class FlyingSlig : public BaseAliveGameObject
{
public:
    EXPORT FlyingSlig* ctor_4342B0(Path_FlyingSlig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_434870(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_434AD0();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_434C10();
    }

    // TODO: Remaining virtuals

private:

    EXPORT void dtor_434990();

    EXPORT FlyingSlig* vdtor_434870(signed int flags);

    EXPORT void vScreenChanged_434C10();


    EXPORT void vUpdate_434AD0();
    EXPORT __int16 sub_436A90();
    EXPORT void sub_4348A0();
    EXPORT void sub_4396E0();

public:

    EXPORT void state_0_0_4355B0();
    EXPORT void state_0_1_Dead_4364E0();
    EXPORT void state_0_2_4356D0();
    EXPORT void state_0_3_435750();
    EXPORT void state_0_4_435BC0();
    EXPORT void state_0_5_435820();
    EXPORT void state_0_6_435940();
    EXPORT void state_0_7_435990();
    EXPORT void state_0_8_435AC0();
    EXPORT void state_0_9_435E40();
    EXPORT void state_0_10_435F10();
    EXPORT void state_0_11_435FD0();
    EXPORT void state_0_12_PlayerControlled_436040();
    EXPORT void state_0_13_4360F0();
    EXPORT void state_0_14_ChantShake_436180();
    EXPORT void state_0_15_4362C0();
    EXPORT void state_0_16_4355E0();
public:

    EXPORT void state_1_0_4385E0();
    EXPORT void state_1_1_4386A0();
    EXPORT void state_1_2_4388B0();
    EXPORT void state_1_3_438AA0();
    EXPORT void state_1_4_438CC0();
    EXPORT void state_1_5_438DD0();
    EXPORT void state_1_6_439030();
    EXPORT void state_1_7_439150();
    EXPORT void state_1_8_Speak_4391D0();
    EXPORT void state_1_9_nullsub_33();
    EXPORT void state_1_10_4387D0();
    EXPORT void state_1_11_438E40();
    EXPORT void state_1_12_438B10();
    EXPORT void state_1_13_438F60();
    EXPORT void state_1_14_438BF0();
    EXPORT void state_1_15_4387F0();
    EXPORT void state_1_16_438730();
    EXPORT void state_1_17_438B80();
    EXPORT void state_1_18_438C90();
    EXPORT void state_1_19_4390D0();
    EXPORT void state_1_20_439110();
    EXPORT void state_1_21_438EB0();
    EXPORT void state_1_22_438EE0();
    EXPORT void state_1_23_438F20();
    EXPORT void state_1_24_438D60();
    EXPORT void state_1_25_4389E0();
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
    __int16 field_17C_launch_id;

    enum Flags_17E
    {
        eBit1 = 0x1,
        eBit2 = 0x2,
        eBit3 = 0x4,
        eBit4 = 0x8,
        eBit5 = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8 = 0x80,
        eBit9 = 0x100,
        eBit10 = 0x200,
        eBit11 = 0x400,
        eBit12 = 0x800,
        eBit13_Persistant = 0x1000,
    };
    BitField16<Flags_17E> field_17E_flags;
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
    TFlyingSligFn field_29C_pTable1;
    LevelIds field_2A0_abe_level;
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
