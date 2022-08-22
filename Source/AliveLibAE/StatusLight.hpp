#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

struct Path_StatusLight final : public Path_TLV
{
    s16 mInputSwitchId;
    Scale_short mScale;
    s16 mLinkedStatusLightSwitchId1;
    s16 mLinkedStatusLightSwitchId2;
    s16 mLinkedStatusLightSwitchId3;
    s16 mLinkedStatusLightSwitchId4;
    s16 mLinkedStatusLightSwitchId5;
    Choice_short mIgnoreGridSnapping;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_StatusLight, 0x20);

class StatusLight final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    StatusLight(Path_StatusLight* pTlv, u32 tlvInfo);
    ~StatusLight();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    bool isLinkedAndDisabled(s16 switchId);

private:
    u32 field_F4_tlvInfo = 0;
    s16 mInputSwitchId = 0;
    s16 mLinkedStatusLightSwitchId1 = 0;
    s16 mLinkedStatusLightSwitchId2 = 0;
    s16 mLinkedStatusLightSwitchId3 = 0;
    s16 mLinkedStatusLightSwitchId4 = 0;
    s16 mLinkedStatusLightSwitchId5 = 0;
    Choice_short mIgnoreGridSnapping = Choice_short::eNo_0;
    FP field_108_xpos = {};
    FP field_10C_ypos = {};
};
ALIVE_ASSERT_SIZEOF(StatusLight, 0x110);
