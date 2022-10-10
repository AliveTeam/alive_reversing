#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "FixedPoint.hpp"
#include "../relive_lib/Layer.hpp"

void Forcelink_ScreenWave();

enum class EReliveLevelIds : s16;
enum class Layer : s16;

namespace AO {

struct ScreenWave_Data;

class ScreenWave final : public ::BaseGameObject
{
public:
    ScreenWave(FP xpos, FP ypos, Layer layer, FP width, FP speed, s32 radius);
    ~ScreenWave();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    Layer field_10_layer = Layer::eLayer_0;
    ScreenWave_Data* field_14_ppRes = nullptr;
    FP field_18_xpos = {};
    FP field_1C_ypos = {};
    FP field_20_fp1 = {};
    FP field_24_fp1 = {};
    s32 field_28 = 0;
    FP field_2C = {};
    FP field_30_speed = {};
    s16 field_34_max_radius = 0;
    s16 field_36_screen_xpos = 0;
    s16 field_38_screen_ypos = 0;
    EReliveLevelIds field_3A_level = EReliveLevelIds::eNone;
    s16 field_3C_path = 0;
};

} // namespace AO
