#include "stdafx.h"
#include "Slurg.hpp"
#include "Function.hpp"
#include "../relive_lib/Shadow.hpp"
#include "../relive_lib/Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Events.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"

ALIVE_VAR(1, 0x5C1C08, u16, sSlurg_Step_Watch_Points_Idx_5C1C08, 0);
ALIVE_ARY(1, 0x5BD4DC, s8, 2, sSlurg_Step_Watch_Points_Count_5BD4DC, {});
ALIVE_ARY(1, 0x5C1B28, Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28, {});

void Slurg::Clear_Slurg_Step_Watch_Points()
{
    sSlurg_Step_Watch_Points_Idx_5C1C08 = !sSlurg_Step_Watch_Points_Idx_5C1C08;
    sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;

    for (auto& point : sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08].mPoints)
    {
        point = {};
    }
}

TintEntry sSlurgTints_560BCC[18] = {
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
    {EReliveLevelIds::eNone, 102u, 127u, 118u}};

Slurg::Slurg(relive::Path_Slurg* pTlv, const Guid& tlvId)
    : BaseAliveGameObject(0)
{
    mSlurgTlv = pTlv;

    mSlurgState = SlurgStates::eMoving_0;

    const AnimRecord& rec = AnimRec(AnimId::Slurg_Move);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(AnimId::Slurg_Move, ppRes);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    SetType(ReliveTypes::eSlurg);

    mXPos = FP_FromInteger((pTlv->mTopLeftX + pTlv->mBottomRightX) / 2);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    mTlvInfo = tlvId;
    if (pTlv->mScale == relive::reliveScale::eHalf)
    {
        mSlurgSpriteScale = FP_FromDouble(0.5);
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBat_Half_14;
        mScale = Scale::Bg;
    }
    else if (pTlv->mScale == relive::reliveScale::eFull)
    {
        mSlurgSpriteScale = FP_FromInteger(1);
        mAnim.mRenderLayer = Layer::eLayer_SligGreeterFartsBats_33;
        mScale = Scale::Fg;
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
            mScale == Scale::Fg ? kFgFloor : kBgFloor)
        == 1)
    {
        mYPos = hitY;
    }

    mSlurgSwitchId = pTlv->mSwitchId;

    mSlurgFlags.Clear();

    if (pTlv->mFacing == relive::reliveXDirection::eRight)
    {
        mSlurgFlags.Set(SlurgFlags::eGoingRight);
    }

    VStackOnObjectsOfType(ReliveTypes::eSlurg);
    mVisualFlags.Set(VisualFlags::eDoPurpleLightEffect);
    mShadow = relive_new Shadow();
}

s32 Slurg::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const Slurg_State*>(pData);
    auto pTlv = static_cast<relive::Path_Slurg*>(sPathInfo->TLV_From_Offset_Lvl_Cam(pState->mTlvInfo));

    if (!ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLURG.BAN", nullptr);
    }

    auto pSlurg = relive_new Slurg(pTlv, pState->mTlvInfo);

    pSlurg->mXPos = pState->mXPos;
    pSlurg->mYPos = pState->mYPos;
    pSlurg->mVelX = pState->mVelX;
    pSlurg->mAnim.mFrameChangeCounter = pState->mFrameChangeCounter;

    // OG BUG: This wasn't restored
    pSlurg->mAnim.mCurrentFrame = pState->mAnimCurrentFrame;
    pSlurg->mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, pState->mFlipX & 1);
    pSlurg->mAnim.mFlags.Set(AnimFlags::eBit3_Render, pState->mRender & 1);

    pSlurg->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->mDrawable & 1);

    if (IsLastFrame(&pSlurg->mAnim))
    {
        pSlurg->mAnim.mFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pSlurg->mSlurgState = pState->mSlurgState;

    pSlurg->mSlurgFlags.Set(SlurgFlags::eGoingRight, pState->mSlurgFlags.Get(SlurgFlags::eGoingRight));
    pSlurg->mSlurgFlags.Set(SlurgFlags::eMoving, pState->mSlurgFlags.Get(SlurgFlags::eMoving));
    return sizeof(Slurg_State);
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
    mAnim.Set_Animation_Data(AnimId::Slurg_Burst, nullptr);

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
        mAnim.Set_Animation_Data(AnimId::Slurg_Turn_Around, nullptr);
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
            if (mSlurgFlags.Get(SlurgFlags::eGoingRight))
            {
                mVelX = -FP_FromInteger(1);
            }

            mSlurgFlags.Toggle(SlurgFlags::eMoving);

            if (mSlurgFlags.Get(SlurgFlags::eMoving))
            {
                mXPos += mVelX;
            }
            break;

        case SlurgStates::ePausing_1:
            mVelX = FP_FromInteger(0);
            if (mAnim.mCurrentFrame == 0
                && gMap.Is_Point_In_Current_Camera(mCurrentLevel, mCurrentPath, mXPos, mYPos, 0))
            {
                SfxPlayMono(relive::SoundEffects::SlurgPause, 0);
            }

            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mSlurgState = SlurgStates::eMoving_0;
                mAnim.Set_Animation_Data(AnimId::Slurg_Move, nullptr);
            }
            break;

        case SlurgStates::eBurst_2:
            if (mAnim.mFlags.Get(AnimFlags::eBit18_IsLastFrame))
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
            if (mSlurgFlags.Get(SlurgFlags::eGoingRight))
            {
                GoLeft();
            }
        }
        else if (pTlv->mTlvType == ReliveTypes::eScrabRightBound)
        {
            if (!mSlurgFlags.Get(SlurgFlags::eGoingRight))
            {
                GoRight();
            }
        }
        pTlv = sPathInfo->TlvGetAt(pTlv, mXPos, mYPos, mXPos, mYPos);
    }

    if (mSlurgFlags.Get(SlurgFlags::eGoingRight))
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
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Slurg_State*>(pSaveBuffer);

    pState->mType = AETypes::eSlurg_129;
    pState->mXPos = mXPos;
    pState->mYPos = mYPos;
    pState->mVelX = mVelX;
    pState->mSlurgSpriteScale = mSlurgSpriteScale;
    pState->mFlipX = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX);
    pState->mCurrentMotion = mCurrentMotion;
    pState->mAnimCurrentFrame = mAnim.mCurrentFrame;
    pState->mFrameChangeCounter = mAnim.mFrameChangeCounter;
    pState->mDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->mRender = mAnim.mFlags.Get(AnimFlags::eBit3_Render);
    pState->mFrameTableOffset = mAnim.mFrameTableOffset;
    pState->mTlvInfo = mTlvInfo;
    pState->mSlurgState = mSlurgState;
    pState->mSlurgFlags.Set(SlurgFlags::eGoingRight, mSlurgFlags.Get(SlurgFlags::eGoingRight));
    pState->mSlurgFlags.Set(SlurgFlags::eMoving, mSlurgFlags.Get(SlurgFlags::eMoving));
    return sizeof(Slurg_State);
}

void Slurg::GoLeft()
{
    mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
    mSlurgFlags.Clear(SlurgFlags::eGoingRight);

    mSlurgState = SlurgStates::ePausing_1;
    mAnim.Set_Animation_Data(AnimId::Slurg_Turn_Around, nullptr);
}

void Slurg::GoRight()
{
    mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    mSlurgFlags.Set(SlurgFlags::eGoingRight);

    mSlurgState = SlurgStates::ePausing_1;
    mAnim.Set_Animation_Data(AnimId::Slurg_Turn_Around, nullptr);
}
