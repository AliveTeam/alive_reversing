#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "AnimResources.hpp"
namespace AO {

struct GibPart final
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_z;
    FP field_C_dx;
    FP field_10_dy;
    FP field_14_dz;
    Animation field_18_anim;
};
ALIVE_ASSERT_SIZEOF(GibPart, 0xB0);

struct Gib_Data final
{
    AnimId field_0_head;
    AnimId field_4_arm;
    AnimId field_8_body;
    //s32 field_C_max_w;
    //s32 field_10_max_h;
    //s32 field_14_resource_id;
};
//ALIVE_ASSERT_SIZEOF(Gib_Data, 0x18);


enum GibType : s32
{
    Abe_0 = 0,
    Slig_1 = 1,
    Slog_2 = 2,
    Elum_3 = 3,
    Mud_4 = 4,
    Metal_5 = 5
};

class Gibs final : public BaseAnimatedWithPhysicsGameObject
{
public:
    Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale);
    ~Gibs();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

    s32 field_D4_padding[4];
    const Gib_Data* field_E4_pGibData;
    FP field_E8_z;
    FP field_EC_dz;
    s32 field_F0_timer;
    GibPart field_F4_parts[7];
    s16 field_5C4_parts_used_count;
    s16 field_5C6_pad;
};
ALIVE_ASSERT_SIZEOF(Gibs, 0x5C8);

} // namespace AO
