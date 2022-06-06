#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"

class BackgroundMusic final : public BaseGameObject
{
public:
    static void Stop();
    static void Play();

    explicit BackgroundMusic(s32 musicId);

    virtual void VUpdate() override;

private:
    s16 field_20_music_id;
    // s16 field_22_padding;
};
ALIVE_ASSERT_SIZEOF(BackgroundMusic, 0x24);
