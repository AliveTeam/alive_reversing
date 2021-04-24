#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"

class Sparks : public ::BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Sparks* ctor_416390(FP xpos, FP ypos, FP scale);
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
private:
    EXPORT void vUpdate_416570();
    EXPORT void vScreenChanged_416720();
    EXPORT void dtor_416550();
    EXPORT Sparks* vdtor_416520(s32 flags);
private:
    int field_F4_padding;
    s16 field_F8_padding;
    s16 field_FA_16_random;
};
ALIVE_ASSERT_SIZEOF(Sparks, 0xFC);
