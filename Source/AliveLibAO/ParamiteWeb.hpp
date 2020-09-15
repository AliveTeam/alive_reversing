#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

class AnimationUnknown;

class ParamiteWeb : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParamiteWeb* ctor_48A920(FP xpos, __int16 bottom, __int16 top, FP scale);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT ParamiteWeb* Vdtor_48AEC0(signed int flags);

    EXPORT BaseGameObject* dtor_48AAB0();

    EXPORT void VScreenChanged_48AEA0();

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_48AE70();

    EXPORT void VRender_48AB10(int **ppOt);

    int field_D4[4];
    __int16 field_E4_number_of_segments;
    __int16 field_E6_segment_length;
    __int16 field_E8_ttl;
    __int16 field_EA_ttl_remainder;
    AnimationUnknown* field_EC_pRes;
    __int16 field_F0_bEnabled;
    __int16 field_F2_padding;
};
ALIVE_ASSERT_SIZEOF(ParamiteWeb, 0xF4);

END_NS_AO

