#pragma once

#include "BaseAliveGameObject.hpp"

class ZapLine;

namespace AO {

class Shrykull final : public BaseAliveGameObject
{
public:
    Shrykull();
    ~Shrykull();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VOnThrowableHit(BaseGameObject*) override;
    virtual void VUpdate() override;

    bool CanKill(BaseAnimatedWithPhysicsGameObject* pObj);
    bool CanElectrocute(BaseGameObject* pObj) const;

    enum class State : s16
    {
        eTransform_0 = 0,
        eZapTargets_1 = 1,
        eDetransform_2 = 2,
        eFinish_3 = 3,
        eKillTargets_4 = 4,
    };
    State mState = State::eTransform_0;
    s32 mZapIntervalTimer = 0;
    s32 mFlashTimer = 0;
    ZapLine* mZapLine = nullptr;
    IBaseAliveGameObject* mZapTarget = nullptr;
    bool mCanElectrocute = false;
    bool mResetRingTimer = false;
};

} // namespace AO
