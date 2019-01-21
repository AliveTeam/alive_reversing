#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

struct BloodParticle
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_offx;
    FP field_C_offy;
    Prim_Sprt field_10_prims[2];
};
ALIVE_ASSERT_SIZEOF(BloodParticle, 0x40);

class Blood : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged();

    EXPORT Blood* ctor_40F0B0(FP xpos, FP ypos, FP xOff, FP yOff, FP scale, __int16 count);
private:
    EXPORT void dtor_40F5D0();

    EXPORT BaseGameObject* vDtor_40F5A0(signed int /*flags*/);
    EXPORT void vUpdate_40F650();
    EXPORT void vRender_40F780(int** /*pOrderingTable*/);
    EXPORT void vScreenChanged_40FAD0();
private:
    int field_E4_not_used[4];
    BYTE** field_F4_ppResBuf;
    BloodParticle* field_F8_pResBuf;
    Prim_SetTPage field_FC_tPages[2];
    __int16 field_11C_texture_mode;
    __int16 field_11E_xpos;
    __int16 field_120_ypos;
    __int16 field_122_to_render_count;
    BYTE field_124_rand_seed;
    // pad
    __int16 field_126_total_count;
    int field_128_timer;
    __int16 field_12C_render_layer;
    //__int16 field_12E_pad;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x130);
