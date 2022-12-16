#pragma once

#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "../relive_lib/Primitives_common.hpp"
#include "MapWrapper.hpp"
#include "Layer.hpp"

class FG1 final : public BaseGameObject
{
public:
    FG1(Fg1Resource& pFg1Res, CamResource& camRes);
    ~FG1();

    virtual void VRender(PrimHeader** ppOt) override;

    virtual void VUpdate() override
    {
        // Empty
    }

    virtual void VScreenChanged() override;

    Poly_FT4 mPolys[4] = {};
    Fg1Resource mFG1Res;
    CamResource mCamRes;
};
