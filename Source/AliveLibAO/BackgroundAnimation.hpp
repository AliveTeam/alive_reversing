#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 field_18_animation_id;
    s16 field_1A_is_semi_trans;
    TPageAbr field_1C_semi_trans_mode;
    // pad
    s16 field_1E_sound_effect;
};
ALIVE_ASSERT_SIZEOF(Path_BackgroundAnimation, 0x20);

class BackgroundAnimation : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BackgroundAnimation* ctor_405A90(Path_BackgroundAnimation* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_405CB0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BackgroundAnimation* Vdtor_405D70(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_405D30();

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_405D40();

    virtual void VUpdate() override;

    EXPORT void VUpdate_405C30();

    s32 field_D4_padding;
    s32 field_D8_padding;
    s32 field_DC_padding;
    s32 field_E0_padding;
    u8** field_E4_res;
    s16 field_E8_xpos;
    s16 field_EA_ypos;
    s16 field_EC_w;
    s16 field_EE_h;
    s32 field_F0_tlvInfo;
    s32 field_F4_padding;
    FP field_F8_animXPos;
    FP field_FC_animYPos;
    s16 field_100_sound_effect;
    s16 field_102_padding;
    s32 field_104_sound_channels_mask;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x108);

} // namespace AO
