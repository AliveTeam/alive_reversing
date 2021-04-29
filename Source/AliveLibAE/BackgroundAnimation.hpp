#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Animation.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Layer.hpp"

struct Path_BackgroundAnimation final : public Path_TLV
{
    u16 field_10_res_id;
    Choice_short field_12_is_semi_trans;
    TPageAbr field_14_semi_trans_mode;
    // pad
    s16 field_16_sound_effect;
    s16 field_18_id;
    Layer field_1A_layer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_BackgroundAnimation, 0x1C);

class BackgroundAnimation : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT BackgroundAnimation* ctor_40D270(Path_BackgroundAnimation* pPathParams, TlvItemInfoUnion tlvInfo);
    EXPORT void vUpdate_40D450();
    EXPORT void vScreenChanged_40D550();
    EXPORT void dtor_40D4C0();
    EXPORT BaseGameObject* vdtor_40D420(s32 flags);

private:
    AnimHeader** field_F4_res;
    TlvItemInfoUnion field_F8_tlvInfo;
    FP field_FC_animXPos;
    FP field_100_animYPos;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
