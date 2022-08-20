#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

struct Path_WellBase;
struct Path_WellLocal;
struct Path_WellExpress;

class Well final : public BaseGameObject
{
public:
    Well(Path_WellBase* pTlv, FP xpos, FP ypos, s32 tlvInfo);
    ~Well();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    void WellExpress_Init(Path_WellExpress* pTlv, FP /*xpos*/, FP ypos);
    void WellLocal_Init(Path_WellLocal* pTlv, FP /*xpos*/, FP ypos);

private:
    s32 mTlvInfo = 0;
    s16 mSwitchId = 0;
    FP mLeafScale = {};
    FP mExitX = {};
    FP mExitY = {};
    FP mLeafX = {};
    FP mLeafY = {};
    Choice_short mEmitLeaves = Choice_short::eNo_0;
};
ALIVE_ASSERT_SIZEOF(Well, 0x40);
