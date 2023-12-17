#include "stdafx.h"
#include "Gibs.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "../relive_lib/AnimResources.hpp"
#include "Map.hpp"
#include "../relive_lib/FixedPoint.hpp"

static const TintEntry kGibTints_55C744[16] = {
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
    {EReliveLevelIds::eCredits, 87u, 103u, 67u}};

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
        case GibType::Abe_0:
            [[fallthrough]];

        case GibType::Mud_3:
            headGib = AnimId::Abe_Head_Gib;
            armGib = AnimId::Abe_Arm_Gib;
            bodyGib = AnimId::Abe_Body_Gib;
            break;

        case GibType::Slig_1:
            headGib = AnimId::Slig_Head_Gib;
            armGib = AnimId::Slig_Arm_Gib;
            bodyGib = AnimId::Slig_Body_Gib;
            break;

        case GibType::Slog_2:
            headGib = AnimId::Slog_Head_Gib;
            armGib = AnimId::Slog_Body_Gib; // No arms
            bodyGib = AnimId::Slog_Body_Gib;
            break;

        case GibType::BlindMud_4:
            headGib = AnimId::BlindMud_Head_Gib;
            armGib = AnimId::BlindMud_Arm_Gib;
            bodyGib = AnimId::BlindMud_Body_Gib;
            break;

        case GibType::Metal_5:
            // No body parts, all metal
            headGib = AnimId::Metal_Gib;
            armGib = AnimId::Metal_Gib;
            bodyGib = AnimId::Metal_Gib;
            break;

        case GibType::Glukkon_6:
            headGib = AnimId::Glukkon_Head_Gib;
            armGib = AnimId::Glukkon_Arm_Gib;
            bodyGib = AnimId::Glukkon_Body_Gib;
            break;

        case GibType::Aslik_7:
            headGib = AnimId::Aslik_Head_Gib;
            armGib = AnimId::Aslik_Arm_Gib;
            bodyGib = AnimId::Aslik_Body_Gib;
            break;

        case GibType::Dripik_8:
            headGib = AnimId::Dripik_Head_Gib;
            armGib = AnimId::Dripik_Arm_Gib;
            bodyGib = AnimId::Dripik_Body_Gib;
            break;

        case GibType::Phleg_9:
            headGib = AnimId::Phleg_Head_Gib;
            armGib = AnimId::Phleg_Arm_Gib;
            bodyGib = AnimId::Phleg_Body_Gib;
            break;

        case GibType::Fleech_10:
            headGib = AnimId::Fleech_Head_Gib;
            armGib = AnimId::Fleech_Body_Gib; // No arms
            bodyGib = AnimId::Fleech_Body_Gib;
            break;
    }


    const AnimRecord& headGibRec = AnimRec(headGib);

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
    if (!mMakeSmaller)
    {
        mVelY = yOff + GibRand(scale);
        mDz = FP_Abs(GibRand(scale) / FP_FromInteger(2));
    }

    sGibRandom = 12;

    mVelY = (yOff + GibRand(scale)) / FP_FromInteger(2);
    mDz = FP_Abs(GibRand(scale) / FP_FromInteger(4));

    if (gibType == GibType::Abe_0)
    {
        SetTint(sAbeTintTable, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::Mud_3)
    {
        SetTint(kGibTints_55C744, gMap.mCurrentLevel);
    }
    else if (gibType == GibType::BlindMud_4)
    {
        mRGB.SetRGB(63, 63, 63);
    }

    mPartsUsedCount = 4;

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

        pPart->mAnimation.SetRGB(mRGB.r, mRGB.g, mRGB.b);

        pPart->x = mXPos;
        pPart->y = mYPos;
        pPart->z = mZ;

        pPart->dx = xOff + GibRand(scale);

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

        pPart->mAnimation.SetSemiTrans(false);

        if (headGibRec.mPalOverride == PalId::BlindMud)
        {
            pPart->mAnimation.LoadPal(GetPalRes(PalId::BlindMud));
        }

        pPart++;
    }
}

Gibs::~Gibs()
{
    for (s32 i = 0; i < mPartsUsedCount; i++)
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
        const FP dz = -mDz;
        mDz = dz;
        mZ += dz;
    }

    for (s32 i = 0; i < mPartsUsedCount; i++)
    {
        mGibParts[i].x += mGibParts[i].dx;
        mGibParts[i].y += mGibParts[i].dy;
        mGibParts[i].z += mGibParts[i].dz;

        mGibParts[i].dy += FP_FromDouble(0.25);

        if (mGibParts[i].z + FP_FromInteger(100) < FP_FromInteger(15))
        {
            const FP dz = -mGibParts[i].dz;
            mGibParts[i].dz = dz;
            mGibParts[i].z += dz;
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

   const FP camXPos = gScreenManager->CamXPos();
    const FP camYPos = gScreenManager->CamYPos();

    for (s32 i = 0; i < mPartsUsedCount; i++)
    {
        // Part is within camera X?
        if (mGibParts[i].x >= camXPos && mGibParts[i].x <= camXPos + FP_FromInteger(640))
        {
            // Part is within camera Y?
            if (mGibParts[i].y >= camYPos && mGibParts[i].y <= camYPos + FP_FromInteger(240))
            {
                mGibParts[i].mAnimation.SetSpriteScale(FP_FromInteger(100) / (mGibParts[i].z + FP_FromInteger(100)));

                if (mMakeSmaller)
                {
                    mGibParts[i].mAnimation.SetSpriteScale(mGibParts[i].mAnimation.GetSpriteScale() / FP_FromInteger(2));
                }

                if (mGibParts[i].mAnimation.GetSpriteScale() < FP_FromInteger(1))
                {
                    mGibParts[i].mAnimation.SetRenderLayer(Layer::eLayer_Foreground_Half_17);
                }
                else
                {
                    mGibParts[i].mAnimation.SetRenderLayer(Layer::eLayer_FG1_37);
                }

                if (mGibParts[i].mAnimation.GetSpriteScale() <= FP_FromInteger(1))
                {
                    const s32 xpos = FP_GetExponent(mGibParts[i].x - camXPos);
                    const s32 ypos = FP_GetExponent(mGibParts[i].y - camYPos);

                    mGibParts[i].mAnimation.VRender(xpos, ypos, ot, 0, 0);

                    PSX_RECT frameRect = {};
                    mGibParts[i].mAnimation.Get_Frame_Rect(&frameRect);
                }
            }
        }
    }
}
