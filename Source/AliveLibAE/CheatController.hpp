#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Input.hpp"

void CheatController_ForceLink();

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
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    CheatController();
    ~CheatController();

    EXPORT void Update_421C70();

public:
    s16 field_20;
    s16 field_22;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    s32 field_34;
    s32 field_38;
    s32 field_3C;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x40);

ALIVE_VAR_EXTERN(CheatController*, pCheatController_5BC120);
