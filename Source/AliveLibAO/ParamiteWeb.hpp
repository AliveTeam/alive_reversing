#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

class AnimationUnknown;

class ParamiteWeb final : public BaseAnimatedWithPhysicsGameObject
{
public:
    ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale);
    ~ParamiteWeb();
    
    void VScreenChanged_48AEA0();

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    virtual void VRender(PrimHeader** ppOt) override;

    void VUpdate_48AE70();

    void VRender_48AB10(PrimHeader** ppOt);

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

} // namespace AO
