#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "Rock.hpp"
#include "RockSack.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "AnimResources.hpp"
#include "FixedPoint_common.hpp"
#include "Abe.hpp"
#include "Function.hpp"
#include "../relive_lib/Shadow.hpp"

namespace AO {

void RockSack::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RockSack_Idle));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RockSack_SoftHit));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::RockSack_HardHit));
}

void RockSack::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (GetAnimation().GetCurrentFrame() == 2)
    {
        if (field_114_can_play_wobble_sound)
        {
            if (Math_NextRandom() < 40u || field_116_force_wobble_sound)
            {
                field_114_can_play_wobble_sound = 0;
                field_116_force_wobble_sound = 0;
                SFX_Play_Pitch(relive::SoundEffects::SackWobble, 24, Math_RandomRange(-2400, -2200));
            }
        }
    }
    else
    {
        field_114_can_play_wobble_sound = 1;
    }

    if (field_110_has_been_hit)
    {
        if (field_110_has_been_hit == 1)
        {
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_Idle));
                field_110_has_been_hit = 0;
            }
        }
    }
    else
    {
        if (GetAnimation().GetFrameChangeCounter() == 0)
        {
            GetAnimation().SetFrameChangeCounter(Math_RandomRange(2, 10));
        }

        const PSX_RECT bPlayerRect = sActiveHero->VGetBoundingRect();
        const PSX_RECT bRect = VGetBoundingRect();

        if (bRect.x <= bPlayerRect.w
            && bRect.w >= bPlayerRect.x
            && bRect.h >= bPlayerRect.y
            && bRect.y <= bPlayerRect.h
            && GetSpriteScale() == sActiveHero->GetSpriteScale())
        {
            if (!gThrowableArray || !gThrowableArray->field_10_count)
            {
                if (!gThrowableArray)
                {
                    gThrowableArray = relive_new ThrowableArray();
                }

                gThrowableArray->Add(field_112_rock_amount);

                auto pRock = relive_new Rock(
                    mXPos,
                    mYPos - FP_FromInteger(30),
                    field_112_rock_amount);
                if (pRock)
                {
                    pRock->VThrow(field_118_x_vel, field_11C_y_vel);
                }

                SfxPlayMono(relive::SoundEffects::SackHit, 0);
                Environment_SFX_42A220(EnvironmentSfx::eDeathNoise_7, 0, 0x7FFF, 0);
            }

            if (sActiveHero->mCurrentMotion == eAbeMotions::Motion_33_RunJumpMid)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_HardHit));
            }
            else
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::RockSack_SoftHit));
            }

            field_110_has_been_hit = 1;
        }
    }
}

RockSack::~RockSack()
{
    Path::TLV_Reset(field_10C_tlvInfo, -1, 0, 0);
}

RockSack::RockSack(relive::Path_RockSack* pTlv, const Guid& tlvId)
    : BaseAliveGameObject()
{
    SetType(ReliveTypes::eRockSack);

    LoadAnimations();
    Animation_Init(GetAnimRes(AnimId::RockSack_Idle));

    GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);

    field_10C_tlvInfo = tlvId;
    field_110_has_been_hit = 0;
    mVisualFlags.Clear(VisualFlags::eApplyShadowZoneColour);
    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);
    field_118_x_vel = FP_FromRaw(pTlv->mVelX << 8);
    field_11C_y_vel = FP_FromRaw(-256 * pTlv->mVelY);

    if (pTlv->mRockFallDirection == relive::reliveXDirection::eLeft)
    {
        field_118_x_vel = -field_118_x_vel;
    }

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }
    else
    {
        SetSpriteScale(FP_FromInteger(1));
        SetScale(Scale::Fg);
    }

    field_112_rock_amount = pTlv->mRockAmount;
    field_114_can_play_wobble_sound = 1;
    field_116_force_wobble_sound = 1;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mLoadedPals.push_back(ResourceManagerWrapper::LoadPal(PalId::BlueRockSack));
        GetAnimation().LoadPal(GetPalRes(PalId::BlueRockSack));
    }

    CreateShadow();
}

void RockSack::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

}
