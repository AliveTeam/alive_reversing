#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Primitives.hpp"

enum class LevelIds : __int16;

class EffectBase : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT EffectBase* ctor_4AB7A0(__int16 layer, char abr);
    EXPORT void dtor_4AB8F0();
private:
    EXPORT EffectBase* vdtor_4AB8C0(signed int flags);
    EXPORT void vRender_4AB970(PrimHeader** ppOt);
protected:
    __int16 field_20_path_id;
    LevelIds field_22_level_id;
    Prim_Tile field_24_tile[2];
    Prim_SetTPage field_4C_tPage[2];
    __int16 field_6C_layer;
    __int16 field_6E_r;
    __int16 field_70_g;
    __int16 field_72_b;
    __int16 field_74_bSemiTrans;
    //__int16 field_76_pad;
};
ALIVE_ASSERT_SIZEOF(EffectBase, 0x78);
