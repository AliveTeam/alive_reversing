#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

namespace AO {

class CheatController final : public BaseGameObject
{
public:
    CheatController();
    ~CheatController();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s16 field_10;
    s16 field_12;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x14);

ALIVE_VAR_EXTERN(CheatController*, pCheatController_4FF958);

ALIVE_VAR_EXTERN(s16, sEnableFartGasCheat_507704);
ALIVE_VAR_EXTERN(s16, sVoiceCheat_507708);
ALIVE_VAR_EXTERN(s16, sEnableCheatFMV_50770C);
ALIVE_VAR_EXTERN(s16, sEnableCheatLevelSelect_507710);

} // namespace AO
