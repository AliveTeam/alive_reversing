#include "stdafx.h"
#include "ParticleBurst.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Function.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "ScreenManager.hpp"
#include "Map.hpp"
#include "stdlib.hpp"

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
    mSpriteScale = scale;
    field_F4_ppRes = ResourceManager::Allocate_New_Locked_Resource(ResourceManager::ResourceType::Resource_3DGibs, 0, sizeof(ParticleBurst_Item) * numOfParticles);
    if (field_F4_ppRes)
    {
        field_F8_pRes = reinterpret_cast<ParticleBurst_Item*>(*field_F4_ppRes);
        for (u32 i = 0; i < numOfParticles; i++)
        {
            // Placement new each element
            new (&field_F8_pRes[i]) ParticleBurst_Item();
        }

        field_104_type = type;
        switch (field_104_type)
        {
            case BurstType::eFallingRocks_0:
            {
                const AnimRecord& rocksRec = AnimRec(AnimId::Explosion_Rocks);
                Animation_Init(AnimId::Explosion_Rocks, Add_Resource(ResourceManager::Resource_Animation, rocksRec.mResourceId));
                mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);
                break;
            }

            case BurstType::eSticks_1:
            {
                const AnimRecord& sticksRec = AnimRec(AnimId::Explosion_Sticks);
                Animation_Init(AnimId::Explosion_Sticks, Add_Resource(ResourceManager::Resource_Animation, sticksRec.mResourceId));
                mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);
                break;
            }

            case BurstType::eBigPurpleSparks_2:
            {
                const AnimRecord& flareRec = AnimRec(AnimId::DeathFlare_2);
                Animation_Init(AnimId::DeathFlare_2, Add_Resource(ResourceManager::Resource_Animation, flareRec.mResourceId));
                mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
                mAnim.mFlags.Set(AnimFlags::eBit16_bBlending);
                mAnim.mRenderMode = TPageAbr::eBlend_1;
                break;
            }

            case BurstType::eBigRedSparks_3:
            case BurstType::eGreenSparks_5:
            case BurstType::eSmallPurpleSparks_6:
            {
                const AnimRecord& flareRec = AnimRec(AnimId::DeathFlare_2);
                Animation_Init(AnimId::DeathFlare_2, Add_Resource(ResourceManager::Resource_Animation, flareRec.mResourceId));
                mAnim.mRenderMode = TPageAbr::eBlend_1;
                mAnim.mFlags.Set(AnimFlags::eBit15_bSemiTrans);
                mAnim.mFlags.Clear(AnimFlags::eBit16_bBlending);

                if (field_104_type == BurstType::eBigRedSparks_3)
                {
                    mAnim.mRed = 254;
                    mAnim.mGreen = 148;
                    mAnim.mBlue = 18;
                }
                else if (field_104_type == BurstType::eSmallPurpleSparks_6)
                {
                    mAnim.mRed = 127;
                    mAnim.mGreen = 127;
                    mAnim.mBlue = 127;
                }
                else
                {
                    mAnim.mRed = 0;
                    mAnim.mGreen = 255;
                    mAnim.mBlue = 32;
                }
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

            field_FC_number_of_particles = static_cast<s16>(numOfParticles);
            field_100_timer = sGnFrame + 91;
            mXPos = xpos;
            mYPos = ypos;

            for (u32 i = 0; i < numOfParticles; i++)
            {
                field_F8_pRes[i].field_18_animation.field_68_anim_ptr = &mAnim;
                field_F8_pRes[i].field_18_animation.mRenderLayer = mAnim.mRenderLayer;
                field_F8_pRes[i].field_18_animation.field_6C_scale = FP_FromDouble(0.95) * mSpriteScale;

                field_F8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBit3_Render);
                field_F8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBit25_bDecompressDone); // TODO: HIWORD &= ~0x0100u ??

                field_F8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBit15_bSemiTrans, mAnim.mFlags.Get(AnimFlags::eBit15_bSemiTrans));

                field_F8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBit16_bBlending, mAnim.mFlags.Get(AnimFlags::eBit16_bBlending));

                if (type == BurstType::eBigPurpleSparks_2)
                {
                    if (i % 2)
                    {
                        field_F8_pRes[i].field_18_animation.mFlags.Set(AnimFlags::eBit16_bBlending);
                    }
                }

                field_F8_pRes[i].field_18_animation.mRed = mAnim.mRed;
                field_F8_pRes[i].field_18_animation.mGreen = mAnim.mGreen;
                field_F8_pRes[i].field_18_animation.mBlue = mAnim.mBlue;

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
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

FP* ParticleBurst::Random_Speed(FP* random)
{
    const FP v2 = FP_FromRaw((Math_NextRandom() - 128) << LOBYTE(field_106_count));
    *random = v2 * mSpriteScale;
    return random;
}

ParticleBurst::~ParticleBurst()
{
    if (field_F4_ppRes)
    {
        ResourceManager::FreeResource_49C330(field_F4_ppRes);
    }
}

void ParticleBurst::VRender(PrimHeader** ppOt)
{
    bool bFirst = true;
    if (sNum_CamSwappers_5C1B66 == 0)
    {
        mAnim.field_14_scale = mSpriteScale;
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
                mAnim.field_14_scale = FP_FromInteger(100) / (zPos + FP_FromInteger(300));
                mAnim.field_14_scale *= mSpriteScale;
                mAnim.field_14_scale *= FP_FromInteger(field_106_count) / FP_FromInteger(13);

                if (mAnim.field_14_scale <= FP_FromInteger(1))
                {
                    mAnim.VRender(
                        FP_GetExponent(field_F8_pRes[i].x - camX),
                        FP_GetExponent(field_F8_pRes[i].y - camY),
                        ppOt,
                        0,
                        0);

                    bFirst = false;

                    PSX_RECT frameRect = {};
                    mAnim.Get_Frame_Rect(&frameRect);
                    if (field_106_count == 9)
                    {
                        if (mAnim.mRed > 5)
                        {
                            mAnim.mRed -= 6;
                        }
                        else
                        {
                            mAnim.mRed = 0;
                        }

                        if (mAnim.mGreen > 5)
                        {
                            mAnim.mGreen -= 6;
                        }
                        else
                        {
                            mAnim.mGreen = 0;
                        }

                        if (mAnim.mBlue > 5)
                        {
                            mAnim.mBlue -= 6;
                        }
                        else
                        {
                            mAnim.mBlue = 0;
                        }
                    }
                    pScreenManager->InvalidateRectCurrentIdx(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h);
                }
            }
            else
            {
                field_F8_pRes[i].field_18_animation.field_6C_scale = FP_FromInteger(100) / (zPos + FP_FromInteger(300));
                field_F8_pRes[i].field_18_animation.field_6C_scale *= mSpriteScale;
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
                        if (field_F8_pRes[i].field_18_animation.mRed > 5)
                        {
                            field_F8_pRes[i].field_18_animation.mRed -= 6;
                        }
                        else
                        {
                            field_F8_pRes[i].field_18_animation.mRed = 0;
                        }

                        if (field_F8_pRes[i].field_18_animation.mGreen > 5)
                        {
                            field_F8_pRes[i].field_18_animation.mGreen -= 6;
                        }
                        else
                        {
                            field_F8_pRes[i].field_18_animation.mGreen = 0;
                        }

                        if (field_F8_pRes[i].field_18_animation.mBlue > 5)
                        {
                            field_F8_pRes[i].field_18_animation.mBlue -= 6;
                        }
                        else
                        {
                            field_F8_pRes[i].field_18_animation.mBlue = 0;
                        }
                    }
                    pScreenManager->InvalidateRectCurrentIdx(
                        frameRect.x,
                        frameRect.y,
                        frameRect.w,
                        frameRect.h);
                }
            }
        }
    }
}

void ParticleBurst::VUpdate()
{
    const s32 v3 = mSpriteScale != FP_FromInteger(1) ? 2 : 4;
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
                SFX_Play_Camera(SoundEffect::ParticleBurst_27, volume, CameraPos::eCamLeft_3);
            }
            else if (next_rand >= 85)
            {
                SFX_Play_Camera(SoundEffect::ParticleBurst_27, volume, CameraPos::eCamRight_4);
            }
            else
            {
                SFX_Play_Camera(SoundEffect::ParticleBurst_27, volume, CameraPos::eCamCurrent_0);
            }
        }
    }

    if (static_cast<s32>(sGnFrame) > field_100_timer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}
