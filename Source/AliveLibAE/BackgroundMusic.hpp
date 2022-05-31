#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"

class BackgroundMusic final : public BaseGameObject
{
public:
    static void Stop_4CB000();
    static void Play_4CB030();

    explicit BackgroundMusic(s32 musicId);

    virtual void VUpdate() override;

private:
    s16 field_20_music_id;
    // s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x24);
