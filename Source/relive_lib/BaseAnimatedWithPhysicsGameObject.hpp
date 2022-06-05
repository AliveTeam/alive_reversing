#pragma once

// Another glue/adapter interface
class IBaseAnimatedWithPhysicsGameObject
{
public:
    virtual ~IBaseAnimatedWithPhysicsGameObject()
    {

    }

    // HACK: Remove me when glue interfaces are gone
    IBaseGameObject& GetBaseGameObject()
    {
        return *dynamic_cast<IBaseGameObject*>(this);
    }

    virtual s16 Scale() = 0; // Note: is FP in AO, needs conversion
    virtual FP SpriteScale() = 0;

    virtual FP XPos() = 0;
    virtual FP YPos() = 0;
};
