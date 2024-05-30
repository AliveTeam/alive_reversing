#pragma once

#include "BaseGameObject.hpp"

class BaseAliveGameObject;

class PossessionFlicker final : public BaseGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    PossessionFlicker(BaseAliveGameObject* pToApplyFlicker, s32 duration, s32 r, s32 g, s32 b);
    ~PossessionFlicker();

    const Guid& ObjectId() const;
private:
    Guid mTargetObjId;
    s32 mTimeToFlicker = 0;

    s16 mNewRed = 0;
    s16 mNewGreen = 0;
    s16 mNewBlue = 0;

    s16 mOldRed = 0;
    s16 mOldGreen = 0;
    s16 mOldBlue = 0;
};
