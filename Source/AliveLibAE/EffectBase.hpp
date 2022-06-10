#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

enum class EReliveLevelIds : s16;

class EffectBase : public BaseGameObject
{
public:
    
    virtual void VRender(PrimHeader** ppOt) override;

    EffectBase(Layer layer, TPageAbr abr);
    ~EffectBase();

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
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x78);
