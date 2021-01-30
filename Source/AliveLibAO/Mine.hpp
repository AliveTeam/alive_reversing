#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

struct Path_Mine : public Path_TLV
{
    __int16 field_18_num_patterns;
    __int16 field_1A_pattern;
    __int16 field_1C_scale;
    __int16 field_1E_disabled_resources;
    __int16 field_20_persists_offscreen;
    __int16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x24);

class Mine : public BaseAliveGameObject
{
public:
    EXPORT Mine* ctor_43A330(Path_Mine* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_43A640();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT Mine* Vdtor_43AD50(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43AC10();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_43AC40(BaseGameObject* pFrom);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_43AB90(BaseGameObject* pFrom);

    virtual void VOnPickUpOrSlapped() override;

    EXPORT void VOnPickUpOrSlapped_43AA30();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_43A970(PrimHeader** ppOt);

    virtual void VUpdate() override;

    EXPORT void vUpdate_43A7F0();

    EXPORT __int16 IsColliding_43AA60();

    unsigned __int16 field_10C_detonating;
    __int16 field_10E_disabled_resources;
    int field_110_tlv;
    int field_114_gnframe;
    Animation field_118_animation;
    int field_1B0_flags;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1B4);

}

