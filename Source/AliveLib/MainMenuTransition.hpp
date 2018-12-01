
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
    EXPORT MainMenuTransition* ctor_464110(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 fadeSpeed, char abr);
    EXPORT void sub_464370(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 speed);
    EXPORT void Update_464400();
    EXPORT void Render_464470(int** ot);
    EXPORT void dtor_4642F0();
    EXPORT BaseGameObject* vdtor_4642C0(signed int flags);

    virtual void VRender(int** ot) override;
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    __int16 field_20_current_value;
    __int16 field_22_change_by_speed;
    __int16 field_24_fade_direction;
public:
    __int16 field_26_bDone;
private:
    __int16 field_28_bKillOnDone;
    __int16 field_2A;
    MainMenuTransition_Polys field_2C_polys[2];
    Prim_SetTPage field_22C_tPage[2];
    __int16 field_24C_layer;
    __int16 field_24E_width;
    __int16 field_250_k120;
    __int16 field_252_tbl_idx;
    //__int16 field_254;
    char field_colour_fade_value;
    //char field_257;
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition, 0x258);
