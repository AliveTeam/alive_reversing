#include "stdafx.h"
#include "Sparks.hpp"
#include "Function.hpp"
#include "stdlib.hpp"

Sparks::Sparks(FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eSparks_22);

    const AnimRecord& rec = AnimRec(AnimId::Sparks);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_DC_bApplyShadows &= ~1u;

    field_20_animation.mRenderLayer = Layer::eLayer_FG1_37;
    field_20_animation.mRenderMode = TPageAbr::eBlend_1;

    field_20_animation.mBlue = 80;
    field_20_animation.mGreen = 80;
    field_20_animation.mRed = 80;

    field_CC_sprite_scale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
    field_FA_16_random = Math_RandomRange(0, 16);

    field_B8_xpos = xpos;
    field_BC_ypos = ypos;

    field_C4_velx = FP_FromInteger(Math_RandomRange(-8, 8));
    field_C8_vely = FP_FromInteger(Math_RandomRange(-6, -3));
}

void Sparks::VUpdate()
{
    if (field_FA_16_random > 0)
    {
        field_FA_16_random--;
    }

    if (field_FA_16_random == 0)
    {
        const AnimRecord& animRec = AnimRec(AnimId::Sparks);
        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        field_FA_16_random = -1;
    }

    field_C8_vely += FP_FromDouble(0.8);

    field_C4_velx *= FP_FromDouble(0.9);
    field_C8_vely *= FP_FromDouble(0.9);

    field_C4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    field_C8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    field_BC_ypos += field_C8_vely;
    field_B8_xpos += field_C4_velx;

    if (!gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Sparks::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}
