#include "stdafx_ao.h"
#include "Function.hpp"
#include "Dove.hpp"
#include "DynamicArray.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Math.hpp"
#include "Midi.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "ScreenManager.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"

namespace AO {

static bool bTheOneControllingTheMusic = false;
static DynamicArrayT<Dove> gDovesArray{10};
static s32 sAbePortalTimer = 0;
static s16 sAbePortalWidth = 30;
static s16 sAbePortalDirection = -1;

Dove::Dove(AnimId animId, s32 tlvInfo, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eBird;
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AO::AnimRec(animId).mResourceId, 1, 0);
    Animation_Init(animId, ppRes);
    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    gDovesArray.Push_Back(this);

    mAnim.field_14_scale = scale;
    mSpriteScale = scale;
    if (scale == FP_FromInteger(1))
    {
        mScale = Scale::Fg;
        mAnim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mScale = Scale::Bg;
        mAnim.mRenderLayer = Layer::eLayer_8;
    }

    mVelX = FP_FromInteger((Math_NextRandom() / 12 - 11));
    if (mVelX >= FP_FromInteger(0))
    {
        mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mDoveState = State::eOnGround_0;

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() & 3));
    mAnim.SetFrame(Math_NextRandom() & 7);
    mKeepInGlobalArray = FALSE;
    mTlvInfo = tlvInfo;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(30, 30, 30);
    }

    if (bTheOneControllingTheMusic)
    {
        return;
    }

    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_24, 0, 1);
    bTheOneControllingTheMusic = true;
}

Dove::Dove(AnimId animId, FP xpos, FP ypos, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eBird;

    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AO::AnimRec(animId).mResourceId, 1, 0);
    Animation_Init(animId, ppRes);

    mAnim.mFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    mAnim.field_14_scale = scale;
    mSpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mAnim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mAnim.mRenderLayer = Layer::eLayer_8;
    }

    mVelX = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (scale >= FP_FromInteger(0))
    {
        mAnim.mFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mAnim.mFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mVelY = FP_FromInteger(-4 - ((Math_NextRandom()) & 3));
    mDoveState = Dove::State::eFlyAway_1;
    mKeepInGlobalArray = TRUE;
    mFlyAwayCounter = 0;

    mXPos = xpos;
    mYPos = ypos;
    mPrevX_Unused = xpos;
    mPrevY_Unused = ypos;

    mTlvInfo = 0;

    mAnim.SetFrame((Math_NextRandom() & 6) + 1);

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mRGB.SetRGB(30, 30, 30);
    }

    if (bTheOneControllingTheMusic)
    {
        return;
    }
    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_24, 0, 1);
    bTheOneControllingTheMusic = 1;
}

Dove::~Dove()
{
    if (!mKeepInGlobalArray)
    {
        gDovesArray.Remove_Item(this);
        if (mTlvInfo)
        {
            gMap.TLV_Reset(mTlvInfo, -1, 0, 0);
        }
    }

    if (bTheOneControllingTheMusic)
    {
        SND_Seq_Stop_477A60(SeqId::Unknown_24);
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

ALIVE_VAR(1, 0x4FF944, s32, bExtraSeqStarted_4FF944, 0);

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
        SND_Seq_Stop_477A60(SeqId::Unknown_24);
        bTheOneControllingTheMusic = FALSE;
    }
}

void Dove::VRender(PrimHeader** ppOt)
{
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

void Dove::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!bTheOneControllingTheMusic)
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_24, 0, 1);
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
                    SND_Seq_Stop_477A60(SeqId::Unknown_24);
                    bTheOneControllingTheMusic = 0;
                }
            }

            if (FP_GetExponent(FP_Abs(mXPos - sControlledCharacter_50767C->mXPos)) < 100)
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
                        SND_Seq_Stop_477A60(SeqId::Unknown_24);
                        bTheOneControllingTheMusic = 0;
                    }
                }
            }
            break;

        case State::eFlyAway_1:
            mFlyAwayCounter++;
            if (mFlyAwayCounter == 0)
            {
                mAnim.Set_Animation_Data(AnimId::Dove_Flying, nullptr);
                if (!bExtraSeqStarted_4FF944)
                {
                    bExtraSeqStarted_4FF944 = 16;
                    SfxPlayMono(SoundEffect::Dove_16, 0, 0);
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

            mAnim.mFlags.Set(AnimFlags::eBit5_FlipX, mVelX < FP_FromInteger(0));
            break;

        case State::eJoin_2:
        {
            if (static_cast<s32>(sGnFrame) > mJoinDeadTimer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            const FP k4Directed = mAnim.mFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(4) : FP_FromInteger(-4);
            mVelX = (k4Directed + mJoinX - mXPos) / FP_FromInteger(8);
            mXPos += mVelX;
            mVelY = (mJoinY - mYPos) / FP_FromInteger(8);
            mYPos += mVelY;
        }
            return;

        case State::eCircle_3:
            mPrevX_Unused = mXPos;
            mPrevY_Unused = mYPos;

            mAngle += 4;

            // Spin around this point
            mXPos = ((Math_Sine_451110(mAngle) * FP_FromInteger(30)) * mSpriteScale) + mJoinX;
            mYPos = ((Math_Cosine_4510A0(mAngle) * FP_FromInteger(35)) * mSpriteScale) + mJoinY;
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
            mPrevX_Unused = mXPos;
            mAngle += 4;
            mPrevY_Unused = mYPos;
            mXPos = ((Math_Sine_451110(mAngle) * FP_FromInteger(sAbePortalWidth)) * mSpriteScale) + mJoinX;
            mYPos = ((Math_Cosine_4510A0(mAngle) * FP_FromInteger(35)) * mSpriteScale) + mJoinY;
            return;

        default:
            break;
    }

    const s32 doveScreenYPos = FP_GetExponent(FP_Abs(mYPos - pScreenManager->mCamPos->y));
    if (doveScreenYPos > pScreenManager->mCamYOff)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const s32 doveScreenXPos = FP_GetExponent(FP_Abs(mXPos - pScreenManager->mCamPos->x));
    if (doveScreenXPos > pScreenManager->mCamXOff)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
