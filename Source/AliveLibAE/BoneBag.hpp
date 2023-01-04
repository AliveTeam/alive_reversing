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
    s16 mBoneAmount = 0;
    bool mPlayWobbleSound = false;
    bool mForcePlayWobbleSound = false;
    FP mTlvVelX = {};
    FP mTlvVelY = {};
};
