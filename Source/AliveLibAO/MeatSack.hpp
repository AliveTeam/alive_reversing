#pragma once

#include "BaseAliveGameObject.hpp"

namespace relive
{
    struct Path_MeatSack;
}

namespace AO
{

class MeatSack final : public BaseAliveGameObject
{

public:
    MeatSack(relive::Path_MeatSack* pTlv, const Guid& tlvId);
    ~MeatSack();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    Guid mTlvId;
    bool mHasBeenHit = false;
    s16 mMeatAmount = 0;
    bool mPlayWobbleSound = false;
    FP mTlvVelX = {};
    FP mTlvVelY = {};
};

} // namespace AO