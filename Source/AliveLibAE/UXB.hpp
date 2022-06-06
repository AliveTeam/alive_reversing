#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"
#include "Sfx.hpp"

enum class UXBState : u16
{
    eDelay_0 = 0,
    eActive_1 = 1,
    eExploding_2 = 2,
    eDeactivated_3 = 3
};

struct Path_UXB final : public Path_TLV
{
    enum class StartState : s16
    {
        eOn_0 = 0,
        eOff_1 = 1,
    };
    s16 field_10_pattern_length;
    s16 field_12_pattern;
    Scale_short field_14_scale;
    StartState field_16_start_state;
    s32 field_18_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_UXB, 0x1c);

struct SaveState_UXB final
{
    AETypes field_0_id;
    s16 field_2_padding;
    TlvItemInfoUnion field_4_tlv;
    u32 field_8_next_state_frame;
    UXBState field_C_state;
    UXBState field_E_starting_state;
    u16 field_10_disabled_resources;
    u16 field_12_pattern_index;
    u16 field_14_red_blink_count;
    u16 field_16_is_red;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SaveState_UXB, 24);

enum UXB_Flags_1C8
{
    eUnused_Bit0 = 0x1,
    eIsRed_Bit1 = 0x2,
};

class UXB final : public BaseAliveGameObject
{
public:
    UXB(Path_UXB* params, TlvItemInfoUnion itemInfo);
    ~UXB();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    static s32 CreateFromSaveState(const u8*);

private:
    void InitBlinkAnim(Animation* pAnimation);
    void PlaySFX(SoundEffect sfxIdx);
    s32 IsColliding();

private:
    UXBState field_118_state;
    UXBState field_11A_starting_state;
    u16 field_11C_disabled_resources;
    u8 field_11E_padding[2];
    TlvItemInfoUnion field_120_tlv;
    u32 field_124_next_state_frame;
    Animation field_128_animation;
    u16 field_1C0_pattern_length;
    u16 field_1C2_pattern_index;
    u16 field_1C4_pattern;
    u16 field_1C6_red_blink_count;
    BitField16<UXB_Flags_1C8> field_1C8_flags;
    s16 field_1CA_padding;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1CC);
