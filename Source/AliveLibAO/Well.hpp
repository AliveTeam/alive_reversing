#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

namespace AO {

class Well final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Well(relive::Path_WellBase* pTlv, FP xpos, FP ypos, const Guid& tlvId);
    ~Well();

    void WellLocal_Init(relive::Path_WellLocal* pTlv, FP xpos, FP ypos);
    void WellExpress_Init(relive::Path_WellExpress* pTlv, FP xpos, FP ypos);

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    Guid mTlvInfo;
    s16 mSwitchId = 0;
    FP mLeafScale = {};
    FP mExitX = {};
    FP mExitY = {};
    FP mLeafX = {};
    FP mLeafY = {};
    relive::reliveChoice mEmitLeaves = relive::reliveChoice::eNo;
};

} // namespace AO
