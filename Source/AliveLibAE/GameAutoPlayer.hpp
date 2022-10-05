#pragma once

#include "BaseGameAutoPlayer.hpp"

class Recorder final : public BaseRecorder
{
public:
    void SaveObjectStates() override;
};

class Player final : public BasePlayer
{
public:
    bool ValidateObjectStates() override;
};

class [[nodiscard]] GameAutoPlayer final : public BaseGameAutoPlayer
{
public:
    GameAutoPlayer()
        : BaseGameAutoPlayer(mAERecorder, mAEPlayer)
    {

    }

private:
    u32 ReadInput(u32 padIdx) override;

    Recorder mAERecorder;
    Player mAEPlayer;
};
