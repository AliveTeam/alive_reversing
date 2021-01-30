#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

void ZapLine_ForceLink();

namespace AO {

struct ZapLineSprites
{
    Prim_Sprt field_0_sprts[2];
};
ALIVE_ASSERT_SIZEOF(ZapLineSprites, 0x30);

struct ZapPoint
{
    FP field_0;
    FP field_4;
    FP field_8;
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
    EXPORT ZapLine* ctor_4789A0(FP x1, FP y1, FP x2, FP y2, __int16 aliveTime, ZapLineType type, __int16 layer);

    EXPORT BaseGameObject* dtor_478E90();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ZapLine* Vdtor_479B20(signed int flags);

    EXPORT void CalculateSourceAndDestinationPositions_478CF0(FP xPosSource, FP yPosSource, FP xPosDest, FP yPosDest);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_479B00();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_479840(PrimHeader** ppOt);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4796B0();

    // Note inlined
    void CalculateSpritePositionsOuter();

    EXPORT void CalculateZapPoints_479380();

    EXPORT void CalculateThinSpriteSegmentPositions_4791F0();

    EXPORT void CalculateThickSpriteSegmentPositions_478F20();

    EXPORT void UpdateSpriteVertexPositions_4795B0();

    EXPORT void CalculateSpritePositionsInner_479400(int idx1, int idx2, int idx3, __int16 idx4);

    int field_D4[4];

    enum class ZapLineState : __int16
    {
        eInit_0 = 0,
        eInitSpritePositions_1 = 1,
        eInitSpriteVertices_2 = 2,
        eUpdateSpritePositions_3 = 3,
        eUpdateSpriteVertices_4 = 4
    };

    ZapLineState field_E4_state;

     __int16 field_E6_pad;
    BYTE** field_E8_ppRes;
    Prim_SetTPage field_EC_tPage_p8[2];
    __int16 field_10C_x_position_source;
    __int16 field_10E_y_position_source;
    __int16 field_110_x_position_destination;
    __int16 field_112_y_position_destination;
    __int16 field_114_tPageMode;
    __int16 field_116_alive_timer;
    __int16 field_118_max_alive_time;
    ZapLineType field_11A_type;
    __int16 field_11C_tPageAbr;
    __int16 field_11E_number_of_segments;
    __int16 field_120_number_of_pieces_per_segment;
    __int16 field_122_number_of_sprites;
    ZapLineSprites* field_124_pSprts;
    PSX_Point* field_128_sprite_positions;
    ZapPoint* field_12C_zap_points;
    FP_Point* field_130_sprite_segment_positions;
    PSX_RECT field_134_rects[2];
};
ALIVE_ASSERT_SIZEOF(ZapLine, 0x144);

}

