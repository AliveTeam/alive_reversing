#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "Map.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Layer.hpp"
#include "Path.hpp"

namespace AO {

class ZzzSpawner final : public ::BaseGameObject
{
public:
    ZzzSpawner(relive::Path_ZzzSpawner* pTlv, const Guid& tlvId);
    ~ZzzSpawner();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    FP mXPos = {};
    FP mYPos = {};
    FP mSpriteScale = {};
    Guid mTlvInfo;
    s16 mSwitchId = 0;
    s32 mTimer = 0;
    s16 mZzzInterval = 0;
};
ALIVE_ASSERT_SIZEOF(ZzzSpawner, 0x2C);

} // namespace AO
