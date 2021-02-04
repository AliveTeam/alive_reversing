#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Layer.hpp"

struct ZapLineSprites
{
    Prim_Sprt field_0_sprts[2];
};
ALIVE_ASSERT_SIZEOF(ZapLineSprites, 0x30);

// TODO: Give better names to the ZapPoint fields.
struct ZapPoint
{
    FP field_0_part_1;
    FP field_4_part_2;
    FP field_8_part_3;
};
ALIVE_ASSERT_SIZEOF(ZapPoint, 0xC);

enum class ZapLineType : __int16
{
    eThick_0 = 0, // Used by Chant Suppressors and Greeters.
    eThin_1 = 1   // Used by the Shrykull.
};

class ZapLine : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ZapLine* ctor_4CC690(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest, __int16 aliveTime, ZapLineType type, Layer layer);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
private:
    EXPORT ZapLine* vdtor_4CCAA0(signed int flags);
public:
    EXPORT void CalculateSourceAndDestinationPositions_4CCAD0(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest);
private:
    EXPORT void dtor_4CCCB0();
    EXPORT void vScreenChanged_4CDBE0();
    EXPORT void CalculateThickSpriteSegmentPositions_4CCD50();
    EXPORT void CalculateThinSpriteSegmentPositions_4CD110();
    EXPORT void CalculateZapPoints_4CD340();
    EXPORT void CalculateSpritePositionsInner_4CD400(int idx1, int idx2, int idx3, __int16 idx4);
    EXPORT void UpdateSpriteVertexPositions_4CD650();
    EXPORT void CalculateSpritePositionsOuter_4CD5D0();
    EXPORT void vUpdate_4CD790();
    EXPORT void vRender_4CD8C0(PrimHeader** ppOt);

private:
    enum class ZapLineState : __int16
    {
        eInit_0 = 0,
        eInitSpritePositions_1 = 1,
        eInitSpriteVertices_2 = 2,
        eUpdateSpritePositions_3 = 3,
        eUpdateSpriteVertices_4 = 4
    };

    ZapLineState field_F4_state;
    __int16 field_F6_padding;
    BYTE** field_F8_ppRes;
    Prim_SetTPage field_FC_tPage_p8[2];
    __int16 field_11C_x_position_source;
    __int16 field_11E_y_position_source;
    __int16 field_120_x_position_destination;
    __int16 field_122_y_position_destination;
    __int16 field_124_tPageMode;
    __int16 field_126_alive_timer;
    __int16 field_128_max_alive_time;
    ZapLineType field_12A_type;
    __int16 field_12C_tPageAbr;
    __int16 field_12E_number_of_segments;
    __int16 field_130_number_of_pieces_per_segment;
    __int16 field_132_number_of_sprites;
    ZapLineSprites* field_134_pSprites;
    PSX_Point* field_138_sprite_positions;
    ZapPoint* field_13C_zap_points;
    FP_Point* field_140_sprite_segment_positions;
    PSX_RECT field_144_rects[2];
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x154);
