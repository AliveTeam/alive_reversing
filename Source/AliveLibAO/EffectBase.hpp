#pragma once

#include "BaseGameObject.hpp"
#include "Primitives.hpp"
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Layer.hpp"

namespace AO {

enum class LevelIds : s16;

class EffectBase : public BaseGameObject
{
public:
    EXPORT EffectBase* ctor_461550(Layer layer, TPageAbr abr);

    EXPORT BaseGameObject* dtor_461630();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT EffectBase* Vdtor_461750(s32 flags);

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_461690(PrimHeader** ppOt);

protected:
    s16 field_10_path_id;
    LevelIds field_12_level_id;
    Prim_Tile field_14_tile[2];
    Prim_SetTPage field_3C_tPage[2];
    Layer field_5C_layer;
    s16 field_5E_r;
    s16 field_60_g;
    s16 field_62_b;
    s16 field_64_bSemiTrans;
    s16 field_66_pad;
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x68);

} // namespace AO
