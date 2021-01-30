#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class TorturedMudokonState : __int16
{
    eBeingTortured_0 = 0,
    eKilled_1 = 1,
    eReleased_2 = 2
};

struct Path_TorturedMudokon : public Path_TLV
{
    __int16 field_10_speed_id;
    __int16 field_12_release_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TorturedMudokon, 0x14);

class TorturedMudokon : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT TorturedMudokon* ctor_47BC60(Path_TorturedMudokon* pTlv, int tlvInfo);
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
private:
    EXPORT void SetupTearsAnimation_47BE60(Animation* pAnim);
    EXPORT void SetupZapAnimation_47BEF0(Animation* pAnim);
    EXPORT void vScreenChanged_47C440();
    EXPORT void vRender_47C460(PrimHeader** ppOt);
    EXPORT void dtor_47C380();
    EXPORT TorturedMudokon* vdtor_47BE30(signed int flags);
    EXPORT void vUpdate_47BF80();
private:
    Animation field_F4_tears_animation;
    Animation field_18C_zap_animation;
    BYTE** field_224_ppRes;
    int field_228_padding;
    int field_22C_padding;
    int field_230_tlvInfo;
    int field_234_flash_colour_timer;
    __int16 field_238_flash_colour_counter;
    __int16 field_23A_speed_id;
    __int16 field_23C_release_id;
    TorturedMudokonState field_23E_state;
    __int16 field_240_pain_sound_pitch;
    __int16 field_242_padding;
};
ALIVE_ASSERT_SIZEOF(TorturedMudokon, 0x244);
