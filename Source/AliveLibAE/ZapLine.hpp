#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Layer.hpp"

struct ZapLineSprites final
{
    Poly_FT4 field_0_sprts[2];
};
ALIVE_ASSERT_SIZEOF(ZapLineSprites, 0x30);

// TODO: Give better names to the ZapPoint fields.
struct ZapPoint final
{
    FP field_0_part_1;
    FP field_4_part_2;
    FP field_8_part_3;
};
ALIVE_ASSERT_SIZEOF(ZapPoint, 0xC);

enum class ZapLineType : s16
{
    eThick_0 = 0, // Used by Chant Suppressors and Greeters.
    eThin_1 = 1   // Used by the Shrykull.
};

class ZapLine final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ZapLine(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest, s32 aliveTime, ZapLineType type, Layer layer);
    ~ZapLine();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

public:
    void CalculateSourceAndDestinationPositions(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest);

private:
    void CalculateThickSpriteSegmentPositions();
    void CalculateThinSpriteSegmentPositions();
    void CalculateZapPoints();
    void CalculateSpritePositionsInner(s32 idx1, s32 idx2, s32 idx3, s16 idx4);
    void UpdateSpriteVertexPositions();
    void CalculateSpritePositionsOuter();

private:
    enum class ZapLineState : s16
    {
        eInit_0 = 0,
        eInitSpritePositions_1 = 1,
        eInitSpriteVertices_2 = 2,
        eUpdateSpritePositions_3 = 3,
        eUpdateSpriteVertices_4 = 4
    };

    ZapLineState mState = ZapLineState::eInit_0;

    s16 mXPosSrc = 0;
    s16 mYPosSrc = 0;
    s16 mXPosDst = 0;
    s16 mYPosDst = 0;
    s16 mAliveTimer = 0;
    s16 mMaxAliveTime = 0;
    ZapLineType mZapLineType = ZapLineType::eThick_0;
    s16 mNumberOfSegments = 0;
    s16 mNumberOfPiecesPerSegment = 0;
    s16 mNumberOfSprites = 0;
    ZapLineSprites* mSprites = nullptr;
    PSX_Point* mSpritePositions = nullptr;
    ZapPoint* mZapPoints = nullptr;
    FP_Point* mSpriteSegmentPositions = nullptr;
};
