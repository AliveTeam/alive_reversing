#pragma once

#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Layer.hpp"

namespace AO {

enum class LevelIds : s16;

class EffectBase : public BaseGameObject
{
public:
    EffectBase(Layer layer, TPageAbr abr);
    ~EffectBase();

    virtual void VRender(PrimHeader** ppOt) override;

protected:
    s16 mPathId;
    LevelIds mLevelId;
    Prim_Tile mTile[2];
    Prim_SetTPage mTPage[2];
    Layer mLayer;
    s16 mRed;
    s16 mGreen;
    s16 mBlue;
    s16 mSemiTrans;
    s16 field_66_pad;
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x68);

} // namespace AO
