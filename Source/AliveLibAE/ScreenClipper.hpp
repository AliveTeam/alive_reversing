#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
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
    void Update_Clip_Rect(PSX_Point xy, PSX_Point wh);

private:
    Prim_PrimClipper field_20_clippers[2];
    PSX_RECT field_40_rect;

public:
    Layer field_48_ot_layer;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x4C);
