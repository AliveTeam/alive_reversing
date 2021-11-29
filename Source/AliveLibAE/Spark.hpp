#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

struct SparkRes final
{
    FP field_0_x0;
    FP field_4_y0;
    FP field_8_x1;
    FP field_C_y1;
    u8 field_10_ang;
    // 3 byte pad
    FP field_14_radius;
    FP field_18_len;
    Line_G2 field_1C_pLineG2s[2];
};
ALIVE_ASSERT_SIZEOF(SparkRes, 0x4C);

class Spark final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    EXPORT Spark* ctor_4CBBB0(FP xpos, FP ypos, FP scale, u8 count, s16 min, s16 max, s16 type);

private:
    EXPORT void vUpdate_4CBEF0();
    EXPORT void vRender_4CC050(PrimHeader** ppOt);
    EXPORT void vScreenChange_4CC4A0();
    EXPORT Spark* vdtor_4CBE30(s32 flags);
    EXPORT void dtor_4CBE60();

private:
    Prim_SetTPage field_20_tPage[2];
    FP field_40_xpos;
    FP field_44_ypos;
    FP field_48_scale;
    s16 field_4C_r;
    s16 field_4E_g;
    s16 field_50_b;
    Layer field_52_layer;
    u8** field_54_ppSprxRes;
    SparkRes* field_58_pRes;
    s16 field_5C_count;
    s32 field_60_timer;
    s16 field_64_type;
};
ALIVE_ASSERT_SIZEOF(Spark, 0x68);
