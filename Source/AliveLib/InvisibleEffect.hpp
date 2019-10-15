#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

class BaseAliveGameObject;

class InvisibleEffect : public BaseGameObject
{
public:
    EXPORT InvisibleEffect* ctor_45F280(BaseAliveGameObject* pTarget);
    EXPORT void dtor_45F410();
    EXPORT void sub_45FA00();
    EXPORT void sub_45FA30();
    EXPORT void sub_45FA50();
    EXPORT void sub_45F9E0();
    EXPORT void vUpdate_45F4A0();
    EXPORT BaseGameObject* vdtor_45F3E0(signed int flags);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    unsigned __int16 field_20_state_or_op;
    __int16 field_22;
    WORD* field_24_pAlloc;
    PSX_RECT field_28;
    WORD* field_30_pPalAlloc;
    PSX_RECT field_34;
    int field_3C;
    int field_40;
    int field_44_objId;
    char field_48_old_render_mode;
    char field_49;
    __int16 field_4A_flags;
};
ALIVE_ASSERT_SIZEOF(InvisibleEffect, 0x4C);
