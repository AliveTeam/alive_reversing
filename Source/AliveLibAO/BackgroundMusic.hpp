#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace AO {

class BackgroundMusic final : public ::BaseGameObject
{
public:
    BackgroundMusic(s16 musicId);

    virtual void VUpdate() override;

    static void Stop();
    static void Play();

    s16 mMusicId = 0;
};

extern s16 gBackgroundMusicSeqId;

} // namespace AO
