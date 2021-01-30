#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_UXB : public Path_TLV
{
    __int16 field_18_num_patterns;
    __int16 field_1A_pattern;
    __int16 field_1C_scale;
    __int16 field_1E_state;
    int field_20_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_UXB, 0x24);

class UXB : public BaseAliveGameObject
{
public:
    EXPORT UXB* ctor_488C80(Path_UXB* pTlv, int tlvInfo);

    void InitBlinkAnim();

    EXPORT BaseGameObject* dtor_4891B0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT BaseGameObject* Vdtor_489C70(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_489BD0();

    virtual __int16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT __int16 VTakeDamage_489AB0(BaseGameObject* pFrom);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_489A30(BaseGameObject* pFrom);

    virtual void VOnPickUpOrSlapped() override;

    EXPORT void VOnPickUpOrSlapped_4897E0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_489380();

    EXPORT __int16 IsColliding_489900();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_4896C0(PrimHeader** ppOt);

    __int16 field_10C_state;
    __int16 field_10E_starting_state;
    __int16 field_110_disabled_resources;
    __int16 field_112;
    int field_114_tlvInfo;
    int field_118_next_state_frame;
    Animation field_11C_anim;
    __int16 field_1B4_pattern_length;
    __int16 field_1B6_pattern_index;
    __int16 field_1B8_pattern;
    __int16 field_1BA_red_blink_count;
    __int16 field_1BC_flags;
    __int16 field_1BE;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1C0);

}

