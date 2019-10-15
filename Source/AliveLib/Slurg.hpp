#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"
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
    Slurg_Path_Data field_10;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Slurg, 0x18);

struct Slurg_Spawner_Path_Data : public Slurg_Path_Data
{
    __int16 field_8_delay_between_slurgs;
    __int16 field_A_max_slurgs;
    __int16 field_C_switch_id;
    __int16 field_E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Slurg_Spawner_Path_Data, 0x10);

struct Path_SlurgSpawner : public Path_Slurg
{
    Slurg_Spawner_Path_Data field_18;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlurgSpawner, 0x28);

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

// NOTE: Apparently this object is never used - would kind of make sense as it reads
// the slurg spawned count from the TLV but never updates it.
// Also it hasn't got quiksave support.
class SlurgSpawner : public BaseGameObject
{
public:
    EXPORT SlurgSpawner* ctor_4C82E0(Path_SlurgSpawner* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void vUpdate_4C83C0();
    EXPORT void dtor_4C83A0();
    EXPORT SlurgSpawner* vdtor_4C8370(signed int flags);
    EXPORT void vScreenChanged_4C84A0();

private:
    int field_20_tlvInfo;
    Path_SlurgSpawner* field_24_slurg_tlv;
    Slurg_Spawner_Path_Data field_28;
    int field_38;
    __int16 field_3C;
    __int16 field_3E_delay_counter;
    BYTE field_40_spawned_count;
    char field_41;
    __int16 field_42;
};
ALIVE_ASSERT_SIZEOF(SlurgSpawner, 0x44);


struct Slurg_State
{
    Types field_0_type;
    __int16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    FP field_C_velx;
    FP field_10_scale;
    __int16 field_14_flipX;
    __int16 field_16_current_motion;
    __int16 field_18_anim_current_frame;
    __int16 field_1A_anim_frame_change_counter;
    char field_1D;
    char field_1C;
    __int16 field_1E;
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
    __int16 field_116_pad;
    BitField16<SlurgFlags> field_118_flags;
    __int16 field_11A_switch_id;
    Slurg_States field_11C_state;
    __int16 field_11E_delay_timer;
    __int16 field_120_delay_random;
    //__int16 field_122_pad;
    PathLine* field_124_pLine;
    Path_TLV* field_128_pTlv;
    int field_12C_tlvInfo;
    FP field_130_scale;
};
ALIVE_ASSERT_SIZEOF(Slurg, 0x134);

ALIVE_VAR_EXTERN(WORD, sSlurg_Step_Watch_Points_Idx_5C1C08);
ALIVE_ARY_EXTERN(char, 2, sSlurg_Step_Watch_Points_Count_5BD4DC);
ALIVE_ARY_EXTERN(Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28);
