#pragma once

#include "BaseGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class LevelIds : __int16;

struct Path_BirdPortal : public Path_TLV
{
    __int16 field_10_side;
    __int16 field_12_dest_level;
    __int16 field_14_dest_path;
    __int16 field_16_dest_camera;
    __int16 field_18_scale;
    __int16 field_1A_moive_id;
    __int16 field_1C_portal_type;
    __int16 field_1E_num_muds_for_shrykul;
    __int16 field_20_create_id;
    __int16 field_22_delete_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BirdPortal, 0x24);

class BirdPortal : public BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_497E00(Path_BirdPortal* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT BirdPortal* vdtor_498050(signed int flags);

    EXPORT void dtor_4980A0();

private:
    int field_20_tlvInfo;
    __int16 field_24_portal_type;
    __int16 field_26_side;
    __int16 field_28_state;
    __int16 field_2A;
    FP field_2C_xpos;
    FP field_30_ypos;
    int field_34_exit_x;
    int field_38_exit_y;
    FP field_3C_YPos;
    int field_40_throwable_indicator_id;
    int field_44_dove_ids[6];
    int field_5C_timer;
    FP field_60_scale;
    __int16 field_64_movie_id;
    __int16 field_66_delete_id;
    __int16 field_68;
    __int16 field_6A;
    int field_6C_terminator_id;
    int field_70_terminator_id;
    int field_74_screen_clipper_id;
    int field_78_screen_clipper_id;
    __int16 field_7C_dest_level;
    __int16 field_7E_dest_path;
    __int16 field_80_dest_camera;
    __int16 field_82_num_muds_for_shrykul;
    __int16 field_84;
    __int16 field_86;
    int field_88;
    LevelIds field_8C_level;
    __int16 field_8E_path;
    int field_90_sfx_ret;
    int field_94;
    int field_98;
    int field_9C;
    int field_A0;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
    int field_B4;
    int field_B8;
    int field_BC;
    int field_C0;
    int field_C4;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
};
ALIVE_ASSERT_SIZEOF(BirdPortal, 0x100);
