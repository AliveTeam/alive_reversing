#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "PsxDisplay.hpp"
#include "CameraSwapper.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Grid.hpp"

namespace AO {

struct ParticleBurst_Item final
{
    FP field_0_x;
    FP field_4_y;
    FP field_8_z;
    FP field_C_x_speed;
    FP field_10_y_speed;
    FP field_14_z_speed;
    AnimationUnknown field_18_animation;
};
ALIVE_ASSERT_SIZEOF(ParticleBurst_Item, 0x88);

static inline FP Random_Speed(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << 13) * scale;
}

ParticleBurst::ParticleBurst(FP xpos, FP ypos, s32 particleCount, FP scale, BurstType type)
{
    mBaseGameObjectTypeId = ReliveTypes::eParticleBurst;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    field_E4_ppRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::ResourceType::Resource_3DGibs, 0, sizeof(ParticleBurst_Item) * particleCount);
    if (field_E4_ppRes)
    {
        field_E8_pRes = reinterpret_cast<ParticleBurst_Item*>(*field_E4_ppRes);
        for (s32 i = 0; i < particleCount; i++)
        {
            // Placement new each element
            new (&field_E8_pRes[i]) ParticleBurst_Item();
        }

        field_F4_type = type;
        switch (type)
        {
            case BurstType::eFallingRocks_0:
            {
                const AnimRecord& rockRec = AO::AnimRec(AnimId::Rock_Gib);
                u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rockRec.mResourceId, 1, 0);
                Animation_Init_417FD0(rockRec.mFrameTableOffset, rockRec.mMaxW, rockRec.mMaxH, ppRes, 1);
                mAnim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
                mAnim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
                break;
            }

            case BurstType::eSticks_1:
            {
                const AnimRecord& sticksRec = AO::AnimRec(AnimId::Stick_Gib);
                u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, sticksRec.mResourceId, 1, 0);
                Animation_Init_417FD0(sticksRec.mFrameTableOffset, sticksRec.mMaxW, sticksRec.mMaxH, ppRes, 1);
                scale = FP_FromDouble(0.4) * scale;
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit16_bBlending);
                break;
            }

            case BurstType::eBigPurpleSparks_2:
            {
                const AnimRecord& flareRec = AO::AnimRec(AnimId::DeathFlare_2);
                u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, flareRec.mResourceId, 1, 0);
                Animation_Init_417FD0(flareRec.mFrameTableOffset, flareRec.mMaxW, flareRec.mMaxH, ppRes, 1);
                mAnim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
                mAnim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
                mAnim.field_B_render_mode = TPageAbr::eBlend_1;
                break;
            }

            case BurstType::eBigRedSparks_3:
            {
                const AnimRecord& flareRec = AO::AnimRec(AnimId::DeathFlare_2);
                u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, flareRec.mResourceId, 1, 0);
                Animation_Init_417FD0(flareRec.mFrameTableOffset, flareRec.mMaxW, flareRec.mMaxH, ppRes, 1);

                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderMode = TPageAbr::eBlend_1;
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans);
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit16_bBlending);

                mBaseAnimatedWithPhysicsGameObject_Anim.mRed = 254;
                mBaseAnimatedWithPhysicsGameObject_Anim.mGreen = 148;
                mBaseAnimatedWithPhysicsGameObject_Anim.mBlue = 18;
                break;
            }

            case BurstType::eMeat_4:
            {
                const AnimRecord& meatRec = AO::AnimRec(AnimId::Meat_Gib);
                u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, meatRec.mResourceId, 1, 0);
                Animation_Init_417FD0(meatRec.mFrameTableOffset, meatRec.mMaxW, meatRec.mMaxH, ppRes, 1);
                mAnim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
                mAnim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
                break;
            }

            default:
                break;
        }

        if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        }
        else
        {
            if (mBaseAnimatedWithPhysicsGameObject_SpriteScale == FP_FromInteger(1))
            {
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Above_FG1_39;
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
                mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_Above_FG1_Half_20;
            }

            field_EC_count = static_cast<s16>(particleCount);
            field_F0_timer = sGnFrame + 91;
            mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
            mBaseAnimatedWithPhysicsGameObject_YPos = ypos;

            for (s32 i = 0; i < particleCount; i++)
            {
                field_E8_pRes[i].field_18_animation.field_68_anim_ptr = &mBaseAnimatedWithPhysicsGameObject_Anim;
                field_E8_pRes[i].field_18_animation.mRenderLayer = mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer;
                field_E8_pRes[i].field_18_animation.field_6C_scale = FP_FromDouble(0.95) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;

                field_E8_pRes[i].field_18_animation.mAnimFlags.Set(AnimFlags::eBit3_Render);

                field_E8_pRes[i].field_18_animation.mAnimFlags.Set(AnimFlags::eBit15_bSemiTrans, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit15_bSemiTrans));

                field_E8_pRes[i].field_18_animation.mAnimFlags.Set(AnimFlags::eBit16_bBlending, mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit16_bBlending));

                if (type == BurstType::eBigPurpleSparks_2)
                {
                    if (i % 2)
                    {
                        field_E8_pRes[i].field_18_animation.mAnimFlags.Set(AnimFlags::eBit16_bBlending);
                    }
                }

                field_E8_pRes[i].field_18_animation.mRed = mBaseAnimatedWithPhysicsGameObject_Anim.mRed;
                field_E8_pRes[i].field_18_animation.mGreen = mBaseAnimatedWithPhysicsGameObject_Anim.mGreen;
                field_E8_pRes[i].field_18_animation.mBlue = mBaseAnimatedWithPhysicsGameObject_Anim.mBlue;

                field_E8_pRes[i].field_0_x = xpos;
                field_E8_pRes[i].field_4_y = ypos;
                field_E8_pRes[i].field_8_z = FP_FromInteger(0);

                field_E8_pRes[i].field_C_x_speed = Random_Speed(scale);
                field_E8_pRes[i].field_10_y_speed = -Random_Speed(scale);
                field_E8_pRes[i].field_14_z_speed = -FP_Abs(Random_Speed(scale));
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
            {
                mBaseAnimatedWithPhysicsGameObject_RGB.b = 60;
                mBaseAnimatedWithPhysicsGameObject_RGB.g = 60;
                mBaseAnimatedWithPhysicsGameObject_RGB.r = 60;
            }
        }
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

ParticleBurst::~ParticleBurst()
{
    if (field_E4_ppRes)
    {
        ResourceManager::FreeResource_455550(field_E4_ppRes);
    }
}

void ParticleBurst::VUpdate()
{
    for (s32 i = 0; i < field_EC_count; i++)
    {
        ParticleBurst_Item* pItem = &field_E8_pRes[i];

        pItem->field_0_x += pItem->field_C_x_speed;
        pItem->field_4_y += pItem->field_10_y_speed;
        pItem->field_8_z += pItem->field_14_z_speed;

        pItem->field_10_y_speed += FP_FromDouble(0.25);

        u16 result = 0;
        pItem->field_0_x = CamX_VoidSkipper(pItem->field_0_x, pItem->field_C_x_speed, 16, &result);
        pItem->field_4_y = CamY_VoidSkipper(pItem->field_4_y, pItem->field_10_y_speed, 16, &result);

        if (pItem->field_8_z + FP_FromInteger(300) < FP_FromInteger(15))
        {
            pItem->field_14_z_speed = -pItem->field_14_z_speed;
            pItem->field_8_z += pItem->field_14_z_speed;

            if (field_F4_type == BurstType::eMeat_4)
            {
                if (gMap.Is_Point_In_Current_Camera_4449C0(
                        gMap.mCurrentLevel,
                        gMap.mCurrentPath,
                        pItem->field_0_x,
                        pItem->field_4_y,
                        0))
                {
                    SFX_Play_Pitch(SoundEffect::KillEffect_78, 50, Math_RandomRange(-900, -300));
                }
            }
            else
            {
                // TODO: Never used by OG ??
                // Math_RandomRange(-64, 46);

                const s16 volume = static_cast<s16>(Math_RandomRange(-10, 10) + ((field_F0_timer - sGnFrame) / 91) + 25);

                const u8 next_rand = Math_NextRandom();
                if (next_rand < 43)
                {
                    SFX_Play_Camera(SoundEffect::ParticleBurst_32, volume, CameraPos::eCamLeft_3);
                }
                else if (next_rand >= 85)
                {
                    SFX_Play_Camera(SoundEffect::ParticleBurst_32, volume, CameraPos::eCamRight_4);
                }
                else
                {
                    SFX_Play_Camera(SoundEffect::ParticleBurst_32, volume, CameraPos::eCamCurrent_0);
                }
            }
        }
    }

    if (static_cast<s32>(sGnFrame) > field_F0_timer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void ParticleBurst::VRender(PrimHeader** ppOt)
{
    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = mBaseAnimatedWithPhysicsGameObject_SpriteScale;

    const FP_Point* pCamPos = pScreenManager->mCamPos;
    const FP screen_left = pCamPos->field_0_x - FP_FromInteger(pScreenManager->mCamXOff);
    const FP screen_right = pCamPos->field_0_x + FP_FromInteger(pScreenManager->mCamXOff);

    const FP screen_top = pCamPos->field_4_y + FP_FromInteger(pScreenManager->mCamYOff);
    const FP screen_bottom = pCamPos->field_4_y - FP_FromInteger(pScreenManager->mCamYOff);

    bool bFirst = true;
    for (s32 i = 0; i < field_EC_count; i++)
    {
        ParticleBurst_Item* pItem = &field_E8_pRes[i];
        if (pItem->field_0_x >= screen_left && pItem->field_0_x <= screen_right)
        {
            if (pItem->field_4_y >= screen_bottom && pItem->field_4_y <= screen_top)
            {
                PSX_RECT rect = {};
                if (bFirst)
                {
                    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300));
                    mBaseAnimatedWithPhysicsGameObject_Anim.VRender(
                        FP_GetExponent(PsxToPCX(pItem->field_0_x - screen_left, FP_FromInteger(11))),
                        FP_GetExponent(pItem->field_4_y - screen_bottom),
                        ppOt,
                        0,
                        0);
                    mBaseAnimatedWithPhysicsGameObject_Anim.Get_Frame_Rect(&rect);
                    bFirst = false;
                }
                else
                {
                    pItem->field_18_animation.field_6C_scale = FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300));
                    pItem->field_18_animation.VRender2(
                        FP_GetExponent(PsxToPCX(pItem->field_0_x - screen_left, FP_FromInteger(11))),
                        FP_GetExponent(pItem->field_4_y - screen_bottom),
                        ppOt);
                    pItem->field_18_animation.GetRenderedSize(&rect);
                }

                pScreenManager->InvalidateRect(
                    rect.x,
                    rect.y,
                    rect.w,
                    rect.h,
                    pScreenManager->mIdx);
            }
        }
    }
}

} // namespace AO
