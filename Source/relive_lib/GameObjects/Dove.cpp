#include "stdafx.h"
#include "Dove.hpp"
#include "../Sound/Midi.hpp"
#include "Math.hpp"
#include "../Events.hpp"
#include "Sfx.hpp"
#include "../Grid.hpp"
#include "../MapWrapper.hpp"
#include "../FixedPoint.hpp"
#include "../../AliveLibAO/Midi.hpp"
#include "../GameType.hpp"
#include "ScreenManager.hpp"
#include "IBaseAliveGameObject.hpp"

static bool bTheOneControllingTheMusic = false;
static DynamicArrayT<Dove> gDovesArray{10};
static s32 sAbePortalTimer = 0;
static s16 sAbePortalWidth = 0;
static s16 sAbePortalDirection = 0;

void Dove::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dove_Flying));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dove_Idle));
}

static inline void PlayAmbientSeq()
{
    if (GetGameType() == GameType::eAo)
    {
        SND_SEQ_PlaySeq(AO::SeqId::Unknown_24, 0, 1);
    }
    else
    {
        SND_SEQ_PlaySeq(::SeqId::NecrumAmbient2_17, 0, 1);
    }
}

static inline void StopAmbientSeq()
{
    if (GetGameType() == GameType::eAo)
    {
        SND_SEQ_Stop(AO::SeqId::Unknown_24);
    }
    else
    {
        SND_SEQ_Stop(SeqId::NecrumAmbient2_17);
    }
}

Dove::Dove(AnimId animId, const Guid& tlvId, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0),
    mDoveState(State::eOnGround_0),
    mKeepInGlobalArray(false),
    mTlvInfo(tlvId)
{
    SetType(ReliveTypes::eDove);

    LoadAnimations();
    Animation_Init(GetAnimRes(animId));

    GetAnimation().SetSemiTrans(false);

    gDovesArray.Push_Back(this);

    GetAnimation().SetSpriteScale(scale);
    SetSpriteScale(scale);
    if (scale == FP_FromInteger(1))
    {
        SetScale(Scale::Fg);
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }
    else
    {
        SetScale(Scale::Bg);
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }

    mVelX = FP_FromInteger(Math_NextRandom() / 12 - 11);
    GetAnimation().SetFlipX(mVelX < FP_FromInteger(0));

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    GetAnimation().SetFrame(Math_NextRandom() % 8);

    if (GetMap().mCurrentLevel == EReliveLevelIds::eStockYards || GetMap().mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(30, 30, 30);
    }

    if (bTheOneControllingTheMusic)
    {
        return;
    }

    PlayAmbientSeq();
    bTheOneControllingTheMusic = true;
}

Dove::Dove(AnimId animId, FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0),
    mDoveState(State::eFlyAway_1),
    mKeepInGlobalArray(true),
    mTlvInfo({})
{
    SetType(ReliveTypes::eDove);

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(animId));
    Animation_Init(GetAnimRes(animId));

    GetAnimation().SetSemiTrans(false);
    GetAnimation().SetSpriteScale(scale);
    SetSpriteScale(scale);

    if (scale == FP_FromInteger(1))
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_27);
    }
    else
    {
        GetAnimation().SetRenderLayer(Layer::eLayer_8);
    }

    mVelX = FP_FromInteger(Math_NextRandom() / 12 - 11);
    GetAnimation().SetFlipX(mVelX < FP_FromInteger(0));

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));

    mXPos = xpos;
    mYPos = ypos;

    // TODO: factor out
    s32 rndVal = GetGameType() == GameType::eAo ? 8 : 7;

    GetAnimation().SetFrame(Math_NextRandom() % rndVal);

    if (GetMap().mCurrentLevel == EReliveLevelIds::eStockYards || GetMap().mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(30, 30, 30);
    }

    if (bTheOneControllingTheMusic)
    {
        return;
    }
    PlayAmbientSeq();
    bTheOneControllingTheMusic = true;
}

Dove::~Dove()
{
    if (!mKeepInGlobalArray)
    {
        gDovesArray.Remove_Item(this);
        if (mTlvInfo.IsValid())
        {
            GetMap().TLV_Reset(mTlvInfo);
        }
    }

    if (bTheOneControllingTheMusic)
    {
        StopAmbientSeq();
        bTheOneControllingTheMusic = false;
    }
}

void Dove::AsAlmostACircle(FP xpos, FP ypos, u8 angle)
{
    AsACircle(xpos, ypos, angle);
    mDoveState = State::eAlmostACircle_4;
}

void Dove::AsACircle(FP xpos, FP ypos, u8 angle)
{
    mJoinX = xpos;
    mJoinY = ypos;
    mAngle = angle;
    mDoveState = State::eCircle_3;
}

void Dove::AsJoin(FP xpos, FP ypos)
{
    mJoinX = xpos;
    mJoinY = ypos;
    mDoveState = State::eJoin_2;
    mJoinDeadTimer = MakeTimer(47);
}

void Dove::FlyAway(bool spookedInstantly)
{
    if (mDoveState != State::eFlyAway_1)
    {
        mDoveState = State::eFlyAway_1;
        if (spookedInstantly)
        {
            mFlyAwayCounter = -1;
        }
        else
        {
            // extra delay before flying away
            mFlyAwayCounter = -10 - Math_NextRandom() % 10;
        }
    }
}

static bool sExtraSeqStarted = false;

void Dove::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!bTheOneControllingTheMusic)
    {
        PlayAmbientSeq();
        bTheOneControllingTheMusic = true;
    }

    switch (mDoveState)
    {
        case State::eOnGround_0:
            if (EventGet(kEventSpeaking))
            {
                Dove::All_FlyAway(false); // something is speaking, leg it
            }

            if (EventGet(kEventNoise))
            {
                if (GetGameType() == GameType::eAo)
                {
                    if (FP_GetExponent(FP_Abs(mXPos - sControlledCharacter->mXPos)) < 100)
                    {
                        Dove::All_FlyAway(false);
                    }
                }
                else
                {
                    // player getting near
                    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sControlledCharacter))
                    {
                        Dove::All_FlyAway(true);
                    }
                    if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4), sControlledCharacter))
                    {
                        // noise is too near, leg it
                        Dove::All_FlyAway(false);
                    }
                }
            }
            break;

        case State::eFlyAway_1:
            mFlyAwayCounter++;
            if (mFlyAwayCounter == 0)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Dove_Flying));
                if (!sExtraSeqStarted)
                {
                    sExtraSeqStarted = true;
                    SfxPlayMono(relive::SoundEffects::Dove, 0);
                }
            }

            if (mFlyAwayCounter > 0)
            {
                mXPos += mVelX;
                mYPos += mVelY;
            }

            mVelY = (mVelY * FP_FromDouble(1.03));
            mVelX = (mVelX * FP_FromDouble(1.03));

            if (mFlyAwayCounter >= (25 - (Math_NextRandom() % 8)))
            {
                mFlyAwayCounter += (Math_NextRandom() % 8) - 25;
                mVelX = -mVelX;
            }

            GetAnimation().SetFlipX(mVelX < FP_FromInteger(0));
            break;

        case State::eJoin_2:
        {
            if (static_cast<s32>(sGnFrame) > mJoinDeadTimer)
            {
                SetDead(true);
            }

            const FP xOff = GetAnimation().GetFlipX() ? FP_FromInteger(4) : FP_FromInteger(-4);
            mVelX = (xOff + mJoinX - mXPos) / FP_FromInteger(8);
            mVelY = (mJoinY - mYPos) / FP_FromInteger(8);
            mXPos += mVelX;
            mYPos += mVelY;
        }
            return;

        case State::eCircle_3:
            mAngle += 4;

            // Spin around this point
            mXPos = ((Math_Sine(mAngle) * FP_FromInteger(30)) * GetSpriteScale()) + mJoinX;
            mYPos = ((Math_Cosine(mAngle) * FP_FromInteger(35)) * GetSpriteScale()) + mJoinY;
            return;

        case State::eAlmostACircle_4:
            if (sAbePortalTimer != static_cast<s32>(sGnFrame))
            {
                // increase or decrease the width of the Abe portal
                sAbePortalTimer = sGnFrame;
                sAbePortalWidth += sAbePortalDirection;

                if (sAbePortalWidth == 0)
                {
                    // expanding
                    sAbePortalDirection = 1;
                }
                else if (sAbePortalWidth == 30)
                {
                    // contracting
                    sAbePortalDirection = -1;
                }
            }
            mAngle += 4;
            mXPos = ((Math_Sine(mAngle) * FP_FromInteger(sAbePortalWidth)) * GetSpriteScale()) + mJoinX;
            mYPos = ((Math_Cosine(mAngle) * FP_FromInteger(35)) * GetSpriteScale()) + mJoinY;
            return;

        default:
            break;
    }

    if (GetGameType() == GameType::eAo)
    {
        const s32 doveScreenYPos = FP_GetExponent(FP_Abs(mYPos - gScreenManager->mCamPos->y));
        if (doveScreenYPos > gScreenManager->mCamYOff)
        {
            SetDead(true);
        }

        const s32 doveScreenXPos = FP_GetExponent(FP_Abs(mXPos - gScreenManager->mCamPos->x));
        if (doveScreenXPos > gScreenManager->mCamXOff)
        {
            SetDead(true);
        }
    }
    else
    {
        if (!GetMap().Is_Point_In_Current_Camera(
                mCurrentLevel,
                mCurrentPath,
                mXPos,
                mYPos,
                0))
        {
            SetDead(true);
        }
    }
}

void Dove::All_FlyAway(bool spookedInstantly)
{
    for (s32 i = 0; i < gDovesArray.Size(); i++)
    {
        Dove* pDove = gDovesArray.ItemAt(i);
        if (!pDove)
        {
            break;
        }

        pDove->FlyAway(spookedInstantly);
    }

    sExtraSeqStarted = false;
    if (bTheOneControllingTheMusic)
    {
        StopAmbientSeq();
        bTheOneControllingTheMusic = false;
    }
}
