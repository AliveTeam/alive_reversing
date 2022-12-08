#include "stdafx.h"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Function.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "stdlib.hpp"
#include "../relive_lib/AnimationUnknown.hpp"
#include "FixedPoint.hpp"

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


ParticleBurst::ParticleBurst(FP xpos, FP ypos, u32 numOfParticles, FP scale, BurstType type, s32 count)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eParticleBurst);

    // TODO: Check it
    if (numOfParticles > 5)
    {
        numOfParticles /= 2;
    }

    if (count > 13)
    {
        count = 13;
    }
    else if (count <= 0)
    {
        count = 1;
    }

    field_106_count = static_cast<s16>(count);
    SetSpriteScale(scale);
    field_F8_pRes = relive_new ParticleBurst_Item[numOfParticles];
    if (field_F8_pRes)
    {
        field_104_type = type;
        switch (field_104_type)
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
                GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                break;
            }

            case BurstType::eBigRedSparks_3:
            case BurstType::eGreenSparks_5:
            case BurstType::eSmallPurpleSparks_6:
            {
                Animation_Init(ResourceManagerWrapper::LoadAnimation(AnimId::DeathFlare_2));
                GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                GetAnimation().SetSemiTrans(true);
                GetAnimation().SetBlending(false);

                if (field_104_type == BurstType::eBigRedSparks_3)
                {
                    GetAnimation().SetRGB(254, 148, 18);
                }
                else if (field_104_type == BurstType::eSmallPurpleSparks_6)
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

            field_FC_number_of_particles = static_cast<s16>(numOfParticles);
            field_100_timer = sGnFrame + 91;
            mXPos = xpos;
            mYPos = ypos;

            for (u32 i = 0; i < numOfParticles; i++)
            {
                field_F8_pRes[i].field_18_animation.field_68_anim_ptr = &GetAnimation();
                field_F8_pRes[i].field_18_animation.SetRenderLayer(GetAnimation().GetRenderLayer());
                field_F8_pRes[i].field_18_animation.field_6C_scale = FP_FromDouble(0.95) * GetSpriteScale();

                field_F8_pRes[i].field_18_animation.SetRender(true);
                //field_F8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBit25_bDecompressDone); // TODO: HIWORD &= ~0x0100u ??

                field_F8_pRes[i].field_18_animation.SetSemiTrans(GetAnimation().GetSemiTrans());

                field_F8_pRes[i].field_18_animation.SetBlending(GetAnimation().GetBlending());

                if (type == BurstType::eBigPurpleSparks_2)
                {
                    if (i % 2)
                    {
                        field_F8_pRes[i].field_18_animation.SetBlending(true);
                    }
                }

                field_F8_pRes[i].field_18_animation.SetRGB(GetAnimation().GetRgb());

                field_F8_pRes[i].x = mXPos;
                field_F8_pRes[i].y = mYPos;
                field_F8_pRes[i].field_8_z = FP_FromInteger(0);

                Random_Speed(&field_F8_pRes[i].field_C_x_speed);
                Random_Speed(&field_F8_pRes[i].field_10_y_speed);
                // OG bug sign could be wrong here as it called random again to Abs() it!
                FP zRandom = {};
                field_F8_pRes[i].field_14_z_speed = -FP_Abs(*Random_Speed(&zRandom));
            }
        }
    }
    else
    {
        SetDead(true);
    }
}

FP* ParticleBurst::Random_Speed(FP* random)
{
    const FP v2 = FP_FromRaw((static_cast<u32>(Math_NextRandom()) - 128) << (field_106_count & 0xFF));
    *random = v2 * GetSpriteScale();
    return random;
}

ParticleBurst::~ParticleBurst()
{
    relive_delete[] field_F8_pRes;
}

void ParticleBurst::VRender(PrimHeader** ppOt)
{
    bool bFirst = true;
    if (gNumCamSwappers == 0)
    {
        GetAnimation().SetSpriteScale(GetSpriteScale());
        const FP camX = pScreenManager->CamXPos();
        const FP camY = pScreenManager->CamYPos();

        for (s32 i = 0; i < field_FC_number_of_particles; i++)
        {
            if (field_F8_pRes[i].x < camX)
            {
                continue;
            }

            if (field_F8_pRes[i].x > camX + FP_FromInteger(640))
            {
                continue;
            }

            if (field_F8_pRes[i].y < camY)
            {
                continue;
            }

            if (field_F8_pRes[i].y > camY + FP_FromInteger(240))
            {
                continue;
            }

            const FP zPos = field_F8_pRes[i].field_8_z;

            // TODO: Much duplicated code in each branch
            if (bFirst)
            {
                GetAnimation().SetSpriteScale(FP_FromInteger(100) / (zPos + FP_FromInteger(300)));
                GetAnimation().SetSpriteScale(GetAnimation().GetSpriteScale() * GetSpriteScale());
                GetAnimation().SetSpriteScale(GetAnimation().GetSpriteScale() * FP_FromInteger(field_106_count) / FP_FromInteger(13));

                if (GetAnimation().GetSpriteScale() <= FP_FromInteger(1))
                {
                    GetAnimation().VRender(
                        FP_GetExponent(field_F8_pRes[i].x - camX),
                        FP_GetExponent(field_F8_pRes[i].y - camY),
                        ppOt,
                        0,
                        0);

                    bFirst = false;

                    PSX_RECT frameRect = {};
                    GetAnimation().Get_Frame_Rect(&frameRect);
                    if (field_106_count == 9)
                    {
                        RGB16& rgb = GetAnimation().GetRgb();
                        if (rgb.r > 5)
                        {
                            rgb.r -= 6;
                        }
                        else
                        {
                            rgb.r = 0;
                        }

                        if (rgb.g > 5)
                        {
                            rgb.g -= 6;
                        }
                        else
                        {
                            rgb.g = 0;
                        }

                        if (rgb.b > 5)
                        {
                            rgb.b -= 6;
                        }
                        else
                        {
                            rgb.b = 0;
                        }
                    }
                }
            }
            else
            {
                field_F8_pRes[i].field_18_animation.field_6C_scale = FP_FromInteger(100) / (zPos + FP_FromInteger(300));
                field_F8_pRes[i].field_18_animation.field_6C_scale *= GetSpriteScale();
                field_F8_pRes[i].field_18_animation.field_6C_scale *= FP_FromInteger(field_106_count) / FP_FromInteger(13);

                if (field_F8_pRes[i].field_18_animation.field_6C_scale <= FP_FromInteger(1))
                {
                    field_F8_pRes[i].field_18_animation.VRender(
                        FP_GetExponent(field_F8_pRes[i].x - camX),
                        FP_GetExponent(field_F8_pRes[i].y - camY),
                        ppOt,
                        0,
                        0);

                    PSX_RECT frameRect = {};
                    field_F8_pRes[i].field_18_animation.GetRenderedSize(&frameRect);

                    if (field_106_count == 9)
                    {
                        RGB16& rgb = field_F8_pRes[i].field_18_animation.GetRgb();
                        if (rgb.r > 5)
                        {
                            rgb.r -= 6;
                        }
                        else
                        {
                            rgb.r = 0;
                        }

                        if (rgb.g > 5)
                        {
                            rgb.g -= 6;
                        }
                        else
                        {
                            rgb.g = 0;
                        }

                        if (rgb.b > 5)
                        {
                            rgb.b -= 6;
                        }
                        else
                        {
                            rgb.b = 0;
                        }
                    }
                }
            }
        }
    }
}

void ParticleBurst::VUpdate()
{
    const s32 v3 = GetSpriteScale() != FP_FromInteger(1) ? 2 : 4;
    for (s32 i = 0; i < field_FC_number_of_particles; i++)
    {
        field_F8_pRes[i].x += field_F8_pRes[i].field_C_x_speed;
        field_F8_pRes[i].y += field_F8_pRes[i].field_10_y_speed;
        field_F8_pRes[i].field_8_z += field_F8_pRes[i].field_14_z_speed;

        field_F8_pRes[i].field_10_y_speed += FP_FromDouble(0.25);

        if (field_106_count == 9)
        {
            if ((sGnFrame + i) & v3)
            {
                field_F8_pRes[i].x -= FP_FromInteger(1);
            }
            else
            {
                field_F8_pRes[i].x += FP_FromInteger(1);
            }
        }

        if (field_F8_pRes[i].field_8_z + FP_FromInteger(300) < FP_FromInteger(15))
        {
            field_F8_pRes[i].field_14_z_speed = -field_F8_pRes[i].field_14_z_speed;
            field_F8_pRes[i].field_8_z += field_F8_pRes[i].field_14_z_speed;

            // TODO: Never used by OG ??
            //Math_RandomRange_496AB0(-64, 46);

            // TODO: This might be wrong
            const s16 volume = static_cast<s16>(Math_RandomRange(-10, 10) + ((field_100_timer - sGnFrame) / 91) + 25);

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

    if (static_cast<s32>(sGnFrame) > field_100_timer)
    {
        SetDead(true);
    }

    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }
}
