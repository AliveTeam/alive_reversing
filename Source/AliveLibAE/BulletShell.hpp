#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class PathLine;

class BulletShell : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT BulletShell* ctor_4AD340(FP xpos, FP ypos, __int16 direction, FP scale);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
private:
    EXPORT BulletShell* vdtor_4AD4F0(signed int flags);
    EXPORT void dtor_4AD520();
    EXPORT void vUpdate_4AD550();
private:
    PathLine *field_F4_pLine;
    __int16 field_F8_padding;
    __int16 field_FA_padding;
    __int16 field_FC_hitCount;
    __int16 field_FE_padding;
    FP field_100_speed;
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0x104);
