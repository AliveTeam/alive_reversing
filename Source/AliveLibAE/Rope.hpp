#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class AnimationUnknown;

class Rope : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Rope* ctor_4A0A70(u16 left, s16 top, u16 bottom, FP scale);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
private:
    EXPORT void dtor_4A0DB0();
    EXPORT Rope* vdtor_4A0D80(s32 flags);
    EXPORT void vRender_4A0E30(PrimHeader** ppOt);
private:
    s16 field_F4_rope_segment_count;
public:
    s16 field_F6_rope_length;
    BYTE **field_F8_ppRopeRes;
    AnimationUnknown* field_FC_pRopeRes;
    s16 field_100_left;
    s16 field_102_top;
    s16 field_104_right;
    s16 field_106_bottom;
};
ALIVE_ASSERT_SIZEOF(Rope, 0x108);

EXPORT void CC ClipPoly_Vertically_4A09E0(Poly_FT4* pPoly, s32 minY, s32 maxY);
