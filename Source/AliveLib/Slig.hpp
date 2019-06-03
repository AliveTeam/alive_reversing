#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_Slig : public Path_TLV
{
    __int16 field_10_scale;

    // TODO: Check these - GameEnder is probably only AO for instance ?
    enum class StartState : __int16
    {
        Listening_0 = 0,
        Paused_1 = 1,
        Sleeping_2 = 2,
        Chase_3 = 3,
        RunOffScreen_4 = 4,
        GameEnder_5 = 5,
    };
    StartState field_12_start_state;

    __int16 field_14_pause_time;
    __int16 field_16_pause_left_min;
    __int16 field_18_pause_left_max;
    __int16 field_1A_pause_right_min;
    __int16 field_1C_pause_right_max;
    __int16 field_1E_chal_number;
    __int16 field_20_chal_timer;
    __int16 field_22_num_times_to_shoot;
    __int16 field_24_unknown;
    __int16 field_26_code1;
    __int16 field_28_code2;
    __int16 field_2A_chase_abe;
    __int16 field_2C_start_direction;
    __int16 field_2E_panic_timeout;
    __int16 field_30_num_panic_sounds;
    __int16 field_32_panic_sound_timeout;
    __int16 field_34_stop_chase_delay;
    __int16 field_36_time_to_wait_before_chase;
    __int16 field_38_slig_id;
    __int16 field_3A_listen_time;
    __int16 field_3C_percent_say_what;
    __int16 field_3E_percent_beat_mud;
    __int16 field_40_talk_to_abe;
    __int16 field_42_dont_shoot;
    __int16 field_44_Z_shoot_delay;
    __int16 field_46_stay_awake;
    __int16 field_48_disable_resources;
    __int16 field_4A_noise_wake_up_distance;
    __int16 field_4C_id;
    __int16 field_4E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slig, 0x50);


enum class LevelIds : __int16;

class Slig : public BaseAliveGameObject
{
public:
    EXPORT Slig* ctor_4B1370(Path_Slig* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT void Init_4BB0D0();

    EXPORT void dtor_4B1CF0();


    EXPORT Slig* vdtor_4B1790(signed int flags);

private:
    __int16 field_116;
    int field_118_tlvInfo;
    __int16 field_11C;
    __int16 field_11E;
    int field_120_timer;
    __int16 field_124;
    __int16 field_126;
    int field_128_input;
    int field_12C;
    int field_130;
    __int16 field_134;
    __int16 field_136;
    __int16 field_138;
    __int16 field_13A;
    __int16 field_13C;
    __int16 field_13E;
    __int16 field_140;
    __int16 field_142;
    __int16 field_144;
    LevelIds field_146_level;
    __int16 field_148_path;
    __int16 field_14A_camera;
    int field_14C;
    int field_150;
    int field_154_brain_state;
    int field_158;
    __int16 field_15C;
    __int16 field_15E;
    int field_160;
    __int16 field_164;
    __int16 field_166;
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
    __int16 field_17E;
    __int16 field_180;
    __int16 field_182;
    __int16 field_184;
    __int16 field_186;
    __int16 field_188;
    __int16 field_18A;
    __int16 field_18C;
    __int16 field_18E;
    __int16 field_190;
    __int16 field_192;
    __int16 field_194;
    __int16 field_196;
    __int16 field_198;
    __int16 field_19A;
    __int16 field_19C;
    __int16 field_19E;
    __int16 field_1A0;
    __int16 field_1A2;
    __int16 field_1A4;
    __int16 field_1A6;
    __int16 field_1A8;
    __int16 field_1AA;
    __int16 field_1AC;
    __int16 field_1AE;
    __int16 field_1B0;
    __int16 field_1B2;
    __int16 field_1B4;
    __int16 field_1B6;
    __int16 field_1B8;
    __int16 field_1BA;
    __int16 field_1BC;
    __int16 field_1BE;
    __int16 field_1C0;
    __int16 field_1C2;
    __int16 field_1C4;
    __int16 field_1C6;
    __int16 field_1C8;
    __int16 field_1CA;
    __int16 field_1CC;
    __int16 field_1CE;
    __int16 field_1D0;
    __int16 field_1D2;
    __int16 field_1D4;
    __int16 field_1D6;
    __int16 field_1D8;
    __int16 field_1DA;
    __int16 field_1DC;
    __int16 field_1DE;
    __int16 field_1E0;
    __int16 field_1E2;
    __int16 field_1E4;
    __int16 field_1E6;
    __int16 field_1E8;
    __int16 field_1EA;
    __int16 field_1EC;
    __int16 field_1EE;
    __int16 field_1F0;
    __int16 field_1F2;
    __int16 field_1F4;
    __int16 field_1F6;
    __int16 field_1F8;
    __int16 field_1FA;
    __int16 field_1FC;
    __int16 field_1FE;
    __int16 field_200;
    __int16 field_202;
    __int16 field_204;
    __int16 field_206;
    int field_208;
    __int16 field_20C;
    __int16 field_20E;
    __int16 field_210;
    __int16 field_212;
    __int16 field_214;
    __int16 field_216;
    Path_Slig field_218_tlv_data;
    __int16 field_268;
    __int16 field_26A;
    __int16 field_26C;
    __int16 field_26E;
    __int16 field_270;
    __int16 field_272;
    __int16 field_274;
    __int16 field_276;
    __int16 field_278;
    __int16 field_27A;
    __int16 field_27C;
    __int16 field_27E;
    __int16 field_280;
    __int16 field_282;
    __int16 field_284;
    __int16 field_286;
    __int16 field_288;
    __int16 field_28A;
    __int16 field_28C;
    __int16 field_28E;
    __int16 field_290;
    __int16 field_292;
    int field_294;
};
ALIVE_ASSERT_SIZEOF(Slig, 0x298);
