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
    s16 field_20_path_id;
    LevelIds field_22_level_id;
    Prim_Tile field_24_tile[2];
    Prim_SetTPage field_4C_tPage[2];
    Layer field_6C_layer;
    s16 field_6E_r;
    s16 field_70_g;
    s16 field_72_b;
    s16 field_74_bSemiTrans;
    //s16 field_76_pad;
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x78);
