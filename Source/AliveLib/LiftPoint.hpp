#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

class PlatformBase : public BaseAliveGameObject
{
protected:
    EXPORT void AddDynamicCollision_4971C0(int maxW, int maxH, unsigned __int16 frameTableOffset, BYTE** ppAnimData, Path_TLV* pTlv, Map* pMap, int tlvInfo);

    EXPORT void dtor_4973E0();

    EXPORT void sub_4974E0();
protected:
    __int16 field_116;
    int field_118_count;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    PathLine* field_124_pCollisionLine;
    int field_128_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);

enum class LiftPointStopType : __int16
{
    eTopFloor_0 = 0,
    eBottomFloor_1 = 1,
    eMiddleFloor_2 = 2,
    eMiddleLockFloor_3 = 3,
    eStartPointOnly_4 = 4,
};

struct Path_LiftPoint : public Path_TLV
{
    constexpr static auto kType = 7;
    __int16 field_10_id;
    __int16 field_12_bstart_point;
    __int16 field_14_lift_type; // TODO: Enum
    LiftPointStopType field_16_lift_point_stop_type;
    __int16 field_18_scale;
    __int16 field_1A_bIgnore_lift_mover;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LiftPoint, 0x1C);

struct Path_Pulley : public Path_TLV
{
    constexpr static auto kType = 21;
};

class LiftPoint : public PlatformBase
{
public:
    EXPORT LiftPoint* ctor_461030(Path_LiftPoint* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT void vsub_461870();
    EXPORT BOOL vsub_461890();
    EXPORT BOOL vsub_4618C0();
    EXPORT BOOL vsub_4618F0();
    EXPORT int vsub_4619B0();
    EXPORT void vMove_4626A0(FP xSpeed, FP ySpeed, int not_used);
    EXPORT void vRender_462730(int **pOt);
    EXPORT void vUpdate_461AE0();
    EXPORT void sub_497600(FP xVelocity);
    EXPORT static void CCSTD sub_461000(Path_TLV* pTlv);
    EXPORT void vsub_461A00(__int16 arg0, Path_TLV* a2a);

private:
    EXPORT void CreatePulleyIfExists_462C80();
    EXPORT void vScreenChanged_463020();
    EXPORT LiftPoint* vdtor_4619D0(signed int flags);
    EXPORT void dtor_4624E0();

private:
    __int16 field_12C_bMoving;
    __int16 field_12E;
    LiftPointStopType field_130_lift_point_stop_type;
    __int16 field_132;
    int field_134_rope2_id;
    int field_138_rope1_id;
    AnimationEx field_13C_lift_wheel;
    AnimationEx field_1D4_pulley_anim;
    __int16 field_26C_pulley_xpos;
    __int16 field_26E_pulley_ypos;
    FP field_270_floorYLevel;
    BYTE** field_274_ppRes;
public:
    char field_278_lift_point_id;
private:
    char field_279;
    __int16 field_27A;
    int field_27C_pTlv;
    enum LiftFlags
    {
        eBit1_bTopFloor = 0x1,
        eBit2_bMiddleFloor = 0x2,
        eBit3_bBottomFloor = 0x4,
        eBit4_bHasPulley = 0x8,
        eBit5_bMoveToFloorLevel = 0x10,
        eBit6 = 0x20,
        eBit7 = 0x40,
        eBit8_bIgnoreLiftMover = 0x80,
    };
    BitField16<LiftFlags> field_280_flags;
    __int16 field_282;
};
ALIVE_ASSERT_SIZEOF(LiftPoint, 0x284);
