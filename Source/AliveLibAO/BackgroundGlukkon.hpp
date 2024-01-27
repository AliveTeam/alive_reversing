#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_BackgroundGlukkon;
}

namespace AO {

class BackgroundGlukkon final : public BaseAliveGameObject
{
public:
    BackgroundGlukkon(relive::Path_BackgroundGlukkon* pTlv, const Guid& tlvId);
    ~BackgroundGlukkon();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual bool VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VUpdate() override;

    Guid mTlvId;
    enum class State
    {
        eToSetSpeakPauseTimer,
        eSetSpeakPauseTimer,
        eRandomizedLaugh,
        eAfterLaugh_SetSpeakPauseTimer,
        eKilledByShrykull
    };
    State mState = State::eToSetSpeakPauseTimer;
    s32 mSpeakPauseTimer = 0;
};

} // namespace AO
