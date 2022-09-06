#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class Scale_int : s32;

namespace AO {

struct Path_Honey final : public Path_TLV
{
    s16 mSwitchId;
    s16 mState;
    Scale_int mScale;
};
// TODO: size

class Honey final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Honey(FP xpos, FP ypos);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    Guid mTlvInfo;
};
ALIVE_ASSERT_SIZEOF(Honey, 0xE8);

} // namespace AO
