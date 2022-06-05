#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoistRocksEffect.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Math.hpp"
#include "Collisions.hpp"

namespace AO {

void HoistParticle::VUpdate()
{
    VUpdate_431BD0();
}

HoistParticle::HoistParticle(FP xpos, FP ypos, FP scale, s32 frameTableOffset)
{
    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kHoistRocksAOResID, 1, 0);
    s32 maxW = 7;
    if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
    {
        maxW = 5;
    }
    Animation_Init_417FD0(frameTableOffset, maxW, 4, ppRes, 1);

    field_BC_sprite_scale = scale;

    if (scale == FP_FromInteger(1))
    {
        field_10_anim.mRenderLayer = Layer::eLayer_BeforeShadow_25;
    }
    else
    {
        field_10_anim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }

    field_E4_bHitGround = 0;
}

void HoistParticle::VUpdate_431BD0()
{
    if (field_B8_vely >= (field_BC_sprite_scale * FP_FromInteger(10)))
    {
        if (!gMap.Is_Point_In_Current_Camera_4449C0(
                field_B2_lvl_number,
                field_B0_path_number,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                0))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
    }
    else
    {
        field_B8_vely += (field_BC_sprite_scale * FP_FromDouble(0.6));
    }

    const FP oldY = mBaseAnimatedWithPhysicsGameObject_YPos;
    mBaseAnimatedWithPhysicsGameObject_YPos += field_B8_vely;

    if (field_E4_bHitGround == 0)
    {
        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (sCollisions_DArray_504C6C->RayCast(
                mBaseAnimatedWithPhysicsGameObject_XPos,
                oldY,
                mBaseAnimatedWithPhysicsGameObject_XPos,
                mBaseAnimatedWithPhysicsGameObject_YPos,
                &pLine,
                &hitX,
                &hitY,
                field_BC_sprite_scale != FP_FromDouble(0.5) ? 7 : 0x70))
        {
            mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
            field_B8_vely = (field_B8_vely * FP_FromDouble(-0.3));
            field_E4_bHitGround = 1;
        }
    }
}

void HoistRocksEffect::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

HoistRocksEffect::~HoistRocksEffect()
{
    gMap.TLV_Reset(field_18_tlvInfo, -1, 0, 0);
}

HoistRocksEffect::HoistRocksEffect(Path_Hoist* pTlv, s32 tlvInfo)
    : BaseGameObject(TRUE, 0)
{
    field_18_tlvInfo = tlvInfo;
    field_10_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_14_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
}

void HoistRocksEffect::VUpdate()
{
    const auto rnd = Math_RandomRange_450F20(1, 4) - 2;
    if (rnd)
    {
        if (rnd == 1)
        {
            const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock2);
            s32 frameTableOffset = normalHoist.mFrameTableOffset;
            if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
            {
                const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock2);
                frameTableOffset = ruptureHoist.mFrameTableOffset;
            }
            ao_new<HoistParticle>(
                field_10_xpos + FP_FromInteger(Math_RandomRange_450F20(-8, 8)),
                field_14_ypos + FP_FromInteger(Math_RandomRange_450F20(-4, 4)),
                FP_FromInteger(1),
                frameTableOffset);

            mBaseGameObjectUpdateDelay = Math_RandomRange_450F20(30, 50);
        }
        else
        {
            const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock3);
            s32 frameTableOffset = normalHoist.mFrameTableOffset;
            if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
            {
                const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock3);
                frameTableOffset = ruptureHoist.mFrameTableOffset;
            }
            ao_new<HoistParticle>(
                field_10_xpos + FP_FromInteger(Math_RandomRange_450F20(-8, 8)),
                field_14_ypos + FP_FromInteger(Math_RandomRange_450F20(-4, 4)),
                FP_FromInteger(1),
                frameTableOffset);

            mBaseGameObjectUpdateDelay = Math_RandomRange_450F20(5, 10);
        }
    }
    else
    {
        const AnimRecord& normalHoist = AO::AnimRec(AnimId::AO_HoistRock1);
        s32 frameTableOffset = normalHoist.mFrameTableOffset;
        if (gMap.mCurrentLevel == LevelIds::eRuptureFarms_1 || gMap.mCurrentLevel == LevelIds::eRuptureFarmsReturn_13)
        {
            const AnimRecord& ruptureHoist = AO::AnimRec(AnimId::RuptureFarms_HoistRock1);
            frameTableOffset = ruptureHoist.mFrameTableOffset;
        }
        ao_new<HoistParticle>(
            field_10_xpos + FP_FromInteger(Math_RandomRange_450F20(-8, 8)),
            field_14_ypos + FP_FromInteger(Math_RandomRange_450F20(-4, 4)),
            FP_FromInteger(1),
            frameTableOffset);

        mBaseGameObjectUpdateDelay = Math_RandomRange_450F20(10, 20);
    }
}

} // namespace AO
