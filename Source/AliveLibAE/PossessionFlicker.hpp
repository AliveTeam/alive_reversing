#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"

class BaseAliveGameObject;

class PossessionFlicker final : public BaseGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    PossessionFlicker(BaseAliveGameObject* pToApplyFlicker, s32 duration, s32 r, s32 g, s32 b);
    ~PossessionFlicker();

    s32 ObjectId() const;

private:
    s32 field_20_time_to_flicker = 0;

    s16 field_24_r = 0;
    s16 field_26_g = 0;
    s16 field_28_b = 0;

    s16 field_2A_old_r = 0;
    s16 field_2C_old_g = 0;
    s16 field_2E_old_b = 0;

    s32 field_30_obj_id = 0;
};
ALIVE_ASSERT_SIZEOF(PossessionFlicker, 0x34);
