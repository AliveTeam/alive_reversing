#pragma once

#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "FunctionFwd.hpp"

class BaseAliveGameObject;
class PalleteOverwriter;

class Electrocute final : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    // New virtuals
    virtual void VStop_4E6150();
    virtual s32 VSub_4E6630();

    EXPORT Electrocute* ctor_4E5E80(BaseAliveGameObject* pTargetObj, s16 bExtraOverwriter, s16 bKillTarget);

private:
    EXPORT Electrocute* vdtor_4E6060(s32 flags);
    EXPORT void dtor_4E6090();
    EXPORT void vScreenChanged_4E65E0();
    EXPORT void vUpdate_4E6240();
    EXPORT void vStop_4E6150();
    EXPORT s32 vSub_4E6630();

public:
    s32 field_20_target_obj_id;
    s16 field_24_r;
    s16 field_26_g;
    s16 field_28_b;

private:
    s16 field_2A_padding; // NOTE: Crashes if commented out - why?
    s16 field_2C_bKillTarget;
    s16 field_2E_overwriter_count;
    PalleteOverwriter* field_30_pPalOverwriters[3];
    s16 field_3C_extraOverwriter;
    s16 field_3E_padding;
    u16* field_40_pPalData;
    enum class States : s16
    {
        eSetNewColour_0 = 0,
        eAlphaFadeout_1 = 1,
        eHandleDamage_2 = 2,
        eKillElectrocute_3 = 3
    };
    States field_44_state;
    s16 field_46_padding; // Ditto
    s32 field_48_padding;
    PSX_RECT field_4C_pal_rect;
};
ALIVE_ASSERT_SIZEOF(Electrocute, 0x54);
