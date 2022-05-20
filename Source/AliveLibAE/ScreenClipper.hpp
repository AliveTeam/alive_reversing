#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

class ScreenClipper final : public BaseGameObject
{
public:
    ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer);
    ~ScreenClipper();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

public:
    EXPORT void Update_Clip_Rect_416EB0(PSX_Point xy, PSX_Point wh);

private:
    Prim_PrimClipper field_20_clippers[2];
    PSX_RECT field_40_rect;

public:
    Layer field_48_ot_layer;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x4C);
