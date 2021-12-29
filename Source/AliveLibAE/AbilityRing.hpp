#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

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
    AETypes field_0_type;
    s16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    RingTypes field_C_ring_type;
    s16 field_E_padding;
    FP field_10_scale;
    s32 field_14_obj_id;
    FP field_18_right;
    s32 field_1C_count;
    s16 field_20_r;
    s16 field_22_g;
    s16 field_24_b;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(AbilityRing_State, 0x28);

struct AbilityRing_PolyBuffer;
enum class LevelIds : s16;

class AbilityRing final : public BaseGameObject
{
public:
    EXPORT static AbilityRing* CC Factory_482F80(FP xpos, FP ypos, RingTypes type, FP scale);
    EXPORT AbilityRing* ctor_49C730(FP xpos, FP ypos, RingTypes ringType, FP scale);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

    // New virtuals
    virtual void VSetTarget(BaseGameObject* pTarget);

    static EXPORT s32 CC CreateFromSaveState_49DF90(const u8* pBuffer);

private:
    EXPORT void vSetTarget_49D140(BaseGameObject* pTarget);
    EXPORT void dtor_49D0B0();
    EXPORT AbilityRing* vdtor_49D080(s32 flags);
    EXPORT void vUpdate_49D160();
    EXPORT void CollideWithObjects_49D5E0(s16 bDealDamage);
    EXPORT void vRender_49D790(PrimHeader** ppOt);
    EXPORT void vScreenChanged_49DE70();
    EXPORT s32 vGetSaveState_49E070(AbilityRing_State* pSaveState);

private:
    Layer field_20_layer;
    //s16 field_22_pad;
    AbilityRing_PolyBuffer* field_24_pRes;
    u8** field_28_ppRes;
    Prim_SetTPage field_2C_primSetTPage[2];
    PSX_RECT field_4C_collide_rects[64];
    FP field_24C_xpos;
    FP field_250_ypos;
    FP field_254_left;
    FP field_258_right;
    FP field_25C_speed;
    FP field_260_scaleX;
    FP field_264_scaleY;
    FP field_268_ring_thickness;
    s16 field_26C_fadeout_distance;
    s16 field_26E_screenX;
    s16 field_270_screenY;
    s16 field_272_screenXPos;
    s16 field_274_screenYPos;
    s16 field_276_r;
    s16 field_278_g;
    s16 field_27A_b;
    s16 field_27C_semiTrans;
    TPageAbr field_27E_tPageMode;
    // pad
    LevelIds field_280_level;
    s16 field_282_path;
    RingTypes field_284_ring_type;
    //s16 field_286_pad;
    s32 field_288_target_obj_id;
    s32 field_28C_count;
    s16 field_290_bFindingTarget;
    // s16 field_292_pad;
};

ALIVE_ASSERT_SIZEOF(AbilityRing, 0x294);
