#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

void CreateGameEnderController();

enum class GameEnderControllerStates : s16
{
    eInit_0 = 0,
    eDetermineEnding_1 = 1,
    eFinish_2 = 2,
    eBadEnding_3 = 3,
    eGoodEnding_4 = 4,
    eAngelicEnding_5 = 5,
    eAngelicEndingCredits_6 = 6,
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
    static void CreateFromSaveState(SerializedObjectData& pBuffer);
    GameEnderController();
    
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VGetSaveState(SerializedObjectData& pSaveBuffer) override;

private:
    s32 mTimer = 0;
    GameEnderControllerStates mState = GameEnderControllerStates::eInit_0;
};

extern s16 gFeeco_Restart_KilledMudCount;
extern s16 gFeecoRestart_SavedMudCount;
