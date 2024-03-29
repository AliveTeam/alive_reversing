#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "Primitives.hpp"

class BaseAliveGameObject;

class InvisibleEffect final : public BaseGameObject
{
public:
    enum Flags_4A
    {
        eSemiTrans_Bit1 = 0x1,
        eBlending_Bit2 = 0x2,
        eIsInvisible_Bit3 = 0x4 //TODO @ 100% Remove it - Never actually checked for
    };

    enum class InvisibleState : u16
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
    EXPORT BaseGameObject* vdtor_45F3E0(s32 flags);

    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    InvisibleState field_20_state_or_op;
    s16 field_22;
    u16* field_24_pPal1;
    PSX_RECT field_28_pal_rect1;
    u16* field_30_pPal2;
    PSX_RECT field_34_pal_rect2;
    s32 field_3C;
    s32 field_40;
    s32 field_44_objId;
    TPageAbr field_48_old_render_mode;
    s8 field_49;
    BitField16<Flags_4A> field_4A_flags;
};
ALIVE_ASSERT_SIZEOF(InvisibleEffect, 0x4C);
