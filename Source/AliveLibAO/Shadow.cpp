#include "stdafx_ao.h"
#include "Function.hpp"
#include "Shadow.hpp"
#include "Game.hpp"
#include "ResourceManager.hpp"

START_NS_AO

EXPORT Shadow* Shadow::ctor_461FB0()
{
    SetVTable(&field_18_animation, 0x4BA2B8);
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2035, 1, 0);
    field_18_animation.Init_402D20(296, gObjList_animations_505564, 0, 37, 6, ppRes, 1, 0, 0);

    field_14_flags.Clear(Flags::eBit1_ShadowAtBottom);
    field_14_flags.Set(Flags::eBit2_Enabled);

    field_18_animation.field_B_render_mode = 2;

    field_18_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
    field_18_animation.field_4_flags.Clear(AnimFlags::eBit16_bBlending);

    field_18_animation.field_4_flags.Set(AnimFlags::eBit2_Animate);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit8_Loop);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit17);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit18_IsLastFrame);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit20_use_xy_offset);
    field_18_animation.field_4_flags.Set(AnimFlags::eBit21);

    return this;
}

END_NS_AO

