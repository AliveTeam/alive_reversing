#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class ParamiteWeb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale);
    ~ParamiteWeb();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

public:
    s16 field_F4_number_of_segments;
    s16 field_F6_segment_length;
    s16 field_F8_ttl;
    s16 field_FA_ttl_remainder;
    u8** field_FC_ppRes;
    AnimationUnknown* field_100_pRes;
    s16 field_104_bEnabled;
};
ALIVE_ASSERT_SIZEOF(ParamiteWeb, 0x108);
