#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
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
    StatusLight(relive::Path_StatusLight* pTlv, const Guid& tlvId);
    ~StatusLight();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    bool isLinkedAndDisabled(s16 switchId);

private:
    Guid field_F4_tlvInfo;
    s16 mInputSwitchId = 0;
    s16 mLinkedStatusLightSwitchId1 = 0;
    s16 mLinkedStatusLightSwitchId2 = 0;
    s16 mLinkedStatusLightSwitchId3 = 0;
    s16 mLinkedStatusLightSwitchId4 = 0;
    s16 mLinkedStatusLightSwitchId5 = 0;
    relive::reliveChoice mIgnoreGridSnapping = relive::reliveChoice::eNo;
    FP field_108_xpos = {};
    FP field_10C_ypos = {};
};
ALIVE_ASSERT_SIZEOF(StatusLight, 0x110);
