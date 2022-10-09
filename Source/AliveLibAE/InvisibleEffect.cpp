#include "stdafx.h"
#include "InvisibleEffect.hpp"
#include "Function.hpp"
#include "PossessionFlicker.hpp"
#include "../relive_lib/Events.hpp"
#include "../relive_lib/ObjectIds.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"

InvisibleEffect::InvisibleEffect(BaseAliveGameObject* pTarget)
    : BaseGameObject(TRUE, 0)
{
     SetType(ReliveTypes::eInvisibleEffect);

    field_44_objId = pTarget->mBaseGameObjectId;

    mPal1.mPal = std::make_shared<AnimationPal>(*pTarget->mAnim.mAnimRes.mTgaPtr->mPal);
    mPal2.mPal = std::make_shared<AnimationPal>(*pTarget->mAnim.mAnimRes.mTgaPtr->mPal);

    field_4A_flags.Clear();

    if (pTarget->mAnim.mFlags.Get(AnimFlags::eSemiTrans))
    {
        field_4A_flags.Set(Flags_4A::eSemiTrans_Bit1);
    }
    if (pTarget->mAnim.mFlags.Get(AnimFlags::eBlending))
    {
        field_4A_flags.Set(Flags_4A::eBlending_Bit2);
    }

    field_4A_flags.Clear(Flags_4A::eIsInvisible_Bit3);
    field_48_old_render_mode = pTarget->mAnim.mRenderMode;
    field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
}

InvisibleEffect::~InvisibleEffect()
{

}

void InvisibleEffect::InstantInvisibility()
{
    field_4A_flags.Set(Flags_4A::eIsInvisible_Bit3);
    SetUpdateDelay(1);
    field_20_state_or_op = InvisibleState::eSetInvisibile_1;
}

void InvisibleEffect::BecomeVisible()
{
    field_20_state_or_op = InvisibleState::eBecomeVisible_4;
}

void InvisibleEffect::ClearInvisibility()
{
    SetUpdateDelay(1);
    field_20_state_or_op = InvisibleState::eClearInvisibility_5;
}

void InvisibleEffect::BecomeInvisible()
{
    SetUpdateDelay(1);
    field_20_state_or_op = InvisibleState::eSetInvisibile_1;
}

void InvisibleEffect::VUpdate()
{
    auto pTarget = static_cast<BaseAliveGameObject*>(sObjectIds.Find_Impl(field_44_objId));
    if (EventGet(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (!pTarget || pTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    else
    {
        switch (field_20_state_or_op)
        {
            case InvisibleState::eSetRenderMode1_0:
            {
                pTarget->mAnim.mRenderMode = TPageAbr::eBlend_1;
                return;
            }
            case InvisibleState::eSetInvisibile_1:
            {
                for (s32 idx2 = 8; idx2 < 256; idx2++)
                {
                    // Set transparent bit
                    mPal2.mPal->mPal[idx2] |= 0x8000u;
                }

                pTarget->mBaseAliveGameObjectFlags.Set(AliveObjectFlags::eInvisible);

                pTarget->mAnim.mFlags.Clear(AnimFlags::eBlending);
                pTarget->mAnim.mFlags.Set(AnimFlags::eSemiTrans);
                pTarget->mAnim.mRenderMode = TPageAbr::eBlend_1;

                SetUpdateDelay(1);
                field_20_state_or_op = InvisibleState::eBecomeInvisible_2;
                if (field_4A_flags.Get(Flags_4A::eIsInvisible_Bit3))
                {
                    field_20_state_or_op = InvisibleState::eUnknown_3;
                }
                return;
            }
            case InvisibleState::eBecomeInvisible_2:
            {
                /* - should never have been possible
                if (pTarget->mAnim.mPalDepth <= 8)
                {
                    field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
                    return;
                }*/

                bool v3 = false;
                for (s32 idx = 8; idx < 256; idx++)
                {
                    // Red
                    if (mPal2.mPal->mPal[idx] & 0x1F)
                    {
                        v3 = true;
                        mPal2.mPal->mPal[idx] = mPal2.mPal->mPal[idx] - 1;
                    }

                    // Green
                    if (mPal2.mPal->mPal[idx] & 0x3E0)
                    {
                        v3 = true;
                        mPal2.mPal->mPal[idx] = mPal2.mPal->mPal[idx] - 32;
                    }

                    // Blue
                    if (mPal2.mPal->mPal[idx] & 0x7C00)
                    {
                        v3 = true;
                        mPal2.mPal->mPal[idx] = mPal2.mPal->mPal[idx] - 1024;
                    }

                    // Semi trans
                    if (mPal2.mPal->mPal[idx] == 0x8000u)
                    {
                        mPal2.mPal->mPal[idx] = 0;
                    }
                }

                if (!v3)
                {
                    field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
                }
                else
                {
                    // TODO: Allow setting anim
                    pTarget->mAnim.LoadPal(mPal2);
                    SetUpdateDelay(1);
                }


                break;
            }
            case InvisibleState::eUnknown_3:
            {
                LOG_ERROR("Unreachable case InvisibleState::eUnknown_3 hit");
                for (s32 i = 8; i < 256; i++)
                {
                    // Clear transparent bit
                    mPal2.mPal->mPal[i] &= 0x8000u;
                }
                // TODO
                pTarget->mAnim.LoadPal(mPal2);
                //Pal_Set(pTarget->mAnim.mPalVramXY, pTarget->mAnim.mPalDepth, (u8*) field_30_pPal2, &field_34_pal_rect2);

                field_4A_flags.Clear(Flags_4A::eIsInvisible_Bit3);
                SetUpdateDelay(1);
                field_20_state_or_op = InvisibleState::eSetRenderMode1_0;
                break;
            }
            case InvisibleState::eBecomeVisible_4:
            {
                /* TODO - shouldn't be possible
                if (pTarget->mAnim.mPalDepth <= 1)
                {
                    field_20_state_or_op = InvisibleState::eClearInvisibility_5;
                    return;
                }*/

                bool v3 = false;
                for (s32 idx4 = 1; idx4 < 256; idx4++)
                {
                    if ((mPal2.mPal->mPal[idx4] ^ (mPal1.mPal->mPal[idx4])) & 0x1F)
                    {
                        v3 = true;
                        mPal2.mPal->mPal[idx4] = mPal2.mPal->mPal[idx4] + 1;
                    }

                    if ((mPal2.mPal->mPal[idx4] ^ mPal1.mPal->mPal[idx4]) & 0x3E0)
                    {
                        v3 = true;
                        mPal2.mPal->mPal[idx4] = mPal2.mPal->mPal[idx4] + 32;
                    }

                    if ((mPal2.mPal->mPal[idx4] ^ mPal1.mPal->mPal[idx4]) & 0x7C00)
                    {
                        v3 = true;
                        mPal2.mPal->mPal[idx4] = mPal2.mPal->mPal[idx4] + 1024;
                    }
                }

                if (v3)
                {
                    // TODO
                    pTarget->mAnim.LoadPal(mPal2);

                    //Pal_Set(pTarget->mAnim.mPalVramXY, pTarget->mAnim.mPalDepth, (u8*) field_30_pPal2, &field_34_pal_rect2);

                    pTarget->mAnim.mRenderMode = TPageAbr::eBlend_1;
                    SetUpdateDelay(5);
                }
                else
                {
                    field_20_state_or_op = InvisibleState::eClearInvisibility_5;
                }
                break;
            }
            case InvisibleState::eClearInvisibility_5:
            {
                // TODO
                 pTarget->mAnim.LoadPal(mPal1);

                //Pal_Set(pTarget->mAnim.mPalVramXY, pTarget->mAnim.mPalDepth, (u8*) field_24_pPal1, &field_28_pal_rect1);

                pTarget->mAnim.mFlags.Set(AnimFlags::eSemiTrans, field_4A_flags.Get(Flags_4A::eSemiTrans_Bit1));
                pTarget->mAnim.mFlags.Set(AnimFlags::eBlending, field_4A_flags.Get(Flags_4A::eBlending_Bit2));
                pTarget->mAnim.mRenderMode = field_48_old_render_mode;

                pTarget->mBaseAliveGameObjectFlags.Clear(AliveObjectFlags::eInvisible);

                SetUpdateDelay(1);
                relive_new PossessionFlicker(pTarget, 16, 255, 128, 128);
                field_20_state_or_op = InvisibleState::eSetDead_6;
                break;
            }
            case InvisibleState::eSetDead_6:
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
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
