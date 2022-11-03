#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

struct LiftPointSaveState final
{
    ReliveTypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    Guid field_C_tlvInfo;
    Guid field_10_pTlv;
    FP field_14_floorYLevel;
    relive::Path_LiftPoint::LiftPointStopType field_18_lift_point_stop_type;
    bool mMoving;
    bool mTopFloor;
    bool mMiddleFloor;
    bool mBottomFloor;
    bool mMoveToFloorLevel;
    bool mKeepOnMiddleFloor;
};

class LiftPoint final : public PlatformBase
{
public:
    LiftPoint(relive::Path_LiftPoint* pTlv, const Guid& tlvId);
    ~LiftPoint();
    
    void LoadAnimations();

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
    static void sub_461000(relive::Path_TLV* pTlv);
    void vStayOnFloor(bool floor, relive::Path_LiftPoint* pTlv);

    void CreatePulleyIfExists();

public:
    u8 field_278_lift_point_id = 0;
    bool mMoving = false;

private:
    relive::Path_LiftPoint::LiftPointStopType field_130_lift_point_stop_type = relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
    Guid field_134_rope2_id;
    Guid field_138_rope1_id;
    Animation field_13C_lift_wheel = {};
    Animation field_1D4_pulley_anim = {};
    s16 field_26C_pulley_xpos = 0;
    s16 field_26E_pulley_ypos = 0;
    FP field_270_floorYLevel = {};
    Guid field_27C_pTlv;
    bool mTopFloor = false;
    bool mMiddleFloor = false;
    bool mBottomFloor = false;
    bool mHasPulley = false;
    bool mMoveToFloorLevel = false;
    bool mKeepOnMiddleFloor = false;
    bool mIgnoreLiftMover = false;
};
