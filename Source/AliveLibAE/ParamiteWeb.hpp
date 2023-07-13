#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"

class AnimationUnknown;

class ParamiteWeb final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    ParamiteWeb(FP xpos, s32 bottom, s32 top, FP scale);
    ~ParamiteWeb();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(OrderingTable& ot) override;

public:
    s16 mSegmentsCount = 0;
    s16 mSegmentLength = 0;
    s32 mTtl = 0;
    s32 mTtlRemainder = 0;
    AnimationUnknown* mWebRes = nullptr;
    bool mEnabled = false;
};
