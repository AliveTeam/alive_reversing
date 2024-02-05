#include "stdafx.h"
#include "Slurg.hpp"
#include "../relive_lib/Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "../relive_lib/GameObjects/Blood.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/SwitchStates.hpp"
#include "stdlib.hpp"
#include "Map.hpp"
#include "Path.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Math.hpp"
#include "QuikSave.hpp"

u16 gSlurgStepWatchPointsIdx = 0;
s8 gSlurgStepWatchPointsCount[2] = {};
Slurg_Step_Watch_Points gSlurgStepWatchPoints[2] = {};

void Slurg::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slurg_Burst));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slurg_Move));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Slurg_Turn_Around));
}

void Slurg::Clear_Slurg_Step_Watch_Points()
{
    gSlurgStepWatchPointsIdx = !gSlurgStepWatchPointsIdx;
    gSlurgStepWatchPointsCount[gSlurgStepWatchPointsIdx] = 0;

    for (auto& point : gSlurgStepWatchPoints[gSlurgStepWatchPointsIdx].mPoints)
    {
        point = {};
    }
}

static const TintEntry sSlurgTints[16] = {
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

    SetCanExplode(true);
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

    SetTint(&sSlurgTints[0], gMap.mCurrentLevel);

    FP hitX = {};
    FP hitY = {};
    if (gCollisions->Raycast(
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
    SetDoPurpleLightEffect(true);
    CreateShadow();
}

void Slurg::CreateFromSaveState(SerializedObjectData& pData)
{
    const auto pState = pData.ReadTmpPtr<SlurgSaveState>();
    auto pTlv = static_cast<relive::Path_Slurg*>(gPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvId));

    auto pSlurg = relive_new Slurg(pTlv, pState->mTlvId);

    pSlurg->mXPos = pState->mXPos;
    pSlurg->mYPos = pState->mYPos;
    pSlurg->mVelX = pState->mVelX;
    pSlurg->GetAnimation().SetFrameChangeCounter(pState->mFrameChangeCounter);

    // OG BUG: This wasn't restored
    pSlurg->GetAnimation().SetCurrentFrame(pState->mAnimCurrentFrame);
    pSlurg->GetAnimation().SetFlipX(pState->mFlipX & 1);
    pSlurg->GetAnimation().SetRender(pState->mRender & 1);

    pSlurg->SetDrawable(pState->mDrawable & 1);

    if (IsLastFrame(&pSlurg->GetAnimation()))
    {
        pSlurg->GetAnimation().SetIsLastFrame(true);
    }

    pSlurg->mSlurgState = pState->mSlurgState;

    pSlurg->mGoingRight = pState->mGoingRight;
    pSlurg->mMoving = pState->mMoving;
}

Slurg::~Slurg()
{
    if (mTlvInfo.IsValid())
    {
        // NOTE: OG bug? OG passed in 0xFFFFFFFF but it probably should've been mTlvInfo?
        if (mSlurgState == SlurgStates::eBurst_2)
        {
            Path::TLV_Delete({});
        }
        else
        {
            Path::TLV_Reset({});
        }
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
        SetDead(true);
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
        const s32 idx = gSlurgStepWatchPointsIdx == 0;
        const s32 max_count = gSlurgStepWatchPointsCount[idx];
        for (s32 i = 0; i < max_count; i++)
        {
            const Slurg_Step_Watch_Point* pPoint = &gSlurgStepWatchPoints[idx].mPoints[i];
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

            if (GetAnimation().GetIsLastFrame())
            {
                mSlurgState = SlurgStates::eMoving_0;
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Move));
            }
            break;

        case SlurgStates::eBurst_2:
            if (GetAnimation().GetIsLastFrame())
            {
                SetDead(true);
            }
            break;

        default:
            break;
    }

    if (oldXPos != mXPos)
    {
        mSlurgTlv = gPathInfo->TLV_Get_At(
            nullptr,
            mXPos,
            mYPos,
            mXPos,
            mYPos);

        VOnTlvCollision(mSlurgTlv);
    }
}

bool Slurg::VTakeDamage(BaseGameObject* pFrom)
{
    // Slurgs are tough little dudes, only Paramites can smack 'em up.
    if (pFrom->Type() == ReliveTypes::eParamite)
    {
        Burst();
        return true;
    }

    return false;
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
        pTlv = gPathInfo->TLV_Get_At(pTlv, mXPos, mYPos, mXPos, mYPos);
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

void Slurg::VGetSaveState(SerializedObjectData& pSaveBuffer)
{
    if (GetElectrocuted())
    {
        return;
    }

    SlurgSaveState data = {};

    data.mType = ReliveTypes::eSlurg;
    data.mXPos = mXPos;
    data.mYPos = mYPos;
    data.mVelX = mVelX;
    data.mSlurgSpriteScale = mSlurgSpriteScale;
    data.mFlipX = GetAnimation().GetFlipX();
    data.mAnimCurrentFrame = static_cast<s16>(GetAnimation().GetCurrentFrame());
    data.mFrameChangeCounter = static_cast<s16>(GetAnimation().GetFrameChangeCounter());
    data.mDrawable = GetDrawable();
    data.mRender = GetAnimation().GetRender();
   // data.mFrameTableOffset = mAnim.mFrameTableOffset;
    data.mTlvId = mTlvInfo;
    data.mSlurgState = mSlurgState;
    data.mGoingRight = mGoingRight;
    data.mMoving = mMoving;

    pSaveBuffer.Write(data);
}

void Slurg::GoLeft()
{
    GetAnimation().SetFlipX(false);
    mGoingRight = false;
    mSlurgState = SlurgStates::ePausing_1;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Turn_Around));
}

void Slurg::GoRight()
{
    GetAnimation().SetFlipX(true);
    mGoingRight = true;
    mSlurgState = SlurgStates::ePausing_1;
    GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Slurg_Turn_Around));
}
