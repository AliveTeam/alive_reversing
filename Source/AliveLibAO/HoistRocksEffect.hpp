#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_Hoist final : public Path_TLV
{
    enum class Type : s16
    {
        eNextFloor = 0,
        eNextEdge = 1,
        eOffScreen = 2,
    };
    Type field_18_hoist_type;

    enum class EdgeType : s16
    {
        eLeft = 0,
        eRight = 1,
        eBoth = 2,
    };
    EdgeType field_1A_edge_type;

    s16 field_1C_switch_id;

    s16 field_1E_pad;
};
ALIVE_ASSERT_SIZEOF(Path_Hoist, 0x20);

class HoistParticle final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT HoistParticle* ctor_431B00(FP xpos, FP ypos, FP scale, s32 frameTableOffset);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_431BD0();

    s32 field_D4[4];
    s16 field_E4_bHitGround;
    s16 field_E6_pad;
};
ALIVE_ASSERT_SIZEOF(HoistParticle, 0xE8);

class HoistRocksEffect final : public BaseGameObject
{
public:
    EXPORT HoistRocksEffect* ctor_431820(Path_Hoist* pTlv, s32 tlvInfo);

    EXPORT BaseGameObject* dtor_431A90();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT HoistRocksEffect* vdtor_431CF0(s32 flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_431AF0();

    virtual void VUpdate() override;

    EXPORT void VUpdate_431860();

    FP field_10_xpos;
    FP field_14_ypos;
    s32 field_18_tlvInfo;
    s32 field_1C_never_used[4];
};
ALIVE_ASSERT_SIZEOF(HoistRocksEffect, 0x2C);

} // namespace AO
