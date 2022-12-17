#pragma once

#include "Sfx.hpp"
#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Layer.hpp"

enum class TPageAbr : s8;

struct MainMenuTransition_Polys final
{
    Poly_G3 mPolys[8];
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition_Polys, 0x100);

class MainMenuTransition final : public BaseGameObject
{
public:
    MainMenuTransition(Layer layer, s32 fadeDirection, bool killWhenDone, s32 fadeSpeed, TPageAbr abr);
    ~MainMenuTransition();

    void StartTrans(Layer layer, s16 fadeDirection, bool killWhenDone, s16 speed);

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

public:
    s16 mDone = 0;

private:
    s16 field_20_current_value = 0;
    s16 field_22_change_by_speed = 0;
    s16 field_24_fade_direction = 0;
    bool mKillWhenDone = false;
    s16 field_2A = 0;
    MainMenuTransition_Polys field_2C_polys[2] = {};
    Prim_SetTPage mTPage[2] = {};
    Layer mLayer = Layer::eLayer_0;
    s16 field_24E_width = 0;
    s16 field_250_k120 = 0;
    s16 field_252_tbl_idx = 0;
    s8 field_colour_fade_value = 0;
};
