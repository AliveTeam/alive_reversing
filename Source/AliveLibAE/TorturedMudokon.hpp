#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class TorturedMudokonState : s16
{
    eBeingTortured_0 = 0,
    eKilled_1 = 1,
    eReleased_2 = 2
};

struct Path_TorturedMudokon final : public Path_TLV
{
    s16 field_10_kill_switch_id;
    s16 field_12_release_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TorturedMudokon, 0x14);

class TorturedMudokon final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    TorturedMudokon(Path_TorturedMudokon* pTlv, s32 tlvInfo);
    ~TorturedMudokon();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    

private:
    void SetupTearsAnimation_47BE60(Animation* pAnim);
    void SetupZapAnimation_47BEF0(Animation* pAnim);
    void vScreenChanged_47C440();
    void vRender_47C460(PrimHeader** ppOt);
    void vUpdate_47BF80();

private:
    Animation field_F4_tears_animation;
    Animation field_18C_zap_animation;
    u8** field_224_ppRes;
    s32 field_228_padding;
    s32 field_22C_padding;
    s32 field_230_tlvInfo;
    s32 field_234_flash_colour_timer;
    s16 field_238_flash_colour_counter;
    s16 field_23A_kill_switch_id;
    s16 field_23C_release_switch_id;
    TorturedMudokonState field_23E_state;
    s16 field_240_pain_sound_pitch;
    s16 field_242_padding;
};
ALIVE_ASSERT_SIZEOF(TorturedMudokon, 0x244);
