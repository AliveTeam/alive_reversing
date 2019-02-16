#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class RopeSegment;

class Rope : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Rope* ctor_4A0A70(unsigned __int16 left, __int16 top, unsigned __int16 bottom, FP scale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
private:
    EXPORT void dtor_4A0DB0();
    EXPORT Rope* vdtor_4A0D80(signed int flags);
    EXPORT void vRender_4A0E30(int** pOt);
private:
    int field_E4_not_used[4];
    __int16 field_F4_rope_segment_count;
public:
    __int16 field_F6_rope_length;
    BYTE **field_F8_ppRopeRes;
    RopeSegment* field_FC_pRopeRes;
    __int16 field_100_left;
    __int16 field_102_top;
    __int16 field_104_right;
    __int16 field_106_bottom;
};
ALIVE_ASSERT_SIZEOF(Rope, 0x108);
