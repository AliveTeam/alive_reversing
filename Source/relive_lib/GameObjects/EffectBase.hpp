#pragma once

#include "BaseGameObject.hpp"
#include "../Primitives.hpp"
#include "../Layer.hpp"
#include "../MapWrapper.hpp"

namespace relive {
enum class TBlendModes : u32;
}

class EffectBase : public BaseGameObject
{
public:
    virtual void VRender(BasePrimitive** ppOt) override;
    EffectBase(Layer layer, relive::TBlendModes blendMode);
    ~EffectBase();

protected:
    s16 mEffectBasePathId = 0;
    EReliveLevelIds mEffectBaseLevelId = EReliveLevelIds::eNone;
    Poly_G4 mEffectBaseTile[2] = {};
    Layer mEffectBaseLayer = Layer::eLayer_0;
    s32 mEffectBaseRed = 0;
    s32 mEffectBaseGreen = 0;
    s32 mEffectBaseBlue = 0;
    bool mSemiTrans = false;
    const relive::TBlendModes mBlendMode = {};
};
