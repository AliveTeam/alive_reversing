#pragma once

#include "FunctionFwd.hpp"
#include "DynamicArray.hpp"
#include "Animation.hpp"

#pragma pack(push)
#pragma pack(2)
class BaseAliveGameObject : public BaseAnimatedWithPhysicsGameObject
{
    // Todo: VTable Headers
public:
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0_prev_base;
    __int16 field_F4;
    __int16 field_F6;
    int field_F8;
    int field_FC_pPathTLV;
    int field_100_pCollisionLine;
    __int16 field_104;
    __int16 field_106_animation_num;
    __int16 field_108;
    __int16 field_10A;
    int field_10C_health;
    int field_110;
    __int16 field_114_flags;
};
#pragma pack(pop)
ALIVE_ASSERT_SIZEOF(BaseAliveGameObject, 0x116);

