#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class ParamiteWeb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale);
    ~ParamiteWeb();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

public:
    s16 field_F4_number_of_segments = 0;
    s16 field_F6_segment_length = 0;
    s16 field_F8_ttl = 0;
    s16 field_FA_ttl_remainder = 0;
    AnimationUnknown* field_100_pRes = nullptr;
    s16 field_104_bEnabled = 0;
};
ALIVE_ASSERT_SIZEOF(ParamiteWeb, 0x108);
