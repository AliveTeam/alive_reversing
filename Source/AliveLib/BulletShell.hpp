#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

struct PathLine;

class BulletShell : public BaseAnimatedWithPhysicsGameObject
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
    __int16 field_F8;
    __int16 field_FA;
    __int16 field_FC;
    __int16 field_FE;
    FP field_100;
};
ALIVE_ASSERT_SIZEOF(BulletShell, 0x104);
