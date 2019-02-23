#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

class ParticleBurst : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParticleBurst* ctor_41CF50(FP xpos, FP ypos, unsigned int unknown_count1, FP scale, __int16 type, signed __int16 a7);
private:
    int field_E4_not_used[4];
    BYTE** field_F4_ppResBuffer;
    void* field_F8_ptr_unknown;
    __int16 field_FC_count1;
    //__int16 field_FE_pad;
    int field_100_alive_timer;
    __int16 field_104_type;
    __int16 field_106_count2;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0x108);
