#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"
#include "Layer.hpp"

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

enum class LevelIds : s16;

class AbilityRing final : public BaseGameObject
{
public:
    EXPORT BaseGameObject* dtor_455E50();

    EXPORT AbilityRing* ctor_455860(FP xpos, FP ypos, RingTypes type);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_456940(s32 flags);

    EXPORT static AbilityRing* CC Factory_447590(FP xpos, FP ypos, RingTypes ring_type);

    EXPORT void SetTarget_455EC0(BaseAliveGameObject* pTarget);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4568D0();

    EXPORT void CollideWithObjects_456250();

    virtual void VUpdate() override;

    EXPORT void VUpdate_455ED0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_456340(PrimHeader** ppOt);

    Layer field_10_layer;
    s16 field_12_pad;
    AbilityRing_PolyBuffer* field_14_pRes;
    u8** field_18_ppRes;
    Prim_SetTPage field_1C_primSetTPage[2];
    PSX_RECT field_3C_collide_rects[64];
    FP field_23C_xpos;
    FP field_240_ypos;
    FP field_244_left;
    FP field_248_right;
    FP field_24C_speed;
    FP field_250_scaleX;
    FP field_254_scaleY;
    FP field_258_ring_thickness;
    s16 field_25C_fade;
    s16 field_25E_screenX;
    s16 field_260_screenY;
    s16 field_262_screenXPos;
    s16 field_264_screenYPos;
    s16 field_266_r;
    s16 field_268_g;
    s16 field_26A_b;
    s16 field_26C_semiTrans;
    TPageAbr field_26E_tPageMode;
    // pad
    LevelIds field_270_level;
    s16 field_272_path;
    RingTypes field_274_ring_type;
    s16 field_276_padding;
    BaseAliveGameObject* field_278_pTarget_obj;
};
ALIVE_ASSERT_SIZEOF(AbilityRing, 0x27C);



} // namespace AO
