#pragma once

#include "../relive_lib/BaseGameObject.hpp"

class BackgroundMusic final : public BaseGameObject
{
public:
    static void Stop();
    static void Play();

    explicit BackgroundMusic(s32 musicId);

    virtual void VUpdate() override;

private:
    s16 field_20_music_id = 0;
};
