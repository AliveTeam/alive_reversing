#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"

void Forcelink_ScreenWave();

namespace AO {

enum class LevelIds : __int16;
struct ScreenWave_Data;

class ScreenWave : public BaseGameObject
{
public:
    EXPORT ScreenWave* ctor_462A70(FP xpos, FP ypos, __int16 layer, FP width, FP speed, __int16 radius);

    EXPORT BaseGameObject* dtor_463010();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ScreenWave* Vdtor_463860(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_463110();

    virtual void VUpdate() override;

    EXPORT void VUpdate_463080();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_463130(int** ppOt);

    __int16 field_10_layer;
    __int16 field_12_pad;
    ScreenWave_Data** field_14_ppRes;
    FP field_18_xpos;
    FP field_1C_ypos;
    FP field_20_fp1;
    FP field_24_fp1;
    int field_28;
    FP field_2C;
    FP field_30_speed;
    __int16 field_34_max_radius;
    __int16 field_36_screen_xpos;
    __int16 field_38_screen_ypos;
    LevelIds field_3A_level;
    __int16 field_3C_path;
    __int16 field_3E_pad;
};
ALIVE_ASSERT_SIZEOF(ScreenWave, 0x40);

}

