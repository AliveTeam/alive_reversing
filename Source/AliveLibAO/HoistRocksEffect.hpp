#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_Hoist : public Path_TLV
{
    enum class Type : __int16
    {
        eNextFloor = 0,
        eNextEdge = 1,
        eOffScreen = 2,
    };
    Type field_18_hoist_type;

    enum class EdgeType : __int16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    EdgeType field_1A_edge_type;

    __int16 field_1C_id;

    __int16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Hoist, 0x20);

class HoistParticle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HoistParticle* ctor_431B00(FP xpos, FP ypos, FP scale, int frameTableOffset);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_431BD0();

    int field_D4[4];
    __int16 field_E4_bHitGround;
    __int16 field_E6_pad;
};
ALIVE_ASSERT_SIZEOF(HoistParticle, 0xE8);

class HoistRocksEffect : public BaseGameObject
{
public:
    EXPORT HoistRocksEffect* ctor_431820(Path_Hoist* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_431A90();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT HoistRocksEffect* vdtor_431CF0(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_431AF0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_431860();

    FP field_10_xpos;
    FP field_14_ypos;
    int field_18_tlvInfo;
    int field_1C_never_used[4];
};
ALIVE_ASSERT_SIZEOF(HoistRocksEffect, 0x2C);

}

