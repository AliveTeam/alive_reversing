#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"

class BackgroundMusic final : public BaseGameObject
{
public:
    EXPORT static void CC Stop_4CB000();
    EXPORT static void CC Play_4CB030();

    explicit BackgroundMusic(s32 musicId);

    virtual void VUpdate() override;

private:
    s16 field_20_music_id;
    // s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x24);
