#include "stdafx.h"
#include "Dove.hpp"
#include "Function.hpp"
#include "Sound/Midi.hpp"
#include "Path.hpp"
#include "Math.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "stdlib.hpp"
#include "Grid.hpp"

static bool bTheOneControllingTheMusic = false;
static DynamicArrayT<Dove> gDovesArray{3};
static s32 sAbePortalTimer = 0;
static s16 sAbePortalDirection = 0;
static s16 sAbePortalWidth = 0;

Dove::Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, s32 tlvInfo, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDove);

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, resourceID);
    Animation_Init(frameTableOffset, maxW, maxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    gDovesArray.Push_Back(this);

    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Fg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Scale = Scale::Bg;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    mDoveState = State::eOnGround_0;
    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(Math_NextRandom() % 8);
    mKeepInGlobalArray = FALSE;
    mTlvInfo = tlvInfo;

    if (bTheOneControllingTheMusic)
    {
        return;
    }

    SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic = true;
}

Dove::Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eDove);

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, resourceID);
    Animation_Init(frameTableOffset, maxW, maxH, ppRes, 1);

    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;

    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    mDoveState = State::eFlyAway_1;
    mKeepInGlobalArray = TRUE;
    mFlyAwayCounter = 0;

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    mPrevX_Unused = xpos;
    mPrevY_Unused = ypos;

    mTlvInfo = 0;

    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(Math_NextRandom() & 6);

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
        if (mTlvInfo)
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
    //(Math_Sine_496DD0(mAngle) * FP_FromInteger(30)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
    //(Math_Cosine_496CD0(mAngle) * FP_FromInteger(35)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale;
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

ALIVE_VAR(1, 0x5BC10C, s32, bExtraSeqStarted_5BC10C, 0);

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
                if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(2), sControlledCharacter_5C1B8C))
                {
                    Dove::All_FlyAway(1);
                }
                if (VIsObjNearby(ScaleToGridSize(mBaseAnimatedWithPhysicsGameObject_SpriteScale) * FP_FromInteger(4), sControlledCharacter_5C1B8C))
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
                const AnimRecord& rec = AnimRec(AnimId::Dove_Flying);
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(rec.mFrameTableOffset, 0);
                if (!bExtraSeqStarted_5BC10C)
                {
                    bExtraSeqStarted_5BC10C = 13;
                    SfxPlayMono(SoundEffect::Dove_13, 0);
                }
            }

            if (mFlyAwayCounter > 0)
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
                mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            }

            mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(1.03));
            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(1.03));

            if (mFlyAwayCounter >= (25 - (Math_NextRandom() % 8)))
            {
                mFlyAwayCounter += (Math_NextRandom() % 8) - 25;
                mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
            }

            if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            }
            else
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
            break;

        case State::eJoin_2:
        {
            if (static_cast<s32>(sGnFrame) > mJoinDeadTimer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            FP xOff = {};
            if (mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = FP_FromInteger(4);
            }
            else
            {
                xOff = FP_FromInteger(-4);
            }

            mBaseAnimatedWithPhysicsGameObject_VelX = (xOff + mJoinX - mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(8);
            mBaseAnimatedWithPhysicsGameObject_VelY = (mJoinY - mBaseAnimatedWithPhysicsGameObject_YPos) / FP_FromInteger(8);
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
        }
            return;

        case State::eCircle_3:
            mPrevX_Unused = mBaseAnimatedWithPhysicsGameObject_XPos;
            mPrevY_Unused = mBaseAnimatedWithPhysicsGameObject_YPos;

            mAngle += 4;

            // Spin around this point
            mBaseAnimatedWithPhysicsGameObject_XPos = ((Math_Sine_496DD0(mAngle) * FP_FromInteger(30)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mJoinX;
            mBaseAnimatedWithPhysicsGameObject_YPos = ((Math_Cosine_496CD0(mAngle) * FP_FromInteger(35)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mJoinY;
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

            mPrevY_Unused = mBaseAnimatedWithPhysicsGameObject_YPos;
            mAngle += 4;
            mPrevX_Unused = mBaseAnimatedWithPhysicsGameObject_XPos;
            mBaseAnimatedWithPhysicsGameObject_XPos = ((Math_Sine_496DD0(mAngle) * FP_FromInteger(sAbePortalWidth)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mJoinX;
            mBaseAnimatedWithPhysicsGameObject_YPos = ((Math_Cosine_496CD0(mAngle) * FP_FromInteger(35)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + mJoinY;
            return;

        default:
            break;
    }

    if (!gMap.Is_Point_In_Current_Camera(
            mBaseAnimatedWithPhysicsGameObject_LvlNumber,
            mBaseAnimatedWithPhysicsGameObject_PathNumber,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
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
