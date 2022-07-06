#pragma once

#include "BaseGameObject.hpp"
#include "MapWrapper.hpp"
#include "../relive_lib/Animation.hpp"

class Shadow;

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
    Animation mAnim = {};
    FP mXPos = {};
    FP mYPos = {};
    s16 mCurrentPath = 0;
    EReliveLevelIds mCurrentLevel = EReliveLevelIds::eNone;
    FP mVelX = {};
    FP mVelY = {};
    FP mSpriteScale = {};
    RGB16 mRGB;
    Scale mScale = Scale::Fg;
    s16 mYOffset = 0;
    s16 mXOffset = 0;
    enum VisualFlags : s16
    {
        eApplyShadowZoneColour = 0x1,
        eDoPurpleLightEffect = 0x2
    };
    BitField16<VisualFlags> mVisualFlags = {};
    Shadow* mShadow = nullptr;
};
