#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

void ZapLine_ForceLink();

namespace AO {

struct ZapLineSprites final
{
    Prim_Sprt field_0_sprts[2];
};
ALIVE_ASSERT_SIZEOF(ZapLineSprites, 0x30);

// TODO: Give better names to the ZapPoint fields.
struct ZapPoint final
{
    FP field_0;
    FP field_4;
    FP field_8;
};
ALIVE_ASSERT_SIZEOF(ZapPoint, 0xC);

enum class ZapLineType : s16
{
    eThick_0 = 0, // Used by Chant Suppressors and Greeters.
    eThin_1 = 1   // Used by the Shrykull.
};

class ZapLine final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ZapLine(FP x1, FP y1, FP x2, FP y2, s32 aliveTime, ZapLineType type, Layer layer);
    ~ZapLine();

    void CalculateSourceAndDestinationPositions(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest);

    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    // Note inlined
    void CalculateSpritePositionsOuter();

    void CalculateZapPoints_479380();
    void CalculateThinSpriteSegmentPositions();
    void CalculateThickSpriteSegmentPositions();
    void UpdateSpriteVertexPositions();
    void CalculateSpritePositionsInner(s32 idx1, s32 idx2, s32 idx3, s16 idx4);

    s32 field_D4[4] = {};

    enum class ZapLineState : s16
    {
        eInit_0 = 0,
        eInitSpritePositions_1 = 1,
        eInitSpriteVertices_2 = 2,
        eUpdateSpritePositions_3 = 3,
        eUpdateSpriteVertices_4 = 4
    };

    ZapLineState field_E4_state = ZapLineState::eInit_0;

    u8** field_E8_ppRes = nullptr;
    Prim_SetTPage field_EC_tPage_p8[2] = {};
    s16 field_10C_x_position_source = 0;
    s16 field_10E_y_position_source = 0;
    s16 field_110_x_position_destination = 0;
    s16 field_112_y_position_destination = 0;
    TPageMode field_114_tPageMode = TPageMode::e4Bit_0;
    s16 field_116_alive_timer = 0;
    s16 field_118_max_alive_time = 0;
    ZapLineType field_11A_type = ZapLineType::eThick_0;
    TPageAbr field_11C_tPageAbr = TPageAbr::eBlend_0;
    s16 field_11E_number_of_segments = 0;
    s16 field_120_number_of_pieces_per_segment = 0;
    s16 field_122_number_of_sprites = 0;
    ZapLineSprites* field_124_pSprts = nullptr;
    PSX_Point* field_128_sprite_positions = nullptr;
    ZapPoint* field_12C_zap_points = nullptr;
    FP_Point* field_130_sprite_segment_positions = nullptr;
    PSX_RECT field_134_rects[2] = {};
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x144);

} // namespace AO
