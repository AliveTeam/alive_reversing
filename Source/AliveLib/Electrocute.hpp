#pragma once

#include "BaseGameObject.hpp"
#include "Psx.hpp"
#include "FunctionFwd.hpp"

class BaseAliveGameObject;
class PalleteOverwriter;

class Electrocute : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    // New virtuals
    virtual void VSub_4E6150();
    virtual int VSub_4E6630();

    EXPORT Electrocute* ctor_4E5E80(BaseAliveGameObject* pTargetObj, __int16 b1, __int16 bKillTarget);

private:
    EXPORT Electrocute* vdtor_4E6060(signed int flags);
    EXPORT void dtor_4E6090();
    EXPORT void vScreenChanged_4E65E0();
    EXPORT void vUpdate_4E6240();
    EXPORT void vSub_4E6150();
    EXPORT int vSub_4E6630();

private:
    int field_20_target_obj_id;
    __int16 field_24_r;
    __int16 field_26_g;
    __int16 field_28_b;
    __int16 field_2A_pad; // NOTE: Crashes if commented out - why?
    __int16 field_2C_bKillTarget;
    __int16 field_2E_overwriter_count;
    PalleteOverwriter* field_30_pPalOverwriters[3];
    __int16 field_3C_b1;
    __int16 field_3E;
    WORD* field_40_pPalData;
    __int16 field_44_state;
    __int16 field_46_pad;   // Ditto
    int field_48;
    PSX_RECT field_4C_pal_rect;
};
ALIVE_ASSERT_SIZEOF(Electrocute, 0x54);
