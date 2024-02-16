#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace relive
{
struct Path_WellBase;
struct Path_WellExpress;
struct Path_WellLocal;
}

class Well final : public BaseGameObject
{
public:
    Well(relive::Path_WellBase* pTlv, FP xpos, FP ypos, const Guid& tlvId);
    ~Well();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void WellExpress_Init(relive::Path_WellExpress* pTlv, FP /*xpos*/, FP ypos);
    void WellLocal_Init(relive::Path_WellLocal* pTlv, FP /*xpos*/, FP ypos);

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
