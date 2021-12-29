#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

class BaseAliveGameObject;

class PossessionFlicker final : public BaseGameObject
{
public:
    EXPORT PossessionFlicker* ctor_41A8C0(BaseAliveGameObject* pToApplyFlicker, s32 duration, s16 r, s16 g, s16 b);

    EXPORT BaseGameObject* dtor_41A940();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT PossessionFlicker* vdtor_41AA70(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_41AA40();

    virtual void VUpdate() override;

    EXPORT void VUpdate_41A9B0();

    BaseAliveGameObject* field_10_pObj;
    s32 field_14_time_to_flicker;

    s16 field_18_r;
    s16 field_1A_g;
    s16 field_1C_b;

    s16 field_1E_old_r;
    s16 field_20_old_g;
    s16 field_22_old_b;
};
ALIVE_ASSERT_SIZEOF(PossessionFlicker, 0x24);

} // namespace AO
