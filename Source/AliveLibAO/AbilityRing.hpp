#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"
#include "FixedPoint.hpp"

START_NS_AO

class BaseAliveGameObject;

class AbilityRing : public BaseGameObject
{
public:
    EXPORT static AbilityRing* CC Factory_447590(FP xpos, FP ypos, __int16 type);

    __int16 field_10_layer;
    __int16 field_12_pad;
    int field_14_pRes;
    BYTE** field_18_ppRes;
    Prim_SetTPage field_1C_primSetTPage[2];
    PSX_RECT field_3C_collide_rects[64];
    int field_23C_xpos;
    int field_240_ypos;
    int field_244_left;
    int field_248_right;
    int field_24C_speed;
    int field_250_scaleX;
    int field_254_scaleY;
    int field_258_ring_thickness;
    __int16 field_25C_fade;
    __int16 field_25E_screenX;
    __int16 field_260_screenY;
    __int16 field_262_screenXPos;
    __int16 field_264_screenYPos;
    __int16 field_266_r;
    __int16 field_268_g;
    __int16 field_26A_b;
    __int16 field_26C_prims;
    __int16 field_26E_tPageMode;
    __int16 field_270_level;
    __int16 field_272_path;
    __int16 field_274_ring_type;
    __int16 field_276;
    BaseAliveGameObject* field_278_pTarget_obj;
};
ALIVE_ASSERT_SIZEOF(AbilityRing, 0x27C);



END_NS_AO

