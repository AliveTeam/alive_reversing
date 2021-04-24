#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Input.hpp"

void CheatController_ForceLink();

struct CheatEntry
{
    u32 field_0_level_mask;
    s32 field_4_cheat_code_length;
    const InputCommands::Enum *field_8_cheat_code_ary;
    s32 field_C_success_idx;
    void(*field_10_callback)(void);
};
ALIVE_ASSERT_SIZEOF(CheatEntry, 0x14);

class CheatController : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    EXPORT CheatController* ctor_421BD0();
    EXPORT BaseGameObject* vdtor_421C10(s32 flags);
    EXPORT void dtor_421C40();

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
