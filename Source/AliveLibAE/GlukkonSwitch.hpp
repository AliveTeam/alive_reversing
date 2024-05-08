#pragma once

#include "../relive_lib/GameObjects/BaseAnimatedWithPhysicsGameObject.hpp"
#include "GameSpeak.hpp"

namespace relive
{
    struct Path_GlukkonSwitch;
}

class GlukkonSwitch final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    GlukkonSwitch(relive::Path_GlukkonSwitch* pTlv, const Guid& tlvId);
    ~GlukkonSwitch();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    bool PlayerNearMe() const;

private:
    Guid mTlvId;
    enum class State : s16
    {
        eWaitUntilGlukkonNearby,
        eSuccessFinish,
        eSayHey,
        eWaitForHeyInput,
        eSayWhat,
        eWaitForDoItInput,
        eSuccess,
        eListeningTimedOut,
        eFail
    };
    State mState = State::eWaitUntilGlukkonNearby;
    s16 mOkSwitchId = 0;
    s16 mFailSwitchId = 0;
    GameSpeakListener mListener;
    PSX_Point mTlvTopLeft = {};
    PSX_Point mTlvBottomRight = {};
    s32 mTimer = 0;
};
