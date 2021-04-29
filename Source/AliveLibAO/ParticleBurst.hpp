#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

namespace AO {

struct ParticleBurst_Item;

enum class BurstType : s16
{
    eType_0 = 0,
    eType_1 = 1,
    eType_2 = 2,
    eType_3 = 3,
    eType_4 = 4,
};

class ParticleBurst final : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParticleBurst* ctor_40D0F0(FP xpos, FP ypos, s16 unknown_count1, FP scale, BurstType type);

    EXPORT BaseGameObject* dtor_40D5A0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT ParticleBurst* Vdtor_40DA40(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_40D600();

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_40D7F0(PrimHeader** ppOt);

    s32 field_D4[4];
    u8** field_E4_ppRes;
    ParticleBurst_Item* field_E8_pRes;
    s16 field_EC_count;
    s16 field_EE;
    s32 field_F0_timer;
    BurstType field_F4_type;
    s16 field_F6;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0xF8);

} // namespace AO
