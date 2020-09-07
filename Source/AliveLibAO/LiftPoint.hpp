#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Animation.hpp"
#include "PlatformBase.hpp"

START_NS_AO

class Map;



class LiftPoint : public PlatformBase
{
public:
    enum Flags
    {
        // These have been shuffled around compared to AE for more pain
        eBit1 = 0x1,
        eBit2_bTopFloor = 0x2,
        eBit3_bMiddleFloor = 0x4,
        eBit4_bBottomFloor = 0x8,
        eBit5 = 0x10,
        eBit6_bMoveToFloorLevel = 0x20,
        eBit7 = 0x40,
        eBit8_bMudUsing = 0x80,
    };

    EXPORT LiftPoint* ctor_434710(unsigned __int8* pTlv, Map* pPath, int tlvInfo);
    
    EXPORT void Move_435740(FP xSpeed, FP ySpeed, int not_used);

    // flags & 0x2 && !flags & 0x20
    bool OnTopFloor() const
    {
        return field_27A_flags.Get(Flags::eBit2_bTopFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
    }

    // flags & 0x8 && !flags & 0x20
    bool OnBottomFloor() const
    {
         return field_27A_flags.Get(Flags::eBit4_bBottomFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
    }

    // flags & 4 && !(flags & 0x20);
    bool OnMiddleFloor() const
    {
        return field_27A_flags.Get(Flags::eBit3_bMiddleFloor) && !field_27A_flags.Get(Flags::eBit6_bMoveToFloorLevel);
    }

    // flags & 2 && !(flags & 0x20); // OnTopFloor
    // flags & 4 && !(flags & 0x20); // OnMiddleFloor
    // flags & 8 && !(flags & 0x20); // OnBottomFloor
    bool OnAnyFloor() const
    {
        return OnBottomFloor() || OnTopFloor() || OnMiddleFloor();
    }

    __int16 field_12C_bMoving;
    __int16 field_12E;
    __int16 field_130;
    __int16 field_132;
    int field_134_pRope2;
    int field_138_pRope1;
    Animation field_13C_lift_wheel;
    Animation field_1D4_pulley_anim;
    int field_26C_pulley_xpos;
    int field_270_pulley_ypos;
    int field_274;
    char field_278;
    char field_279;
    BitField16<Flags> field_27A_flags;
};

ALIVE_ASSERT_SIZEOF(LiftPoint, 0x27C);

END_NS_AO

