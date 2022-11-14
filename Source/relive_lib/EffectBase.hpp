#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
#include "../relive_lib/Layer.hpp"
#include "../relive_lib/MapWrapper.hpp"

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
    Prim_Tile mEffectBaseTile[2] = {};
    Prim_SetTPage mEffectBaseTPage[2] = {};
    Layer mEffectBaseLayer = Layer::eLayer_0;
    s32 mEffectBaseRed = 0;
    s32 mEffectBaseGreen = 0;
    s32 mEffectBaseBlue = 0;
    s16 mSemiTrans = 0;
};
