#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"

class BaseAliveGameObject;

class InvisibleEffect : public BaseGameObject
{
public:
    enum Flags_4A
    {
        eSemiTrans_Bit1 = 0x1,
        eBlending_Bit2 = 0x2,
        eIsInvisible_Bit3 = 0x4 //TODO @ 100% Remove it - Never actually checked for
    };

    enum class InvisibleState : unsigned __int16
    {
        eSetRenderMode1_0 = 0,
        eSetInvisibile_1 = 1,
        eBecomeInvisible_2 = 2,
        eUnknown_3 = 3,
        eBecomeVisible_4 = 4,
        eClearInvisibility_5 = 5,
        eSetDead_6 = 6
    };

    EXPORT InvisibleEffect* ctor_45F280(BaseAliveGameObject* pTarget);
    EXPORT void dtor_45F410();
    EXPORT void InstantInvisibility_45FA00();
    EXPORT void BecomeVisible_45FA30();
    EXPORT void ClearInvisibility_45FA50();
    EXPORT void BecomeInvisible_45F9E0();
    EXPORT void vUpdate_45F4A0();
    EXPORT BaseGameObject* vdtor_45F3E0(signed int flags);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    InvisibleState field_20_state_or_op;
    __int16 field_22;
    WORD* field_24_pAlloc;
    PSX_RECT field_28;
    WORD* field_30_pPalAlloc;
    PSX_RECT field_34;
    int field_3C;
    int field_40;
    int field_44_objId;
    TPageAbr field_48_old_render_mode;
    char field_49;
    BitField16<Flags_4A> field_4A_flags;
};
ALIVE_ASSERT_SIZEOF(InvisibleEffect, 0x4C);
