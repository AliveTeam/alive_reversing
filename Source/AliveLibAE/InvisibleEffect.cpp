#include "stdafx.h"
#include "InvisibleEffect.hpp"
#include "../relive_lib/Function.hpp"
#include "PossessionFlicker.hpp"
#include "../relive_lib/Clamp.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

#define TRANSITION_FRAMECOUNT 24

InvisibleEffect::InvisibleEffect(BaseAliveGameObject* pTarget)
    : BaseGameObject(true, 0)
{
     SetType(ReliveTypes::eInvisibleEffect);

    mTargetId = pTarget->mBaseGameObjectId;

    mPal1.mPal = std::make_shared<AnimationPal>(*pTarget->GetAnimation().mAnimRes.mTgaPtr->mPal);
    mPal2.mPal = std::make_shared<AnimationPal>(*pTarget->GetAnimation().mAnimRes.mTgaPtr->mPal);

    mSemiTrans = false;
    mBlending = false;
    mIsInvisible = false;

    if (pTarget->GetAnimation().GetSemiTrans())
    {
        mSemiTrans = true;
    }
    if (pTarget->GetAnimation().GetBlending())
    {
        mBlending = true;
    }

    mOldRenderMode = pTarget->GetAnimation().GetRenderMode();
    mState = InvisibleState::eSetRenderMode1_0;
}

InvisibleEffect::~InvisibleEffect()
{

}

void InvisibleEffect::InstantInvisibility()
{
    mIsInvisible = true;
    SetUpdateDelay(1);
    mState = InvisibleState::eSetInvisibile_1;
}

void InvisibleEffect::BecomeVisible()
{
    mTransitionFrameCount = 0;
    mState = InvisibleState::eBecomeVisible_4;
}

void InvisibleEffect::ClearInvisibility()
{
    SetUpdateDelay(1);
    mState = InvisibleState::eClearInvisibility_5;
}

void InvisibleEffect::BecomeInvisible()
{
    SetUpdateDelay(1);
    mTransitionFrameCount = 0;
    mState = InvisibleState::eSetInvisibile_1;
}

void InvisibleEffect::VUpdate()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(mTargetId));
    if (EventGet(kEventDeathReset))
    {
        SetDead(true);
    }

    if (!pTarget || pTarget->GetDead())
    {
        SetDead(true);
    }
    else
    {
        switch (mState)
        {
            case InvisibleState::eSetRenderMode1_0:
            {
                pTarget->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                return;
            }
            case InvisibleState::eSetInvisibile_1:
            {
                for (s32 idx2 = 8; idx2 < 256; idx2++)
                {
                    // Set transparent bit
                    mPal2.mPal->mPal[idx2].a = 255;
                }

                pTarget->SetInvisible(true);

                pTarget->GetAnimation().SetBlending(false);
                pTarget->GetAnimation().SetSemiTrans(true);
                pTarget->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);

                SetUpdateDelay(1);
                mState = InvisibleState::eBecomeInvisible_2;
                if (mIsInvisible)
                {
                    mState = InvisibleState::eUnknown_3;
                }
                return;
            }
            case InvisibleState::eBecomeInvisible_2:
            {
                /* - should never have been possible
                if (pTarget->mAnim.mPalDepth <= 8)
                {
                    mState = InvisibleState::eSetRenderMode1_0;
                    return;
                }*/

                if (mTransitionFrameCount < TRANSITION_FRAMECOUNT)
                {
                    for (s32 idx = 8; idx < 256; idx++)
                    {
                        // Red
                        if (mPal2.mPal->mPal[idx].r)
                        {
                            mPal2.mPal->mPal[idx].r = ClampedAdd(mPal2.mPal->mPal[idx].r, -8);
                        }

                        // Green
                        if (mPal2.mPal->mPal[idx].g)
                        {
                            mPal2.mPal->mPal[idx].g = ClampedAdd(mPal2.mPal->mPal[idx].g, -8);
                        }

                        // Blue
                        if (mPal2.mPal->mPal[idx].b)
                        {
                            mPal2.mPal->mPal[idx].b = ClampedAdd(mPal2.mPal->mPal[idx].b, -8);
                        }

                        // Semi trans
                        if (mPal2.mPal->mPal[idx].a == 255 && mPal2.mPal->mPal[idx].r == 0 && mPal2.mPal->mPal[idx].g == 0 && mPal2.mPal->mPal[idx].b == 0)
                        {
                            mPal2.mPal->mPal[idx].a = 0;
                        }
                    }

                    // TODO: Allow setting anim
                    pTarget->GetAnimation().LoadPal(mPal2);
                    SetUpdateDelay(1);

                    mTransitionFrameCount++;
                }
                else
                {
                    mTransitionFrameCount = 0;
                    mState = InvisibleState::eSetRenderMode1_0;
                }

                break;
            }
            case InvisibleState::eUnknown_3:
            {
                LOG_ERROR("Unreachable case InvisibleState::eUnknown_3 hit");
                for (s32 i = 8; i < 256; i++)
                {
                    // Clear transparent bit
                    mPal2.mPal->mPal[i].a = 0;
                }
                // TODO
                pTarget->GetAnimation().LoadPal(mPal2);
                //Pal_Set(pTarget->mAnim.mPalVramXY, pTarget->mAnim.mPalDepth, (u8*) field_30_pPal2, &field_34_pal_rect2);

                mIsInvisible = false;
                SetUpdateDelay(1);
                mState = InvisibleState::eSetRenderMode1_0;
                break;
            }
            case InvisibleState::eBecomeVisible_4:
            {
                /* TODO - shouldn't be possible
                if (pTarget->mAnim.mPalDepth <= 1)
                {
                    mState = InvisibleState::eClearInvisibility_5;
                    return;
                }*/

                if (mTransitionFrameCount < TRANSITION_FRAMECOUNT)
                {
                    for (s32 idx4 = 1; idx4 < 256; idx4++)
                    {
                        if (mPal1.mPal->mPal[idx4].r ^ mPal2.mPal->mPal[idx4].r)
                        {
                            mPal2.mPal->mPal[idx4].r = ClampedAdd<u8>(mPal2.mPal->mPal[idx4].r, 8,  0, mPal1.mPal->mPal[idx4].r);
                        }

                        if (mPal1.mPal->mPal[idx4].g ^ mPal2.mPal->mPal[idx4].g)
                        {
                            mPal2.mPal->mPal[idx4].g = ClampedAdd<u8>(mPal2.mPal->mPal[idx4].g, 8, 0, mPal1.mPal->mPal[idx4].g);
                        }

                        if (mPal1.mPal->mPal[idx4].b ^ mPal2.mPal->mPal[idx4].b)
                        {
                            mPal2.mPal->mPal[idx4].b = ClampedAdd<u8>(mPal2.mPal->mPal[idx4].b, 8, 0, mPal1.mPal->mPal[idx4].b);
                        }
                    }

                    // TODO
                    pTarget->GetAnimation().LoadPal(mPal2);

                    //Pal_Set(pTarget->mAnim.mPalVramXY, pTarget->mAnim.mPalDepth, (u8*) field_30_pPal2, &field_34_pal_rect2);

                    pTarget->GetAnimation().SetRenderMode(TPageAbr::eBlend_1);
                    SetUpdateDelay(5);

                    mTransitionFrameCount++;
                }
                else
                {
                    mTransitionFrameCount = 0;
                    mState = InvisibleState::eClearInvisibility_5;
                }

                break;
            }
            case InvisibleState::eClearInvisibility_5:
            {
                // TODO
                 pTarget->GetAnimation().LoadPal(mPal1);

                //Pal_Set(pTarget->mAnim.mPalVramXY, pTarget->mAnim.mPalDepth, (u8*) field_24_pPal1, &field_28_pal_rect1);

                pTarget->GetAnimation().SetSemiTrans(mSemiTrans);
                pTarget->GetAnimation().SetBlending(mBlending);
                pTarget->GetAnimation().SetRenderMode(mOldRenderMode);

                pTarget->SetInvisible(false);

                SetUpdateDelay(1);
                relive_new PossessionFlicker(pTarget, 16, 255, 128, 128);
                mState = InvisibleState::eSetDead_6;
                break;
            }
            case InvisibleState::eSetDead_6:
            {
                SetDead(true);
                return;
            }
            default:
            {
                return;
            }
        }
    }
}

void InvisibleEffect::VScreenChanged()
{
    // Null @ 45F9C0
}
