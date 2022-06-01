#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Slurg_Step_Watch_Point final
{
    u16 field_0_xPos;
    u16 field_2_yPos;
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Point, 0x4);

struct Slurg_Step_Watch_Points final
{
    Slurg_Step_Watch_Point field_0_points[5];
};
ALIVE_ASSERT_SIZEOF(Slurg_Step_Watch_Points, 0x14);

struct Slurg_Path_Data final
{
    s16 field_0_moving_timer;
    XDirection_short field_2_start_direction;
    Scale_short field_4_scale;
    s16 field_6_switch_id;
};

struct Path_Slurg : public Path_TLV
{
    Slurg_Path_Data field_10_slurg_data;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slurg, 0x18);

enum class Slurg_States : s16
{
    eMoving_0 = 0,
    eStopped_1 = 1,
    eBurst_2 = 2
};

enum SlurgFlags
{
    Bit1_Direction = 0x1,
    Bit2_StartToMove = 0x2,
};

struct Slurg_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_scale;
    s16 field_14_flipX;
    s16 field_16_current_motion;
    s16 field_18_anim_current_frame;
    s16 field_1A_anim_frame_change_counter;
    s8 field_1D_bDrawable;
    s8 field_1C_bRender;
    s16 field_1E_padding;
    s32 field_20_frame_table_offset;
    s32 field_24_tlvInfo;
    Slurg_States field_28_state;
    BitField16<SlurgFlags> field_2A_flags;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_State, 0x2C);

class Slurg final : public BaseAliveGameObject
{
public:
    static void Clear_Slurg_Step_Watch_Points();

    Slurg(Path_Slurg* pTlv, u32 tlvInfo);
    ~Slurg();

    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision(Path_TLV* pTlv) override;

    static s32 CreateFromSaveState(const u8* pData);

private:
    void GoLeft();
    void GoRight();
    void Burst_4C8AE0();

    s16 field_116_padding;
    BitField16<SlurgFlags> field_118_flags;
    s16 field_11A_switch_id;
    Slurg_States field_11C_state;
    s16 field_11E_moving_timer;
    s16 field_120_delay_random;
    //s16 field_122_padding;
    PathLine* field_124_pLine;
    Path_TLV* field_128_pTlv;
    s32 field_12C_tlvInfo;
    FP field_130_scale;
};
ALIVE_ASSERT_SIZEOF(Slurg, 0x134);

ALIVE_VAR_EXTERN(u16, sSlurg_Step_Watch_Points_Idx_5C1C08);
ALIVE_ARY_EXTERN(s8, 2, sSlurg_Step_Watch_Points_Count_5BD4DC);
ALIVE_ARY_EXTERN(Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28);
