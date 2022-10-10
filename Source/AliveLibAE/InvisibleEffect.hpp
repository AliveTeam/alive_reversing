#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Psx.hpp"
#include "../relive_lib/Primitives.hpp"

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

    explicit InvisibleEffect(BaseAliveGameObject* pTarget);
    ~InvisibleEffect();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void InstantInvisibility();
    void BecomeVisible();
    void ClearInvisibility();
    void BecomeInvisible();

private:
    InvisibleState field_20_state_or_op = InvisibleState::eSetRenderMode1_0;

    //u16* field_24_pPal1 = nullptr;
    //PSX_RECT field_28_pal_rect1 = {};
    //u16* field_30_pPal2 = nullptr;
    //PSX_RECT field_34_pal_rect2 = {};
    PalResource mPal1;
    PalResource mPal2;

    Guid field_44_objId;
    TPageAbr field_48_old_render_mode = TPageAbr::eBlend_0;
    BitField16<Flags_4A> field_4A_flags = {};
};
