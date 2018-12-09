#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class Blood : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged();

    EXPORT Blood* ctor_40F0B0(FP /*xpos*/, FP /*ypos*/, FP /*xOff*/, FP /*yOff*/, FP /*scale*/, __int16 /*count*/);
private:
    EXPORT BaseGameObject* vDtor_40F5A0(signed int /*flags*/);
    EXPORT void vUpdate_40F650();
    EXPORT void vRender_40F780(int** /*pOrderingTable*/);
    EXPORT void vScreenChanged_40FAD0();
private:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4_pPrimBuffer;
    int field_F8_pBloodBuffer;
    Prim_SetTPage field_FC_tPages[2];
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122_to_render_count;
    __int16 field_124;
    __int16 field_126_total_count;
    int field_128;
    __int16 field_12C_render_layer;
    __int16 field_12E;
};
ALIVE_ASSERT_SIZEOF(Blood, 0x130);
