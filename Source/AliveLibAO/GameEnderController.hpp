#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

void GameEnderController_ForceLink();

namespace AO {

enum class GameEnderController_States : s16
{
    eInit_0 = 0,
    eDetermineEnding_1 = 1,
    eFinish_2 = 2,
    eBadEnding_3 = 3,
    ePerfectEnding_4 = 4
};

class GameEnderController final : public BaseGameObject
{
public:
    static EXPORT void CC CreateGameEnderController_41C7D0();

    EXPORT BaseGameObject* dtor_41C850();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT GameEnderController* Vdtor_41CD00(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41CCE0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41C860();

    s32 field_10_timer;
    GameEnderController_States field_14_state;
    s16 field_16_padding;
};
ALIVE_ASSERT_SIZEOF(GameEnderController, 0x18);



} // namespace AO
