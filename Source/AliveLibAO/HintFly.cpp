#include "stdafx_ao.h"
#include "../relive_lib/Function.hpp"
#include "HintFly.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "../relive_lib/GameObjects/ScreenManager.hpp"
#include "Game.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/PathDataExtensionsTypes.hpp"
#include "PathData.hpp"
#include "Path.hpp"
#include "Map.hpp"
#include "HintFlyData.hpp"

#ifdef GetMessage
#undef GetMessage
#endif

namespace AO {

class HintFlyMessages final
{
public:
    const char_type* GetMessage(u32 msgId) const
    {
        if (msgId < ALIVE_COUNTOF(sHintFlyMessages))
        {
            return sHintFlyMessages[msgId];
        }
        else
        {
            LOG_WARNING("HintFly message out of bounds using original message table id: %d", msgId);
            return sHintFlyMessages[0];
        }
    }
};
static HintFlyMessages gHintFlyMessages;

struct HintFlyParticle final
{
    FP mXPos;
    FP mYPos;
    s8 mState;
    FP mVelX;
    FP mVelY;
    FP mTargetX;
    FP mTargetY;
    s16 mSoundPitch;
    s16 mSoundPitchSpeed;
    s8 mAngle;
    s8 mAngleSpeed;
    Poly_FT4 mSprt;
};

u8 sHintFlyRndSeed_4C6AA4 = 37;

static u8 HintFly_NextRandom()
{
    return gRandomBytes[sHintFlyRndSeed_4C6AA4++];
}

HintFly::HintFly(relive::Path_HintFly* pTlv, const Guid& tlvId)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::HintFly));
    Animation_Init(GetAnimRes(AnimId::HintFly));

    GetAnimation().SetSemiTrans(false);
    mTlvId = tlvId;
    mMsgIdx = 0;

    mMessageId = pTlv->mMessageId;

    mXPos = FP_FromInteger(pTlv->mTopLeftX);
    mYPos = FP_FromInteger(pTlv->mTopLeftY);

    const char_type* pMsg = gHintFlyMessages.GetMessage(pTlv->mMessageId);

    mCounter = 20;
    mMsgLength = 0;

    // Find the length of the longest word (in particles)
    s16 longestWordLen = 0;
    s16 curWordLen = 0;
    for (;;)
    {
        // End of word?
        if (*pMsg == ' ' || *pMsg == 0)
        {
            if (curWordLen > longestWordLen)
            {
                longestWordLen = curWordLen;
            }
            curWordLen = 0;
            if (!*pMsg)
            {
                break;
            }
        }
        else
        {
            curWordLen += pHintFlyAlphabet_4C7268[(*pMsg) - 'A'][0];
        }
        pMsg++;
    }

    mMsgLength = longestWordLen;
    mMsgLength += 12;

    mHintFlyParticle = relive_new HintFlyParticle[mMsgLength];
    if (mHintFlyParticle)
    {
        mState = State::eIdleWaitForChanting_1;
        mTimer = 0;

        const auto pHeader = GetAnimation().Get_FrameHeader(-1);

        for (s32 i = 0; i < mMsgLength; i++)
        {
            Poly_FT4* pSprt = &mHintFlyParticle[i].mSprt;

            pSprt->SetSemiTransparent(true);
            pSprt->SetShadeTex(true);

            pSprt->SetUV0(0, 0);

            pSprt->mAnim = &GetAnimation();
            pSprt->SetXYWH(0, 0, static_cast<s16>(pHeader->mWidth - 1), static_cast<s16>(pHeader->mHeight - 1));
        }


        mScreenX = FP_GetExponent(mXPos + FP_FromInteger(gScreenManager->mCamXOff) - gScreenManager->mCamPos->x);
        mScreenY = FP_GetExponent(mYPos + FP_FromInteger(gScreenManager->mCamYOff) - gScreenManager->mCamPos->y);

        for (s32 i = 0; i < mCounter; i++)
        {
            HintFlyParticle* pParticle = &mHintFlyParticle[i];
            InitParticle(pParticle);
        }
    }
    else
    {
        SetDead(true);
    }
}

void HintFly::InitParticle(HintFlyParticle* pParticle)
{
    pParticle->mState = 3;
    pParticle->mXPos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + mScreenX - 16);
    pParticle->mYPos = FP_FromInteger((HintFly_NextRandom() & 0x1F) + mScreenY - 16);
    pParticle->mAngle = HintFly_NextRandom();
    pParticle->mAngleSpeed = (HintFly_NextRandom() % 4) + 12;
    if (HintFly_NextRandom() % 2)
    {
        pParticle->mAngleSpeed = -pParticle->mAngleSpeed;
    }
}

void HintFly::VScreenChanged()
{
    SetDead(true);
}

HintFly::~HintFly()
{
    relive_delete[] mHintFlyParticle;
    Path::TLV_Reset(mTlvId);
}

void HintFly::FormWordAndAdvanceToNextWord()
{
    const char_type* msgPtr = &gHintFlyMessages.GetMessage(mMessageId)[mMsgIdx];
    LOG_INFO("Word is %s", msgPtr);

    // Find how long the word is
    s16 letterCount = 0;
    const char_type* pEndWord = msgPtr;
    for (; *pEndWord != ' '; pEndWord++)
    {
        if (!*pEndWord)
        {
            break;
        }
        letterCount++;
    }

    // Next time start at the next word
    mMsgIdx += letterCount;

    // Also skip the space if we had one else display will be screwed
    if (*pEndWord == ' ')
    {
        mMsgIdx++;
    }

    const s32 xBase = mScreenX - (16 * letterCount) / 2;
    const s32 yBase = mScreenY - 8;

    FP xBaseFP = FP_FromInteger(xBase);
    const FP yBaseFP = FP_FromInteger(yBase);

    s32 particleIdx = 0;
    for (s32 i = 0; i < letterCount; i++)
    {
        const auto pArray = pHintFlyAlphabet_4C7268[msgPtr[i] - 'A'];
        //const auto pArray = HintFlyLetter_E_circumflex_4C7120; // letter test code
        // First element is the count of "pixels" that make up a word
        const s32 total = pArray[0];
        for (s32 j = 0; j < total; j++)
        {
            // Position each "pixel" of the word
            HintFlyParticle* pParticleIter = &mHintFlyParticle[particleIdx++];

            auto xVal = FP_FromInteger(pArray[j + 1] >> 4);
            auto yVal = FP_FromInteger((pArray[j + 1]) & 0xF);

            pParticleIter->mTargetX = (FP_FromDouble(0.8) * xVal) + xBaseFP;
            pParticleIter->mTargetY = (FP_FromDouble(0.8) * yVal) + yBaseFP;
            pParticleIter->mVelX = FP_FromRaw((pParticleIter->mTargetX.fpValue - pParticleIter->mXPos.fpValue) >> 4);
            pParticleIter->mVelY = FP_FromRaw((pParticleIter->mTargetY.fpValue - pParticleIter->mYPos.fpValue) >> 4);
            pParticleIter->mState = 1;

            pParticleIter->mSoundPitch = Math_RandomRange(-127, 127);
            pParticleIter->mSoundPitchSpeed = pParticleIter->mSoundPitch >> 4;
        }
        xBaseFP += FP_FromInteger(16);
    }

    while (particleIdx < mCounter)
    {
        mHintFlyParticle[particleIdx++].mState = 3;
    }
}

void HintFly::UpdateParticles()
{
    for (s32 i = 0; i < mCounter; i++)
    {
        HintFlyParticle* pParticle = &mHintFlyParticle[i];
        if (pParticle->mState == 1)
        {
            const FP xTargetDelta = FP_Abs(pParticle->mTargetX - pParticle->mXPos);
            const FP yTargetDelta = FP_Abs(pParticle->mTargetY - pParticle->mYPos);

            if (xTargetDelta >= FP_FromInteger(1) || yTargetDelta >= FP_FromInteger(1))
            {
                pParticle->mXPos += pParticle->mVelX;
                pParticle->mYPos += pParticle->mVelY;
                pParticle->mSoundPitch += pParticle->mSoundPitchSpeed;
            }
            else
            {
                pParticle->mState = 2;
                pParticle->mSoundPitch = 0;
            }

            if (i < 1 && Math_RandomRange(0, 100) < 40)
            {
                SFX_Play_Pitch(relive::SoundEffects::HintFly, Math_RandomRange(24, 30), pParticle->mSoundPitch);
            }
        }
        else if (pParticle->mState == 2)
        {
            if (!(HintFly_NextRandom() & 3))
            {
                pParticle->mXPos = pParticle->mTargetX + FP_FromInteger(HintFly_NextRandom() & 1);
            }

            if (i < 1 && Math_RandomRange(0, 100) < 40)
            {
                SFX_Play_Pitch(relive::SoundEffects::HintFly, Math_RandomRange(24, 30), 0);
            }
        }
        else if (pParticle->mState == 3)
        {
            pParticle->mAngle = pParticle->mAngle + pParticle->mAngleSpeed;
            pParticle->mXPos += (Math_Sine(pParticle->mAngle) * FP_FromInteger(5));
            pParticle->mYPos += (Math_Cosine(pParticle->mAngle) * FP_FromInteger(2));
            if (i < 1 && Math_RandomRange(0, 100) < 20)
            {
                SfxPlayMono(relive::SoundEffects::HintFly, Math_RandomRange(18, 24));
            }
        }
    }
}

void HintFly::VUpdate()
{
    if (EventGet(Event::kEventDeathReset))
    {
        SetDead(true);
    }

    switch (mState)
    {
        case State::eIdleWaitForChanting_1:
            UpdateParticles();

            if (EventGet(Event::kEventAbeOhm))
            {
                mMsgIdx = 0;
                mState = State::eState_3;
                mTimer = MakeTimer(15);
            }
            return;

        case State::eState_2:
            if (mCounter == mTargetCount)
            {
                UpdateParticles();

                if (!EventGet(Event::kEventAbeOhm))
                {
                    for (s32 i = 0; i < mCounter; i++)
                    {
                        if (mHintFlyParticle[i].mState == 3)
                        {
                            break;
                        }
                        mHintFlyParticle[i].mState = 3;
                    }
                    mHintFlyIdx = 0;
                    mState = State::eState_5;
                    return;
                }

                if (mCounter == mTargetCount)
                {
                    FormWordAndAdvanceToNextWord();
                    mState = State::eState_4;
                    mTimer = MakeTimer(30);
                }
            }

            for (s32 i = 0; i < 8; i++)
            {
                if (mCounter < mTargetCount)
                {
                    InitParticle(&mHintFlyParticle[mCounter++]);
                }
                else
                {
                    mCounter--;
                    if (mHintFlyIdx < mCounter)
                    {
                        InitParticle(&mHintFlyParticle[mHintFlyIdx++]);
                    }
                }

                if (mCounter == mTargetCount)
                {
                    break;
                }
            }

            // TODO: This block is duplicated above
            UpdateParticles();

            if (!EventGet(Event::kEventAbeOhm))
            {
                for (s32 i = 0; i < mCounter; i++)
                {
                    if (mHintFlyParticle[i].mState == 3)
                    {
                        break;
                    }
                    mHintFlyParticle[i].mState = 3;
                }
                mHintFlyIdx = 0;
                mState = State::eState_5;
                return;
            }

            if (mCounter == mTargetCount)
            {
                FormWordAndAdvanceToNextWord();
                mState = State::eState_4;
                mTimer = MakeTimer(30);
            }
            break;

        case State::eState_3:
            UpdateParticles();

            if (!EventGet(Event::kEventAbeOhm))
            {
                for (s32 i = 0; i < mCounter; i++)
                {
                    if (mHintFlyParticle[i].mState == 3)
                    {
                        break;
                    }
                    mHintFlyParticle[i].mState = 3;
                }
                mHintFlyIdx = 0;
                mState = State::eState_5;
                return;
            }

            if (static_cast<s32>(sGnFrame) > mTimer)
            {
                s16 len = 0;
                const char_type* pMsgIter = gHintFlyMessages.GetMessage(mMessageId) + mMsgIdx;
                while (*pMsgIter != ' ' && *pMsgIter != '\0')
                {
                    len += pHintFlyAlphabet_4C7268[(*pMsgIter) - 'A'][0];
                    pMsgIter++;
                }
                mHintFlyIdx = 0;
                mTargetCount = len + 12;
                mState = State::eState_2;
            }
            return;

        case State::eState_4:
            UpdateParticles();

            if (EventGet(Event::kEventAbeOhm))
            {
                if (static_cast<s32>(sGnFrame) > mTimer)
                {
                    for (s32 i = 0; i < mCounter; i++)
                    {
                        if (mHintFlyParticle[i].mState == 3)
                        {
                            break;
                        }
                        mHintFlyParticle[i].mState = 3;
                    }
                    mState = State::eState_3;
                    mTimer = MakeTimer(15);
                }
            }
            else
            {
                for (s32 i = 0; i < mCounter; i++)
                {
                    if (mHintFlyParticle[i].mState == 3)
                    {
                        break;
                    }
                    mHintFlyParticle[i].mState = 3;
                }
                mHintFlyIdx = 0;
                mState = State::eState_5;
            }
            return;

        case State::eState_5:
            if (mCounter == 20)
            {
                if (EventGet(Event::kEventAbeOhm))
                {
                    mState = State::eIdleWaitForChanting_1;
                }
            }
            [[fallthrough]];

        case State::eState_6:
            UpdateParticles();
            if (mHintFlyIdx >= 20)
            {
                mCounter -= 8;
                if (mCounter < 20)
                {
                    mCounter = 20;
                }
            }
            else
            {
                InitParticle(&mHintFlyParticle[mHintFlyIdx++]);
            }

            if (mCounter == 20 && !EventGet(Event::kEventAbeOhm))
            {
                mState = State::eIdleWaitForChanting_1;
            }
            return;

        default:
            return;
    }
}

void HintFly::VRender(OrderingTable& ot)
{
    for (s32 i = 0; i < mCounter; i++)
    {
        HintFlyParticle* pParticle = &mHintFlyParticle[i];
        Poly_FT4* pSprt = &pParticle->mSprt;

        const s16 flyX = FP_GetExponent(PsxToPCX(pParticle->mXPos, FP_FromInteger(11)));
        const s16 flyY = FP_GetExponent(pParticle->mYPos);

        const s16 flyW = static_cast<s16>(abs(pSprt[0].X0() - pSprt[0].X3()));
        const s16 flyH = static_cast<s16>(abs(pSprt[0].Y0() - pSprt[0].Y3()));

        pSprt->SetXYWH(flyX, flyY, flyW, flyH);
        pSprt->SetBlendMode(relive::TBlendModes::eBlend_1);

        ot.Add(Layer::eLayer_Above_FG1_39, pSprt);
    }
}

} // namespace AO
