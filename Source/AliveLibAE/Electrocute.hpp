#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Psx.hpp"
#include "../AliveLibCommon/Function.hpp"

class BaseAliveGameObject;
class PalleteOverwriter;

class Electrocute final : public BaseGameObject
{
public:
    Electrocute(BaseAliveGameObject* pTargetObj, bool bExtraOverwriter, bool bKillTarget);
    ~Electrocute();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    // New virtuals
    virtual void VStop();
    virtual s32 VSub_4E6630();

private:
    s32 vSub_4E6630();

public:
    Guid field_20_target_obj_id;
    s16 field_24_r = 0;
    s16 field_26_g = 0;
    s16 field_28_b = 0;

private:
    s16 field_2C_bKillTarget = 0;
    s16 field_2E_overwriter_count = 0;
    PalleteOverwriter* field_30_pPalOverwriters[3] = {};
    s16 field_3C_extraOverwriter = 0;
   // u16* field_40_pPalData = nullptr;
    AnimationPal mPalData;
    enum class States : s16
    {
        eSetNewColour_0 = 0,
        eAlphaFadeout_1 = 1,
        eHandleDamage_2 = 2,
        eKillElectrocute_3 = 3
    };
    States field_44_state = States::eSetNewColour_0;
};
