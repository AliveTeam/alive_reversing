#include "stdafx_ao.h"
#include "Function.hpp"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "Game.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "BaseAliveGameObject.hpp"
#include "Grid.hpp"

namespace AO {

struct ParticleBurst_Item final
{
    FP x;
    FP y;
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
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eParticleBurst);
    mSpriteScale = scale;

    field_E8_pRes = relive_new ParticleBurst_Item[particleCount];
    if (field_E8_pRes)
    {
        field_F4_type = type;
        switch (type)
        {
            case BurstType::eFallingRocks_0:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Rock_Gib));
                Animation_Init(GetAnimRes(AnimId::Rock_Gib));
                mAnim.mFlags.Clear(AnimFlags::eSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBlending);
                break;
            }

            case BurstType::eSticks_1:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Stick_Gib));
                Animation_Init(GetAnimRes(AnimId::Stick_Gib));
                scale = FP_FromDouble(0.4) * scale;
                mAnim.mFlags.Clear(AnimFlags::eSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBlending);
                break;
            }

            case BurstType::eBigPurpleSparks_2:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                Animation_Init(GetAnimRes(AnimId::DeathFlare_2));
                mAnim.mFlags.Set(AnimFlags::eSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBlending);
                mAnim.mRenderMode = TPageAbr::eBlend_1;
                break;
            }

            case BurstType::eBigRedSparks_3:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                Animation_Init(GetAnimRes(AnimId::DeathFlare_2));

                mAnim.mRenderMode = TPageAbr::eBlend_1;
                mAnim.mFlags.Set(AnimFlags::eSemiTrans);
                mAnim.mFlags.Clear(AnimFlags::eBlending);

                mAnim.mRed = 254;
                mAnim.mGreen = 148;
                mAnim.mBlue = 18;
                break;
            }

            case BurstType::eMeat_4:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Meat_Gib));
                Animation_Init(GetAnimRes(AnimId::Meat_Gib));
                mAnim.mFlags.Clear(AnimFlags::eSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBlending);
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
            if (mSpriteScale == FP_FromInteger(1))
            {
                mScale = Scale::Fg;
                mAnim.mRenderLayer = Layer::eLayer_Above_FG1_39;
            }
            else
            {
                mScale = Scale::Bg;
                mAnim.mRenderLayer = Layer::eLayer_Above_FG1_Half_20;
            }

            field_EC_count = static_cast<s16>(particleCount);
            field_F0_timer = sGnFrame + 91;
            mXPos = xpos;
            mYPos = ypos;

            for (s32 i = 0; i < particleCount; i++)
            {
                field_E8_pRes[i].field_18_animation.field_68_anim_ptr = &mAnim;
                field_E8_pRes[i].field_18_animation.mRenderLayer = mAnim.mRenderLayer;
                field_E8_pRes[i].field_18_animation.field_6C_scale = FP_FromDouble(0.95) * mSpriteScale;

                field_E8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eRender);

                field_E8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eSemiTrans, mAnim.mFlags.Get(AnimFlags::eSemiTrans));

                field_E8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBlending, mAnim.mFlags.Get(AnimFlags::eBlending));

                if (type == BurstType::eBigPurpleSparks_2)
                {
                    if (i % 2)
                    {
                        field_E8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBlending);
                    }
                }

                field_E8_pRes[i].field_18_animation.mRed = mAnim.mRed;
                field_E8_pRes[i].field_18_animation.mGreen = mAnim.mGreen;
                field_E8_pRes[i].field_18_animation.mBlue = mAnim.mBlue;

                field_E8_pRes[i].x = xpos;
                field_E8_pRes[i].y = ypos;
                field_E8_pRes[i].field_8_z = FP_FromInteger(0);

                field_E8_pRes[i].field_C_x_speed = Random_Speed(scale);
                field_E8_pRes[i].field_10_y_speed = -Random_Speed(scale);
                field_E8_pRes[i].field_14_z_speed = -FP_Abs(Random_Speed(scale));
            }

            if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
            {
                mRGB.SetRGB(60, 60, 60);
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
    relive_delete[] field_E8_pRes;
}

void ParticleBurst::VUpdate()
{
    for (s32 i = 0; i < field_EC_count; i++)
    {
        ParticleBurst_Item* pItem = &field_E8_pRes[i];

        pItem->x += pItem->field_C_x_speed;
        pItem->y += pItem->field_10_y_speed;
        pItem->field_8_z += pItem->field_14_z_speed;

        pItem->field_10_y_speed += FP_FromDouble(0.25);

        u16 result = 0;
        pItem->x = CamX_VoidSkipper(pItem->x, pItem->field_C_x_speed, 16, &result);
        pItem->y = CamY_VoidSkipper(pItem->y, pItem->field_10_y_speed, 16, &result);

        if (pItem->field_8_z + FP_FromInteger(300) < FP_FromInteger(15))
        {
            pItem->field_14_z_speed = -pItem->field_14_z_speed;
            pItem->field_8_z += pItem->field_14_z_speed;

            if (field_F4_type == BurstType::eMeat_4)
            {
                if (gMap.Is_Point_In_Current_Camera(
                        gMap.mCurrentLevel,
                        gMap.mCurrentPath,
                        pItem->x,
                        pItem->y,
                        0))
                {
                    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 50, Math_RandomRange(-900, -300));
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
                    SFX_Play_Camera(relive::SoundEffects::ParticleBurst, volume, CameraPos::eCamLeft_3);
                }
                else if (next_rand >= 85)
                {
                    SFX_Play_Camera(relive::SoundEffects::ParticleBurst, volume, CameraPos::eCamRight_4);
                }
                else
                {
                    SFX_Play_Camera(relive::SoundEffects::ParticleBurst, volume, CameraPos::eCamCurrent_0);
                }
            }
        }
    }

    if (static_cast<s32>(sGnFrame) > field_F0_timer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (EventGet(kEventDeathReset))
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

    mAnim.field_14_scale = mSpriteScale;

    const FP_Point* pCamPos = pScreenManager->mCamPos;
    const FP screen_left = pCamPos->x - FP_FromInteger(pScreenManager->mCamXOff);
    const FP screen_right = pCamPos->x + FP_FromInteger(pScreenManager->mCamXOff);

    const FP screen_top = pCamPos->y + FP_FromInteger(pScreenManager->mCamYOff);
    const FP screen_bottom = pCamPos->y - FP_FromInteger(pScreenManager->mCamYOff);

    bool bFirst = true;
    for (s32 i = 0; i < field_EC_count; i++)
    {
        ParticleBurst_Item* pItem = &field_E8_pRes[i];
        if (pItem->x >= screen_left && pItem->x <= screen_right)
        {
            if (pItem->y >= screen_bottom && pItem->y <= screen_top)
            {
                PSX_RECT rect = {};
                if (bFirst)
                {
                    mAnim.field_14_scale = FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300));
                    mAnim.VRender(
                        FP_GetExponent(PsxToPCX(pItem->x - screen_left, FP_FromInteger(11))),
                        FP_GetExponent(pItem->y - screen_bottom),
                        ppOt,
                        0,
                        0);
                    mAnim.Get_Frame_Rect(&rect);
                    bFirst = false;
                }
                else
                {
                    pItem->field_18_animation.field_6C_scale = FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300));
                    pItem->field_18_animation.VRender(
                        FP_GetExponent(PsxToPCX(pItem->x - screen_left, FP_FromInteger(11))),
                        FP_GetExponent(pItem->y - screen_bottom),
                        ppOt, 0, 0);
                    pItem->field_18_animation.GetRenderedSize(&rect);
                }
            }
        }
    }
}

} // namespace AO
