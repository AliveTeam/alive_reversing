#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

enum class GameEnderController_States : s16
{
    eInit_0 = 0,
    eDetermineEnding_1 = 1,
    eFinish_2 = 2,
    eBadEnding_3 = 3,
    ePerfectEnding_4 = 4
};

class GameEnderController final : public ::BaseGameObject
{
public:
    static void CreateGameEnderController();

    GameEnderController();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s32 field_10_timer = 0;
    GameEnderController_States field_14_state = GameEnderController_States::eInit_0;
};



} // namespace AO
