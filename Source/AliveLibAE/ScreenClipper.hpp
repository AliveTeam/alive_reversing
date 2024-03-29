#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

class ScreenClipper final : public BaseGameObject
{
public:
    EXPORT ScreenClipper* ctor_416D60(PSX_Point xy, PSX_Point wh, Layer layer);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    void vRender_416F70(PrimHeader** ot);

private:
    EXPORT void dtor_416E30();
    EXPORT BaseGameObject* vdtor_416E00(s32 flags);

public:
    EXPORT void Update_Clip_Rect_416EB0(PSX_Point xy, PSX_Point wh);

private:
    Prim_PrimClipper field_20_clippers[2];
    PSX_RECT field_40_rect;

public:
    Layer field_48_ot_layer;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x4C);
