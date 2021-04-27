#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "Layer.hpp"

enum class LevelIds : s16;

class EffectBase : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT EffectBase* ctor_4AB7A0(Layer layer, TPageAbr abr);
    EXPORT void dtor_4AB8F0();

private:
    EXPORT EffectBase* vdtor_4AB8C0(s32 flags);
    EXPORT void vRender_4AB970(PrimHeader** ppOt);

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
