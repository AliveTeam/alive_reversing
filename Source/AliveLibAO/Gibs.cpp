#include "stdafx_ao.h"
#include "Function.hpp"
#include "Gibs.hpp"
#include "Math.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/ScreenManager.hpp"

namespace AO {

const Gib_Data kGibData_4C30B0[6] = {
    {AnimId::Abe_Head_Gib, AnimId::Abe_Arm_Gib, AnimId::Abe_Body_Gib},
    {AnimId::Slig_Head_Gib, AnimId::Slig_Arm_Gib, AnimId::Slig_Body_Gib},
    {AnimId::Slog_Head_Gib, AnimId::Slog_Body_Gib, AnimId::Slog_Body_Gib},
    {AnimId::Elum_Head_Gib, AnimId::Elum_Arm_Gib, AnimId::Elum_Body_Gib},
    {AnimId::Abe_Head_Gib, AnimId::Abe_Arm_Gib, AnimId::Abe_Body_Gib},
    {AnimId::Metal_Gib, AnimId::Metal_Gib, AnimId::Metal_Gib}};

const TintEntry sAbeGibTints_4C6438[5] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eDesert, 125u, 125u, 95u},
    {EReliveLevelIds::eDesertTemple, 120u, 120u, 90u},
    {EReliveLevelIds::eNone, 102u, 102u, 102u}};

const TintEntry sMudGibTints_4CD320[3] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eNone, 87u, 103u, 67u},
};

static FP GibRand(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << 13) * scale;
}

Gibs::Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_E4_pGibData = &kGibData_4C30B0[gibType];
    const AnimRecord& headRec = AO::AnimRec(field_E4_pGibData->field_0_head);
    u8** ppAnimData = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, headRec.mResourceId, 1, 0);

    // The base class renders the head gib
    Animation_Init(field_E4_pGibData->field_0_head, ppAnimData);

    mSpriteScale = scale;
    mXPos = xpos;
    mYPos = ypos + FP_FromInteger(2);

    field_F0_timer = sGnFrame + 91;

    if (scale == FP_FromInteger(1))
    {
        field_E8_z = FP_FromInteger(0);
        mAnim.mRenderLayer = Layer::eLayer_FG1_37;
        mScale = Scale::Fg;
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_E8_z = FP_FromInteger(100);
        mAnim.mRenderLayer = Layer::eLayer_Foreground_Half_17;
        mScale = Scale::Bg;
    }
    else
    {
        // Not a valid scale
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    mVelX = xOff + GibRand(scale);
    mVelY = yOff + GibRand(scale);

    if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
    {
        field_EC_dz = -(GibRand(scale) / FP_FromInteger(2));
    }
    else
    {
        field_EC_dz = GibRand(scale) / FP_FromInteger(2);
    }

    u8** ppPal = nullptr;
    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        if (gibType == GibType::Abe_0 || gibType == GibType::Mud_4)
        {
            ppPal = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kAbeblowAOResID, 0, 0);
        }
        else if (gibType == GibType::Slog_2)
        {
            ppPal = ResourceManager::GetLoadedResource(ResourceManager::Resource_Palt, AOResourceID::kSlogBlowAOResID, 0, 0);
        }
    }

    if (gibType == GibType::Abe_0)
    {
        SetTint(sAbeGibTints_4C6438, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::Mud_4)
    {
        SetTint(sMudGibTints_4CD320, gMap.mCurrentLevel);
    }

    field_5C4_parts_used_count = 7;

    GibPart* pPart = &field_F4_parts[0];
    for (s16 i = 0; i < field_5C4_parts_used_count; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, headRec.mResourceId, 1, 0); // add ref
            if (!pPart->field_18_animation.Init(field_E4_pGibData->field_4_arm, this, ppAnimData))
            {
                field_5C4_parts_used_count = i;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }
        }
        else
        {
            // 2 body parts
            ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, headRec.mResourceId, 1, 0); // add ref
            if (!pPart->field_18_animation.Init(field_E4_pGibData->field_8_body, this, ppAnimData))
            {
                field_5C4_parts_used_count = i;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }
        }

        pPart->field_18_animation.mRenderLayer = mAnim.mRenderLayer;
        pPart->field_18_animation.field_14_scale = scale;

        pPart->field_18_animation.mFlags.Clear(AnimFlags::eBit16_bBlending);
        pPart->field_18_animation.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

        pPart->field_18_animation.mRed = static_cast<u8>(mRGB.r);
        pPart->field_18_animation.mGreen = static_cast<u8>(mRGB.g);
        pPart->field_18_animation.mBlue = static_cast<u8>(mRGB.b);

        pPart->x = mXPos;
        pPart->y = mYPos;
        pPart->field_8_z = field_E8_z;

        pPart->field_C_dx = xOff + GibRand(scale);
        pPart->field_10_dy = yOff + GibRand(scale);

        if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
        {
            pPart->field_14_dz = -(GibRand(scale) / FP_FromInteger(2));
        }
        else
        {
            pPart->field_14_dz = GibRand(scale) / FP_FromInteger(2);
        }

        if (ppPal)
        {
            pPart->field_18_animation.LoadPal(ppPal, 0);
        }

        pPart++;
    }
}

void Gibs::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;
    field_E8_z += field_EC_dz;

    mVelY += FP_FromDouble(0.25);

    if (field_E8_z + FP_FromInteger(100) < FP_FromInteger(15))
    {
        field_E8_z -= field_EC_dz;
        field_EC_dz = -field_EC_dz;
    }

    for (s32 i = 0; i < field_5C4_parts_used_count; i++)
    {
        field_F4_parts[i].x += field_F4_parts[i].field_C_dx;
        field_F4_parts[i].y += field_F4_parts[i].field_10_dy;
        field_F4_parts[i].field_8_z += field_F4_parts[i].field_14_dz;

        field_F4_parts[i].field_10_dy += FP_FromDouble(0.25);

        if ((field_F4_parts[i].field_8_z + FP_FromInteger(100)) < FP_FromInteger(15))
        {
            field_F4_parts[i].field_8_z -= field_F4_parts[i].field_14_dz;
            field_F4_parts[i].field_14_dz = -field_F4_parts[i].field_14_dz;
        }
    }

    if (static_cast<s32>(sGnFrame) > field_F0_timer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

Gibs::~Gibs()
{
    for (s16 i = 0; i < field_5C4_parts_used_count; i++)
    {
        field_F4_parts[i].field_18_animation.VCleanUp();
    }
}

void Gibs::VRender(PrimHeader** ppOt)
{
    if (sNumCamSwappers_507668 != 0)
    {
        // Don't do anything during screen change
        return;
    }

    mSpriteScale = FP_FromInteger(100) / (field_E8_z + FP_FromInteger(100));

    // Head part rendering
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

    const FP_Point* pCamPos = pScreenManager->mCamPos;
    const FP left = pCamPos->x - FP_FromInteger(pScreenManager->mCamXOff);
    const FP right = pCamPos->x + FP_FromInteger(pScreenManager->mCamXOff);

    const FP up = pCamPos->y - FP_FromInteger(pScreenManager->mCamYOff);
    const FP down = pCamPos->y + FP_FromInteger(pScreenManager->mCamYOff);

    for (s32 i = 0; i < field_5C4_parts_used_count; i++)
    {
        GibPart* pGib = &field_F4_parts[i];
        if (pGib->x >= left && pGib->x <= right)
        {
            if (pGib->y >= up && pGib->y <= down)
            {
                pGib->field_18_animation.field_14_scale = FP_FromInteger(100) / (pGib->field_8_z + FP_FromInteger(100));
                if (pGib->field_18_animation.field_14_scale < FP_FromInteger(1))
                {
                    pGib->field_18_animation.mRenderLayer = Layer::eLayer_Foreground_Half_17;
                }
                else
                {
                    pGib->field_18_animation.mRenderLayer = Layer::eLayer_FG1_37;
                }

                pGib->field_18_animation.VRender(
                    FP_GetExponent(pGib->x - left),
                    FP_GetExponent(pGib->y - up),
                    ppOt,
                    0,
                    0);

                PSX_RECT frameRect = {};
                pGib->field_18_animation.Get_Frame_Rect(&frameRect);
                pScreenManager->InvalidateRectCurrentIdx(
                    frameRect.x,
                    frameRect.y,
                    frameRect.w,
                    frameRect.h);
            }
        }
    }
}

} // namespace AO
