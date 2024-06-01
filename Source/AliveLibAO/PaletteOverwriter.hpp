#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"

namespace AO {

class PalleteOverwriter final : public ::BaseGameObject
{
public:
    PalleteOverwriter(AnimationPal& /*pal*/, s16 colour);
    ~PalleteOverwriter();

    virtual void VScreenChanged() override;
    virtual void VRender(OrderingTable& /*ot*/) override;
    virtual void VUpdate() override;

    s16 field_A8_palBuffer[8] = {};
    s16 field_B8_pal_x_index = 0;
    s16 field_BA_pal_w = 0;
    s16 field_BC_bFirstUpdate = 0;
    s16 field_BE_bDone = 0;
};

}
