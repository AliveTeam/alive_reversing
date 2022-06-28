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

    void SetRGB(s16 rValue, s16 gValue, s16 bValue)
    {
        r = rValue;
        g = gValue;
        b = bValue;
    }
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
    enum VisualFlags : s16
    {
        eApplyShadowZoneColour = 0x1,
        eDoPurpleLightEffect = 0x2
    };
    BitField16<VisualFlags> mVisualFlags = {};

    // TODO: Common shadow field
};
