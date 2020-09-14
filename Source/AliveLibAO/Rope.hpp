#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

class AnimationUnknown;

class Rope : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Rope* ctor_458520(unsigned __int16 left, __int16 top, unsigned __int16 bottom, FP scale);

    EXPORT BaseGameObject* dtor_458720();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Rope* Vdtor_458B60(signed int flags);

    virtual void VUpdate() override;



    int field_D4[4];
    __int16 field_E4_rope_segment_count;
    __int16 field_E6_rope_length;
    AnimationUnknown* field_E8_pRopeRes;
    __int16 field_EC_left;
    __int16 field_EE_top;
    __int16 field_F0_right;
    __int16 field_F2_bottom;
};
ALIVE_ASSERT_SIZEOF(Rope, 0xF4);

EXPORT void CC ClipPoly_Vertically_4584B0(Poly_FT4* pPoly, int minY, int maxY);

END_NS_AO

