#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

START_NS_AO

struct Path_BellHammer : public Path_TLV
{
    __int16 field_18_id;
    __int16 field_1A_action;
    __int16 field_1C_scale;
    __int16 field_1E_direction;
};
ALIVE_ASSERT_SIZEOF(Path_BellHammer, 0x20);

class BellHammer : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BellHammer* ctor_405010(Path_BellHammer* pTlv, int tlvInfo);
    EXPORT BaseGameObject* dtor_405220();

    virtual void VScreenChanged() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT void VScreenChanged_4054B0();
    EXPORT BellHammer* Vdtor_4054C0(signed int flags);

    static EXPORT void CC OnResLoaded_405210(BellHammer* pThis);

    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    unsigned __int16 field_E4;
    __int16 field_E6;
    int field_E8_tlvInfo;
    int field_EC_pending_resource_count;
    int field_F0;
};
ALIVE_ASSERT_SIZEOF(BellHammer, 0xF4);


END_NS_AO

