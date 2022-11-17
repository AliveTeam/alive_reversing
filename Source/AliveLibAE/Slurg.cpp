#include "stdafx.h"
#include "Slurg.hpp"
#include "Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Path.hpp"

u16 sSlurg_Step_Watch_Points_Idx_5C1C08 = 0;
s8 sSlurg_Step_Watch_Points_Count_5BD4DC[2] = {};
Slurg_Step_Watch_Points sSlurg_Step_Watch_Points_5C1B28[2] = {};

void Slurg::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slurg_Burst));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slurg_Move));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slurg_Turn_Around));
}

void Slurg::Clear_Slurg_Step_Watch_Points()
{
    sSlurg_Step_Watch_Points_Idx_5C1C08 = !sSlurg_Step_Watch_Points_Idx_5C1C08;
    sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;

    for (auto& point : sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08].mPoints)
    {
        point = {};
    }
}

TintEntry sSlurgTints_560BCC[16] = {
    {EReliveLevelIds::eMenu, 102u, 127u, 118u},
    {EReliveLevelIds::eMines, 102u, 127u, 118u},
    {EReliveLevelIds::eNecrum, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eCredits, 102u, 127u, 118u}};

Slurg::Slurg(relive::Path_Slurg* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    LoadAnimations();

    mSlurgTlv = pTlv;

    mSlurgState = SlurgStates::eMoving_0;

    Animation_Init(GetAnimRes(AnimId::Slurg_Move));

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    SetType(ReliveTypes::eSlurg);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mTlvInfo = tlvId;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSlurgSpriteScale = FP_FromDouble(0.5);
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBat_Half_14);
        SetScale(Scale::Bg);
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mSlurgSpriteScale = FP_FromInteger(1);
        GetAnimation().SetRenderLayer(Layer::eLayer_SligGreeterFartsBats_33);
        SetScale(Scale::Fg);
    }

    mMovingTimer = pTlv->mMovingTimer;
    mRngForMovingTimer = pTlv->mMovingTimer;

    SetTint(&sSlurgTints_560BCC[0], gMap.mCurrentLevel);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions->Raycast(
            mXPos,
            mYPos,
            mXPos,
            mYPos + FP_FromInteger(24),
            &mSlurgLine,
            &hitX,
            &hitY,
            GetScale() == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    mSlurgSwitchId = pTlv->mSlurgSwitchId;

    mGoingRight = false;
    mMoving = false;

    if (pTlv->mFacing == relive::reliveXDirection::eRight)
    {
        mGoingRight =  true;
    }

    VStackOnObjectsOfType(ReliveTypes::eSlurg);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    CreateShadow();
}

s32 Slurg::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const SlurgSaveState*>(pData);
    auto pTlv = static_cast<relive::Path_Slurg*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId));

    auto pSlurg = relive_new Slurg(pTlv, pState->mTlvId);

    pSlurg->mXPos = pState->mXPos;
    pSlurg->mYPos = pState->mYPos;
    pSlurg->mVelX = pState->mVelX;
    pSlurg->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

    // OG BUG: This wasn't restored
    pSlurg->GetAnimation().SetCurrentFrame(pState->mAnimCurrentFrame);
    pSlurg->GetAnimation().mFlags.Set(AnimFlags::eFlipX, pState->mFlipX & 1);
    pSlurg->GetAnimation().mFlags.Set(AnimFlags::eRender, pState->mRender & 1);

    pSlurg->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->mDrawable & 1);

    if (IsLastFrame(&pSlurg->GetAnimation()))
    {
        pSlurg->GetAnimation().mFlags.Set(AnimFlags::eIsLastFrame);
    }

    pSlurg->mSlurgState = pState->mSlurgState;

    pSlurg->mGoingRight = pState->mGoingRight;
    pSlurg->mMoving = pState->mMoving;
    return sizeof(SlurgSaveState);
}

Slurg::~Slurg()
{
    if (mTlvInfo.IsValid())
    {
        Path::TLV_Reset({}, -1, 0, mSlurgState == SlurgStates::eBurst_2);
    }
}

void Slurg::Burst()
{
    mSlurgState = SlurgStates::eBurst_2;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Burst));

    relive_new Blood(mXPos,
                                mYPos,
                                FP_FromInteger(0),
                                FP_FromInteger(5),
                                mSlurgSpriteScale,
                                20);

    EventBroadcast(kEventLoudNoise, this);
    SfxPlayMono(relive::SoundEffects::SlurgKill, 127, mSlurgSpriteScale);

    if (mSlurgSwitchId > 1)
    {
        SwitchStates_Add(mSlurgSwitchId, 1);
    }
}

void Slurg::VUpdate()
{
    const FP oldXPos = mXPos;
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (mMovingTimer == 0)
    {
        mMovingTimer = Math_RandomRange(mRngForMovingTimer, mRngForMovingTimer + 20);
        mSlurgState = SlurgStates::ePausing_1;
        GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Turn_Around));
    }

    const PSX_RECT bRect = VGetBoundingRect();

    if (mSlurgState != SlurgStates::eBurst_2)
    {
        const s32 idx = sSlurg_Step_Watch_Points_Idx_5C1C08 == 0;
        const s32 max_count = sSlurg_Step_Watch_Points_Count_5BD4DC[idx];
        for (s32 i = 0; i < max_count; i++)
        {
            const Slurg_Step_Watch_Point* pPoint = &sSlurg_Step_Watch_Points_5C1B28[idx].mPoints[i];
            if (pPoint->x > bRect.x - 2 && pPoint->x < bRect.w + 2 && pPoint->y > bRect.y - 4 && pPoint->y < bRect.h + 4)
            {
                Burst();
                break;
            }
        }
    }

    switch (mSlurgState)
    {
        case SlurgStates::eMoving_0:
            mVelX = FP_FromInteger(1);
            mMovingTimer--;
            if (mGoingRight)
            {
                mVelX = -FP_FromInteger(1);
            }

            mMoving = !mMoving;

            if (mMoving)
            {
                mXPos += mVelX;
            }
            break;

        case SlurgStates::ePausing_1:
            mVelX = FP_FromInteger(0);
            if (GetAnimation().GetCurrentFrame() == 0
                && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                SfxPlayMono(relive::SoundEffects::SlurgPause, 0);
            }

            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                mSlurgState = SlurgStates::eMoving_0;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Move));
            }
            break;

        case SlurgStates::eBurst_2:
            if (GetAnimation().mFlags.Get(AnimFlags::eIsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            break;
    }

    if (oldXPos != mXPos)
    {
        mSlurgTlv = sPathInfo->TlvGetAt(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(mSlurgTlv);
    }
}

s16 Slurg::VTakeDamage(BaseGameObject* pFrom)
{
    // Slurgs are tough little dudes, only Paramites can smack 'em up.
    if (pFrom->Type() == ReliveTypes::eParamite)
    {
        Burst();
        return 1;
    }

    return 0;
}

void Slurg::VOnTlvCollision(relive::Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->mTlvType == ReliveTypes::eScrabLeftBound)
        {
            if (mGoingRight)
            {
                GoLeft();
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eScrabRightBound)
        {
            if (!mGoingRight)
            {
                GoRight();
            }
        }
        pTlv = sPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }

    if (mGoingRight)
    {
        if (WallHit(mSlurgSpriteScale * FP_FromInteger(8), -(mSlurgSpriteScale * FP_FromInteger(6))) || Check_IsOnEndOfLine(1, 1))
        {
            GoLeft();
        }
    }
    else
    {
        if (WallHit(mSlurgSpriteScale * FP_FromInteger(8), mSlurgSpriteScale * FP_FromInteger(6)) || Check_IsOnEndOfLine(0, 1))
        {
            GoRight();
        }
    }
}

s32 Slurg::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(AliveObjectFlags::eElectrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<SlurgSaveState*>(pSaveBuffer);

    pState->mType = ReliveTypes::eSlurg;
    pState->mXPos = mXPos;
    pState->mYPos = mYPos;
    pState->mVelX = mVelX;
    pState->mSlurgSpriteScale = mSlurgSpriteScale;
    pState->mFlipX = GetAnimation().mFlags.Get(AnimFlags::eFlipX);
    pState->mCurrentMotion = mCurrentMotion;
    pState->mAnimCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    pState->mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    pState->mDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->mRender = GetAnimation().mFlags.Get(AnimFlags::eRender);
   // pState->mFrameTableOffset = mAnim.mFrameTableOffset;
    pState->mTlvId = mTlvInfo;
    pState->mSlurgState = mSlurgState;
    pState->mGoingRight = mGoingRight;
    pState->mMoving = mMoving;
    return sizeof(SlurgSaveState);
}

void Slurg::GoLeft()
{
    GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
    mGoingRight = false;
    mSlurgState = SlurgStates::ePausing_1;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Turn_Around));
}

void Slurg::GoRight()
{
    GetAnimation().mFlags.Set(AnimFlags::eFlipX);
    mGoingRight = true;
    mSlurgState = SlurgStates::ePausing_1;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Turn_Around));
}
