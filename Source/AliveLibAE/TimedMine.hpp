#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

struct Path_TimedMine final : public Path_TLV
{
    s16 field_10_id;
    s16 field_12_state;
    s16 field_14_scale;
    s16 field_16_timeout;
    s32 field_18_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_TimedMine, 0x1C);

enum TimedMine_Flags_1C4
{
    eStickToLiftPoint_0 = 0x1, // TODO: This is never set.
    ePadding_1 = 0x2,
};

class TimedMine final : public BaseAliveGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;
    virtual void VOnThrowableHit(BaseGameObject* pFrom) override;
    virtual void VOnPickUpOrSlapped() override;
    EXPORT TimedMine* ctor_410600(Path_TimedMine* pPath, TlvItemInfoUnion tlv);

private:
    EXPORT void Update_410A80();
    EXPORT void Render_410CD0(PrimHeader** ppOt);
    EXPORT void InitBlinkAnimation_4108E0(Animation* pAnimation);
    EXPORT void StickToLiftPoint_411100();
    EXPORT void dtor_410970();
    EXPORT TimedMine* vdtor_4108B0(s32 flags);
    EXPORT void vScreenChanged_411270();
    EXPORT s16 vTakeDamage_410FA0(BaseGameObject* pFrom);
    EXPORT void vOnThrowableHit_410F00(BaseGameObject* pHitBy);
    EXPORT void vOnPickUpOrSlapped_410E30();

public:
    u16 field_118_armed;
    u16 field_11A_explode_timeout;
    u32 field_11C_tlv;
    u32 field_120_gnframe;
    Animation field_124_animation;
    u32 field_1BC_gnframe_2;
    u32 field_1C0_detonation_timer;
    BitField32<TimedMine_Flags_1C4> field_1C4_flags;
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1C8);
