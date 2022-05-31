#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

namespace AO {

class BaseAliveGameObject;
class PalleteOverwriter;

class Electrocute final : public BaseGameObject
{
public:
    Electrocute(BaseAliveGameObject* pTargetObj, s32 bExtraOverwriter);
    ~Electrocute();

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void Stop();

    BaseAliveGameObject* field_10_obj_target;
    s16 field_14_overwriter_count;
    s16 field_16_pad;
    PalleteOverwriter* field_18_pPalOverwriters[3];
    s16 field_24_extraOverwriter;
    s16 field_26_pad;
    u16* field_28_pPalData;
    s16 field_2C_r;
    s16 field_2E_g;
    s16 field_30_b;
    enum class States : s16
    {
        eSetNewColour_0 = 0,
        eAlphaFadeout_1 = 1,
        eHandleDamage_2 = 2,
        eKillElectrocute_3 = 3
    };
    States field_32_state;
    s32 field_34_pad;
    PSX_RECT field_38_pal_rect;
};
ALIVE_ASSERT_SIZEOF(Electrocute, 0x40);

} // namespace AO
