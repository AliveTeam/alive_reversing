#pragma once

#include "../relive_lib/Function.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Primitives.hpp"

namespace AO
{

struct MainMenuTransition_Polys final
{
    Poly_G3 field_0_polys[8];
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition_Polys, 0x100);


class MainMenuTransition final : public ::BaseGameObject
{
public:
    MainMenuTransition(Layer layer, s32 fadeDirection, s32 bKillWhenDone, s32 speed, relive::TBlendModes blendMode);
    ~MainMenuTransition();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(BasePrimitive** ppOt) override;

    void StartTrans_436560(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed);

    s16 field_10_current_Value = 0;
    s16 field_12_change_by_speed = 0;
    s16 field_14_fade_direction = 0;
    s16 field_16_bDone = 0;
    s16 field_18_bKillOnDone = 0;
    MainMenuTransition_Polys field_1C_polys[2] = {};
    Layer field_23C_layer = Layer::eLayer_0;
    s16 field_23E_width = 0;
    s16 field_240_k120 = 0;
    s16 field_242_idx = 0;
    s8 field_246_colour_fade_value = 0;
};

} // namespace AO

