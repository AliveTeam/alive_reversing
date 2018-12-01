#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Animation.hpp"
#include "Path.hpp"
#include "Factory.hpp"

struct BackgroundAnimation_TLV : public Path_TLV
{
    unsigned __int16 field_10_res_id;
    __int16 field_12_is_semi_trans;
    __int16 field_14_semi_trans_mode;
    __int16 field_16_sound_effect;
    __int16 field_18_id;
    unsigned __int16 field_1A_layer;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation_TLV, 0x1C);

class BackgroundAnimation : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged();

    EXPORT BackgroundAnimation* ctor_40D270(BackgroundAnimation_TLV* pPathParams, TlvItemInfoUnion tlvInfo);
    EXPORT void vUpdate_40D450();
    EXPORT void vScreenChanged_40D550();
    EXPORT void dtor_40D4C0();
    EXPORT BaseGameObject* vdtor_40D420(signed int flags);
private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    AnimHeader **field_F4_res;
    TlvItemInfoUnion field_F8_arg_a3;
    FP field_FC_xpos;
    FP field_100_ypos;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
