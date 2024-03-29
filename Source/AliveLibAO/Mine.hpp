#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

struct Path_Mine final : public Path_TLV
{
    s16 field_18_num_patterns;
    s16 field_1A_pattern;
    Scale_short field_1C_scale;
    s16 field_1E_disabled_resources;
    Choice_short field_20_persists_offscreen;
    s16 field_22_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Mine, 0x24);

class Mine final : public BaseAliveGameObject
{
public:
    EXPORT Mine* ctor_43A330(Path_Mine* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_43A640();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT Mine* Vdtor_43AD50(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_43AC10();

    virtual s16 VTakeDamage(BaseGameObject* pFrom) override;

    EXPORT s16 VTakeDamage_43AC40(BaseGameObject* pFrom);

    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;

    EXPORT void VOnThrowableHit_43AB90(BaseGameObject* pFrom);

    virtual void VOnPickUpOrSlapped() override;

    EXPORT void VOnPickUpOrSlapped_43AA30();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_43A970(PrimHeader** ppOt);

    virtual void VUpdate() override;

    EXPORT void vUpdate_43A7F0();

    EXPORT s16 IsColliding_43AA60();

    u16 field_10C_detonating;
    s16 field_10E_disabled_resources;
    s32 field_110_tlv;
    s32 field_114_gnframe;
    Animation field_118_animation;
    s32 field_1B0_flags;
};
ALIVE_ASSERT_SIZEOF(Mine, 0x1B4);

} // namespace AO
