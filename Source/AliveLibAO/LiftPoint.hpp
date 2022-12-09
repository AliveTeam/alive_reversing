#pragma once

#include "../relive_lib/Animation.hpp"
#include "PlatformBase.hpp"
#include "Map.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class Rope;

class LiftPoint final : public PlatformBase
{
public:
    LiftPoint(relive::Path_LiftPoint* pTlv, Map* pPath, const Guid& tlvId);
    ~LiftPoint();

    void LoadAnimations();
    void Move(FP xSpeed, FP ySpeed, s32 not_used);
    void StayOnFloor(s16 floor, relive::Path_LiftPoint* pTlv);
    void Sub_Unknown(relive::Path_TLV* pTlv);
    bool OnTopFloor() const;
    bool OnBottomFloor() const;
    bool OnMiddleFloor() const;
    bool OnAnyFloor() const;
    bool OnAFloorLiftMoverCanUse() const;

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    void CreatePulleyIfExists(s16 camX, s16 camY);
    void MoveObjectsOnLift(FP xVelocity);

    bool mMoving = false;
    s16 field_278_lift_point_id = 0;
    bool mKeepOnMiddleFloor = false;
    bool mIgnoreLiftMover = false;
private:

    s16 field_12E = 0;
    relive::Path_LiftPoint::LiftPointStopType mLiftPointStopType = relive::Path_LiftPoint::LiftPointStopType::eTopFloor;
    s16 field_132 = 0;
    Rope* field_134_pRope2 = nullptr;
    Rope* field_138_pRope1 = nullptr;
    Animation field_13C_lift_wheel;
    Animation field_1D4_pulley_anim;
    s16 field_26C_pulley_xpos = 0;
    s16 field_26E_pulley_ypos = 0;
    FP field_270_floorYLevel = {};
    bool mTopFloor = false;
    bool mMiddleFloor = false;
    bool mBottomFloor = false;
    bool mHasPulley = false;
    bool mMoveToFloorLevel = false;
};


} // namespace AO
