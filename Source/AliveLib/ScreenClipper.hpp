#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

class ScreenClipper : public BaseGameObject
{
public:
    EXPORT ScreenClipper* ctor_416D60(PSX_Point xy, PSX_Point wh, __int16 layer);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VRender(int** ot) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    void vRender_416F70(int** ot);
private:
    EXPORT void dtor_416E30();
    EXPORT BaseGameObject* vdtor_416E00(signed int flags);
public:
    EXPORT void Update_Clip_Rect_416EB0(PSX_Point xy, PSX_Point wh);
private:
    Prim_PrimClipper field_20_clippers[2];
    PSX_RECT field_40_rect;
public:
    __int16 field_48_ot_layer;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x4C);
