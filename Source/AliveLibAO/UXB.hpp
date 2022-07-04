#pragma once

#include "../AliveLibCommon/Function.hpp"
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
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_UXB, 0x24);

class UXB final : public BaseAliveGameObject
{
public:
    UXB(Path_UXB* pTlv, s32 tlvInfo);
    ~UXB();

    void InitBlinkAnim();

    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s16 IsColliding();

    UXBState field_10C_state = UXBState::eDelay_0;
    UXBState field_10E_starting_state = UXBState::eDelay_0;
    s16 field_110_disabled_resources = 0;
    s32 field_114_tlvInfo = 0;
    s32 field_118_next_state_frame = 0;
    Animation field_11C_anim;
    s16 field_1B4_pattern_length = 0;
    s16 field_1B6_pattern_index = 0;
    s16 field_1B8_pattern = 0;
    s16 field_1BA_red_blink_count = 0;
    enum flags_1BC
    {
        eUnused_Bit0 = 0x1,
        eIsRed_Bit1 = 0x2,
    };
    BitField16<flags_1BC> field_1BC_flags = {};
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1C0);

} // namespace AO
