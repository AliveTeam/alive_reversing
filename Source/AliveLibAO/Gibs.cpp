#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "Gibs.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Map.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "CameraSwapper.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/FixedPoint.hpp"

namespace AO {

static const Gib_Data sGibData[6] = {
    {AnimId::Abe_Head_Gib, AnimId::Abe_Arm_Gib, AnimId::Abe_Body_Gib},
    {AnimId::Slig_Head_Gib, AnimId::Slig_Arm_Gib, AnimId::Slig_Body_Gib},
    {AnimId::Slog_Head_Gib, AnimId::Slog_Body_Gib, AnimId::Slog_Body_Gib},
    {AnimId::Elum_Head_Gib, AnimId::Elum_Arm_Gib, AnimId::Elum_Body_Gib},
    {AnimId::Abe_Head_Gib, AnimId::Abe_Arm_Gib, AnimId::Abe_Body_Gib},
    {AnimId::Metal_Gib, AnimId::Metal_Gib, AnimId::Metal_Gib}};

static const TintEntry sAbeGibTints[5] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eDesert, 125u, 125u, 95u},
    {EReliveLevelIds::eDesertTemple, 120u, 120u, 90u},
    {EReliveLevelIds::eNone, 102u, 102u, 102u}};

static const TintEntry sMudGibTints[3] = {
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eNone, 87u, 103u, 67u},
};

static FP GibRand(FP scale)
{
    return FP_FromRaw((static_cast<u32>(Math_NextRandom()) - 128) << 13) * scale;
}

void Gibs::LoadAnimations()
{
    for (u32 i = 0; i < ALIVE_COUNTOF(sGibData); i++)
    {
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sGibData[i].mHead));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sGibData[i].mArm));
        mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(sGibData[i].mBody));
    }
}

Gibs::Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    LoadAnimations();

    mGibData = &sGibData[gibType];

    // The base class renders the head gib
    Animation_Init(GetAnimRes(mGibData->mHead));

    SetSpriteScale(scale);
    mXPos = xpos;
    mYPos = ypos + FP_FromInteger(2);

    mAliveTimer = MakeTimer(91);

    if (scale == FP_FromInteger(1))
    {
        field_E8_z = FP_FromInteger(0);
        GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);
        SetScale(Scale::Fg);
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_E8_z = FP_FromInteger(100);
        GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        SetScale(Scale::Bg);
    }
    else
    {
        // Not a valid scale
        SetDead(true);
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

    PalId gibPal = PalId::Default;
    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        if (gibType == GibType::Abe_0 || gibType == GibType::Mud_4)
        {
            gibPal = PalId::StockYardsAbeGib;
        }
        else if (gibType == GibType::Slog_2)
        {
            gibPal = PalId::StockYardsSlogGib;
        }
    }

    if (gibType == GibType::Abe_0)
    {
        SetTint(sAbeGibTints, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::Mud_4)
    {
        SetTint(sMudGibTints, gMap.mCurrentLevel);
    }

    mPartsUsedCount = 7;

    GibPart* pPart = &mGibParts[0];
    for (s16 i = 0; i < mPartsUsedCount; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->mAnimation.Init(GetAnimRes(mGibData->mArm), this))
            {
                mPartsUsedCount = i;
                SetDead(true);
                return;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->mAnimation.Init(GetAnimRes(mGibData->mBody), this))
            {
                mPartsUsedCount = i;
                SetDead(true);
                return;
            }
        }

        pPart->mAnimation.SetRenderLayer(GetAnimation().GetRenderLayer());
        pPart->mAnimation.SetSpriteScale(scale);

        pPart->mAnimation.SetBlending(false);
        pPart->mAnimation.SetSemiTrans(false);

        pPart->mAnimation.SetRGB(mRGB.r, mRGB.g, mRGB.b);

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

        if (gibPal != PalId::Default)
        {
            mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(gibPal));
            pPart->mAnimation.LoadPal(GetPalRes(gibPal));
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

    for (s32 i = 0; i < mPartsUsedCount; i++)
    {
        mGibParts[i].x += mGibParts[i].field_C_dx;
        mGibParts[i].y += mGibParts[i].field_10_dy;
        mGibParts[i].field_8_z += mGibParts[i].field_14_dz;

        mGibParts[i].field_10_dy += FP_FromDouble(0.25);

        if ((mGibParts[i].field_8_z + FP_FromInteger(100)) < FP_FromInteger(15))
        {
            mGibParts[i].field_8_z -= mGibParts[i].field_14_dz;
            mGibParts[i].field_14_dz = -mGibParts[i].field_14_dz;
        }
    }

    if (static_cast<s32>(sGnFrame) > mAliveTimer)
    {
        SetDead(true);
    }
}

Gibs::~Gibs()
{
    for (s16 i = 0; i < mPartsUsedCount; i++)
    {
        mGibParts[i].mAnimation.VCleanUp();
    }
}

void Gibs::VRender(OrderingTable& ot)
{
    if (gNumCamSwappers != 0)
    {
        // Don't do anything during screen change
        return;
    }

    SetSpriteScale(FP_FromInteger(100) / (field_E8_z + FP_FromInteger(100)));

    // Head part rendering
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

    const FP_Point* pCamPos = gScreenManager->mCamPos;
    const FP left = pCamPos->x - FP_FromInteger(gScreenManager->mCamXOff);
    const FP right = pCamPos->x + FP_FromInteger(gScreenManager->mCamXOff);

    const FP up = pCamPos->y - FP_FromInteger(gScreenManager->mCamYOff);
    const FP down = pCamPos->y + FP_FromInteger(gScreenManager->mCamYOff);

    for (s32 i = 0; i < mPartsUsedCount; i++)
    {
        GibPart* pGib = &mGibParts[i];
        if (pGib->x >= left && pGib->x <= right)
        {
            if (pGib->y >= up && pGib->y <= down)
            {
                pGib->mAnimation.SetSpriteScale(FP_FromInteger(100) / (pGib->field_8_z + FP_FromInteger(100)));
                if (pGib->mAnimation.GetSpriteScale() < FP_FromInteger(1))
                {
                    pGib->mAnimation.SetRenderLayer(Layer::eLayer_Foreground_Half_17);
                }
                else
                {
                    pGib->mAnimation.SetRenderLayer(Layer::eLayer_FG1_37);
                }

                pGib->mAnimation.VRender(
                    FP_GetExponent(pGib->x - left),
                    FP_GetExponent(pGib->y - up),
                    ppOt,
                    0,
                    0);
            }
        }
    }
}

} // namespace AO
