#pragma once

#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"
#include "MapWrapper.hpp"

enum class TPageAbr : s8;

class EffectBase : public BaseGameObject
{
public:
    virtual void VRender(PrimHeader** ppOt) override;
    EffectBase(Layer layer, TPageAbr abr);
    ~EffectBase();

protected:
    s16 mEffectBasePathId = 0;
    EReliveLevelIds mEffectBaseLevelId = EReliveLevelIds::eNone;
    Poly_G4 mEffectBaseTile[2] = {};
    Prim_SetTPage mEffectBaseTPage[2] = {};
    Layer mEffectBaseLayer = Layer::eLayer_0;
    s32 mEffectBaseRed = 0;
    s32 mEffectBaseGreen = 0;
    s32 mEffectBaseBlue = 0;
    s16 mSemiTrans = 0;
};
