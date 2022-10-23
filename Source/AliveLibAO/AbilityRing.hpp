#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibAE/Psx.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/MapWrapper.hpp"

namespace AO {

// Note: enum taken from AE so some ring types are unused
enum class RingTypes : s16
{
    // Red rings
    eExplosive_Pulse_0 = 0,
    eExplosive_Emit_1 = 1,        // Actually blows stuff up
    eExplosive_Emit_Effect_2 = 2, // Graphical effect only
    eExplosive_Give_3 = 3,

    // Blue rings
    eShrykull_Pulse_Small_4 = 4,
    eShrykull_Pulse_Large_5 = 5,
    eShrykull_Pulse_Orange_6 = 6, // Never used?

    // Green rings
    eInvisible_Pulse_Small_7 = 7,
    eInvisible_Pulse_Large_8 = 8,
    eInvisible_Pulse_Emit_9 = 9,
    eInvisible_Pulse_Give_10 = 10,

    // Yellow rings
    eHealing_Emit_Effect_11 = 11, // Graphical effect only
    eHealing_Emit_12 = 12,        // Actually heal muds
    eHealing_Give_13 = 13,
    eHealing_Pulse_14 = 14,
};

class BaseAliveGameObject;

struct AbilityRing_PolyBuffer final
{
    Poly_F4 mPolys[2];
};
ALIVE_ASSERT_SIZEOF(AbilityRing_PolyBuffer, 56);


class AbilityRing final : public ::BaseGameObject
{
public:
    AbilityRing(FP xpos, FP ypos, RingTypes type);
    ~AbilityRing();

    static AbilityRing* Factory(FP xpos, FP ypos, RingTypes ring_type);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void SetTarget(BaseAliveGameObject* pTarget);
    void CollideWithObjects();

    Layer mRingLayer = Layer::eLayer_0;
    AbilityRing_PolyBuffer* mRingPolyBuffer = {};
    Prim_SetTPage mRingPrimSetTPage[2] = {};
    PSX_RECT mRingCollideRects[64] = {};
    FP mRingXPos = {};
    FP mRingYPos = {};
    FP mRingLeft = {};
    FP mRingRight = {};
    FP mRingSpeed = {};
    FP mRingScaleX = {};
    FP mRingScaleY = {};
    FP mRingThickness = {};
    s16 mRingFadeoutDistance = 0;
    s16 mRingScreenX = 0;
    s16 mRingScreenY = 0;
    s16 mRingScreenXPos = 0;
    s16 mRingScreenYPos = 0;
    s16 mRingRed = 0;
    s16 mRingGreen = 0;
    s16 mRingBlue = 0;
    s16 mRingSemiTrans = 0;
    EReliveLevelIds mRingLevel = EReliveLevelIds::eNone;
    s16 mRingPath = 0;
    RingTypes mRingType = RingTypes::eExplosive_Pulse_0;
    BaseAliveGameObject* mTargetObj = nullptr; // stored as object id in AE
};



} // namespace AO
