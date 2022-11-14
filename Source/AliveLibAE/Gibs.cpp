#include "stdafx.h"
#include "Gibs.hpp"
#include "Function.hpp"
#include "Game.hpp"
#include "Abe.hpp"
#include "Math.hpp"
#include "stdlib.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "AnimResources.hpp"
#include "Map.hpp"

const TintEntry kGibTints_55C744[16] = {
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

s16 sGibRandom_550E80 = 13;

FP Random_40FAF0(FP scale)
{
    return FP_FromRaw((Math_NextRandom() - 128) << sGibRandom_550E80) * scale;
}

void Gibs::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Abe_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slig_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slig_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slig_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slog_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slog_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BlindMud_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BlindMud_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::BlindMud_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Metal_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Glukkon_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Glukkon_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Glukkon_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Aslik_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Aslik_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Aslik_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dripik_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dripik_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dripik_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Phleg_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Phleg_Arm_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Phleg_Body_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fleech_Head_Gib));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Fleech_Body_Gib));
}

Gibs::Gibs(GibType gibType, FP xpos, FP ypos, FP xOff, FP yOff, FP scale, bool bMakeSmaller)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    field_F4_not_used = nullptr;

    LoadAnimations();

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

    // The base class renders the head gib
    Animation_Init(GetAnimRes(headGib));

    if (mBaseGameObjectFlags.Get(BaseGameObject::eListAddFailed_Bit1))
    {
        return;
    }

    SetSpriteScale(scale);
    mXPos = xpos;
    mYPos = ypos + FP_FromInteger(2);

    field_100_timer = sGnFrame + 91;

    if (scale == FP_FromInteger(1))
    {
        field_F8_z = FP_FromInteger(0);
        GetAnimation().SetRenderLayer(Layer::eLayer_FG1_37);
        SetScale(Scale::Fg);
    }
    else if (scale == FP_FromDouble(0.5))
    {
        field_F8_z = FP_FromInteger(100);
        GetAnimation().SetRenderLayer(Layer::eLayer_Foreground_Half_17);
        SetScale(Scale::Bg);
    }
    else
    {
        // Not a valid scale
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    field_5D6_bMakeSmaller = bMakeSmaller;
    mVelX = xOff + Random_40FAF0(scale);

    // OG Bug? WTF?? Looks like somehow they didn't condition this param correctly
    // because mVelY and field_FC_dz are always overwritten
    if (!field_5D6_bMakeSmaller)
    {
        mVelY = yOff + Random_40FAF0(scale);
        field_FC_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(2));
    }

    sGibRandom_550E80 = 12;

    mVelY = (yOff + Random_40FAF0(scale)) / FP_FromInteger(2);
    field_FC_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(4));

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

    field_5D4_parts_used_count = 4;

    GibPart* pPart = &field_104_parts[0];
    for (s16 i = 0; i < field_5D4_parts_used_count; i++)
    {
        if (i < 2)
        {
            // 2 arm parts
            if (!pPart->field_18_animation.Init(
                    GetAnimRes(armGib),
                    this))
            {
                field_5D4_parts_used_count = i;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }
        }
        else
        {
            // 2 body parts
            if (!pPart->field_18_animation.Init(
                    GetAnimRes(bodyGib),
                    this))
            {
                field_5D4_parts_used_count = i;
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
                return;
            }
        }

        pPart->field_18_animation.SetRenderLayer(GetAnimation().GetRenderLayer());
        pPart->field_18_animation.SetSpriteScale(scale);
        pPart->field_18_animation.mFlags.Clear(AnimFlags::eBlending);

        pPart->field_18_animation.SetRGB(mRGB.r, mRGB.g, mRGB.b);

        pPart->x = mXPos;
        pPart->y = mYPos;
        pPart->field_8_z = field_F8_z;

        pPart->field_C_dx = xOff + Random_40FAF0(scale);

        if (field_5D6_bMakeSmaller)
        {
            pPart->field_10_dy = (yOff + Random_40FAF0(scale)) / FP_FromInteger(2);
            pPart->field_14_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(4));
        }
        else
        {
            pPart->field_10_dy = yOff + Random_40FAF0(scale);
            pPart->field_14_dz = FP_Abs(Random_40FAF0(scale) / FP_FromInteger(2));
        }

        pPart->field_18_animation.mFlags.Clear(AnimFlags::eSemiTrans);

        if (headGibRec.mPalOverride == PalId::BlindMud)
        {
            pPart->field_18_animation.LoadPal(GetPalRes(PalId::BlindMud));
        }

        pPart++;
    }
}

Gibs::~Gibs()
{
    for (s32 i = 0; i < field_5D4_parts_used_count; i++)
    {
        field_104_parts[i].field_18_animation.VCleanUp();
    }
}

void Gibs::VUpdate()
{
    mXPos += mVelX;
    mYPos += mVelY;
    field_F8_z += field_FC_dz;

    mVelY += FP_FromDouble(0.25);

    if (field_F8_z + FP_FromInteger(100) < FP_FromInteger(15))
    {
        const FP dz = -field_FC_dz;
        field_FC_dz = dz;
        field_F8_z += dz;
    }

    for (s32 i = 0; i < field_5D4_parts_used_count; i++)
    {
        field_104_parts[i].x += field_104_parts[i].field_C_dx;
        field_104_parts[i].y += field_104_parts[i].field_10_dy;
        field_104_parts[i].field_8_z += field_104_parts[i].field_14_dz;

        field_104_parts[i].field_10_dy += FP_FromDouble(0.25);

        if (field_104_parts[i].field_8_z + FP_FromInteger(100) < FP_FromInteger(15))
        {
            const FP dz = -field_104_parts[i].field_14_dz;
            field_104_parts[i].field_14_dz = dz;
            field_104_parts[i].field_8_z += dz;
        }
    }

    if (static_cast<s32>(sGnFrame) > field_100_timer)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Gibs::VRender(PrimHeader** ppOt)
{
    if (gNumCamSwappers > 0)
    {
        // Don't do anything during screen change
        return;
    }

    SetSpriteScale(FP_FromInteger(100) / (field_F8_z + FP_FromInteger(100)));
    if (field_5D6_bMakeSmaller)
    {
        SetSpriteScale(GetSpriteScale() / FP_FromInteger(2));
    }

    // Head part rendering
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);

   const FP camXPos = pScreenManager->CamXPos();
    const FP camYPos = pScreenManager->CamYPos();

    for (s32 i = 0; i < field_5D4_parts_used_count; i++)
    {
        // Part is within camera X?
        if (field_104_parts[i].x >= camXPos && field_104_parts[i].x <= camXPos + FP_FromInteger(640))
        {
            // Part is within camera Y?
            if (field_104_parts[i].y >= camYPos && field_104_parts[i].y <= camYPos + FP_FromInteger(240))
            {
                field_104_parts[i].field_18_animation.SetSpriteScale(FP_FromInteger(100) / (field_104_parts[i].field_8_z + FP_FromInteger(100)));

                if (field_5D6_bMakeSmaller)
                {
                    field_104_parts[i].field_18_animation.SetSpriteScale(field_104_parts[i].field_18_animation.GetSpriteScale() / FP_FromInteger(2));
                }

                if (field_104_parts[i].field_18_animation.GetSpriteScale() < FP_FromInteger(1))
                {
                    field_104_parts[i].field_18_animation.SetRenderLayer(Layer::eLayer_Foreground_Half_17);
                }
                else
                {
                    field_104_parts[i].field_18_animation.SetRenderLayer(Layer::eLayer_FG1_37);
                }

                if (field_104_parts[i].field_18_animation.GetSpriteScale() <= FP_FromInteger(1))
                {
                    const s32 xpos = FP_GetExponent(field_104_parts[i].x - camXPos);
                    const s32 ypos = FP_GetExponent(field_104_parts[i].y - camYPos);

                    field_104_parts[i].field_18_animation.VRender(xpos, ypos, ppOt, 0, 0);

                    PSX_RECT frameRect = {};
                    field_104_parts[i].field_18_animation.Get_Frame_Rect(&frameRect);
                }
            }
        }
    }
}
