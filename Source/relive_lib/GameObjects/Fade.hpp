#pragma once

#include "EffectBase.hpp"
#include "../Layer.hpp"

enum class FadeOptions
{
    eFadeOut,
    eFadeIn
};

class Fade final : public EffectBase
{
public:
    Fade(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed, relive::TBlendModes abr);
    ~Fade();
    void Init(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed);
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& ot) override;

public:
    bool mDone = false;

private:
    s32 mFadeColour = 0;
    s32 mSpeed = 0;
    FadeOptions mFadeOption = FadeOptions::eFadeOut;
    bool mDestroyOnDone = false;
};
