#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

namespace AO {

class CheatController : public BaseGameObject
{
public:
    EXPORT CheatController* ctor_40FBF0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VScreenChanged() override;

    EXPORT BaseGameObject* dtor_40FC20();

    EXPORT CheatController* Vdtor_40FCD0(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_40FC40();

    s16 field_10;
    s16 field_12;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x14);

ALIVE_VAR_EXTERN(CheatController*, pCheatController_4FF958);

ALIVE_VAR_EXTERN(s16, sEnableFartGasCheat_507704);
ALIVE_VAR_EXTERN(s16, sVoiceCheat_507708);
ALIVE_VAR_EXTERN(s16, sEnableCheatFMV_50770C);
ALIVE_VAR_EXTERN(s16, sEnableCheatLevelSelect_507710);

}
