#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class AnimationUnknown;

class Rope final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    Rope(s32 left, s32 top, s32 bottom, FP scale);
    ~Rope();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    s16 field_F4_rope_segment_count = 0;

public:
    s16 field_F6_rope_length = 0;
    u8** field_F8_ppRopeRes = nullptr;
    AnimationUnknown* field_FC_pRopeRes = nullptr;
    s16 field_100_left = 0;
    s16 field_102_top = 0;
    s16 field_104_right = 0;
    s16 field_106_bottom = 0;
};
ALIVE_ASSERT_SIZEOF(Rope, 0x108);

void ClipPoly_Vertically_4A09E0(Poly_FT4* pPoly, s32 minY, s32 maxY);
