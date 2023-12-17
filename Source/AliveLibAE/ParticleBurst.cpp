#include "stdafx.h"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "../relive_lib/FixedPoint.hpp"

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

FP* ParticleBurst::Random_Speed(FP* random)
{
    const FP v2 = FP_FromRaw((static_cast<u32>(Math_NextRandom()) - 128) << (mUnknownCount & 0xFF));
    *random = v2 * GetSpriteScale();
    return random;
}

ParticleBurst::ParticleBurst(FP xpos, FP ypos, u32 particleCount, FP scale, BurstType type, s32 unknownCount)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eParticleBurst);

    // NOTE: likely a quick OWI hack for AE to improve the performance
    // of particle bursts in the PSX version
    if (particleCount > 5)
    {
        particleCount /= 2;
    }

    if (unknownCount > 13)
    {
        unknownCount = 13;
    }
    else if (unknownCount <= 0)
    {
        unknownCount = 1;
    }

    mUnknownCount = static_cast<s16>(unknownCount);
    SetSpriteScale(scale);
    mParticleItems = relive_new ParticleBurst_Item[particleCount];
    if (mParticleItems)
    {
        mType = type;
        switch (mType)
        {
            case BurstType::eFallingRocks_0:
            {
                Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::Explosion_Rocks));
                GetAnimation().SetSemiTrans(false);
                GetAnimation().SetBlending(true);
                break;
            }

            case BurstType::eSticks_1:
            {
                Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::Explosion_Sticks));
                GetAnimation().SetSemiTrans(false);
                GetAnimation().SetBlending(true);
                break;
            }

            case BurstType::eBigPurpleSparks_2:
            {
                Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                GetAnimation().SetSemiTrans(true);
                GetAnimation().SetBlending(true);
                GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                break;
            }

            case BurstType::eBigRedSparks_3:
            case BurstType::eGreenSparks_5:
            case BurstType::eSmallPurpleSparks_6:
            {
                Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                GetAnimation().SetBlendMode(relive::TBlendModes::eBlend_1);
                GetAnimation().SetSemiTrans(true);
                GetAnimation().SetBlending(false);

                if (mType == BurstType::eBigRedSparks_3)
                {
                    GetAnimation().SetRGB(254, 148, 18);
                }
                else if (mType == BurstType::eSmallPurpleSparks_6)
                {
                    GetAnimation().SetRGB(127, 127, 127);
                }
                else
                {
                    GetAnimation().SetRGB(0, 255, 32);
                }
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
                //mParticleItems[i].field_18_animation.mFlags.Set(AnimFlags::eBit25_bDecompressDone); // TODO: HIWORD &= ~0x0100u ??

                mParticleItems[i].field_18_animation.SetSemiTrans(GetAnimation().GetSemiTrans());

                mParticleItems[i].field_18_animation.SetBlending(GetAnimation().GetBlending());

                if (type == BurstType::eBigPurpleSparks_2)
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

                Random_Speed(&mParticleItems[i].field_C_x_speed);
                Random_Speed(&mParticleItems[i].field_10_y_speed);
                // OG bug sign could be wrong here as it called random again to Abs() it!
                FP zRandom = {};
                mParticleItems[i].field_14_z_speed = -FP_Abs(*Random_Speed(&zRandom));
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

        if (mUnknownCount == 9)
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

        if (pItem->field_8_z + FP_FromInteger(300) < FP_FromInteger(15))
        {
            pItem->field_14_z_speed = -pItem->field_14_z_speed;
            pItem->field_8_z += pItem->field_14_z_speed;

            // TODO: Never used by OG ??
            // Math_RandomRange_496AB0(-64, 46);

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

    const FP camX = gScreenManager->CamXPos();
    const FP camY = gScreenManager->CamYPos();

    bool bFirst = true;
    for (s32 i = 0; i < mParticleCount; i++)
    {
        ParticleBurst_Item* pItem = &mParticleItems[i];
        if (pItem->x >= camX && pItem->x <= camX + FP_FromInteger(640))
        {
            if (pItem->y >= camY && pItem->y <= camY + FP_FromInteger(240))
            {
                if (bFirst)
                {
                    GetAnimation().SetSpriteScale(FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300)));
                    GetAnimation().SetSpriteScale(GetAnimation().GetSpriteScale() * GetSpriteScale());
                    GetAnimation().SetSpriteScale(GetAnimation().GetSpriteScale() * FP_FromInteger(mUnknownCount) / FP_FromInteger(13));

                    if (GetAnimation().GetSpriteScale() <= FP_FromInteger(1))
                    {
                        GetAnimation().VRender(
                            FP_GetExponent(pItem->x - camX),
                            FP_GetExponent(pItem->y - camY),
                            ot, 0, 0);
                        if (mUnknownCount == 9)
                        {
                            FadeoutRgb(GetAnimation().GetRgb());
                        }
                        bFirst = false;
                    }
                }
                else
                {
                    pItem->field_18_animation.mSpriteScale = FP_FromInteger(100) / (pItem->field_8_z + FP_FromInteger(300));
                    pItem->field_18_animation.mSpriteScale *= GetSpriteScale();
                    pItem->field_18_animation.mSpriteScale *= FP_FromInteger(mUnknownCount) / FP_FromInteger(13);

                    if (pItem->field_18_animation.mSpriteScale <= FP_FromInteger(1))
                    {
                        pItem->field_18_animation.VRender(
                            FP_GetExponent(pItem->x - camX),
                            FP_GetExponent(pItem->y - camY),
                            ot, 0, 0);
                        if (mUnknownCount == 9)
                        {
                            FadeoutRgb(pItem->field_18_animation.GetRgb());
                        }
                    }
                }
            }
        }
    }
}

