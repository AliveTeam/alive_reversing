#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/MapWrapper.hpp"

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

struct AbilityRing_State final
{
    AETypes mRingObjectType;
    FP mRingXPos;
    FP mRingYPos;
    RingTypes mRingType;
    FP mRingScale;
    Guid mRingTlvInfo;
    FP mRingRight;
    s32 mRingCount;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
};
//ALIVE_ASSERT_SIZEOF_ALWAYS(AbilityRing_State, 0x28);

struct AbilityRing_PolyBuffer;
enum class LevelIds : s16;

class AbilityRing final : public BaseGameObject
{
public:
    static AbilityRing* Factory(FP xpos, FP ypos, RingTypes type, FP scale);
    virtual void VSetTarget(BaseGameObject* pTarget);
    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    AbilityRing(FP xpos, FP ypos, RingTypes ringType, FP scale);
    ~AbilityRing();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    void CollideWithObjects(s16 bDealDamage);

    Layer mRingLayer = Layer::eLayer_0;
    AbilityRing_PolyBuffer* mRingPolyBuffer = nullptr;
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
    Guid mRingTargetObjId;
    s32 mRingCount = 0;
    s16 mRefreshTargetObjId = 0;
};

ALIVE_ASSERT_SIZEOF(AbilityRing, 0x294);
