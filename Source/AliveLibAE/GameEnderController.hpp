#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

void CreateGameEnderController_43B7A0();

enum class GameEnderControllerStates : s16
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

struct GameEnderControllerSaveState final
{
    ReliveTypes mType;
    Guid mObjId;
    s32 mTimer;
    GameEnderControllerStates mState;
};

class GameEnderController final : public BaseGameObject
{
public:
    static s32 CreateFromSaveState(const u8* pBuffer);
    GameEnderController();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;

private:
    s32 mTimer = 0;
    GameEnderControllerStates mState = GameEnderControllerStates::eInit_0;
};

extern s16 gFeeco_Restart_KilledMudCount;
extern s16 gFeecoRestart_SavedMudCount;
