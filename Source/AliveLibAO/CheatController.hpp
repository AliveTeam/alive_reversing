#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace AO {

class CheatController final : public ::BaseGameObject
{
public:
    CheatController();
    ~CheatController();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
};

extern CheatController* gCheatController;

extern bool gEnableFartGasCheat;
extern bool gVoiceCheat;
extern bool gEnableCheatFMV;
extern bool gEnableCheatLevelSelect;

} // namespace AO
