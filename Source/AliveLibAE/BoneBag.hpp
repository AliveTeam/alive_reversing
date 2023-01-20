#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_BoneBag;
}

class BoneBag final : public BaseAliveGameObject
{
public:
    BoneBag(relive::Path_BoneBag* pTlv, const Guid& tlvId);
    ~BoneBag();

    void LoadAnimations();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    Guid mTlvInfo;
    bool mHasBeenHit = false;
    bool mPlayWobbleSound = true;
    bool mForcePlayWobbleSound = true;
    FP mTlvVelX = {};
    FP mTlvVelY = {};
    s16 mBoneAmount = 0;
};
