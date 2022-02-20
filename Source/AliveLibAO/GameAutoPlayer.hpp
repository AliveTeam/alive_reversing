#pragma once

#include "BaseGameAutoPlayer.hpp"

namespace AO {

class Recorder final : public BaseRecorder
{
public:
    void SaveObjectStates() override;
};

class Player final : public BasePlayer
{
public:
    void ValidateObjectStates() override;
};

class [[nodiscard]] GameAutoPlayer final : public BaseGameAutoPlayer
{
public:
    GameAutoPlayer()
        : BaseGameAutoPlayer(mAORecorder, mAOPlayer)
    {

    }

private:
    u32 ReadInput(u32 padIdx) override;

    Recorder mAORecorder;
    Player mAOPlayer;
};

extern GameAutoPlayer gGameAutoPlayer;

} // namespace AO
