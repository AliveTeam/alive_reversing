#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"
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
    s32 mRingTlvInfo;
    FP mRingRight;
    s32 mRingCount;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(AbilityRing_State, 0x28);

struct AbilityRing_PolyBuffer;
enum class LevelIds : s16;

class AbilityRing final : public BaseGameObject
{
public:
    static AbilityRing* Factory(FP xpos, FP ypos, RingTypes type, FP scale);
    AbilityRing(FP xpos, FP ypos, RingTypes ringType, FP scale);
    ~AbilityRing();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    // New virtuals
    virtual void VSetTarget(BaseGameObject* pTarget);

    static s32 CreateFromSaveState(const u8* pBuffer);

private:
    void CollideWithObjects(s16 bDealDamage);

private:
    Layer mRingLayer;
    AbilityRing_PolyBuffer* mRingPolyBuffer;
    u8** mRingRes;
    Prim_SetTPage mRingPrimSetTPage[2];
    PSX_RECT mRingCollideRects[64];
    FP mRingXPos;
    FP mRingYPos;
    FP mRingLeft;
    FP mRingRight;
    FP mRingSpeed;
    FP mRingScaleX;
    FP mRingScaleY;
    FP mRingThickness;
    s16 mRingFadeoutDistance;
    s16 mRingScreenX;
    s16 mRingScreenY;
    s16 mRingScreenXPos;
    s16 mRingScreenYPos;
    s16 mRingRed;
    s16 mRingGreen;
    s16 mRingBlue;
    s16 mRingSemiTrans;
    TPageAbr mRingTPageMode;
    EReliveLevelIds mRingLevel;
    s16 mRingPath;
    RingTypes mRingType;
    s32 mRingTargetObjId;
    s32 mRingCount;
    s16 mRingFoundTarget;
};

ALIVE_ASSERT_SIZEOF(AbilityRing, 0x294);
