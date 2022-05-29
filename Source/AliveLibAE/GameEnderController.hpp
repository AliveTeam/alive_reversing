#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

EXPORT void CC CreateGameEnderController_43B7A0();

enum class GameEnderController_States : s16
{
    eInit_0 = 0,
    eDetermineEnding_1 = 1,
    eFinish_2 = 2,
    eBadEnding_3 = 3,
    eGoodEnding_4 = 4,
    eAngelicEnding_5 = 5,
    eAngelicEndingCredits_6 = 6,
    ePadding_7 = 7,
    ePadding_8 = 8,
    ePadding_9 = 9,
};

struct GameEnderController_State final
{
    AETypes field_0_type;
    s16 field_2_padding;
    s32 field_4_obj_id;
    s32 field_8_timer;
    GameEnderController_States field_C_state;
    s16 field_E_padding;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(GameEnderController_State, 0x10);

class GameEnderController final : public BaseGameObject
{
public:
    EXPORT static s32 CC CreateFromSaveState_43BD10(const u8* pBuffer);
    GameEnderController();

    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    EXPORT void vScreenChanged_43BC80();
    EXPORT s32 vGetSaveState_43BCD0(GameEnderController_State* pState);
    EXPORT void vUpdate_43B920();

private:
    s32 field_20_timer;
    GameEnderController_States field_24_state;
    s16 field_26_padding;
};
ALIVE_ASSERT_SIZEOF(GameEnderController, 0x28);

ALIVE_VAR_EXTERN(s16, sFeeco_Restart_KilledMudCount_5C1BC6);
ALIVE_VAR_EXTERN(s16, sFeecoRestart_SavedMudCount_5C1BC8);
