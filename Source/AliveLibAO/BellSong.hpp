#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

enum class BellsongTypes : s16
{
    eWhistle = 0,
    eChimes = 1
};

class BellSong final : public BaseGameObject
{
public:
    BellSong(BellsongTypes type, u32 code);

    virtual void VUpdate() override;

    s32 field_10_code_idx = 0;
    s16 field_14_bDone = 0;
    BellsongTypes field_16_type = BellsongTypes::eWhistle;
    s32 field_18_code = 0;
    s16 field_1C_code_len = 0;
};
ALIVE_ASSERT_SIZEOF(BellSong, 0x20);

} // namespace AO
