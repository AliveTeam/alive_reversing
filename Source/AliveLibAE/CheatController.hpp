#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Input.hpp"

struct CheatEntry final
{
    u32 field_0_level_mask;
    s32 field_4_cheat_code_length;
    const InputCommands::Enum* field_8_cheat_code_ary;
    s32 field_C_success_idx;
    void (*field_10_callback)(void);
};
ALIVE_ASSERT_SIZEOF(CheatEntry, 0x14);

class CheatController final : public BaseGameObject
{
public:
    CheatController();
    ~CheatController();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

public:
    s16 field_20 = 0;
    s16 field_22 = 0;
    s32 field_24 = 0;
    s32 field_28 = 0;
    s32 field_2C = 0;
    s32 field_30 = 0;
    s32 field_34 = 0;
    s32 field_38 = 0;
    s32 field_3C = 0;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x40);

ALIVE_VAR_EXTERN(CheatController*, pCheatController_5BC120);
