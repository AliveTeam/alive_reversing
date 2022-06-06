#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "Primitives.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Layer.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

class EffectBase : public BaseGameObject
{
public:
    EffectBase(Layer layer, TPageAbr abr);
    ~EffectBase();

    virtual void VRender(PrimHeader** ppOt) override;

protected:
    s16 mEffectBasePathId;
    EReliveLevelIds mEffectBaseLevelId;
    Prim_Tile mEffectBaseTile[2];
    Prim_SetTPage mEffectBaseTPage[2];
    Layer mEffectBaseLayer;
    s16 mEffectBaseRed;
    s16 mEffectBaseGreen;
    s16 mEffectBaseBlue;
    s16 mSemiTrans;
    s16 field_66_pad;
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x68);

} // namespace AO
