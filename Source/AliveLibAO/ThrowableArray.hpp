#pragma once
#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

enum class LevelIds : s16;

EXPORT void CC LoadRockTypes_454370(LevelIds levelNumber, u16 path);

class ThrowableArray final : public BaseGameObject
{
public:
    ThrowableArray();
    ~ThrowableArray();

    EXPORT void Remove_4540D0(s16 count);

    EXPORT void Add_453F70(s16 count);

    EXPORT void VUpdate_4542B0();
    EXPORT void vScreenChange_454300();

    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    s16 field_10_count;
    s16 field_12_flags;
};
ALIVE_ASSERT_SIZEOF(ThrowableArray, 0x14);

ALIVE_VAR_EXTERN(ThrowableArray*, gpThrowableArray_50E26C);

} // namespace AO
