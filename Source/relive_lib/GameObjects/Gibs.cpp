#include "stdafx.h"
#include "Gibs.hpp"
#include "Math.hpp"
#include "ScreenManager.hpp"
#include "../AnimResources.hpp"
#include "../MapWrapper.hpp"
#include "../FixedPoint.hpp"
#include "../GameType.hpp"
#include "../../AliveLibAE/Game.hpp"

static const TintEntry sAbeTintTable[21] = {
    {EReliveLevelIds::eMenu, 102u, 102u, 102u},
    {EReliveLevelIds::eMines, 102u, 102u, 102u},
    {EReliveLevelIds::eNecrum, 102u, 102u, 80u},
    {EReliveLevelIds::eMudomoVault, 120u, 90u, 120u},
    {EReliveLevelIds::eMudancheeVault, 102u, 70u, 90u},
    {EReliveLevelIds::eFeeCoDepot, 120u, 102u, 82u},
    {EReliveLevelIds::eBarracks, 102u, 102u, 102u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 70u, 90u},
    {EReliveLevelIds::eBonewerkz, 102u, 102u, 102u},
    {EReliveLevelIds::eBrewery, 102u, 102u, 102u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 102u, 102u},
    {EReliveLevelIds::eMudomoVault_Ender, 120u, 90u, 120u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 120u, 102u, 82u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 102u, 102u},
    {EReliveLevelIds::eBonewerkz_Ender, 120u, 90u, 80u},
    {EReliveLevelIds::eCredits, 102u, 102u, 102u},
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eDesert, 125u, 125u, 95u},
    {EReliveLevelIds::eDesertTemple, 120u, 120u, 90u},
    {EReliveLevelIds::eNone, 102u, 102u, 102u}};

static const TintEntry sMudGibTints[19] = {
    {EReliveLevelIds::eMenu, 87u, 103u, 67u},
    {EReliveLevelIds::eMines, 87u, 103u, 67u},
    {EReliveLevelIds::eNecrum, 87u, 103u, 67u},
    {EReliveLevelIds::eMudomoVault, 87u, 103u, 67u},
    {EReliveLevelIds::eMudancheeVault, 87u, 103u, 67u},
    {EReliveLevelIds::eFeeCoDepot, 87u, 103u, 67u},
    {EReliveLevelIds::eBarracks, 87u, 103u, 67u},
    {EReliveLevelIds::eMudancheeVault_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eBonewerkz, 87u, 103u, 67u},
    {EReliveLevelIds::eBrewery, 87u, 103u, 67u},
    {EReliveLevelIds::eBrewery_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eMudomoVault_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eBarracks_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eBonewerkz_Ender, 87u, 103u, 67u},
    {EReliveLevelIds::eCredits, 87u, 103u, 67u},
    {EReliveLevelIds::eStockYards, 25u, 25u, 25u},
    {EReliveLevelIds::eStockYardsReturn, 25u, 25u, 25u},
    {EReliveLevelIds::eNone, 87u, 103u, 67u}};

static s16 sGibRandom = 13;

static FP GibRand(FP scale)
{
    return FP_FromRaw((static_cast<u32>(Math_NextRandom()) - 128) << sGibRandom) * scale;
}

void Gibs::LoadAnimations(AnimId head, AnimId arm, AnimId body)
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(head));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(arm));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(body));
}

Gibs::Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, bool bMakeSmaller)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    AnimId headGib = AnimId::None;
    AnimId armGib = AnimId::None;
    AnimId bodyGib = AnimId::None;

    switch (gibType)
    {
        case GibType::eAbe:
            [[fallthrough]];

        case GibType::eMud:
            headGib = AnimId::Abe_Head_Gib;
            armGib = AnimId::Abe_Arm_Gib;
            bodyGib = AnimId::Abe_Body_Gib;
            break;

        case GibType::eSlig:
            headGib = AnimId::Slig_Head_Gib;
            armGib = AnimId::Slig_Arm_Gib;
            bodyGib = AnimId::Slig_Body_Gib;
            break;

        case GibType::eSlog:
            headGib = AnimId::Slog_Head_Gib;
            armGib = AnimId::Slog_Body_Gib; // No arms
            bodyGib = AnimId::Slog_Body_Gib;
            break;

        case GibType::eBlindMud:
            headGib = AnimId::BlindMud_Head_Gib;
            armGib = AnimId::BlindMud_Arm_Gib;
            bodyGib = AnimId::BlindMud_Body_Gib;
            break;

        case GibType::eMetal:
            // No body parts, all metal
            headGib = AnimId::Metal_Gib;
            armGib = AnimId::Metal_Gib;
            bodyGib = AnimId::Metal_Gib;
            break;

        case GibType::eGlukkon:
            headGib = AnimId::Glukkon_Head_Gib;
            armGib = AnimId::Glukkon_Arm_Gib;
            bodyGib = AnimId::Glukkon_Body_Gib;
            break;

        case GibType::eAslik:
            headGib = AnimId::Aslik_Head_Gib;
            armGib = AnimId::Aslik_Arm_Gib;
            bodyGib = AnimId::Aslik_Body_Gib;
            break;

        case GibType::eDripik:
            headGib = AnimId::Dripik_Head_Gib;
            armGib = AnimId::Dripik_Arm_Gib;
            bodyGib = AnimId::Dripik_Body_Gib;
            break;

        case GibType::ePhleg:
            headGib = AnimId::Phleg_Head_Gib;
            armGib = AnimId::Phleg_Arm_Gib;
            bodyGib = AnimId::Phleg_Body_Gib;
            break;

        case GibType::eFleech:
            headGib = AnimId::Fleech_Head_Gib;
            armGib = AnimId::Fleech_Body_Gib; // No arms
            bodyGib = AnimId::Fleech_Body_Gib;
            break;

        case GibType::eElum:
            headGib = AnimId::Elum_Head_Gib;
            armGib = AnimId::Elum_Arm_Gib;
            bodyGib = AnimId::Elum_Body_Gib;
            break;
    }

    const AnimRecord& headGibRec = PerGameAnimRec(headGib);

    // TODO: It is assumed all 3 gib parts use the same pal - might not be true for mods
    if (headGibRec.mPalOverride == PalId::BlindMud)
    {
        mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::BlindMud));
    }

    LoadAnimations(headGib, armGib, bodyGib);

    // The base class renders the head gib
    Animation_Init(GetAnimRes(headGib));

    if (GetListAddFailed())
    {
        return;
    }

    SetSpriteScale(scale);
    mXPos = xpos;
    mYPos = ypos + FP_FromInteger(2);

    mAliveTimer = MakeTimer(91);

    if (scale == FP_FromInteger(1))
    {
        mZ = FP_FromInteger(0);
        GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);
        SetScale(Scale::Fg);
    }
    else if (scale == FP_FromDouble(0.5))
    {
        mZ = FP_FromInteger(100);
        GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        SetScale(Scale::Bg);
    }
    else
    {
        // Not a valid scale
        SetDead(true);
    }

    mMakeSmaller = bMakeSmaller;
    mVelX = xOff + GibRand(scale);

    // OG Bug? WTF?? Looks like somehow they didn't condition this param correctly
    // because mVelY and mDz are always overwritten
    
    // NOTE: we don't want to desync the AE recording due to different
    // RNG values so we keep this for now
    if (!mMakeSmaller && GetGameType() == GameType::eAe)
    {
        mVelY = yOff + GibRand(scale);
        mDz = FP_Abs(GibRand(scale) / FP_FromInteger(2));
    }

    if (GetGameType() == GameType::eAe)
    {
        sGibRandom = 12;
        mVelY = (yOff + GibRand(scale)) / FP_FromInteger(2);
        mDz = FP_Abs(GibRand(scale) / FP_FromInteger(4));
    }
    else
    {
        mVelY = yOff + GibRand(scale);
        if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
        {
            mDz = -(GibRand(scale) / FP_FromInteger(2));
        }
        else
        {
            mDz = GibRand(scale) / FP_FromInteger(2);
        }
    }

    PalId gibPal = PalId::Default;
    if (GetMap().mCurrentLevel == EReliveLevelIds::eStockYards || GetMap().mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        if (gibType == GibType::eAbe || gibType == GibType::eMud)
        {
            gibPal = PalId::StockYardsAbeGib;
        }
        else if (gibType == GibType::eSlog)
        {
            gibPal = PalId::StockYardsSlogGib;
        }
    }

    if (gibType == GibType::eAbe)
    {
        SetTint(sAbeTintTable, GetMap().mCurrentLevel);
    }
    else if (gibType == GibType::eMud)
    {
        SetTint(sMudGibTints, GetMap().mCurrentLevel);
    }
    else if (gibType == GibType::eBlindMud)
    {
        mRGB.SetRGB(63, 63, 63);
    }

    mPartsUsedCount = GetGameType() == GameType::eAo ? 7 : 4;

    GibPart* pPart = &mGibParts[0];
    for (s16 i = 0; i < mPartsUsedCount; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->mAnimation.Init(GetAnimRes(armGib), this))
            {
                mPartsUsedCount = i;
                SetDead(true);
                return;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->mAnimation.Init(GetAnimRes(bodyGib), this))
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
        pPart->z = mZ;

        pPart->dx = xOff + GibRand(scale);

        if (GetGameType() == GameType::eAe)
        {
            if (mMakeSmaller)
            {
                pPart->dy = (yOff + GibRand(scale)) / FP_FromInteger(2);
                pPart->dz = FP_Abs(GibRand(scale) / FP_FromInteger(4));
            }
            else
            {
                pPart->dy = yOff + GibRand(scale);
                pPart->dz = FP_Abs(GibRand(scale) / FP_FromInteger(2));
            }
        }
        else
        {
            pPart->dy = yOff + GibRand(scale);
            if ((GibRand(scale) / FP_FromInteger(2)) >= FP_FromInteger(0))
            {
                pPart->dz = -(GibRand(scale) / FP_FromInteger(2));
            }
            else
            {
                pPart->dz = GibRand(scale) / FP_FromInteger(2);
            }
        }

        if (headGibRec.mPalOverride == PalId::BlindMud)
        {
            pPart->mAnimation.LoadPal(GetPalRes(PalId::BlindMud));
        }
        else if (gibPal != PalId::Default)
        {
            mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(gibPal));
            pPart->mAnimation.LoadPal(GetPalRes(gibPal));
        }

        pPart++;
    }
}

Gibs::~Gibs()
{
    for (s16 i = 0; i < mPartsUsedCount; i++)
    {
        mGibParts[i].mAnimation.VCleanUp();
    }
}

void Gibs::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;
    mZ += mDz;

    mVelY += FP_FromDouble(0.25);

    if (mZ + FP_FromInteger(100) < FP_FromInteger(15))
    {
        mZ -= mDz;
        mDz = -mDz;
    }

    for (s32 i = 0; i < mPartsUsedCount; i++)
    {
        mGibParts[i].x += mGibParts[i].dx;
        mGibParts[i].y += mGibParts[i].dy;
        mGibParts[i].z += mGibParts[i].dz;

        mGibParts[i].dy += FP_FromDouble(0.25);

        if (mGibParts[i].z + FP_FromInteger(100) < FP_FromInteger(15))
        {
            mGibParts[i].z -= mGibParts[i].dz;
            mGibParts[i].dz = -mGibParts[i].dz;
        }
    }

    if (static_cast<s32>(sGnFrame) > mAliveTimer)
    {
        SetDead(true);
    }
}

void Gibs::VRender(OrderingTable& ot)
{
    if (gNumCamSwappers > 0)
    {
        // Don't do anything during screen change
        return;
    }

    SetSpriteScale(FP_FromInteger(100) / (mZ + FP_FromInteger(100)));
    if (mMakeSmaller)
    {
        SetSpriteScale(GetSpriteScale() / FP_FromInteger(2));
    }

    // Head part rendering
    BaseAnimatedWithPhysicsGameObject::VRender(ot);

    // AO consts
    const FP_Point* pCamPos = gScreenManager->mCamPos;
    const FP left = pCamPos->x - FP_FromInteger(gScreenManager->mCamXOff);
    const FP right = pCamPos->x + FP_FromInteger(gScreenManager->mCamXOff);

    const FP up = pCamPos->y - FP_FromInteger(gScreenManager->mCamYOff);
    const FP down = pCamPos->y + FP_FromInteger(gScreenManager->mCamYOff);

    // AE consts
    const FP camXPos = gScreenManager->CamXPos();
    const FP camYPos = gScreenManager->CamYPos();

    bool withinCameraX = false;
    bool withinCameraY = false;

    s32 renderX = 0;
    s32 renderY = 0;

    for (s32 i = 0; i < mPartsUsedCount; i++)
    {
        GibPart* pGib = &mGibParts[i];
        if (GetGameType() == GameType::eAo)
        {
            withinCameraX = pGib->x >= left && pGib->x <= right;
            withinCameraY = pGib->y >= up && pGib->y <= down;

            renderX = FP_GetExponent(pGib->x - left);
            renderY = FP_GetExponent(pGib->y - up);
        }
        else
        {
            withinCameraX = pGib->x >= camXPos && pGib->x <= camXPos + FP_FromInteger(640);
            withinCameraY = pGib->y >= camYPos && pGib->y <= camYPos + FP_FromInteger(240);

            renderX = FP_GetExponent(pGib->x - camXPos);
            renderY = FP_GetExponent(pGib->y - camYPos);
        }

        // Part is within camera X?
        if (withinCameraX)
        {
            // Part is within camera Y?
            if (withinCameraY)
            {
                pGib->mAnimation.SetSpriteScale(FP_FromInteger(100) / (pGib->z + FP_FromInteger(100)));

                if (mMakeSmaller)
                {
                    pGib->mAnimation.SetSpriteScale(pGib->mAnimation.GetSpriteScale() / FP_FromInteger(2));
                }

                if (pGib->mAnimation.GetSpriteScale() < FP_FromInteger(1))
                {
                    pGib->mAnimation.SetRenderLayer(Layer::eLayer_Foreground_Half_17);
                }
                else
                {
                    pGib->mAnimation.SetRenderLayer(Layer::eLayer_FG1_37);
                }

                pGib->mAnimation.VRender(renderX, renderY, ot, 0, 0);
            }
        }
    }
}
