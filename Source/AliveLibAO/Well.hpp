#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

namespace relive {
struct Path_WellBase;
struct Path_WellExpress;
struct Path_WellLocal;
}

namespace AO {

class Well final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Well(relive::Path_WellBase* pTlv, FP xpos, FP ypos, const Guid& tlvId);
    ~Well();

    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;

private:
    void WellExpress_Init(relive::Path_WellExpress* pTlv, FP xpos, FP ypos);
    void WellLocal_Init(relive::Path_WellLocal* pTlv, FP xpos, FP ypos);
    void InitBgAnim(s16 bgAnimId);
    
private:
    Guid mTlvInfo;
    s16 mSwitchId = 0;
    FP mLeafScale = {};
    FP mExitX = {};
    FP mExitY = {};
    FP mLeafX = {};
    FP mLeafY = {};
    bool mEmitLeaves = false;
};

} // namespace AO
