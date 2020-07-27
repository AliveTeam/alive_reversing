#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Primitives.hpp"
#include "Font.hpp"

START_NS_AO

class MainMenuFade : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT MainMenuFade* ctor_42A5A0(__int16 xpos, __int16 ypos, unsigned __int16 idx_1, __int16 bDestroyOnDone);

    virtual void VUpdate() override;

    EXPORT void VUpdate_42A730();

    virtual void VRender(int** pOrderingTable) override;

    EXPORT void VRender_42A7A0(int** ppOt);

    int field_D4[4];
    __int16 field_E4;
    __int16 field_E6;
    __int16 field_E8_bDestroyOnDone;
    __int16 field_EA;
};
ALIVE_ASSERT_SIZEOF(MainMenuFade, 0xEC);

struct MainMenuTransition_Polys
{
    Poly_G3 field_0_polys[8];
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition_Polys, 0x100);

// TODO: Move to own file
class MainMenuTransition : public BaseGameObject
{
public:
    EXPORT MainMenuTransition* ctor_436370(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 speed, char abr);

    EXPORT BaseGameObject* dtor_436500();

    virtual void VUpdate() override;

    EXPORT void VUpdate_4365C0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_4369D0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MainMenuTransition* Vdtor_4369F0(signed int flags);

    EXPORT void StartTrans_436560(__int16 layer, __int16 fadeDirection, __int16 bKillWhenDone, __int16 speed);
    __int16 field_10_current_Value;
    __int16 field_12_change_by_speed;
    __int16 field_14_fade_direction;
    __int16 field_16_bDone;
    __int16 field_18_bKillOnDone;
    __int16 field_1A;
    MainMenuTransition_Polys field_1C_polys[2];
    Prim_SetTPage field_21C_tPage[2];
    __int16 field_23C_layer;
    __int16 field_23E_width;
    __int16 field_240_k120;
    __int16 field_242;
    char field_244;
    char field_245;
    char field_246_colour_fade_value;
    char field_247;
};
ALIVE_ASSERT_SIZEOF(MainMenuTransition, 0x248);

struct Path_TLV;

class Menu : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Menu* ctor_47A6F0(Path_TLV* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_47AAB0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Menu* Vdtor_47FC40(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_47ABB0();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_47AC00(int** ppOt);

    virtual void VScreenChanged() override;

    EXPORT void AbeHeadDoorOpen_47B550();

    EXPORT void CopyRight_Update_47B4C0();

    EXPORT void FMV_Select_Update_47E8D0();

    EXPORT void Empty_Render_47AC80(int**);
    
    EXPORT void FMV_Select_Render_47EEA0(int**);

    
    int field_D4[4];
    BYTE** field_E4_res_array[6];
    AliveFont field_FC_font;
    Animation field_134_anim;

    using TUpdateFn = decltype(&Menu::AbeHeadDoorOpen_47B550);

    TUpdateFn field_1CC_fn_update;

    using TRenderFn = decltype(&Menu::Empty_Render_47AC80);

    TRenderFn field_1D0_fn_render;

    int field_1D4_tlvInfo;
    int field_1D8_timer;
    int field_1DC_idle_input_counter;
    __int16 field_1E0_selected_index;
    __int16 field_1E2_rgb;
    __int16 field_1E4_colour_counter;
    __int16 field_1E6;
    MainMenuTransition* field_1E8_pMenuTrans;
    BaseGameObject* field_1EC_pObj1;
    MainMenuFade* field_1F0_pObj2;
    const char* field_1F4_text;
    __int16 field_1F8;
    __int16 field_1FA;
    __int16 field_1FC;
    __int16 field_1FE;
    __int16 field_200;
    __int16 field_202;
    __int16 field_204_flags;
    __int16 field_206;
    int field_208_counter;
    __int16 field_20C;
    __int16 field_20E_level;
    __int16 field_210_path;
    __int16 field_212_camera;
    __int16 field_214_abe_xpos;
    __int16 field_216_abe_ypos;
    __int16 field_218;
    __int16 field_21A;
    int field_21C;
    int field_220;
    __int16 field_224;
    __int16 field_226;
    int field_228;
    int field_22C;
    int field_230;
};
ALIVE_ASSERT_SIZEOF(Menu, 0x234);

ALIVE_VAR_EXTERN(int, gMainMenuInstanceCount_9F2DE0);

END_NS_AO

