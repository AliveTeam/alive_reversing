#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FixedPoint.hpp"

namespace AO {

struct Path_BackgroundAnimation : public Path_TLV
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
    EXPORT BackgroundAnimation* ctor_405A90(Path_BackgroundAnimation* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_405CB0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BackgroundAnimation* Vdtor_405D70(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_405D30();

    virtual void VStopAudio() override;

    EXPORT void VStopAudio_405D40();

    virtual void VUpdate() override;

    EXPORT void VUpdate_405C30();

    int field_D4_padding;
    int field_D8_padding;
    int field_DC_padding;
    int field_E0_padding;
    BYTE** field_E4_res;
    s16 field_E8_xpos;
    s16 field_EA_ypos;
    s16 field_EC_w;
    s16 field_EE_h;
    int field_F0_tlvInfo;
    int field_F4_padding;
    FP field_F8_animXPos;
    FP field_FC_animYPos;
    s16 field_100_sound_effect;
    s16 field_102_padding;
    int field_104_sound_channels_mask;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x108);

}

