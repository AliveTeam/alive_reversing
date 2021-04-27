#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Map.hpp"
#include "Path.hpp"

namespace AO {

enum class UXB_State : s16
{
    eArmed_0 = 0,
    eDisarmed_1 = 1,
};

struct Path_UXB : public Path_TLV
{
    s16 field_18_num_patterns;
    s16 field_1A_pattern;
    Scale_short field_1C_scale;
    UXB_State field_1E_state;
    s32 field_20_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_UXB, 0x24);

class UXB : public BaseAliveGameObject
{
public:
    EXPORT UXB* ctor_488C80(Path_UXB* pTlv, s32 tlvInfo);

    void InitBlinkAnim();

    EXPORT BaseGameObject* dtor_4891B0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_489C70(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_489BD0();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_489AB0(BaseGameObject* pFrom);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_489A30(BaseGameObject* pFrom);

    virtual void VOnPickUpOrSlapped() override;

    EXPORT void VOnPickUpOrSlapped_4897E0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_489380();

    EXPORT s16 IsColliding_489900();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_4896C0(PrimHeader** ppOt);

    s16 field_10C_state;
    s16 field_10E_starting_state;
    s16 field_110_disabled_resources;
    s16 field_112;
    s32 field_114_tlvInfo;
    s32 field_118_next_state_frame;
    Animation field_11C_anim;
    s16 field_1B4_pattern_length;
    s16 field_1B6_pattern_index;
    s16 field_1B8_pattern;
    s16 field_1BA_red_blink_count;
    s16 field_1BC_flags;
    s16 field_1BE;
};
ALIVE_ASSERT_SIZEOF(UXB, 0x1C0);

} // namespace AO
