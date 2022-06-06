#pragma once

#include "BaseGameObject.hpp"

// Another glue/adapter interface
class IBaseAnimatedWithPhysicsGameObject : public BaseGameObject
{
public:
    explicit IBaseAnimatedWithPhysicsGameObject(s16 resourceArraySize)
        : BaseGameObject(TRUE, resourceArraySize)
    {

    }

    virtual ~IBaseAnimatedWithPhysicsGameObject()
    {

    }

    // HACK: Remove me when glue interfaces are gone
    BaseGameObject& GetBaseGameObject()
    {
        return *dynamic_cast<BaseGameObject*>(this);
    }

    virtual s16 Scale() = 0; // Note: is FP in AO, needs conversion
    virtual FP SpriteScale() = 0;

    virtual FP XPos() = 0;
    virtual FP YPos() = 0;
};
