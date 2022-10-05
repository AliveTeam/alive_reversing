#pragma once

#include "BaseGameAutoPlayer.hpp"

namespace AO {
class BaseGameObject;

class Recorder final : public BaseRecorder
{
public:
    void SaveObjectStates() override;
};

class Player final : public BasePlayer
{
public:
    bool ValidateObjectStates() override;

private:
    bool ValidateBaseAliveGameObject(BaseGameObject* pObj);
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

} // namespace AO
