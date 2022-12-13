#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
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
    MainMenuTransition(Layer layer, s32 fadeDirection, s32 bKillWhenDone, s32 speed, TPageAbr abr);
    ~MainMenuTransition();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void StartTrans_436560(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed);

    s16 field_10_current_Value = 0;
    s16 field_12_change_by_speed = 0;
    s16 field_14_fade_direction = 0;
    s16 field_16_bDone = 0;
    s16 field_18_bKillOnDone = 0;
    MainMenuTransition_Polys field_1C_polys[2] = {};
    Prim_SetTPage field_21C_tPage[2] = {};
    Layer field_23C_layer = Layer::eLayer_0;
    s16 field_23E_width = 0;
    s16 field_240_k120 = 0;
    s16 field_242_idx = 0;
    s8 field_246_colour_fade_value = 0;
};

} // namespace AO

