#include "stdafx_ao.h"
#include "Dove.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Math.hpp"
#include "Midi.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "../relive_lib/FixedPoint.hpp"
#include "Path.hpp"

namespace AO {

static bool bTheOneControllingTheMusic = false;
static DynamicArrayT<Dove> gDovesArray{10};
static s32 sAbePortalTimer = 0;
static s16 sAbePortalWidth = 30;
static s16 sAbePortalDirection = -1;

void Dove::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dove_Flying));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dove_Idle));
}

Dove::Dove(AnimId animId, const Guid& tlvId, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBird);

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
    if (mVelX >= FP_FromInteger(0))
    {
        GetAnimation().SetFlipX(false);
    }
    else
    {
        GetAnimation().SetFlipX(true);
    }

    mDoveState = State::eOnGround_0;

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() & 3));
    GetAnimation().SetFrame(Math_NextRandom() & 7);
    mKeepInGlobalArray = false;
    mTlvInfo = tlvId;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(30, 30, 30);
    }

    if (bTheOneControllingTheMusic)
    {
        return;
    }

    SND_SEQ_PlaySeq(SeqId::Unknown_24, 0, 1);
    bTheOneControllingTheMusic = true;
}

Dove::Dove(AnimId animId, FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBird);

    LoadAnimations();
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
    if (scale >= FP_FromInteger(0))
    {
        GetAnimation().SetFlipX(false);
    }
    else
    {
        GetAnimation().SetFlipX(true);
    }

    mVelY = FP_FromInteger(-4 - ((Math_NextRandom()) & 3));
    mDoveState = Dove::State::eFlyAway_1;
    mKeepInGlobalArray = true;
    mFlyAwayCounter = 0;

    mXPos = xpos;
    mYPos = ypos;

    mTlvInfo = {};

    GetAnimation().SetFrame((Math_NextRandom() & 6) + 1);

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(30, 30, 30);
    }

    if (bTheOneControllingTheMusic)
    {
        return;
    }
    SND_SEQ_PlaySeq(SeqId::Unknown_24, 0, 1);
    bTheOneControllingTheMusic = 1;
}

Dove::~Dove()
{
    if (!mKeepInGlobalArray)
    {
        gDovesArray.Remove_Item(this);
        if (mTlvInfo.IsValid())
        {
            Path::TLV_Reset(mTlvInfo, -1, 0, 0);
        }
    }

    if (bTheOneControllingTheMusic)
    {
        SND_Seq_Stop(SeqId::Unknown_24);
        bTheOneControllingTheMusic = 0;
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

    // TODO: Removed unused code
}

void Dove::AsJoin(FP xpos, FP ypos)
{
    mJoinX = xpos;
    mJoinY = ypos;
    mDoveState = State::eJoin_2;
    mJoinDeadTimer = sGnFrame + 47;
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

s32 bExtraSeqStarted_4FF944 = 0;


void Dove::VRender(PrimHeader** ppOt)
{
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

void Dove::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!bTheOneControllingTheMusic)
    {
        SND_SEQ_PlaySeq(SeqId::Unknown_24, 0, 1);
        bTheOneControllingTheMusic = 1;
    }

    switch (mDoveState)
    {
        case State::eOnGround_0:
            if (EventGet(kEventSpeaking))
            {
                for (s32 i = 0; i < gDovesArray.Size(); i++)
                {
                    Dove* pDoveIter = gDovesArray.ItemAt(i);
                    if (!pDoveIter)
                    {
                        break;
                    }
                    pDoveIter->FlyAway(0); // something is speaking, leg it
                }

                bExtraSeqStarted_4FF944 = 0;
                if (bTheOneControllingTheMusic)
                {
                    SND_Seq_Stop(SeqId::Unknown_24);
                    bTheOneControllingTheMusic = 0;
                }
            }

            if (FP_GetExponent(FP_Abs(mXPos - sControlledCharacter->mXPos)) < 100)
            {
                if (EventGet(kEventNoise))
                {
                    for (s32 i = 0; i < gDovesArray.Size(); i++)
                    {
                        Dove* pDoveIter = gDovesArray.ItemAt(i);
                        if (!pDoveIter)
                        {
                            break;
                        }
                        pDoveIter->FlyAway(0);
                    }

                    bExtraSeqStarted_4FF944 = 0;
                    if (bTheOneControllingTheMusic)
                    {
                        SND_Seq_Stop(SeqId::Unknown_24);
                        bTheOneControllingTheMusic = 0;
                    }
                }
            }
            break;

        case State::eFlyAway_1:
            mFlyAwayCounter++;
            if (mFlyAwayCounter == 0)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Dove_Flying));
                if (!bExtraSeqStarted_4FF944)
                {
                    bExtraSeqStarted_4FF944 = 16;
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

            if (mFlyAwayCounter >= 25 - (Math_NextRandom() & 7))
            {
                mFlyAwayCounter = (Math_NextRandom() & 7) + mFlyAwayCounter - 25;
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

            const FP k4Directed = GetAnimation().GetFlipX() ? FP_FromInteger(4) : FP_FromInteger(-4);
            mVelX = (k4Directed + mJoinX - mXPos) / FP_FromInteger(8);
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
                sAbePortalTimer = sGnFrame;
                sAbePortalWidth += sAbePortalDirection;

                if (sAbePortalWidth == 0)
                {
                    sAbePortalDirection = 1;
                }
                else if (sAbePortalWidth == 30)
                {
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

void Dove::All_FlyAway()
{
    for (s32 i = 0; i < gDovesArray.Size(); i++)
    {
        Dove* pDove = gDovesArray.ItemAt(i);
        if (!pDove)
        {
            break;
        }

        pDove->FlyAway(0);
    }

    bExtraSeqStarted_4FF944 = 0;
    if (bTheOneControllingTheMusic)
    {
        SND_Seq_Stop(SeqId::Unknown_24);
        bTheOneControllingTheMusic = false;
    }
}

} // namespace AO
