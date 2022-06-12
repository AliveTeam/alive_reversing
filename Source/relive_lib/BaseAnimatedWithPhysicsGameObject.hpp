#pragma once

#include "BaseGameObject.hpp"
#include "MapWrapper.hpp"

struct TintEntry
{
    EReliveLevelIds field_0_level;
    u8 field_1_r;
    u8 field_2_g;
    u8 field_3_b;
};

enum class Scale : s16
{
    Bg = 0,
    Fg = 1,
};

struct RGB16 final
{
    s16 r = 0;
    s16 g = 0;
    s16 b = 0;
};

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


public:
    // TODO: Common anim field

    FP mBaseAnimatedWithPhysicsGameObject_XPos = {};
    FP mBaseAnimatedWithPhysicsGameObject_YPos = {};
    s16 mBaseAnimatedWithPhysicsGameObject_PathNumber = 0;
    EReliveLevelIds mBaseAnimatedWithPhysicsGameObject_LvlNumber = EReliveLevelIds::eNone;
    FP mBaseAnimatedWithPhysicsGameObject_VelX = {};
    FP mBaseAnimatedWithPhysicsGameObject_VelY = {};
    FP mBaseAnimatedWithPhysicsGameObject_SpriteScale = {};
    RGB16 mBaseAnimatedWithPhysicsGameObject_RGB;
    Scale mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
    s16 mBaseAnimatedWithPhysicsGameObject_YOffset = 0;
    s16 mBaseAnimatedWithPhysicsGameObject_XOffset = 0;
    s16 mApplyShadows = 0;

    // TODO: Common shadow field
};
