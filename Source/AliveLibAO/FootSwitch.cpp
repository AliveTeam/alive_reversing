#include "stdafx_ao.h"
#include "FootSwitch.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Path.hpp"

namespace AO {

void FootSwitch::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Temple));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Foot_Switch_Temple_Pressed));
}

FootSwitch::FootSwitch(relive::Path_FootSwitch* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eFootSwitch);

    LoadAnimations();

    Animation_Init(GetAnimRes(AnimId::Foot_Switch_Temple));

    GetAnimation().SetRenderLayer(Layer::eLayer_BeforeShadow_25);

    mSwitchId = pTlv->mSwitchId;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        SetSpriteScale(FP_FromDouble(0.5));
        SetScale(Scale::Bg);
    }

    mState = States::eWaitForStepOnMe_0;
    mAction = pTlv->mAction;
    mTriggeredBy = pTlv->mTriggeredBy;

    mXPos = FP_FromInteger(pTlv->mTopLeftX + 12);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    SwitchStates_Set(mSwitchId, 0);

    mTlvId = tlvId;
}

FootSwitch::~FootSwitch()
{
    if (mStoodOnMe)
    {
        mStoodOnMe->mBaseGameObjectRefCount--;
        mStoodOnMe = nullptr;
    }
    Path::TLV_Reset(mTlvId, -1, 0, 0);
}

void FootSwitch::VUpdate()
{
    switch (mState)
    {
        case States::eWaitForStepOnMe_0:
            mStoodOnMe = WhoIsStoodOnMe();
            if (mStoodOnMe)
            {
                mStoodOnMe->mBaseGameObjectRefCount++;
                SwitchStates_Do_Operation(mSwitchId, mAction);
                mState = States::eWaitForGetOffMe_1;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Foot_Switch_Temple_Pressed));
                SfxPlayMono(relive::SoundEffects::FootSwitchPress, 0);
            }
            break;

        case States::eWaitForGetOffMe_1:
        {
            const PSX_RECT bRect = VGetBoundingRect();

            if (mStoodOnMe->mXPos < FP_FromInteger(bRect.x) || mStoodOnMe->mXPos > FP_FromInteger(bRect.w) || mStoodOnMe->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
            {
                mState = States::eWaitForStepOnMe_0;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Foot_Switch_Temple));
                mStoodOnMe->mBaseGameObjectRefCount--;
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
    const PSX_RECT bRectSwitch = VGetBoundingRect();
    // NOTE: AE  y -= 3 not done in AO

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

                if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= theirRect.w && bRectSwitch.w >= theirRect.x && bRectSwitch.h >= theirRect.y && bRectSwitch.y <= theirRect.h && pAliveObj->GetSpriteScale() == GetSpriteScale())
                {
                    return pAliveObj;
                }
            }
        }
    }
    else if (mTriggeredBy == relive::Path_FootSwitch::FootSwitchTriggerBy::eAbe)
    {
        const PSX_RECT bRect = sActiveHero->VGetBoundingRect();
        const s32 xpos = FP_GetExponent(sActiveHero->mXPos);

        if (xpos > bRectSwitch.x && xpos < bRectSwitch.w && bRectSwitch.x <= bRect.w && bRectSwitch.w >= bRect.x && bRectSwitch.h >= bRect.y && bRectSwitch.y <= bRect.h && sActiveHero->GetSpriteScale() == GetSpriteScale())
        {
            return sActiveHero;
        }
    }
    return nullptr;
}

} // namespace AO
