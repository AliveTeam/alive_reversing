#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"

namespace AO {

class BaseAliveGameObject;

struct AbilityRing_PolyBuffer
{
    Poly_F4 mPolys[2];
};
ALIVE_ASSERT_SIZEOF(AbilityRing_PolyBuffer, 56);

enum class LevelIds : __int16;

class AbilityRing : public BaseGameObject
{
public:
    EXPORT BaseGameObject* dtor_455E50();

    EXPORT AbilityRing* ctor_455860(FP xpos, FP ypos, __int16 type);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_456940(signed int flags);

    EXPORT static AbilityRing* CC Factory_447590(FP xpos, FP ypos, __int16 type);

    EXPORT void SetTarget_455EC0(BaseAliveGameObject* pTarget);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4568D0();

    EXPORT void CollideWithObjects_456250();

    virtual void VUpdate() override;

    EXPORT void VUpdate_455ED0();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_456340(PrimHeader** ppOt);

    __int16 field_10_layer;
    __int16 field_12_pad;
    AbilityRing_PolyBuffer* field_14_pRes;
    BYTE** field_18_ppRes;
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
    __int16 field_25C_fade;
    __int16 field_25E_screenX;
    __int16 field_260_screenY;
    __int16 field_262_screenXPos;
    __int16 field_264_screenYPos;
    __int16 field_266_r;
    __int16 field_268_g;
    __int16 field_26A_b;
    __int16 field_26C_semiTrans;
    __int16 field_26E_tPageMode;
    LevelIds field_270_level;
    __int16 field_272_path;
    __int16 field_274_ring_type;
    __int16 field_276_padding;
    BaseAliveGameObject* field_278_pTarget_obj;
};
ALIVE_ASSERT_SIZEOF(AbilityRing, 0x27C);



}

