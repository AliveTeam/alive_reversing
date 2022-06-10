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

static bool bTheOneControllingTheMusic_4FF94C = false;
static DynamicArrayT<Dove> gDovesArray_4FF938{10};
static s32 abePortalTimer_4FF950 = 0;
static s16 abePortalWidth_4C50AC = 30;
static s16 abePortalDirection_4C50B0 = -1;

Dove::Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, s32 tlvInfo, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eBird;
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, resourceID, 1, 0);
    Animation_Init_417FD0(
        frameTableOffset,
        maxW,
        maxH,
        ppRes,
        1);
    mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit15_bSemiTrans);

    gDovesArray_4FF938.Push_Back(this);

    mBaseAnimatedWithPhysicsGameObject_Anim.field_14_scale = scale;
    mBaseAnimatedWithPhysicsGameObject_SpriteScale = scale;
    if (scale == FP_FromInteger(1))
    {
        mBaseAnimatedWithPhysicsGameObject_Scale = 1;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_27;
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Scale = 0;
        mBaseAnimatedWithPhysicsGameObject_Anim.mRenderLayer = Layer::eLayer_8;
    }

    mBaseAnimatedWithPhysicsGameObject_VelX = FP_FromInteger((Math_NextRandom() / 12 - 11));
    if (mBaseAnimatedWithPhysicsGameObject_VelX >= FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    field_EE_state = State::eOnGround_0;

    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-4 - (Math_NextRandom() & 3));
    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame(Math_NextRandom() & 7);
    field_EC_keepInGlobalArray = FALSE;
    field_E8_tlvInfo = tlvInfo;

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mBaseAnimatedWithPhysicsGameObject_Blue = 30;
        mBaseAnimatedWithPhysicsGameObject_Green = 30;
        mBaseAnimatedWithPhysicsGameObject_Red = 30;
    }

    if (bTheOneControllingTheMusic_4FF94C)
    {
        return;
    }

    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_24, 0, 1);
    bTheOneControllingTheMusic_4FF94C = true;
}

Dove::Dove(s32 frameTableOffset, s32 maxW, s32 maxH, s32 resourceID, FP xpos, FP ypos, FP scale)
{
    mBaseGameObjectTypeId = ReliveTypes::eBird;

    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, resourceID, 1, 0);
    Animation_Init_417FD0(
        frameTableOffset,
        maxW,
        maxH,
        ppRes,
        1);

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
    if (scale >= FP_FromInteger(0))
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    }
    else
    {
        mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    }

    mBaseAnimatedWithPhysicsGameObject_VelY = FP_FromInteger(-4 - ((Math_NextRandom()) & 3));
    field_EE_state = Dove::State::eFlyAway_1;
    field_EC_keepInGlobalArray = TRUE;
    field_E4_counter = 0;

    mBaseAnimatedWithPhysicsGameObject_XPos = xpos;
    mBaseAnimatedWithPhysicsGameObject_YPos = ypos;
    field_100_prevX = xpos;
    field_104_prevY = ypos;

    field_E8_tlvInfo = 0;

    mBaseAnimatedWithPhysicsGameObject_Anim.SetFrame((Math_NextRandom() & 6) + 1);

    if (gMap.mCurrentLevel == EReliveLevelIds::eStockYards || gMap.mCurrentLevel == EReliveLevelIds::eStockYardsReturn)
    {
        mBaseAnimatedWithPhysicsGameObject_Blue = 30;
        mBaseAnimatedWithPhysicsGameObject_Green = 30;
        mBaseAnimatedWithPhysicsGameObject_Red = 30;
    }

    if (bTheOneControllingTheMusic_4FF94C)
    {
        return;
    }
    SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_24, 0, 1);
    bTheOneControllingTheMusic_4FF94C = 1;
}

Dove::~Dove()
{
    if (!field_EC_keepInGlobalArray)
    {
        gDovesArray_4FF938.Remove_Item(this);
        if (field_E8_tlvInfo)
        {
            gMap.TLV_Reset(field_E8_tlvInfo, -1, 0, 0);
        }
    }

    if (bTheOneControllingTheMusic_4FF94C)
    {
        SND_Seq_Stop_477A60(SeqId::Unknown_24);
        bTheOneControllingTheMusic_4FF94C = 0;
    }
}

void Dove::AsAlmostACircle(FP xpos, FP ypos, u8 angle)
{
    AsACircle(xpos, ypos, angle);
    field_EE_state = State::eAlmostACircle_4;
}

void Dove::AsACircle(FP xpos, FP ypos, u8 angle)
{
    field_F0_xJoin = xpos;
    field_F4_yJoin = ypos;
    field_FC_angle = angle;
    field_EE_state = State::eCircle_3;

    // TODO: Removed unused code
}

void Dove::AsJoin(FP xpos, FP ypos)
{
    field_F0_xJoin = xpos;
    field_F4_yJoin = ypos;
    field_EE_state = State::eJoin_2;
    field_F8_timer = sGnFrame + 47;
}

void Dove::FlyAway(s16 a2)
{
    if (field_EE_state != State::eFlyAway_1)
    {
        field_EE_state = State::eFlyAway_1;
        if (a2)
        {
            field_E4_counter = -1;
        }
        else
        {
            field_E4_counter = -10 - Math_NextRandom() % 10;
        }
    }
}

ALIVE_VAR(1, 0x4FF944, s32, bExtraSeqStarted_4FF944, 0);

void Dove::All_FlyAway()
{
    for (s32 i = 0; i < gDovesArray_4FF938.Size(); i++)
    {
        Dove* pDove = gDovesArray_4FF938.ItemAt(i);
        if (!pDove)
        {
            break;
        }
        pDove->FlyAway(0);
    }

    bExtraSeqStarted_4FF944 = 0;
    if (bTheOneControllingTheMusic_4FF94C)
    {
        SND_Seq_Stop_477A60(SeqId::Unknown_24);
        bTheOneControllingTheMusic_4FF94C = FALSE;
    }
}

void Dove::VRender(PrimHeader** ppOt)
{
    BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
}

void Dove::VUpdate()
{
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!bTheOneControllingTheMusic_4FF94C)
    {
        SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_24, 0, 1);
        bTheOneControllingTheMusic_4FF94C = 1;
    }

    switch (field_EE_state)
    {
        case State::eOnGround_0:
            if (Event_Get(kEventSpeaking))
            {
                for (s32 i = 0; i < gDovesArray_4FF938.Size(); i++)
                {
                    Dove* pDoveIter = gDovesArray_4FF938.ItemAt(i);
                    if (!pDoveIter)
                    {
                        break;
                    }
                    pDoveIter->FlyAway(0); // something is speaking, leg it
                }

                bExtraSeqStarted_4FF944 = 0;
                if (bTheOneControllingTheMusic_4FF94C)
                {
                    SND_Seq_Stop_477A60(SeqId::Unknown_24);
                    bTheOneControllingTheMusic_4FF94C = 0;
                }
            }

            if (FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - sControlledCharacter_50767C->mBaseAnimatedWithPhysicsGameObject_XPos)) < 100)
            {
                if (Event_Get(kEventNoise))
                {
                    for (s32 i = 0; i < gDovesArray_4FF938.Size(); i++)
                    {
                        Dove* pDoveIter = gDovesArray_4FF938.ItemAt(i);
                        if (!pDoveIter)
                        {
                            break;
                        }
                        pDoveIter->FlyAway(0);
                    }

                    bExtraSeqStarted_4FF944 = 0;
                    if (bTheOneControllingTheMusic_4FF94C)
                    {
                        SND_Seq_Stop_477A60(SeqId::Unknown_24);
                        bTheOneControllingTheMusic_4FF94C = 0;
                    }
                }
            }
            break;

        case State::eFlyAway_1:
            field_E4_counter++;
            if (field_E4_counter == 0)
            {
                mBaseAnimatedWithPhysicsGameObject_Anim.Set_Animation_Data(4988, nullptr);
                if (!bExtraSeqStarted_4FF944)
                {
                    bExtraSeqStarted_4FF944 = 16;
                    SFX_Play_Mono(SoundEffect::Dove_16, 0, 0);
                }
            }

            if (field_E4_counter > 0)
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
                mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
            }

            mBaseAnimatedWithPhysicsGameObject_VelY = (mBaseAnimatedWithPhysicsGameObject_VelY * FP_FromDouble(1.03));
            mBaseAnimatedWithPhysicsGameObject_VelX = (mBaseAnimatedWithPhysicsGameObject_VelX * FP_FromDouble(1.03));

            if (field_E4_counter >= 25 - (Math_NextRandom() & 7))
            {
                field_E4_counter = (Math_NextRandom() & 7) + field_E4_counter - 25;
                mBaseAnimatedWithPhysicsGameObject_VelX = -mBaseAnimatedWithPhysicsGameObject_VelX;
            }

            mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Set(AnimFlags::eBit5_FlipX, mBaseAnimatedWithPhysicsGameObject_VelX < FP_FromInteger(0));
            break;

        case State::eJoin_2:
        {
            if (static_cast<s32>(sGnFrame) > field_F8_timer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }

            const FP k4Directed = mBaseAnimatedWithPhysicsGameObject_Anim.mAnimFlags.Get(AnimFlags::eBit5_FlipX) ? FP_FromInteger(4) : FP_FromInteger(-4);
            mBaseAnimatedWithPhysicsGameObject_VelX = (k4Directed + field_F0_xJoin - mBaseAnimatedWithPhysicsGameObject_XPos) / FP_FromInteger(8);
            mBaseAnimatedWithPhysicsGameObject_XPos += mBaseAnimatedWithPhysicsGameObject_VelX;
            mBaseAnimatedWithPhysicsGameObject_VelY = (field_F4_yJoin - mBaseAnimatedWithPhysicsGameObject_YPos) / FP_FromInteger(8);
            mBaseAnimatedWithPhysicsGameObject_YPos += mBaseAnimatedWithPhysicsGameObject_VelY;
        }
            return;

        case State::eCircle_3:
            field_100_prevX = mBaseAnimatedWithPhysicsGameObject_XPos;
            field_104_prevY = mBaseAnimatedWithPhysicsGameObject_YPos;

            field_FC_angle += 4;

            // Spin around this point
            mBaseAnimatedWithPhysicsGameObject_XPos = ((Math_Sine_451110(field_FC_angle) * FP_FromInteger(30)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + field_F0_xJoin;
            mBaseAnimatedWithPhysicsGameObject_YPos = ((Math_Cosine_4510A0(field_FC_angle) * FP_FromInteger(35)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + field_F4_yJoin;
            return;

        case State::eAlmostACircle_4:
            if (abePortalTimer_4FF950 != static_cast<s32>(sGnFrame))
            {
                abePortalTimer_4FF950 = sGnFrame;
                abePortalWidth_4C50AC += abePortalDirection_4C50B0;

                if (abePortalWidth_4C50AC == 0)
                {
                    abePortalDirection_4C50B0 = 1;
                }
                else if (abePortalWidth_4C50AC == 30)
                {
                    abePortalDirection_4C50B0 = -1;
                }
            }
            field_100_prevX = mBaseAnimatedWithPhysicsGameObject_XPos;
            field_FC_angle += 4;
            field_104_prevY = mBaseAnimatedWithPhysicsGameObject_YPos;
            mBaseAnimatedWithPhysicsGameObject_XPos = ((Math_Sine_451110(field_FC_angle) * FP_FromInteger(abePortalWidth_4C50AC)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + field_F0_xJoin;
            mBaseAnimatedWithPhysicsGameObject_YPos = ((Math_Cosine_4510A0(field_FC_angle) * FP_FromInteger(35)) * mBaseAnimatedWithPhysicsGameObject_SpriteScale) + field_F4_yJoin;
            return;

        default:
            break;
    }

    const s32 doveScreenYPos = FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_YPos - pScreenManager_4FF7C8->mCamPos->field_4_y));
    if (doveScreenYPos > pScreenManager_4FF7C8->field_16_ypos)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    const s32 doveScreenXPos = FP_GetExponent(FP_Abs(mBaseAnimatedWithPhysicsGameObject_XPos - pScreenManager_4FF7C8->mCamPos->field_0_x));
    if (doveScreenXPos > pScreenManager_4FF7C8->field_14_xpos)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

} // namespace AO
