#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

namespace AO {

enum class Scale_int : s32;

struct Path_Honey final : public Path_TLV
{
    s16 switch_id;
    s16 state;
    Scale_int scale;
};

class Honey final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Honey(FP xpos, FP ypos);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    s32 field_D4[4] = {};
    s32 field_E4_tlvInfo = 0;
};
ALIVE_ASSERT_SIZEOF(Honey, 0xE8);

} // namespace AO
