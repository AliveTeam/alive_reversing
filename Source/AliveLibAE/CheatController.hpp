#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Input.hpp"

void CheatController_ForceLink();

struct CheatEntry
{
    unsigned int field_0_level_mask;
    int field_4_cheat_code_length;
    const InputCommands *field_8_cheat_code_ary;
    int field_C_success_idx;
    void(*field_10_callback)(void);
};
ALIVE_ASSERT_SIZEOF(CheatEntry, 0x14);

class CheatController : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    EXPORT CheatController* ctor_421BD0();
    EXPORT BaseGameObject* vdtor_421C10(signed int flags);
    EXPORT void dtor_421C40();

    EXPORT void Update_421C70();

public:
    __int16 field_20;
    __int16 field_22;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
};
ALIVE_ASSERT_SIZEOF(CheatController, 0x40);

ALIVE_VAR_EXTERN(CheatController*, pCheatController_5BC120);