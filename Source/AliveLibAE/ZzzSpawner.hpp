#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/FixedPoint_common.hpp"

namespace relive
{
    struct Path_ZzzSpawner;
}

class ZzzSpawner final : public BaseGameObject
{
public:
    ZzzSpawner(relive::Path_ZzzSpawner* pTlv, const Guid& tlvId);
    ~ZzzSpawner();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    FP mXPos = {};
    FP mYPos = {};
    FP mSpriteScale = {};
    Guid mTlvInfo;
    s16 mSwitchId = 0;
    s32 mTimer = 0;
    s16 mZzzInterval = 0;
};
