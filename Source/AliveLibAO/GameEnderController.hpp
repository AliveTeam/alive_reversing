#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

void GameEnderController_ForceLink();

namespace AO {

class GameEnderController : public BaseGameObject
{
public:
    static EXPORT void CC CreateGameEnderController_41C7D0();

    EXPORT BaseGameObject* dtor_41C850();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT GameEnderController* Vdtor_41CD00(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41CCE0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41C860();

    int field_10_timer;
    __int16 field_14_state;
    __int16 field_16;
};
ALIVE_ASSERT_SIZEOF(GameEnderController, 0x18);



}

