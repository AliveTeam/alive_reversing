#pragma once

#include "BaseGameObject.hpp"
#include "../data_conversion/AnimationConverter.hpp"

class BaseAliveGameObject;
class PalleteOverwriter;

class Electrocute final : public BaseGameObject
{
public:
    Electrocute(BaseAliveGameObject* pTargetObj, bool bExtraOverwriter, bool bKillTarget);
    ~Electrocute();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void Stop();
    bool IsLastOverwriterFinished();

public:
    Guid mTargetObjId;
    s16 mTargetRed = 0;
    s16 mTargetGreen = 0;
    s16 mTargetBlue = 0;

private:
    bool mExtraOverwriter = false;
    bool mKillTarget = false;
    s16 mPalOverwritersCount = 0;
    PalleteOverwriter* mPalOverwriters[3] = {};
    AnimationPal mPalData;
    enum class States
    {
        eSetNewColour,
        eAlphaFadeout,
        eHandleDamage,
        eKillElectrocute
    };
    States mState = States::eSetNewColour;
};
