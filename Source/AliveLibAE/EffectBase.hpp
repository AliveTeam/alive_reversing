#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

enum class LevelIds : s16;

class EffectBase : public BaseGameObject
{
public:
    
    virtual void VRender(PrimHeader** ppOt) override;

    EffectBase(Layer layer, TPageAbr abr);
    ~EffectBase();

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
    //s16 field_76_pad;
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x78);
