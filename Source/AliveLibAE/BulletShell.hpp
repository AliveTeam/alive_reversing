#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

class BulletShell final : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BulletShell* ctor_4AD340(FP xpos, FP ypos, s16 direction, FP scale);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

private:
    EXPORT BulletShell* vdtor_4AD4F0(s32 flags);
    EXPORT void dtor_4AD520();
    EXPORT void vUpdate_4AD550();

private:
    PathLine* field_F4_pLine;
    s16 field_F8_padding;
    s16 field_FA_padding;
    s16 field_FC_hitCount;
    s16 field_FE_padding;
    FP field_100_speed;
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0x104);
