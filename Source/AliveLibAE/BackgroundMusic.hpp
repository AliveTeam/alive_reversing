#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

class BackgroundMusic final : public BaseGameObject
{
public:
    static void Stop();
    static void Play();

    explicit BackgroundMusic(s32 musicId);

    virtual void VUpdate() override;

private:
    s16 mMusicId = 0;
};
