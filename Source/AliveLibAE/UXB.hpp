#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

void UXB_ForceLink();

enum class UXBState : WORD
{
    eDelay_0 = 0,
    eActive_1 = 1,
    eExploding_2 = 2,
    eDeactivated_3 = 3
};

struct Path_UXB : public Path_TLV
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
    int field_18_disabled_resources;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_UXB, 0x1c);

struct SaveState_UXB
{
    AETypes field_0_id;
    s16 field_2_padding;
    TlvItemInfoUnion field_4_tlv;
    DWORD field_8_next_state_frame;
    UXBState field_C_state;
    UXBState field_E_starting_state;
    WORD field_10_disabled_resources;
    WORD field_12_pattern_index;
    WORD field_14_red_blink_count;
    WORD field_16_is_red;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SaveState_UXB, 24);

enum UXB_Flags_1C8
{
    eUnused_Bit0 = 0x1,
    eIsRed_Bit1 = 0x2,
};

class UXB : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual void VOnPickUpOrSlapped() override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    EXPORT UXB* ctor_4DE9A0(Path_UXB* params, TlvItemInfoUnion itemInfo);
    EXPORT static int CC CreateFromSaveState_4DFAE0(const BYTE*);
private:
    EXPORT void vOnPickUpOrSlapped_4DF540();
    EXPORT void vOnThrowableHit_4DF7B0(BaseGameObject* pFrom);
    EXPORT s16 vTakeDamage_4DF850(BaseGameObject* pFrom);
    EXPORT void dtor_4DEF60();
    EXPORT BaseGameObject* vdtor_4DEEA0(signed int flags);
    EXPORT void Update_4DF030();
    EXPORT void Render_4DF3D0(PrimHeader** ppOt);
    EXPORT void ScreenChanged_4DF9C0();
    EXPORT int GetSaveState_4DFD40(BYTE* pSaveBuffer);
    EXPORT void InitBlinkAnim_4DEED0(Animation* pAnimation);
    EXPORT void PlaySFX_4DE930(u8 sfxIdx);
    EXPORT signed int IsColliding_4DF630();
private:
    UXBState field_118_state;
    UXBState field_11A_starting_state;
    WORD field_11C_disabled_resources;
    BYTE field_11E_padding[2];
    TlvItemInfoUnion field_120_tlv;
    DWORD field_124_next_state_frame;
    Animation field_128_animation;
    WORD field_1C0_pattern_length;
    WORD field_1C2_pattern_index;
    WORD field_1C4_pattern;
    WORD field_1C6_red_blink_count;
    BitField16<UXB_Flags_1C8> field_1C8_flags;
    s16 field_1CA_padding;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1CC);
