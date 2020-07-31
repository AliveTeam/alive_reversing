#include "stdafx_ao.h"
#include "Function.hpp"
#include "GameEnderController.hpp"
#include "stdlib.hpp"
#include "Map.hpp"

void GameEnderController_ForceLink() {}

START_NS_AO

void CC GameEnderController::CreateGameEnderController_41C7D0()
{
    auto pGameEnderController = ao_new<GameEnderController>();
    if (pGameEnderController)
    {
        pGameEnderController->ctor_487E10(1);
        SetVTable(pGameEnderController, 0x4BAEF0);
        pGameEnderController->field_4_typeId = Types::eGameEnderController_37;
        pGameEnderController->field_14_state = 0;
    }
}

BaseGameObject* GameEnderController::dtor_41C850()
{
    SetVTable(this, 0x4BAEF0);
    return dtor_487DF0();
}

BaseGameObject* GameEnderController::VDestructor(signed int flags)
{
    return Vdtor_41CD00(flags);
}

GameEnderController* GameEnderController::Vdtor_41CD00(signed int flags)
{
    dtor_41C850();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void GameEnderController::VScreenChanged()
{
    VScreenChanged_41CCE0();
}

void GameEnderController::VScreenChanged_41CCE0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

END_NS_AO
