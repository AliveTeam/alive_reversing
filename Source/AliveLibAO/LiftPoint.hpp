#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Animation.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_LiftPoint : public Path_TLV
{
    __int16 field_18_id;
    __int16 field_1A_bstart_point;
    __int16 field_1C_lift_type;
    __int16 field_1E_lift_point_stop_type;
    __int16 field_20_scale;
    __int16 field_22_bIgnore_lift_mover;
};

class Rope;

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
        eBit7_bIgnoreLiftMover = 0x40,
        eBit8_KeepOnMiddleFloor = 0x80,
    };

    EXPORT LiftPoint* ctor_434710(Path_LiftPoint* pTlv, Map* pPath, int tlvInfo);
    
    EXPORT void Move_435740(FP xSpeed, FP ySpeed, int not_used);

    // flags & 0x2 && !flags & 0x20
    bool OnTopFloor() const;

    // flags & 0x8 && !flags & 0x20
    bool OnBottomFloor() const;

    // flags & 4 && !(flags & 0x20);
    bool OnMiddleFloor() const;

    // flags & 2 && !(flags & 0x20); // OnTopFloor
    // flags & 4 && !(flags & 0x20); // OnMiddleFloor
    // flags & 8 && !(flags & 0x20); // OnBottomFloor
    bool OnAnyFloor() const;

    bool OnAFloorLiftMoverCanUse() const;

    virtual void VUpdate() override;

    EXPORT void VUpdate_434D10();

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_435780(int** ppOt);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_435CC0();

    LiftPoint* Vdtor_435D10(signed int flags);

    EXPORT BaseGameObject* dtor_4355E0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT void CreatePulleyIfExists_435AE0(short camX, short camY);

    __int16 field_12C_bMoving;
    __int16 field_12E;
    __int16 field_130_stop_type;
    __int16 field_132;
    Rope* field_134_pRope2;
    Rope* field_138_pRope1;
    Animation field_13C_lift_wheel;
    Animation field_1D4_pulley_anim;
    __int16 field_26C_pulley_xpos;
    __int16 field_26E_pulley_ypos;
    FP field_270_floorYLevel;
    BYTE** field_274_ppRes;
    short field_278_point_id;
    BitField16<Flags> field_27A_flags;
};

ALIVE_ASSERT_SIZEOF(LiftPoint, 0x27C);

END_NS_AO

