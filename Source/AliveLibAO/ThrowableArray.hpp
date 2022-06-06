#pragma once
#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

void LoadRockTypes(EReliveLevelIds levelNumber, u16 path);

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
