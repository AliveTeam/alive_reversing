#include "stdafx.h"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "../Function.hpp"
#include "../Events.hpp"
#include "Sfx.hpp"
#include "ScreenManager.hpp"
#include "../AnimationUnknown.hpp"
#include "../FixedPoint.hpp"
#include "../GameType.hpp"
#include "MapWrapper.hpp"
#include "../Grid.hpp"
#include "../PsxDisplay.hpp"
#include "../../AliveLibAE/Game.hpp"
#include "../../AliveLibAE/Sfx.hpp"

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

FP ParticleBurst::Random_Speed(FP scale)
{
    return FP_FromRaw((static_cast<u32>(Math_NextRandom()) - 128) << mFineScale) * scale;
}

ParticleBurst::ParticleBurst(FP xpos, FP ypos, u32 particleCount, FP scale, BurstType type, s32 fineScale, bool bFadeOut)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eParticleBurst);
    SetSpriteScale(scale);

    if (GetGameType() == GameType::eAe)
    {
        // NOTE: likely a quick OWI hack for AE to improve the performance
        // of particle bursts in the PSX version
        if (particleCount > 5)
        {
            particleCount /= 2;
        }
    }

    if (fineScale > 13)
    {
        fineScale = 13;
    }
    else if (fineScale <= 0)
    {
        fineScale = 1;
    }
    mFineScale = static_cast<s16>(fineScale);

    mFadeout = bFadeOut;

    mParticleItems = relive_new ParticleBurst_Item[particleCount];
    if (mParticleItems)
    {
        mType = type;
        switch (mType)
        {
            case BurstType::eRocks:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Explosion_Rock));
                Animation_Init(GetAnimRes(AnimId::Explosion_Rock));
                GetAnimation().SetSemiTrans(false);
                GetAnimation().SetBlending(true);
                break;
            }

            case BurstType::eSticks:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Explosion_Stick));
                Animation_Init(GetAnimRes(AnimId::Explosion_Stick));
                if (GetGameType() == GameType::eAo)
                {
                    scale = FP_FromDouble(0.4) * scale;
                }
                GetAnimation().SetSemiTrans(false);
                GetAnimation().SetBlending(true);
                break;
            }

            case BurstType::eBigPurpleSparks:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                Animation_Init(GetAnimRes(AnimId::DeathFlare_2));
                GetAnimation().SetSemiTrans(true);
                GetAnimation().SetBlending(true);
                GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                break;
            }

            case BurstType::eBigRedSparks:
            case BurstType::eGreenSparks:
            case BurstType::eSmallPurpleSparks:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                Animation_Init(GetAnimRes(AnimId::DeathFlare_2));
                GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                GetAnimation().SetSemiTrans(true);
                GetAnimation().SetBlending(false);

                if (mType == BurstType::eBigRedSparks)
                {
                    GetAnimation().SetRGB(254, 148, 18);
                }
                else if (mType == BurstType::eSmallPurpleSparks)
                {
                    GetAnimation().SetRGB(127, 127, 127);
                }
                else
                {
                    GetAnimation().SetRGB(0, 255, 32);
                }
                break;
            }

            case BurstType::eMeat:
            {
                mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Meat_Gib));
                Animation_Init(GetAnimRes(AnimId::Meat_Gib));
                GetAnimation().SetSemiTrans(false);
                GetAnimation().SetBlending(true);
                break;
            }

            default:
                break;
        }

        if (GetListAddFailed())
        {
            SetDead(true);
        }
        else
        {
            if (GetSpriteScale() == FP_FromInteger(1))
            {
                SetScale(Scale::Fg);
                GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_39);
            }
            else
            {
                SetScale(Scale::Bg);
                GetAnimation().SetRenderLayer(Layer::eLayer_Above_FG1_Half_20);
            }

            mParticleCount = static_cast<s16>(particleCount);
            mAliveTimer = MakeTimer(91);
            mXPos = xpos;
            mYPos = ypos;

            for (u32 i = 0; i < particleCount; i++)
            {
                mParticleItems[i].field_18_animation.mAnimPtr = &GetAnimation();
                mParticleItems[i].field_18_animation.SetRenderLayer(GetAnimation().GetRenderLayer());
                mParticleItems[i].field_18_animation.mSpriteScale = FP_FromDouble(0.95) * GetSpriteScale();

                mParticleItems[i].field_18_animation.SetRender(true);

                mParticleItems[i].field_18_animation.SetSemiTrans(GetAnimation().GetSemiTrans());

                mParticleItems[i].field_18_animation.SetBlending(GetAnimation().GetBlending());

                if (type == BurstType::eBigPurpleSparks)
                {
                    if (i % 2)
                    {
                        mParticleItems[i].field_18_animation.SetBlending(true);
                    }
                }

                mParticleItems[i].field_18_animation.SetRGB(GetAnimation().GetRgb());

                mParticleItems[i].x = mXPos;
                mParticleItems[i].y = mYPos;
                mParticleItems[i].field_8_z = FP_FromInteger(0);

                mParticleItems[i].field_C_x_speed = Random_Speed(scale);
                mParticleItems[i].field_10_y_speed = GetGameType() == GameType::eAo ? -Random_Speed(scale) : Random_Speed(scale);
                // OG bug sign could be wrong here as it called random again to Abs() it!
                mParticleItems[i].field_14_z_speed = -FP_Abs(Random_Speed(scale));

                if (GetMap().mCurrentLevel == EReliveLevelIds::eStockYards || GetMap().mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
                {
                    mRGB.SetRGB(60, 60, 60);
                }
            }
        }
    }
    else
    {
        SetDead(true);
    }
}

ParticleBurst::~ParticleBurst()
{
    relive_delete[] mParticleItems;
}

void ParticleBurst::VUpdate()
{
    const s32 v3 = GetSpriteScale() != FP_FromInteger(1) ? 2 : 4;
    for (s32 i = 0; i < mParticleCount; i++)
    {
        ParticleBurst_Item* pItem = &mParticleItems[i];

        pItem->x += pItem->field_C_x_speed;
        pItem->y += pItem->field_10_y_speed;
        pItem->field_8_z += pItem->field_14_z_speed;

        pItem->field_10_y_speed += FP_FromDouble(0.25);

        // TODO: sort this out
        if (mFadeout)
        {
            if ((sGnFrame + i) & v3)
            {
                pItem->x -= FP_FromInteger(1);
            }
            else
            {
                pItem->x += FP_FromInteger(1);
            }
        }

        if (GetGameType() == GameType::eAo)
        {
            u16 result = 0;
            pItem->x = CamX_VoidSkipper(pItem->x, pItem->field_C_x_speed, 16, &result);
            pItem->y = CamY_VoidSkipper(pItem->y, pItem->field_10_y_speed, 16, &result);
        }

        if (pItem->field_8_z + FP_FromInteger(300) < FP_FromInteger(15))
        {
            pItem->field_14_z_speed = -pItem->field_14_z_speed;
            pItem->field_8_z += pItem->field_14_z_speed;

            if (mType == BurstType::eMeat)
            {
                if (GetMap().Is_Point_In_Current_Camera(
                        GetMap().mCurrentLevel,
                        GetMap().mCurrentPath,
                        pItem->x,
                        pItem->y,
                        0))
                {
                    SFX_Play_Pitch(relive::SoundEffects::KillEffect, 50, Math_RandomRange(-900, -300));
                }
            }
            else
            {
                // TODO: This might be wrong
                const s16 volume = static_cast<s16>(Math_RandomRange(-10, 10) + ((mAliveTimer - sGnFrame) / 91) + 25);

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

    if (sGnFrame > mAliveTimer)
    {
        SetDead(true);
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}

static inline void FadeoutRgb(RGB16& rgb)
{
    rgb.r = (rgb.r > 5) ? rgb.r - 6 : 0;
    rgb.g = (rgb.g > 5) ? rgb.g - 6 : 0;
    rgb.b = (rgb.b > 5) ? rgb.b - 6 : 0;
}

void ParticleBurst::VRender(OrderingTable& ot)
{
    if (gNumCamSwappers != 0)
    {
        return;
    }

    GetAnimation().SetSpriteScale(GetSpriteScale());

    bool bFirst = true;
    for (s32 i = 0; i < mParticleCount; i++)
    {
        ParticleBurst_Item* pItem = &mParticleItems[i];
        if (gScreenManager->InScreenBounds(pItem->x, pItem->y))
        {
            const auto bounds = gScreenManager->PerGameScreenBounds();

            auto renderX = pItem->x - bounds.screenLeft;
            const auto renderY = pItem->y - bounds.screenTop;

            if (GetGameType() == GameType::eAo)
            {
                renderX = PsxToPCX(renderX, FP_FromInteger(11));
            }

            if (bFirst)
            {
                GetAnimation().SetSpriteScale(FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300)));
                if (GetGameType() == GameType::eAe)
                {
                    GetAnimation().SetSpriteScale(GetAnimation().GetSpriteScale() * GetSpriteScale());
                }
                GetAnimation().SetSpriteScale(GetAnimation().GetSpriteScale() * FP_FromInteger(mFineScale) / FP_FromInteger(13));

                // NOTE: only AE was checking sprite scale <= 1
                if (GetAnimation().GetSpriteScale() <= FP_FromInteger(1) || GetGameType() == GameType::eAo)
                {
                    GetAnimation().VRender(
                        FP_GetExponent(renderX),
                        FP_GetExponent(renderY),
                        ot, 0, 0);
                    if (mFadeout)
                    {
                        FadeoutRgb(GetAnimation().GetRgb());
                    }
                    bFirst = false;
                }
            }
            else
            {
                pItem->field_18_animation.mSpriteScale = FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300));
                if (GetGameType() == GameType::eAe)
                {
                    pItem->field_18_animation.mSpriteScale *= GetSpriteScale();
                }
                pItem->field_18_animation.mSpriteScale *= FP_FromInteger(mFineScale) / FP_FromInteger(13);

                // NOTE: only AE was checking sprite scale <= 1
                if (pItem->field_18_animation.mSpriteScale <= FP_FromInteger(1) || GetGameType() == GameType::eAo)
                {
                    pItem->field_18_animation.VRender(
                        FP_GetExponent(renderX),
                        FP_GetExponent(renderY),
                        ot, 0, 0);
                    if (mFadeout)
                    {
                        FadeoutRgb(pItem->field_18_animation.GetRgb());
                    }
                }
            }
        }
    }
}

