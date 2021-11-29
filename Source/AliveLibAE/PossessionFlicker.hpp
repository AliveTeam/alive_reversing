#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"

class BaseAliveGameObject;

class PossessionFlicker final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT PossessionFlicker* ctor_4319E0(BaseAliveGameObject* pToApplyFlicker, s32 duration, s16 r, s16 g, s16 b);
    s32 ObjectId() const;

private:
    EXPORT PossessionFlicker* vdtor_431AD0(s32 flags);
    EXPORT void dtor_431B00();
    EXPORT void vScreenChanged_431C80();
    EXPORT void vUpdate_431BC0();

private:
    s32 field_20_time_to_flicker;

    s16 field_24_r;
    s16 field_26_g;
    s16 field_28_b;

    s16 field_2A_old_r;
    s16 field_2C_old_g;
    s16 field_2E_old_b;

    s32 field_30_obj_id;
};
ALIVE_ASSERT_SIZEOF(PossessionFlicker, 0x34);
