#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/data_conversion/AnimationConverter.hpp"

class PalleteOverwriter final : public BaseGameObject
{
public:
    PalleteOverwriter(AnimationPal& pal, s16 colour);
    ~PalleteOverwriter();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual void VRender(OrderingTable& /*ot*/) override;

private:
    AnimationPal mPal;
    s16 field_B8_palBuffer[8] = {};
    s16 field_C8_pal_x_index = 0;
    s16 field_CA_pal_w = 0;
    s16 field_CC_bFirstUpdate = 0;

public:
    s16 field_CE_bDone = 0;
};