#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Animation.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Layer.hpp"

struct BackgroundAnimation_TLV : public Path_TLV
{
    unsigned __int16 field_10_res_id;
    __int16 field_12_is_semi_trans;
    TPageAbr field_14_semi_trans_mode;
    // pad
    __int16 field_16_sound_effect;
    __int16 field_18_id;
    Layer field_1A_layer;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(BackgroundAnimation_TLV, 0x1C);

class BackgroundAnimation : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT BackgroundAnimation* ctor_40D270(BackgroundAnimation_TLV* pPathParams, TlvItemInfoUnion tlvInfo);
    EXPORT void vUpdate_40D450();
    EXPORT void vScreenChanged_40D550();
    EXPORT void dtor_40D4C0();
    EXPORT BaseGameObject* vdtor_40D420(signed int flags);
private:
    AnimHeader** field_F4_res;
    TlvItemInfoUnion field_F8_tlvInfo;
    FP field_FC_animXPos;
    FP field_100_animYPos;
};
ALIVE_ASSERT_SIZEOF(BackgroundAnimation, 0x104);
