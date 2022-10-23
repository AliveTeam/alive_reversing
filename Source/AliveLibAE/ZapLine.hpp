#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Layer.hpp"

struct ZapLineSprites final
{
    Prim_Sprt field_0_sprts[2];
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

    ZapLineState field_F4_state = ZapLineState::eInit_0;
    Prim_SetTPage field_FC_tPage_p8[2] = {};
    s16 field_11C_x_position_source = 0;
    s16 field_11E_y_position_source = 0;
    s16 field_120_x_position_destination = 0;
    s16 field_122_y_position_destination = 0;
    s16 field_126_alive_timer = 0;
    s16 field_128_max_alive_time = 0;
    ZapLineType field_12A_type = ZapLineType::eThick_0;
    TPageAbr field_12C_tPageAbr = TPageAbr::eBlend_0;
    s16 field_12E_number_of_segments = 0;
    s16 field_130_number_of_pieces_per_segment = 0;
    s16 field_132_number_of_sprites = 0;
    ZapLineSprites* field_134_pSprites = nullptr;
    PSX_Point* field_138_sprite_positions = nullptr;
    ZapPoint* field_13C_zap_points = nullptr;
    FP_Point* field_140_sprite_segment_positions = nullptr;
    PSX_RECT field_144_rects[2] = {};
};
