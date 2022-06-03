#include "stdafx.h"
#include "BulletShell.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "Collisions.hpp"
#include "Sfx.hpp"

ALIVE_VAR(1, 0xBAF7E0, s16, sShellCount_BAF7E0, 0);

BulletShell::BulletShell(FP xpos, FP ypos, s16 direction, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    sShellCount_BAF7E0++;

    if (sShellCount_BAF7E0 >= 11)
    {
        mBaseGameObjectFlags.Clear(BaseGameObject::eDrawable_Bit4);
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        SetType(AETypes::eNone_0);
        const AnimRecord& rec = AnimRec(AnimId::Bullet_Shell);
        u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        field_CC_sprite_scale = scale;

        if (scale == FP_FromInteger(1))
        {
            field_20_animation.mRenderLayer = Layer::eLayer_Foreground_36;
        }
        else
        {
            field_20_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        }

        field_DC_bApplyShadows &= ~1u;
        field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, direction & 1);

        field_FC_hitCount = 0;

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        if (direction)
        {
            field_C4_velx = FP_FromInteger(Math_RandomRange(-6, -3));
        }
        else
        {
            field_C4_velx = FP_FromInteger(Math_RandomRange(3, 6));
        }
        field_C8_vely = FP_FromInteger(Math_RandomRange(-4, -1));
        field_100_speed = FP_FromInteger(1);
    }
}

BulletShell::~BulletShell()
{
    sShellCount_BAF7E0--;
}

void BulletShell::VUpdate()
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return;
    }

    field_B8_xpos += field_C4_velx;
    field_BC_ypos += field_C8_vely;

    field_C8_vely += field_100_speed;

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            field_B8_xpos,
            field_BC_ypos - field_C8_vely,
            field_B8_xpos,
            field_BC_ypos,
            &BaseAliveGameObjectCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 0x0F : 0xF0)
        == 1)
    {
        if (BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eFloor_0 ||
            BaseAliveGameObjectCollisionLine->field_8_type == eLineTypes::eBackgroundFloor_4)
        {
            field_BC_ypos = hitY - FP_FromInteger(1);
            field_C8_vely = -(field_C8_vely * FP_FromDouble(0.3));
            field_C4_velx = (field_C4_velx * FP_FromDouble(0.3));

            if (field_C4_velx < FP_FromInteger(0) && field_C4_velx > FP_FromInteger(-1))
            {
                field_C4_velx = FP_FromInteger(-1);
            }
            else if (field_C4_velx > FP_FromInteger(0) && field_C4_velx < FP_FromInteger(1))
            {
                field_C4_velx = FP_FromInteger(1);
            }

            s16 volume = 19 * (3 - field_FC_hitCount);
            if (volume <= 19)
            {
                volume = 19;
            }

            SFX_Play_Mono(SoundEffect::BulletShell_6, volume);
            field_FC_hitCount++;
        }
    }

    if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_FC_hitCount >= 3)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
