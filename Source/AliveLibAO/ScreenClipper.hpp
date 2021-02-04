#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

class ScreenClipper : public BaseGameObject
{
public:
    EXPORT ScreenClipper* ctor_40BD60(PSX_Point xy, PSX_Point wh, Layer layer);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    EXPORT void Update_Clip_Rect_40BE40(PSX_Point xy, PSX_Point wh);

private:
    EXPORT void VRender_40BEB0(PrimHeader** ppOt);
    EXPORT BaseGameObject* dtor_40BDE0();
    EXPORT BaseGameObject* Vdtor_40BF40(unsigned int flags);
  
    Prim_PrimClipper field_10_clippers[2];
    PSX_RECT field_30_rect;
public:
    Layer field_38_ot_layer;
private:
    __int16 field_3A_pad;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x3C);

}

