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

static bool bTheOneControllingTheMusic_5BC112 = false;
static DynamicArrayT<Dove> gDovesArray_5BC100{3};

Dove::Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, s32 tlvInfo, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eDove_35);

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, resourceID);
    Animation_Init(frameTableOffset, maxW, maxH, ppRes, 1, 1);

    mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    gDovesArray_5BC100.Push_Back(this);

    mAnim.field_14_scale = scale;
    mSpriteScale = scale;
    if (scale == FP_FromInteger(1))
    {
        mScale = 1;
        mAnim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mScale = 0;
        mAnim.mRenderLayer = Layer::eLayer_8;
    }

    mVelX = FP_FromInteger(Math_NextRandom() / 12 - 11);
    if (mVelX >= FP_FromInteger(0))
    {
        mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    field_FE_state = State::eOnGround_0;
    mAnim.SetFrame(Math_NextRandom() % 8);
    field_FC_keepInGlobalArray = FALSE;
    field_F8_tlvInfo = tlvInfo;

    if (bTheOneControllingTheMusic_5BC112)
    {
        return;
    }

    SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic_5BC112 = true;
}

Dove::Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, FP xpos, FP ypos, FP scale)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(AETypes::eDove_35);

    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, resourceID);
    Animation_Init(frameTableOffset, maxW, maxH, ppRes, 1, 1);

    mAnim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);
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
    if (mVelX >= FP_FromInteger(0))
    {
        mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mVelY = FP_FromInteger(-4 - (Math_NextRandom() % 4));
    field_FE_state = State::eFlyAway_1;
    field_FC_keepInGlobalArray = TRUE;
    field_F4_counter = 0;

    mXPos = xpos;
    mYPos = ypos;
    field_110_prevX = xpos;
    field_114_prevY = ypos;

    field_F8_tlvInfo = 0;

    mAnim.SetFrame(Math_NextRandom() & 6);

    if (bTheOneControllingTheMusic_5BC112)
    {
        return;
    }

    SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
    bTheOneControllingTheMusic_5BC112 = true;
}

Dove::~Dove()
{
    if (!field_FC_keepInGlobalArray)
    {
        gDovesArray_5BC100.Remove_Item(this);
        if (field_F8_tlvInfo)
        {
            Path::TLV_Reset(field_F8_tlvInfo, -1, 0, 0);
        }
    }

    if (bTheOneControllingTheMusic_5BC112)
    {
        SND_SEQ_Stop(SeqId::NecrumAmbient2_17);
        bTheOneControllingTheMusic_5BC112 = 0;
    }
}

void Dove::AsAlmostACircle(FP xpos, FP ypos, u8 angle)
{
    AsACircle(xpos, ypos, angle);
    field_FE_state = State::eAlmostACircle_4;
}

void Dove::AsACircle(FP xpos, FP ypos, u8 angle)
{
    field_100_xJoin = xpos;
    field_104_yJoin = ypos;
    field_10C_angle = angle;
    field_FE_state = State::eCircle_3;

    // TODO: Result thrown away.. some old removed behavior ??
    //(Math_Sine_496DD0(field_10C_angle) * FP_FromInteger(30)) * mSpriteScale;
    //(Math_Cosine_496CD0(field_10C_angle) * FP_FromInteger(35)) * mSpriteScale;
}

void Dove::AsJoin(FP xpos, FP ypos)
{
    field_100_xJoin = xpos;
    field_104_yJoin = ypos;
    field_FE_state = State::eJoin_2;
    field_108_timer = sGnFrame_5C1B84 + 47;
}

void Dove::FlyAway(Bool32 spookedInstantly)
{
    if (field_FE_state != State::eFlyAway_1)
    {
        field_FE_state = State::eFlyAway_1;
        if (spookedInstantly)
        {
            field_F4_counter = -1;
        }
        else
        {
            // extra delay before flying away
            field_F4_counter = -10 - Math_NextRandom() % 10;
        }
    }
}

ALIVE_VAR(1, 0x5BC10C, s32, bExtraSeqStarted_5BC10C, 0);

static s32 sAbePortalTimer_5BC114 = 0;
static s16 sAbePortalDirection_551546 = 0;
static s16 sAbePortalWidth_551544 = 0;

void Dove::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!bTheOneControllingTheMusic_5BC112)
    {
        SND_SEQ_PlaySeq(SeqId::NecrumAmbient2_17, 0, 1);
        bTheOneControllingTheMusic_5BC112 = 1;
    }

    switch (field_FE_state)
    {
        case State::eOnGround_0:
            if (Event_Get(kEventSpeaking))
            {
                Dove::All_FlyAway(0); // something is speaking, leg it
            }

            if (Event_Get(kEventNoise))
            {
                // player getting near
                if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(2), sControlledCharacter))
                {
                    Dove::All_FlyAway(1);
                }
                if (VIsObjNearby(ScaleToGridSize(mSpriteScale) * FP_FromInteger(4), sControlledCharacter))
                {
                    // noise is too near, leg it
                    Dove::All_FlyAway(0);
                }
            }
            break;

        case State::eFlyAway_1:
            field_F4_counter++;
            if (field_F4_counter == 0)
            {
                const AnimRecord& rec = AnimRec(AnimId::Dove_Flying);
                mAnim.Set_Animation_Data(rec.mFrameTableOffset, 0);
                if (!bExtraSeqStarted_5BC10C)
                {
                    bExtraSeqStarted_5BC10C = 13;
                    SFX_Play_Mono(SoundEffect::Dove_13, 0);
                }
            }

            if (field_F4_counter > 0)
            {
                mXPos += mVelX;
                mYPos += mVelY;
            }

            mVelY = (mVelY * FP_FromDouble(1.03));
            mVelX = (mVelX * FP_FromDouble(1.03));

            if (field_F4_counter >= (25 - (Math_NextRandom() % 8)))
            {
                field_F4_counter += (Math_NextRandom() % 8) - 25;
                mVelX = -mVelX;
            }

            if (mVelX >= FP_FromInteger(0))
            {
                mAnim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
            }
            else
            {
                mAnim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
            }
            break;

        case State::eJoin_2:
        {
            if (static_cast<s32>(sGnFrame_5C1B84) > field_108_timer)
            {
                mGameObjectFlags.Set(BaseGameObject::eDead);
            }

            FP xOff = {};
            if (mAnim.mAnimFlags.Get(AnimFlags::eBit5_FlipX))
            {
                xOff = FP_FromInteger(4);
            }
            else
            {
                xOff = FP_FromInteger(-4);
            }

            mVelX = (xOff + field_100_xJoin - mXPos) / FP_FromInteger(8);
            mVelY = (field_104_yJoin - mYPos) / FP_FromInteger(8);
            mXPos += mVelX;
            mYPos += mVelY;
        }
            return;

        case State::eCircle_3:
            field_110_prevX = mXPos;
            field_114_prevY = mYPos;

            field_10C_angle += 4;

            // Spin around this point
            mXPos = ((Math_Sine_496DD0(field_10C_angle) * FP_FromInteger(30)) * mSpriteScale) + field_100_xJoin;
            mYPos = ((Math_Cosine_496CD0(field_10C_angle) * FP_FromInteger(35)) * mSpriteScale) + field_104_yJoin;
            return;

        case State::eAlmostACircle_4:
            if (sAbePortalTimer_5BC114 != static_cast<s32>(sGnFrame_5C1B84))
            {
                // increase or decrease the width of the Abe portal
                sAbePortalWidth_551544 += sAbePortalDirection_551546;
                sAbePortalTimer_5BC114 = sGnFrame_5C1B84;

                if (sAbePortalWidth_551544 == 0)
                {
                    // expanding
                    sAbePortalDirection_551546 = 1;
                }
                else if (sAbePortalWidth_551544 == 30)
                {
                    // contracting
                    sAbePortalDirection_551546 = -1;
                }
            }

            field_114_prevY = mYPos;
            field_10C_angle += 4;
            field_110_prevX = mXPos;
            mXPos = ((Math_Sine_496DD0(field_10C_angle) * FP_FromInteger(sAbePortalWidth_551544)) * mSpriteScale) + field_100_xJoin;
            mYPos = ((Math_Cosine_496CD0(field_10C_angle) * FP_FromInteger(35)) * mSpriteScale) + field_104_yJoin;
            return;

        default:
            break;
    }

    if (!gMap.Is_Point_In_Current_Camera_4810D0(
            mLvlNumber,
            mPathNumber,
            mXPos,
            mYPos,
            0))
    {
        mGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void Dove::All_FlyAway(Bool32 spookedInstantly)
{
    for (s32 i = 0; i < gDovesArray_5BC100.Size(); i++)
    {
        Dove* pDove = gDovesArray_5BC100.ItemAt(i);
        if (!pDove)
        {
            break;
        }

        pDove->FlyAway(spookedInstantly);
    }

    bExtraSeqStarted_5BC10C = 0; // TODO: Never read ??
    if (bTheOneControllingTheMusic_5BC112)
    {
        SND_SEQ_Stop(SeqId::NecrumAmbient2_17);
        bTheOneControllingTheMusic_5BC112 = FALSE;
    }
}
