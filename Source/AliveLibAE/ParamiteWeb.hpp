#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class ParamiteWeb : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParamiteWeb* ctor_4E1840(FP xpos, s16 bottom, s16 top, FP scale);

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VRender(PrimHeader** ppOt) override;

    virtual BaseGameObject* VDestructor(s32 flags) override;

private:
    EXPORT ParamiteWeb* vdtor_4E1AF0(s32 flags);

    EXPORT void dtor_4E1B20();

    EXPORT void vUpdate_4E1F40();

    EXPORT void vScreenChanged_4E1F80();

    EXPORT void vRender_4E1BA0(PrimHeader** ppOt);

public:
    s16 field_F4_number_of_segments;
    s16 field_F6_segment_length;
    s16 field_F8_ttl;
    s16 field_FA_ttl_remainder;
    u8** field_FC_ppRes;
    AnimationUnknown* field_100_pRes;
    s16 field_104_bEnabled;
    s16 field_106_padding;
};
ALIVE_ASSERT_SIZEOF(ParamiteWeb, 0x108);
