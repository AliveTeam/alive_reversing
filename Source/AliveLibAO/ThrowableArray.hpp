#pragma once
#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {;

enum class LevelIds : __int16;

EXPORT void CC LoadRockTypes_454370(LevelIds levelNumber, unsigned __int16 path);

class ThrowableArray : public BaseGameObject
{
public:
    EXPORT ThrowableArray* ctor_453EE0();

    EXPORT BaseGameObject* dtor_453F10();

    EXPORT void Remove_4540D0(__int16 count);

    EXPORT void Add_453F70(__int16 count);

    EXPORT BaseGameObject* Vdtor_454690(signed int flags);
    EXPORT void VUpdate_4542B0();
    EXPORT void vScreenChange_454300();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    __int16 field_10_count;
    __int16 field_12_flags;
};
ALIVE_ASSERT_SIZEOF(ThrowableArray, 0x14);

ALIVE_VAR_EXTERN(ThrowableArray*, gpThrowableArray_50E26C);

};
