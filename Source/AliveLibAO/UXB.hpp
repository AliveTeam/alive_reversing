#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class UXBState : u16
{
    eDelay_0 = 0,
    eActive_1 = 1,
    eExploding_2 = 2,
    eDeactivated_3 = 3
};

enum class UXBStartState : s16
{
    eOn_0 = 0,
    eOff_1 = 1,
};

struct Path_UXB final : public Path_TLV
{
    s16 field_18_pattern_length;
    s16 field_1A_pattern;
    Scale_short field_1C_scale;
    UXBStartState field_1E_state;
    s32 field_20_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_UXB, 0x24);

class UXB final : public BaseAliveGameObject
{
public:
    EXPORT UXB* ctor_488C80(Path_UXB* pTlv, s32 tlvInfo);

    void InitBlinkAnim();

    EXPORT BaseGameObject* dtor_4891B0();

    

    EXPORT BaseGameObject* Vdtor_489C70(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_489BD0();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_489AB0(BaseGameObject* pFrom);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_489A30(BaseGameObject* pFrom);

    virtual void VOnPickUpOrSlapped() override;

    EXPORT void VOnPickUpOrSlapped_4897E0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_489380();

    EXPORT s16 IsColliding_489900();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_4896C0(PrimHeader** ppOt);

    UXBState field_10C_state;
    UXBState field_10E_starting_state;
    s16 field_110_disabled_resources;
    s16 field_112_padding;
    s32 field_114_tlvInfo;
    s32 field_118_next_state_frame;
    Animation field_11C_anim;
    s16 field_1B4_pattern_length;
    s16 field_1B6_pattern_index;
    s16 field_1B8_pattern;
    s16 field_1BA_red_blink_count;
    enum flags_1BC
    {
        eUnused_Bit0 = 0x1,
        eIsRed_Bit1 = 0x2,
    };
    BitField16<flags_1BC> field_1BC_flags;
    s16 field_1BE_padding;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1C0);

} // namespace AO
