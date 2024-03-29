#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

enum class BellsongTypes : s16
{
    eWhistle = 0,
    eChimes = 1
};

class BellSong final : public BaseGameObject
{
public:
    EXPORT BellSong* ctor_4760B0(BellsongTypes type, u32 code);

    virtual void VUpdate() override;

    EXPORT void VUpdate_476130();

    virtual BaseGameObject* VDestructor(s32 flags) override;


    s32 field_10_code_idx;
    s16 field_14_bDone;
    BellsongTypes field_16_type;
    s32 field_18_code;
    s16 field_1C_code_len;
    s16 field_1E_padding;
};
ALIVE_ASSERT_SIZEOF(BellSong, 0x20);

} // namespace AO
