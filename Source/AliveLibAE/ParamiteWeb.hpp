#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class ParamiteWeb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale);
    ~ParamiteWeb();

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    virtual void VRender(PrimHeader** ppOt) override;


private:

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
