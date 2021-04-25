#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class AnimationUnknown;

class ParamiteWeb : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParamiteWeb* ctor_48A920(FP xpos, s16 bottom, s16 top, FP scale);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ParamiteWeb* Vdtor_48AEC0(s32 flags);

    EXPORT BaseGameObject* dtor_48AAB0();

    EXPORT void VScreenChanged_48AEA0();

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VUpdate_48AE70();

    EXPORT void VRender_48AB10(PrimHeader** ppOt);

    s32 field_D4[4];
    s16 field_E4_number_of_segments;
    s16 field_E6_segment_length;
    s16 field_E8_ttl;
    s16 field_EA_ttl_remainder;
    AnimationUnknown* field_EC_pRes;
    s16 field_F0_bEnabled;
    s16 field_F2_padding;
};
ALIVE_ASSERT_SIZEOF(ParamiteWeb, 0xF4);

}

