#pragma once

#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class StatusLight final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    StatusLight(relive::Path_StatusLight* pTlv, const Guid& tlvId);
    ~StatusLight();

    void LoadAnimations();

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
