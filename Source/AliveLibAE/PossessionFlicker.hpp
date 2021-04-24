#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

class BaseAliveGameObject;

class PossessionFlicker : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT PossessionFlicker* ctor_4319E0(BaseAliveGameObject* pToApplyFlicker, int duration, s16 r, s16 g, s16 b);
    int ObjectId() const;
private:
    EXPORT PossessionFlicker* vdtor_431AD0(signed int flags);
    EXPORT void dtor_431B00();
    EXPORT void vScreenChanged_431C80();
    EXPORT void vUpdate_431BC0();
private:
    int field_20_time_to_flicker;

    s16 field_24_r;
    s16 field_26_g;
    s16 field_28_b;

    s16 field_2A_old_r;
    s16 field_2C_old_g;
    s16 field_2E_old_b;

    int field_30_obj_id;
};
ALIVE_ASSERT_SIZEOF(PossessionFlicker, 0x34);
