#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

EXPORT void CC CreateGameEnderController_43B7A0();

enum class GameEnderController_States : __int16
{
    eState0 = 0,
    eState1 = 1,
    eState2 = 2,
    eState3 = 3,
    eState4 = 4,
    eState5 = 5,
    eState6 = 6,
    eState7 = 7,
    eState8 = 8,
    eState9 = 9,
};

struct GameEnderController_State
{
    Types field_0_type;
    __int16 field_2_pad;
    int field_4_obj_id;
    int field_8_timer;
    GameEnderController_States field_C_state;
    __int16 field_E_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GameEnderController_State, 0x10);

class GameEnderController : public BaseGameObject
{
public:
    EXPORT static int CC CreateFromSaveState_43BD10(const BYTE* pBuffer);
    EXPORT GameEnderController* ctor_43B840();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
private:
    EXPORT GameEnderController* vdtor_43B8D0(signed int flags);
    EXPORT void vScreenChanged_43BC80();
    EXPORT int vGetSaveState_43BCD0(GameEnderController_State* pState);
    EXPORT void vUpdate_43B920();

private:
    int field_20_timer;
    GameEnderController_States field_24_state;
    __int16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(GameEnderController, 0x28);

ALIVE_VAR_EXTERN(short, sFeeco_Restart_KilledMudCount_5C1BC6);
ALIVE_VAR_EXTERN(short, sFeecoRestart_SavedMudCount_5C1BC8);
