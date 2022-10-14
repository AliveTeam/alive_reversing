#include "stdafx.h"
#include "Dove.hpp"
#include "Function.hpp"
#include "Sound/Midi.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"
#include "Map.hpp"
#include "Path.hpp"

static bool bTheOneControllingTheMusic = false;
static DynamicArrayT<Dove> gDovesArray{3};
static s32 sAbePortalTimer = 0;
static s16 sAbePortalDirection = 0;
static s16 sAbePortalWidth = 0;

void Dove::LoadAnimations()
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dove_Flying));
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Dove_Idle));
}

Dove::Dove(AnimId animId, const Guid& tlvId, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDove);

    LoadAnimations();
    Animation_Init(GetAnimRes(animId));

    GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);

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
        GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
    }
    else
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX);
    }

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    mDoveState = State::eOnGround_0;
    GetAnimation().SetFrame(Math_NextRandom() % 8);
    mKeepInGlobalArray = FALSE;
    mTlvInfo = tlvId;

    if (bTheOneControllingTheMusic)
    {
        return;
    }

    SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic = true;
}

Dove::Dove(AnimId animId, FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDove);

    LoadAnimations();
    Animation_Init(GetAnimRes(animId));

    GetAnimation().mFlags.Clear(AnimFlags::eSemiTrans);
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
    if (mVelX >= FP_FromInteger(0))
    {
        GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
    }
    else
    {
        GetAnimation().mFlags.Set(AnimFlags::eFlipX);
    }

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    mDoveState = State::eFlyAway_1;
    mKeepInGlobalArray = TRUE;
    mFlyAwayCounter = 0;

    mXPos = xpos;
    mYPos = ypos;

    mTlvInfo = {};

    GetAnimation().SetFrame(Math_NextRandom() & 6);

    if (bTheOneControllingTheMusic)
    {
        return;
    }

    SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic = true;
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
        SND_SEQ_Stop(SeqId::NecrumAmbient2_17);
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

    // TODO: Result thrown away.. some old removed behavior ??
    //(Math_Sine_496DD0(mAngle) * FP_FromInteger(30)) * mSpriteScale;
    //(Math_Cosine_496CD0(mAngle) * FP_FromInteger(35)) * mSpriteScale;
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

s32 bExtraSeqStarted_5BC10C = 0;

void Dove::VUpdate()
{
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!bTheOneControllingTheMusic)
    {
        SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
        bTheOneControllingTheMusic = 1;
    }

    switch (mDoveState)
    {
        case State::eOnGround_0:
            if (EventGet(kEventSpeaking))
            {
                Dove::All_FlyAway(0); // something is speaking, leg it
            }

            if (EventGet(kEventNoise))
            {
                // player getting near
                if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(2), sControlledCharacter))
                {
                    Dove::All_FlyAway(1);
                }
                if (VIsObjNearby(ScaleToGridSize(GetSpriteScale()) * FP_FromInteger(4), sControlledCharacter))
                {
                    // noise is too near, leg it
                    Dove::All_FlyAway(0);
                }
            }
            break;

        case State::eFlyAway_1:
            mFlyAwayCounter++;
            if (mFlyAwayCounter == 0)
            {
                GetAnimation().Set_Animation_Data(GetAnimRes(AnimId::Dove_Flying));
                if (!bExtraSeqStarted_5BC10C)
                {
                    bExtraSeqStarted_5BC10C = 13;
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

            if (mVelX >= FP_FromInteger(0))
            {
                GetAnimation().mFlags.Clear(AnimFlags::eFlipX);
            }
            else
            {
                GetAnimation().mFlags.Set(AnimFlags::eFlipX);
            }
            break;

        case State::eJoin_2:
        {
            if (static_cast<s32>(sGnFrame) > mJoinDeadTimer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            FP xOff = {};
            if (GetAnimation().mFlags.Get(AnimFlags::eFlipX))
            {
                xOff = FP_FromInteger(4);
            }
            else
            {
                xOff = FP_FromInteger(-4);
            }

            mVelX = (xOff + mJoinX - mXPos) / FP_FromInteger(8);
            mVelY = (mJoinY - mYPos) / FP_FromInteger(8);
            mXPos += mVelX;
            mYPos += mVelY;
        }
            return;

        case State::eCircle_3:
            mAngle += 4;

            // Spin around this point
            mXPos = ((Math_Sine_496DD0(mAngle) * FP_FromInteger(30)) * GetSpriteScale()) + mJoinX;
            mYPos = ((Math_Cosine_496CD0(mAngle) * FP_FromInteger(35)) * GetSpriteScale()) + mJoinY;
            return;

        case State::eAlmostACircle_4:
            if (sAbePortalTimer != static_cast<s32>(sGnFrame))
            {
                // increase or decrease the width of the Abe portal
                sAbePortalWidth += sAbePortalDirection;
                sAbePortalTimer = sGnFrame;

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
            mXPos = ((Math_Sine_496DD0(mAngle) * FP_FromInteger(sAbePortalWidth)) * GetSpriteScale()) + mJoinX;
            mYPos = ((Math_Cosine_496CD0(mAngle) * FP_FromInteger(35)) * GetSpriteScale()) + mJoinY;
            return;

        default:
            break;
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mCurrentLevel,
            mCurrentPath,
            mXPos,
            mYPos,
            0))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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

    bExtraSeqStarted_5BC10C = 0; // TODO: Never read ??
    if (bTheOneControllingTheMusic)
    {
        SND_SEQ_Stop(SeqId::NecrumAmbient2_17);
        bTheOneControllingTheMusic = FALSE;
    }
}
