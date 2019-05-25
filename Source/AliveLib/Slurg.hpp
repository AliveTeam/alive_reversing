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

class Slurg : public BaseAliveGameObject
{
public:
    EXPORT static void CC Clear_Slurg_Step_Watch_Points_449A90();

    EXPORT Slurg* ctor_4C84E0(Path_Slurg* pTlv, DWORD tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VOn_TLV_Collision_4087F0(Path_TLV* pTlv) override;

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

private:
    __int16 field_116_pad;
    enum Flags
    {
        Bit1 = 0x1,
        Bit2 = 0x2,
    };
    BitField16<Flags> field_118_flags;
    __int16 field_11A_switch_id;
    enum class States : __int16
    {
        State_0_Stopped = 0,
        State_1_Moving = 1,
        State_2_Burst = 2,
    };
    States field_11C_state;
    __int16 field_11E_delay_timer;
    __int16 field_120_delay_random;
    //__int16 field_122_pad;
    PathLine* field_124_pLine;
    Path_TLV* field_128_pTlv;
    int field_12C_tlvInfo;
    FP field_130_scale;
};
ALIVE_ASSERT_SIZEOF(Slurg, 0x134);
