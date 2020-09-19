#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Slurg_Step_Watch_Point
{
    WORD field_0_xPos;
    WORD field_2_yPos;
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Point, 0x4);

struct Slurg_Step_Watch_Points
{
    Slurg_Step_Watch_Point field_0_points[5];
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Points, 0x14);

struct Slurg_Path_Data
{
    __int16 field_0_pause_delay;
    __int16 field_2_direction;
    __int16 field_4_scale;
    __int16 field_6_id;
};

struct Path_Slurg : public Path_TLV
{
    Slurg_Path_Data field_10_path_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slurg, 0x18);

enum class Slurg_States : __int16
{
    State_0_Moving = 0,
    State_1_Stopped = 1,
    State_2_Burst = 2,
};

enum SlurgFlags
{
    Bit1_Direction = 0x1,
    Bit2_StartToMove = 0x2,
};

struct Slurg_State
{
    Types field_0_type;
    __int16 field_2_padding;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_scale;
    __int16 field_14_flipX;
    __int16 field_16_current_motion;
    __int16 field_18_anim_current_frame;
    __int16 field_1A_anim_frame_change_counter;
    char field_1D_bDrawable;
    char field_1C_bRender;
    __int16 field_1E_padding;
    int field_20_frame_table_offset;
    int field_24_tlvInfo;
    Slurg_States field_28_state;
    BitField16<SlurgFlags> field_2A_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_State, 0x2C);

class Slurg : public BaseAliveGameObject
{
public:
    EXPORT static void CC Clear_Slurg_Step_Watch_Points_449A90();

    EXPORT Slurg* ctor_4C84E0(Path_Slurg* pTlv, DWORD tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

    EXPORT static signed int CC CreateFromSaveState_4C8DF0(const BYTE* pData);

private:
    void GoLeft();
    void GoRight();

private:
    EXPORT Slurg* vdtor_4C8760(signed int flags);
    EXPORT void dtor_4C8A40();
    EXPORT void Burst_4C8AE0();
    EXPORT void vUpdate_4C8790();
    EXPORT __int16 vTakeDamage_4C8BF0(BaseGameObject* pFrom);
    EXPORT void vOn_TLV_Collision_4C8C20(Path_TLV* pTlv);

    EXPORT signed int vSaveState_4C8FC0(Slurg_State* pState);

private:
    __int16 field_116_padding;
    BitField16<SlurgFlags> field_118_flags;
    __int16 field_11A_switch_id;
    Slurg_States field_11C_state;
    __int16 field_11E_delay_timer;
    __int16 field_120_delay_random;
    //__int16 field_122_padding;
    PathLine* field_124_pLine;
    Path_TLV* field_128_pTlv;
    int field_12C_tlvInfo;
    FP field_130_scale;
};
ALIVE_ASSERT_SIZEOF(Slurg, 0x134);

ALIVE_VAR_EXTERN(WORD, sSlurg_Step_Watch_Points_Idx_5C1C08);
ALIVE_ARY_EXTERN(char, 2, sSlurg_Step_Watch_Points_Count_5BD4DC);
ALIVE_ARY_EXTERN(Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28);
