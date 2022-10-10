#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/Layer.hpp"

enum class EReliveLevelIds : s16;

enum class TPageAbr : s8;

namespace AO {

class EffectBase : public ::BaseGameObject
{
public:
    EffectBase(Layer layer, TPageAbr abr);
    ~EffectBase();

    virtual void VRender(PrimHeader** ppOt) override;

protected:
    s16 mEffectBasePathId = 0;
    EReliveLevelIds mEffectBaseLevelId = EReliveLevelIds::eNone;
    Prim_Tile mEffectBaseTile[2] = {};
    Prim_SetTPage mEffectBaseTPage[2] = {};
    Layer mEffectBaseLayer = Layer::eLayer_0;
    s16 mEffectBaseRed = 0;
    s16 mEffectBaseGreen = 0;
    s16 mEffectBaseBlue = 0;
    s16 mSemiTrans = 0;
};

} // namespace AO
