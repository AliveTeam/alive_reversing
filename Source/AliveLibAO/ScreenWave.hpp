#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

void Forcelink_ScreenWave();

namespace AO {

enum class LevelIds : s16;
enum class Layer : s16;

struct ScreenWave_Data;

class ScreenWave : public BaseGameObject
{
public:
    EXPORT ScreenWave* ctor_462A70(FP xpos, FP ypos, Layer layer, FP width, FP speed, s16 radius);

    EXPORT BaseGameObject* dtor_463010();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ScreenWave* Vdtor_463860(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_463110();

    virtual void VUpdate() override;

    EXPORT void VUpdate_463080();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_463130(PrimHeader** ppOt);

    Layer field_10_layer;
    s16 field_12_pad;
    ScreenWave_Data** field_14_ppRes;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_fp1;
    FP field_24_fp1;
    s32 field_28;
    FP field_2C;
    FP field_30_speed;
    s16 field_34_max_radius;
    s16 field_36_screen_xpos;
    s16 field_38_screen_ypos;
    LevelIds field_3A_level;
    s16 field_3C_path;
    s16 field_3E_pad;
};
ALIVE_ASSERT_SIZEOF(ScreenWave, 0x40);

}

