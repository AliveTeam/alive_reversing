#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

namespace AO {

class ScreenClipper final : public BaseGameObject
{
public:
    ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer);
    ~ScreenClipper();
    
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void Update_Clip_Rect_40BE40(PSX_Point xy, PSX_Point wh);

private:
    void VRender_40BEB0(PrimHeader** ppOt);

    Prim_PrimClipper field_10_clippers[2];
    PSX_RECT field_30_rect;

public:
    Layer field_38_ot_layer;

private:
    s16 field_3A_pad;
};
ALIVE_ASSERT_SIZEOF(ScreenClipper, 0x3C);

} // namespace AO
