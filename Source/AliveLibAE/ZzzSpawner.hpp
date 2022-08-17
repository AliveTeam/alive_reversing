#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Path.hpp"
#include "../AliveLibCommon/Function.hpp"

struct Path_ZzzSpawner final : public Path_TLV
{
    Scale_short mScale;
    s16 mSwitchId;
    s16 mZzzInterval;
    s16 field_16_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_ZzzSpawner, 0x18);

class ZzzSpawner final : public BaseGameObject
{
public:
    ZzzSpawner(Path_ZzzSpawner* pTlv, s32 tlvInfo);
    ~ZzzSpawner();
    
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    FP mXPos = {};
    FP mYPos = {};
    FP mSpriteScale = {};
    s32 mTlvInfo = 0;
    s16 mSwitchId = 0;
    s32 mTimer = 0;
    s16 mZzzInterval = 0;
};
ALIVE_ASSERT_SIZEOF(ZzzSpawner, 0x3C);
