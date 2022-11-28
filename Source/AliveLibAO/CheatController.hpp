#pragma once

#include "../relive_lib/BaseGameObject.hpp"

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

extern CheatController* gCheatController;

extern s16 gEnableFartGasCheat;
extern s16 gVoiceCheat;
extern s16 gEnableCheatFMV;
extern s16 gEnableCheatLevelSelect;

} // namespace AO
