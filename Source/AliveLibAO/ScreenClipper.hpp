#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/Layer.hpp"

namespace AO {

class ScreenClipper final : public ::BaseGameObject
{
public:
    ScreenClipper(PSX_Point xy, PSX_Point wh, Layer layer);
    ~ScreenClipper();
    
    virtual void VRender(OrderingTable& ot) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    void Update_Clip_Rect(PSX_Point xy, PSX_Point wh);

private:

    Prim_ScissorRect mClipper = {};
    PSX_RECT mRect = {};

public:
    Layer mOtLayer = Layer::eLayer_0;

};

} // namespace AO
