#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

enum class LiftPointStopType : s16
{
    eTopFloor_0 = 0,
    eBottomFloor_1 = 1,
    eMiddleFloor_2 = 2,
    eMiddleLockFloor_3 = 3,
    eStartPointOnly_4 = 4,
};

struct Path_Pulley final : public Path_TLV
{
    s16 field_10_unused1;
    s16 field_12_unused2;
};

struct Path_LiftPoint final : public Path_TLV
{
    s16 mLiftPointId;
    Choice_short mIsStartPoint;
    s16 field_14_lift_type; // TODO: Not actually used
    LiftPointStopType mLiftPointStopType;
    Scale_short mScale;
    Choice_short mIgnoreLiftMover;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftPoint, 0x1C);

struct LiftPoint_State final
{
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    s32 field_C_tlvInfo;
    s32 field_10_pTlv;
    FP field_14_floorYLevel;
    LiftPointStopType field_18_lift_point_stop_type;
    enum Flags
    {
        eBit1_bMoving = 0x1,
        eBit2_bTopFloor = 0x2,
        eBit3_bMiddleFloor = 0x4,
        eBit4_bBottomFloor = 0x8,
        eBit5_bMoveToFloorLevel = 0x10,
        eBit6 = 0x20,
        eBit7_KeepOnMiddleFloor = 0x40,
    };
    BitField16<Flags> field_1A;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(LiftPoint_State, 0x1C);

class LiftPoint final : public PlatformBase
{
public:
    LiftPoint(Path_LiftPoint* pTlv, s32 tlvInfo);
    ~LiftPoint();
    
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    static s32 CreateFromSaveState(const u8* pData);

    void vKeepOnMiddleFloor();
    bool vOnTopFloor();
    bool vOnMiddleFloor();
    bool vOnBottomFloor();
    bool vOnAnyFloor();
    bool vOnAFloorLiftMoverCanUse();
    bool vMovingToFloorLevel();

    void vMove_4626A0(FP xSpeed, FP ySpeed, s32 not_used);

private:
    void MoveObjectsOnLift(FP xVelocity);
    static void sub_461000(Path_TLV* pTlv);
    void vStayOnFloor(s16 floor, Path_LiftPoint* pTlv);

    void CreatePulleyIfExists();

public:
    s16 field_12C_bMoving = 0;

private:
    LiftPointStopType field_130_lift_point_stop_type = LiftPointStopType::eTopFloor_0;
    s32 field_134_rope2_id = 0;
    s32 field_138_rope1_id = 0;
    Animation field_13C_lift_wheel = {};
    Animation field_1D4_pulley_anim = {};
    s16 field_26C_pulley_xpos = 0;
    s16 field_26E_pulley_ypos = 0;
    FP field_270_floorYLevel = {};
    u8** field_274_ppRes = nullptr;

public:
    u8 field_278_lift_point_id = 0;

private:
    s32 field_27C_pTlv = 0;
    enum LiftFlags
    {
        eBit1_bTopFloor = 0x1,
        eBit2_bMiddleFloor = 0x2,
        eBit3_bBottomFloor = 0x4,
        eBit4_bHasPulley = 0x8,
        eBit5_bMoveToFloorLevel = 0x10,
        eBit6 = 0x20, // This never seems to be used for anything
        eBit7_KeepOnMiddleFloor = 0x40,
        eBit8_bIgnoreLiftMover = 0x80,
    };
    BitField16<LiftFlags> field_280_flags = {};
};
ALIVE_ASSERT_SIZEOF(LiftPoint, 0x284);
