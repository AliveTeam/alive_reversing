#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Animation.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"

namespace AO {

enum class LiftPointStopType : s16
{
    eTopFloor_0 = 0,
    eBottomFloor_1 = 1,
    eMiddleFloor_2 = 2,
    eMiddleLockFloor_3 = 3,
    eStartPointOnly_4 = 4,
};

struct Path_LiftPoint final : public Path_TLV
{
    s16 field_18_id;
    s16 field_1A_bstart_point;
    s16 field_1C_lift_type;
    LiftPointStopType field_1E_lift_point_stop_type;
    s16 field_20_scale;
    s16 field_22_bIgnore_lift_mover;
};

class Rope;

class LiftPoint final : public PlatformBase
{
public:
    enum Flags
    {
        // These have been shuffled around compared to AE for more pain
        eBit1 = 0x1, // Not used ?
        eBit2_bTopFloor = 0x2,
        eBit3_bMiddleFloor = 0x4,
        eBit4_bBottomFloor = 0x8,
        eBit5_bHasPulley = 0x10,
        eBit6_bMoveToFloorLevel = 0x20,
        eBit7_bIgnoreLiftMover = 0x40,
        eBit8_KeepOnMiddleFloor = 0x80,
    };
    EXPORT LiftPoint* ctor_434710(Path_LiftPoint* pTlv, Map* pPath, s32 tlvInfo);

    EXPORT void Move_435740(FP xSpeed, FP ySpeed, s32 not_used);

    void StayOnFloor(s16 floor, Path_LiftPoint* pTlv);

    void Sub_Unknown(Path_TLV* pTlv);

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

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_435780(PrimHeader** ppOt);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_435CC0();

    LiftPoint* Vdtor_435D10(s32 flags);

    EXPORT BaseGameObject* dtor_4355E0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT void CreatePulleyIfExists_435AE0(s16 camX, s16 camY);

    s16 field_12C_bMoving;
    s16 field_12E;
    LiftPointStopType field_130_lift_point_stop_type;
    s16 field_132;
    Rope* field_134_pRope2;
    Rope* field_138_pRope1;
    Animation field_13C_lift_wheel;
    Animation field_1D4_pulley_anim;
    s16 field_26C_pulley_xpos;
    s16 field_26E_pulley_ypos;
    FP field_270_floorYLevel;
    u8** field_274_ppRes;
    s16 field_278_point_id;
    BitField16<Flags> field_27A_flags;
};

ALIVE_ASSERT_SIZEOF(LiftPoint, 0x27C);

} // namespace AO
