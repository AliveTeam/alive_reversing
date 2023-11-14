#pragma once

#include "BaseGameObject.hpp"

class BackgroundMusic final : public BaseGameObject
{
public:
    BackgroundMusic(s32 musicId);

    virtual void VUpdate() override;

    static void Stop();
    static void Play();

    s16 mMusicId = 0;
};

extern s16 gBackgroundMusicSeqId;
