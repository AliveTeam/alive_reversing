#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/EffectBase.hpp"
#include "../relive_lib/Layer.hpp"

enum class FadeOptions
{
    eFadeOut,
    eFadeIn
};

class DeathFadeOut final : public EffectBase
{
public:
    DeathFadeOut(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed, TPageAbr abr);
    ~DeathFadeOut();
    void Init(Layer layer, FadeOptions fade, bool destroyOnDone, s32 speed);
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;


private:
    s32 mCurrentFadeRGB = 0;
    s32 mSpeed = 0;
    FadeOptions mFadeOption = FadeOptions::eFadeOut;

public:
    bool mDone = false;

private:
    bool mDestroyOnDone = false;
};
