#include "stdafx.h"
#include "../relive_lib/Function.hpp"
#include "FootSwitch.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "../relive_lib/GameObjects/Spark.hpp"
#include "../relive_lib/GameObjects/ParticleBurst.hpp"
#include "PathData.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"

static const TintEntry sFootSwitchTints[16] = {
    {EReliveLevelIds::eMenu, 127u, 127u, 127u},
    {EReliveLevelIds::eMines, 127u, 127u, 127u},
    {EReliveLevelIds::eNecrum, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks, 127u, 127u, 127u},
    {EReliveLevelIds::eMudancheeVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery, 127u, 127u, 127u},
    {EReliveLevelIds::eBrewery_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eMudomoVault_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBarracks_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eBonewerkz_Ender, 127u, 127u, 127u},
    {EReliveLevelIds::eCredits, 127u, 127u, 127u}};


static const AnimId sFootSwitchAnimIds[15][2] = {
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Bonewerkz_Idle, AnimId::Foot_Switch_Bonewerkz_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Vault_Idle, AnimId::Foot_Switch_Vault_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Industrial_Idle, AnimId::Foot_Switch_Industrial_Pressed},
    {AnimId::Foot_Switch_Bonewerkz_Idle, AnimId::Foot_Switch_Bonewerkz_Pressed},
};

void FootSwitch::LoadAnimations()
{
    switch (gMap.mCurrentLevel)
    {
        case EReliveLevelIds::eMudomoVault:
        case EReliveLevelIds::eMudancheeVault:
        case EReliveLevelIds::eMudancheeVault_Ender:
        case EReliveLevelIds::eMudomoVault_Ender:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Vault_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Vault_Pressed));
            break;
        case EReliveLevelIds::eBonewerkz:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Bonewerkz_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Bonewerkz_Pressed));
            break;
        default:
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Industrial_Idle));
            mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Industrial_Pressed));
            break;
    }
}

FootSwitch::FootSwitch(relive::Path_FootSwitch* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eFootSwitch);
    mStoodOnMeId = Guid{};

    LoadAnimations();

    const s32 idx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    Animation_Init(GetAnimRes(sFootSwitchAnimIds[idx][0]));

    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);

    SetTint(sFootSwitchTints, gMap.mCurrentLevel);

    mSwitchId = pTlv->mSwitchId;

    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_Half_6);
    }

    mAction = pTlv->mAction;
    mTriggeredBy = pTlv->mTriggeredBy;
    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mState = States::eWaitForStepOnMe;
    SetDoPurpleLightEffect(true);
    mYPos = FP_FromInteger(pTlv->mBottomRightY);
    mCreateSparks = false;
    mTlvId = tlvId;
    mFindStander = true;
}

FootSwitch::~FootSwitch()
{
    mStoodOnMeId = Guid{};
    Path::TLV_Reset(mTlvId);
}

void FootSwitch::VUpdate()
{
    auto pLastStoodOnMe = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mStoodOnMeId));
    if (mFindStander)
    {
        mFindStander = false;
        pLastStoodOnMe = WhoIsStoodOnMe();
        if (pLastStoodOnMe)
        {
            mStoodOnMeId = pLastStoodOnMe->mBaseGameObjectId;
            GetAnimation().Set_Animation_Data(GetAnimRes(sFootSwitchAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));
            mState = States::eWaitForGetOffMe;
        }
    }

    switch (mState)
    {
        case States::eWaitForStepOnMe:
        {
            auto pStoodOnMeNow = WhoIsStoodOnMe();
            if (pStoodOnMeNow)
            {
                mStoodOnMeId = pStoodOnMeNow->mBaseGameObjectId;

                SwitchStates_Do_Operation(mSwitchId, mAction);
                mState = States::eWaitForGetOffMe;

                GetAnimation().Set_Animation_Data(GetAnimRes(sFootSwitchAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1]));

                relive_new ParticleBurst(mXPos,
                                                            mYPos + FP_FromInteger(10),
                                                            3,
                                                            GetSpriteScale(),
                                                            BurstType::eBigRedSparks,
                                                            9, true);

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                {
                    SFX_Play_Pitch(relive::SoundEffects::IndustrialTrigger, 30, 400);
                    SFX_Play_Pitch(relive::SoundEffects::IndustrialNoise1, 60, 800);
                }
                else
                {
                    SfxPlayMono(relive::SoundEffects::FootSwitchPress, 0);
                }
            }

            if (GetAnimation().GetCurrentFrame() == 0)
            {
                mCreateSparks = true;
                return;
            }

            if (mCreateSparks)
            {
                relive_new Spark(mXPos,
                                            mYPos + (GetSpriteScale() * FP_FromInteger(6)),
                                            GetSpriteScale(),
                                            10,
                                            100,
                                            255,
                                            SparkType::eSmallChantParticle_0);

                relive_new ParticleBurst(mXPos,
                                                            mYPos + (GetSpriteScale() * FP_FromInteger(10)),
                                                            1,
                                                            GetSpriteScale(),
                                                            BurstType::eBigRedSparks,
                                                            9, true);

                mCreateSparks = false;
            }

            if (GetAnimation().GetCurrentFrame() == 0)
            {
                mCreateSparks = true;
            }
            break;
        }

        case States::eWaitForGetOffMe:
        {
            const PSX_RECT bRect = VGetBoundingRect();

            // Have they left the switch or died?
            if (!pLastStoodOnMe || // OG bug: If thing on the switch had died this would de-ref null and crash
                pLastStoodOnMe->mXPos < FP_FromInteger(bRect.x) || pLastStoodOnMe->mXPos > FP_FromInteger(bRect.w) || pLastStoodOnMe->GetDead())
            {
                mState = States::eWaitForStepOnMe;
                GetAnimation().Set_Animation_Data(GetAnimRes(sFootSwitchAnimIds[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0]));
                mStoodOnMeId = Guid{};
            }
            break;
        }

        default:
            break;
    }
}

void FootSwitch::VScreenChanged()
{
    SetDead(true);
}

BaseAliveGameObject* FootSwitch::WhoIsStoodOnMe()
{
    PSX_RECT bRectSwitch = VGetBoundingRect();
    bRectSwitch.y -= 3;

    if (mTriggeredBy == relive::Path_FootSwitch::FootSwitchTriggerBy::eAnyone)
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->GetIsBaseAliveGameObject())
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                const PSX_RECT theirRect = pAliveObj->VGetBoundingRect();

                const s32 xpos = FP_GetExponent(pAliveObj->mXPos);

                if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= theirRect.w && bRectSwitch.w >= theirRect.x && bRectSwitch.h >= theirRect.y && bRectSwitch.y <= theirRect.h && pAliveObj->GetScale() == GetScale())
                {
                    return pAliveObj;
                }
            }
        }
    }
    else if (mTriggeredBy == relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe)
    {
        const PSX_RECT bRect = gAbe->VGetBoundingRect();

        const s32 xpos = FP_GetExponent(gAbe->mXPos);

        if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= bRect.w && bRectSwitch.w >= bRect.x && bRectSwitch.h >= bRect.y && bRectSwitch.y <= bRect.h && gAbe->GetScale() == GetScale())
        {
            return gAbe;
        }
    }

    return nullptr;
}
