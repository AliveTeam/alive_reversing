#pragma once
#include "../AliveLibCommon/Function.hpp"
#include "BaseGameObject.hpp"

namespace AO {

enum class LevelIds : s16;

void LoadRockTypes(LevelIds levelNumber, u16 path);

class ThrowableArray final : public BaseGameObject
{
public:
    ThrowableArray();
    ~ThrowableArray();

    void Remove(s16 count);

    void Add(s16 count);
    
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    s16 field_10_count;
    s16 field_12_flags;
};
ALIVE_ASSERT_SIZEOF(ThrowableArray, 0x14);

ALIVE_VAR_EXTERN(ThrowableArray*, gpThrowableArray_50E26C);

} // namespace AO
