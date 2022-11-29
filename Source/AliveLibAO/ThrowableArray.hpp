#pragma once
#include "../relive_lib/BaseGameObject.hpp"

enum class EReliveLevelIds : s16;

namespace AO {

void LoadRockTypes(EReliveLevelIds levelNumber, u16 path);

class ThrowableArray final : public BaseGameObject
{
public:
    ThrowableArray();
    ~ThrowableArray();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;

    void Remove(s16 count);

    void Add(s16 count);
    
public:
    s16 mCount = 0;
private:
    s16 field_12_flags = 0;
};

extern ThrowableArray* gThrowableArray;

} // namespace AO
