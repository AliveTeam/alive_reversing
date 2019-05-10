#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"

enum class RingTypes : __int16
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

struct AbilityRing_State
{
    Types field_0_type;
    __int16 field_2_pad;
    FP field_4_xpos;
    FP field_8_ypos;
    RingTypes field_C_ring_type;
    __int16 field_E;
    FP field_10_scale;
    int field_14_obj_id;
    FP field_18_right;
    int field_1C_count;
    __int16 field_20_r;
    __int16 field_22_g;
    __int16 field_24_b;
    __int16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(AbilityRing_State, 0x28);

struct AbilityRing_PolyBuffer;
enum class LevelIds : __int16;

class AbilityRing : public BaseGameObject
{
public:
    EXPORT static AbilityRing * CC Factory_482F80(FP xpos, FP ypos, RingTypes type, FP scale);
    EXPORT AbilityRing* ctor_49C730(FP xpos, FP ypos, RingTypes ringType, FP scale);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;

    // New virtuals
    virtual void VSetTarget(BaseGameObject* pTarget);

    static EXPORT int CC CreateFromSaveState_49DF90(const BYTE* pBuffer);

private:
    EXPORT void vSetTarget_49D140(BaseGameObject* pTarget);
    EXPORT void dtor_49D0B0();
    EXPORT AbilityRing* vdtor_49D080(signed int flags);
    EXPORT void vUpdate_49D160();
    EXPORT void CollideWithObjects_49D5E0(__int16 bDealDamage);
    EXPORT void vRender_49D790(int** ppOt);
    EXPORT void vScreenChanged_49DE70();
    EXPORT int vGetSaveState_49E070(AbilityRing_State* pSaveState);

private:
    __int16 field_20_layer;
    //__int16 field_22_pad;
    AbilityRing_PolyBuffer* field_24_pRes;
    BYTE** field_28_ppRes;
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
    __int16 field_26C_fade; // Something to do with the fade out amount/time ??
    __int16 field_26E_screenX;
    __int16 field_270_screenY;
    __int16 field_272_screenXPos;
    __int16 field_274_screenYPos;
    __int16 field_276_r;
    __int16 field_278_g;
    __int16 field_27A_b;
    __int16 field_27C_prims;
    __int16 field_27E_tPageMode;
    LevelIds field_280_level;
    __int16 field_282_path;
    RingTypes field_284_ring_type;
    //__int16 field_286_pad;
    int field_288_target_obj_id;
    int field_28C_count;
    __int16 field_290_bFindingTarget;
    // __int16 field_292_pad;
};

ALIVE_ASSERT_SIZEOF(AbilityRing, 0x294);

