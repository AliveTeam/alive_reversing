#pragma once

#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

class RockSack final : public BaseAliveGameObject
{
public:
    RockSack(relive::Path_RockSack* pTlv, const Guid& tlvId);
    ~RockSack();

    void LoadAnimations();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

private:
    Guid mTlvId;
    bool mHasBeenHit = false;
    s16 mRockAmount = 0;
    bool mPlayWobbleSound = false;
    bool mForceWobbleSound = false;
    FP mTlvVelX = {};
    FP mTlvVelY = {};
};
}
