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

    s32 mCodeIdx = 0;
    bool mDone = false;
    BellsongTypes mType = BellsongTypes::eWhistle;
    s32 mCode = 0;
    s16 mCodeLength = 0;
};
ALIVE_ASSERT_SIZEOF(BellSong, 0x20);

} // namespace AO
