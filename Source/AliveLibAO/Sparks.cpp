#include "stdafx_ao.h"
#include "Function.hpp"
#include "Sparks.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

namespace AO {

Sparks::Sparks(FP xpos, FP ypos, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eSpark;

    const AnimRecord& rec = AO::AnimRec(AnimId::Zap_Sparks);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    field_10_anim.mBlue = 80;
    field_10_anim.mGreen = 80;
    field_10_anim.mRed = 80;

    field_CC_bApplyShadows &= ~1u;

    field_10_anim.mRenderLayer = Layer::eLayer_FG1_37;
    field_10_anim.mRenderMode = TPageAbr::eBlend_1;

    field_BC_sprite_scale = scale * ((FP_FromInteger(Math_NextRandom() % 6) / FP_FromInteger(10)) + FP_FromDouble(0.2));
    field_EA_random = Math_RandomRange_450F20(0, 16);

    field_A8_xpos = xpos;
    field_AC_ypos = ypos;

    field_B4_velx = FP_FromInteger(Math_RandomRange_450F20(-8, 8));
    field_B8_vely = FP_FromInteger(Math_RandomRange_450F20(-6, -3));
}

void Sparks::VUpdate()
{
    if (field_EA_random > 0)
    {
        field_EA_random--;
    }

    if (field_EA_random == 0)
    {
        field_10_anim.Set_Animation_Data(1492, 0);
        field_EA_random = -1;
    }

    field_B8_vely += FP_FromDouble(0.8);

    field_B4_velx = field_B4_velx * FP_FromDouble(0.9);
    field_B8_vely = field_B8_vely * FP_FromDouble(0.9);

    field_B4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
    field_B8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;

    if (!gMap.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

void Sparks::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

} // namespace AO
