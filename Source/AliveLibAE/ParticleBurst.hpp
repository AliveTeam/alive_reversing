#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "FunctionFwd.hpp"

struct ParticleBurst_Item;

enum class BurstType : s16
{
    eFallingRocks_0 = 0,
    eSticks_1 = 1,
    eBigPurpleSparks_2 = 2,
    eBigRedSparks_3 = 3,
    eInvalid_4 = 4,
    eGreenSparks_5 = 5,
    eSmallPurpleSparks_6 = 6,
};

class ParticleBurst : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT ParticleBurst* ctor_41CF50(FP xpos, FP ypos, u32 numOfParticles, FP scale, BurstType type, s16 count);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;

private:
    EXPORT FP* Random_Speed_41CEE0(FP* random);
    EXPORT ParticleBurst* vdtor_41D4E0(s32 flags);
    EXPORT void dtor_41D510();
    EXPORT void vRender_41D7B0(PrimHeader** ppOt);
    EXPORT void vUpdate_41D590();

private:
    BYTE** field_F4_ppRes;
    ParticleBurst_Item* field_F8_pRes;
    s16 field_FC_number_of_particles;
    s16 field_FE_padding;
    int field_100_timer;
    BurstType field_104_type;
    s16 field_106_count;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst, 0x108);
