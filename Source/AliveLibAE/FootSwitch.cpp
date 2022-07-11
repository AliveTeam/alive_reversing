#include "stdafx.h"
#include "Function.hpp"
#include "FootSwitch.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Spark.hpp"
#include "ParticleBurst.hpp"
#include "PathData.hpp"
#include "../relive_lib/ObjectIds.hpp"

const TintEntry sFootSwitchTints_5639F4[18] = {
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
    {EReliveLevelIds::eNone, 127u, 127u, 127u}};


const AnimId sFootSwitchData_547D60[15][2] = {
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


FootSwitch::FootSwitch(Path_FootSwitch* pTlv, s32 tlvInfo)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eFootSwitch);
    field_100_obj_id = -1;

    const s32 idx = static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel));

    const AnimRecord& rec = AnimRec(sFootSwitchData_547D60[idx][0]);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(sFootSwitchData_547D60[idx][0], ppRes);

    mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_25;

    SetTint(sFootSwitchTints_5639F4, gMap.mCurrentLevel);

    field_FA_switch_id = pTlv->field_10_switch_id;

    if (pTlv->field_12_scale == Scale_short::eHalf_1)
    {
        mSpriteScale = FP_FromDouble(0.5);
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_BeforeShadow_Half_6;
    }

    field_FC_action = pTlv->field_14_action;
    field_FE_trigger_by = pTlv->field_16_trigger_by;
    mXPos = FP_FromInteger((pTlv->mTopLeft.x + pTlv->mBottomRight.x) / 2);
    field_F8_state = States::eWaitForStepOnMe_0;
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mYPos = FP_FromInteger(pTlv->mBottomRight.y);
    field_104_bCreateSparks = 0;
    field_F4_tlvInfo = tlvInfo;
    field_106_bFindStander = 1;
}

FootSwitch::~FootSwitch()
{
    field_100_obj_id = -1;
    Path::TLV_Reset(field_F4_tlvInfo, -1, 0, 0);
}

void FootSwitch::VUpdate()
{
    auto pLastStoodOnMe = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_100_obj_id));
    if (field_106_bFindStander)
    {
        field_106_bFindStander = FALSE;
        pLastStoodOnMe = WhoIsStoodOnMe();
        if (pLastStoodOnMe)
        {
            field_100_obj_id = pLastStoodOnMe->mBaseGameObjectId;
            mAnim.Set_Animation_Data(sFootSwitchData_547D60[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1], nullptr);
            field_F8_state = States::eWaitForGetOffMe_1;
        }
    }

    switch (field_F8_state)
    {
        case States::eWaitForStepOnMe_0:
        {
            auto pStoodOnMeNow = WhoIsStoodOnMe();
            if (pStoodOnMeNow)
            {
                field_100_obj_id = pStoodOnMeNow->mBaseGameObjectId;

                SwitchStates_Do_Operation(field_FA_switch_id, field_FC_action);
                field_F8_state = States::eWaitForGetOffMe_1;

                mAnim.Set_Animation_Data(sFootSwitchData_547D60[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][1], nullptr);

                relive_new ParticleBurst(mXPos,
                                                            mYPos + FP_FromInteger(10),
                                                            3,
                                                            mSpriteScale,
                                                            BurstType::eBigRedSparks_3,
                                                            9);

                if (gMap.mCurrentLevel == EReliveLevelIds::eMines || gMap.mCurrentLevel == EReliveLevelIds::eBonewerkz || gMap.mCurrentLevel == EReliveLevelIds::eFeeCoDepot || gMap.mCurrentLevel == EReliveLevelIds::eBarracks || gMap.mCurrentLevel == EReliveLevelIds::eBrewery)
                {
                    SFX_Play_Pitch(SoundEffect::IndustrialTrigger_80, 30, 400);
                    SFX_Play_Pitch(SoundEffect::IndustrialNoise1_76, 60, 800);
                }
                else
                {
                    SfxPlayMono(SoundEffect::FootSwitchPress_55, 0);
                }
            }

            if (mAnim.mCurrentFrame == 0)
            {
                field_104_bCreateSparks = 1;
                return;
            }

            if (field_104_bCreateSparks)
            {
                relive_new Spark(mXPos,
                                            mYPos + (mSpriteScale * FP_FromInteger(6)),
                                            mSpriteScale,
                                            10,
                                            100,
                                            255,
                                            SparkType::eSmallChantParticle_0);

                relive_new ParticleBurst(mXPos,
                                                            mYPos + (mSpriteScale * FP_FromInteger(10)),
                                                            1,
                                                            mSpriteScale,
                                                            BurstType::eBigRedSparks_3,
                                                            9);

                field_104_bCreateSparks = 0;
            }

            if (mAnim.mCurrentFrame == 0)
            {
                field_104_bCreateSparks = 1;
            }
            break;
        }

        case States::eWaitForGetOffMe_1:
        {
            const PSX_RECT bRect = VGetBoundingRect();

            // Have they left the switch or died?
            if (!pLastStoodOnMe || // OG bug: If thing on the switch had died this would de-ref null and crash
                pLastStoodOnMe->mXPos < FP_FromInteger(bRect.x) || pLastStoodOnMe->mXPos > FP_FromInteger(bRect.w) || pLastStoodOnMe->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                field_F8_state = States::eWaitForStepOnMe_0;
                mAnim.Set_Animation_Data(sFootSwitchData_547D60[static_cast<s32>(MapWrapper::ToAE(gMap.mCurrentLevel))][0], nullptr);
                field_100_obj_id = -1;
            }
            break;
        }

        default:
            break;
    }
}

void FootSwitch::VScreenChanged()
{
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
}

BaseAliveGameObject* FootSwitch::WhoIsStoodOnMe()
{
    PSX_RECT bRectSwitch = VGetBoundingRect();
    bRectSwitch.y -= 3;

    if (field_FE_trigger_by == FootSwitchTriggerBy::eAnyone_1)
    {
        for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
        {
            BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
            if (!pObj)
            {
                break;
            }

            if (pObj->mBaseGameObjectFlags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
            {
                auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

                const PSX_RECT theirRect = pAliveObj->VGetBoundingRect();

                const s32 xpos = FP_GetExponent(pAliveObj->mXPos);

                if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= theirRect.w && bRectSwitch.w >= theirRect.x && bRectSwitch.h >= theirRect.y && bRectSwitch.y <= theirRect.h && pAliveObj->mScale == mScale)
                {
                    return pAliveObj;
                }
            }
        }
    }
    else if (field_FE_trigger_by == FootSwitchTriggerBy::eAbe_0)
    {
        const PSX_RECT bRect = sActiveHero->VGetBoundingRect();

        const s32 xpos = FP_GetExponent(sActiveHero->mXPos);

        if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= bRect.w && bRectSwitch.w >= bRect.x && bRectSwitch.h >= bRect.y && bRectSwitch.y <= bRect.h && sActiveHero->mScale == mScale)
        {
            return sActiveHero;
        }
    }

    return nullptr;
}
