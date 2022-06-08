#include "stdafx.h"
#include "Leaf.hpp"
#include "Function.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include "stdlib.hpp"

ALIVE_VAR(1, 0x563aa4, u8, sLeafRandIdx_563AA4, 8);

Leaf::Leaf(FP xpos, FP ypos, FP xVel, FP yVel, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_D0_r = 100;
    field_D2_g = 100;
    field_D4_b = 100;

    const AnimRecord& rec = AnimRec(AnimId::Well_Leaf);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_CC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_20_animation.mRenderLayer = Layer::eLayer_27;
        field_D6_scale = 1;
    }
    else
    {
        field_20_animation.mRenderLayer = Layer::eLayer_8;
        field_D6_scale = 0;
    }

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

    field_C4_velx = xVel * field_CC_sprite_scale;
    field_C8_vely = yVel * field_CC_sprite_scale;

    sLeafRandIdx_563AA4++;

    field_F4_bHitSomething &= ~1u;

    s16 randLeftVol = Math_RandomRange(19, 24);
    if (field_CC_sprite_scale == FP_FromDouble(0.5))
    {
        randLeftVol -= 7;
    }

    const s16 randRightVol = Math_RandomRange(-900, -700);
    SFX_Play_Pitch(SoundEffect::Leaf_22, (3 * randLeftVol) / 4, randRightVol);
    SetUpdateDelay(1);
}

void Leaf::VUpdate()
{
    field_C8_vely += FP_FromDouble(0.5);

    field_C4_velx = field_C4_velx * FP_FromDouble(0.8);
    field_C8_vely = field_C8_vely * FP_FromDouble(0.8);

    const s32 randX = (sRandomBytes_546744[sLeafRandIdx_563AA4++] - 127);
    field_C4_velx += (field_CC_sprite_scale * (FP_FromInteger(randX) / FP_FromInteger(64)));

    const s32 randY = (sRandomBytes_546744[sLeafRandIdx_563AA4++] - 127);
    field_C8_vely += (field_CC_sprite_scale * (FP_FromInteger(randY) / FP_FromInteger(64)));

    const FP x2 = field_C4_velx + mBaseAnimatedWithPhysicsGameObject_XPos;
    const FP y2 = field_C8_vely + mBaseAnimatedWithPhysicsGameObject_YPos;

    PathLine* pLine = nullptr;
    FP hitX = {};
    FP hitY = {};
    const auto bCollision = sCollisions->Raycast(
        mBaseAnimatedWithPhysicsGameObject_XPos,
        mBaseAnimatedWithPhysicsGameObject_YPos,
        x2,
        y2,
        &pLine,
        &hitX,
        &hitY,
        eLineTypes::eFlyingSligCeiling_17); // TODO: Must be another set of constants, this makes no sense ??

    // Hit the floor, die but only if in background..
    if (bCollision && field_CC_sprite_scale == FP_FromDouble(0.5) && pLine->field_8_type == eLineTypes::eFloor_0)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    if (field_F4_bHitSomething & 1 || !bCollision || ((field_CC_sprite_scale != FP_FromDouble(0.5) || pLine->field_8_type != eLineTypes::eBackgroundFloor_4) && (field_CC_sprite_scale != FP_FromDouble(1.0) || pLine->field_8_type != eLineTypes::eFloor_0)))
    {
        mBaseAnimatedWithPhysicsGameObject_XPos = x2;
        mBaseAnimatedWithPhysicsGameObject_YPos = y2;
    }
    else
    {
        field_C4_velx = FP_FromInteger(0);
        field_C8_vely = FP_FromInteger(0);

        field_F4_bHitSomething |= 1u;

        mBaseAnimatedWithPhysicsGameObject_XPos = hitX;
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    // Out of the camera, die
    if (!gMap.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Leaf::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
