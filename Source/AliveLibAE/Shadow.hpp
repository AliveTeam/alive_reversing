#pragma once

#include <Function.hpp>
#include "Animation.hpp"
#include "BitField.hpp"
#include "../relive_lib/BaseAnimatedWithPhysicsGameObject.hpp"

class Shadow final
{
public:
    Shadow();
    ~Shadow();

    void Calculate_Position(FP xpos, FP ypos, PSX_RECT* frameRect, FP spriteScale, Scale scale);
    void Render(PrimHeader** ppOt);

private:
    s16 mX1 = 0;
    s16 mY1 = 0;
    s16 mX2 = 0;
    s16 mY2 = 0;
    FP mXPos = {};
    FP mYPos = {};
    FP mScale = {};

public:
    enum Flags
    {
        eShadowAtBottom = 0x1,
        eEnabled = 0x2,
    };
    BitField32<Flags> mFlags = {};
    Animation mAnim = {};
};
ALIVE_ASSERT_SIZEOF(Shadow, 0xB0);
