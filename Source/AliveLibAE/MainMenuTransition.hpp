#pragma once

#include "Sfx.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

struct MainMenuTransition_Polys
{
    Poly_G3 field_0_polys[8];
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition_Polys, 0x100);

class MainMenuTransition : public BaseGameObject
{
public:
    EXPORT MainMenuTransition* ctor_464110(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 fadeSpeed, TPageAbr abr);
    EXPORT void StartTrans_464370(Layer layer, s16 fadeDirection, s16 bKillWhenDone, s16 speed);
    EXPORT void Update_464400();
    EXPORT void Render_464470(PrimHeader** ppOt);
    EXPORT void dtor_4642F0();
    EXPORT BaseGameObject* vdtor_4642C0(s32 flags);
    EXPORT void vScreenChanged_4648D0();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;

private:
    s16 field_20_current_value;
    s16 field_22_change_by_speed;
    s16 field_24_fade_direction;

public:
    s16 field_26_bDone;

private:
    s16 field_28_bKillOnDone;
    s16 field_2A;
    MainMenuTransition_Polys field_2C_polys[2];
    Prim_SetTPage field_22C_tPage[2];
    Layer field_24C_layer;
    s16 field_24E_width;
    s16 field_250_k120;
    s16 field_252_tbl_idx;
    //s16 field_254;
    s8 field_colour_fade_value;
    //s8 field_257;
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition, 0x258);
