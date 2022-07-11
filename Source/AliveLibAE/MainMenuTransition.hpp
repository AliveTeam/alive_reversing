#pragma once

#include "Sfx.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"

struct MainMenuTransition_Polys final
{
    Poly_G3 field_0_polys[8];
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition_Polys, 0x100);

class MainMenuTransition final : public BaseGameObject
{
public:
    MainMenuTransition(Layer layer, s32 fadeDirection, s32 bKillWhenDone, s32 fadeSpeed, TPageAbr abr);
    ~MainMenuTransition();

    void StartTrans(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed);

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    s16 field_20_current_value = 0;
    s16 field_22_change_by_speed = 0;
    s16 field_24_fade_direction = 0;

public:
    s16 field_26_bDone = 0;

private:
    s16 field_28_bKillOnDone = 0;
    s16 field_2A = 0;
    MainMenuTransition_Polys field_2C_polys[2] = {};
    Prim_SetTPage field_22C_tPage[2] = {};
    Layer field_24C_layer = Layer::eLayer_0;
    s16 field_24E_width = 0;
    s16 field_250_k120 = 0;
    s16 field_252_tbl_idx = 0;
    s8 field_colour_fade_value = 0;
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition, 0x258);
