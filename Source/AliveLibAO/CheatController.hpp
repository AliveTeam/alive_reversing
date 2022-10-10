#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

namespace AO {

class CheatController final : public ::BaseGameObject
{
public:
    CheatController();
    ~CheatController();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s16 field_10 = 0;
    s16 field_12 = 0;
};

extern CheatController* pCheatController_4FF958;

extern s16 sEnableFartGasCheat_507704;
extern s16 sVoiceCheat_507708;
extern s16 sEnableCheatFMV_50770C;
extern s16 sEnableCheatLevelSelect_507710;

} // namespace AO
