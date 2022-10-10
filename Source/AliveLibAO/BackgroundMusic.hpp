#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace AO {

class BackgroundMusic final : public ::BaseGameObject
{
public:
    BackgroundMusic(s16 musicId);

    virtual void VUpdate() override;

    static void Stop();
    static void Play();

    s16 field_10_music_id = 0;
};

extern s16 sBackgroundMusic_seq_id_4CFFF8;

} // namespace AO
