#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

START_NS_AO

class ScreenClipper : public BaseGameObject
{
public:
    EXPORT ScreenClipper* ctor_40BD60(PSX_Point xy, PSX_Point wh, __int16 layer);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VRender(int** ot) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    EXPORT void Update_Clip_Rect_40BE40(PSX_Point xy, PSX_Point wh);

private:
    EXPORT void VRender_40BEB0(int** ppOt);
    EXPORT BaseGameObject* dtor_40BDE0();
    EXPORT BaseGameObject* Vdtor_40BF40(unsigned int flags);
  
    Prim_PrimClipper field_10_clippers[2];
    PSX_RECT field_30_rect;
public:
    __int16 field_38_ot_layer;
private:
    __int16 field_3A_pad;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x3C);

END_NS_AO

