#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

struct GibPart
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

struct Gib_Data
{
    s32 field_0_head;
    s32 field_4_arm;
    s32 field_8_body;
    s32 field_C_max_w;
    s32 field_10_max_h;
    s32 field_14_resource_id;
};
ALIVE_ASSERT_SIZEOF(Gib_Data, 0x18);


enum class GibType : s32
{
    Abe_0 = 0,
    Slig_1 = 1,
    Slog_2 = 2,
    Mud_3 = 3,
    BlindMud_4 = 4,
    Metal_5 = 5,
    Glukkon_6 = 6,
    Aslik_7 = 7,
    Dripik_8 = 8,
    Phleg_9 = 9,
    Fleech_10 = 10,
};

class Gibs final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Gibs* ctor_40FB40(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, s16 bMakeSmaller);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT void dtor_410170();
    EXPORT Gibs* vdtor_410100(s32 flags);
    EXPORT void vUpdate_410210();
    EXPORT void vRender_4103A0(PrimHeader** ppOt);

private:
    void* field_F4_not_used; // Used to be gib data, only used in ctor and replaced with AnimRecords
    FP field_F8_z;
    FP field_FC_dz;
    s32 field_100_timer;
    GibPart field_104_parts[7];
    s16 field_5D4_parts_used_count;
    s16 field_5D6_bMakeSmaller;
};
ALIVE_ASSERT_SIZEOF(Gibs, 0x5D8);
