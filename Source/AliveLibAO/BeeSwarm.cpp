#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeSwarm.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "../AliveLibAE/stdlib.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "../relive_lib/PsxDisplay.hpp"
#include "../relive_lib/ScreenManager.hpp"
#include "../relive_lib/Events.hpp"
#include "Math.hpp"
#include "CameraSwapper.hpp"
#include "Sfx.hpp"
#include "../relive_lib/Collisions.hpp"
#include <algorithm>

#undef max
#undef min

namespace AO {

s16 gBeeInstanceCount = 0;
s16 gBeesNearAbe = 0;

BeeSwarm::BeeSwarm(FP xpos, FP ypos, FP speed, s32 numBees, s32 totalChaseTime)
    : BaseAnimatedWithPhysicsGameObject(0)
{
    SetType(ReliveTypes::eBeeSwarm);

    s32 numBeesToUse = 0;
    gBeeInstanceCount++;
    if (gBeeInstanceCount < 3)
    {
        numBeesToUse = numBees;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        numBeesToUse = 1;
    }

    mLoadedAnims.push_back(ResourceManagerWrapper::LoadAnimation(AnimId::Bee_Swarm));
    Animation_Init(GetAnimRes(AnimId::Bee_Swarm));

    if (numBeesToUse <= 25)
    {
        mMaxBeesCount = static_cast<s16>(numBeesToUse);
    }
    else
    {
        mMaxBeesCount = 25;
    }

    field_D78_speed = speed;

    mChaseTargetX = xpos;
    mChaseTargetY = ypos;

    mTotalChaseTime = totalChaseTime;
    field_D68_xpos = xpos;
    mXPos = xpos;
    field_D6C_ypos = ypos;
    mSwarmState = BeeSwarmStates::eIdle_0;
    mChaseTarget = 0;
    field_DA4_update_chase_timer = 0;
    mAliveTimer = 0;
    field_DA0_do_damage_or_pain_sound_timer = 0;
    mRectX = FP_FromInteger(0);
    mRectW = FP_FromInteger(1);
    mRectY = FP_FromInteger(0);
    mRectH = FP_FromInteger(1);
    mCurrentBeesCount = 0;
    field_D7C_pos_offset = FP_FromInteger(0);
    mYPos = ypos;
}

BeeSwarm::~BeeSwarm()
{
    gBeeInstanceCount--;
    gBeesNearAbe = 0;

    if (mChaseTarget)
    {
        mChaseTarget->mBaseGameObjectRefCount--;
    }
}

void BeeSwarm::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (gMap.mCurrentLevel != gMap.mNextLevel
        || gMap.mCurrentPath != gMap.mNextPath)
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (mChaseTarget)
    {
        if (mChaseTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            mSwarmState = BeeSwarmStates::eFlyAwayAndDie_3;
            mChaseTargetY -= FP_FromInteger(240);
            mAliveTimer = sGnFrame + 120;
            gBeesNearAbe = 0;
            mChaseTarget->mBaseGameObjectRefCount--;
            mChaseTarget = nullptr;
        }
    }

    if (!sActiveHero || (mChaseTarget == sActiveHero && sActiveHero->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void BeeSwarm::FollowLine(PathLine* pLine, FP target_x, FP target_y, FP speed)
{
    mLine = pLine;
    mLineFollowSpeed = speed;
    mSwarmState = BeeSwarmStates::eFollowPathLines_2;
    mChaseTargetX = target_x;
    mChaseTargetY = target_y;
    mChaseTarget = nullptr;
}

void BeeSwarm::Chase(BaseAliveGameObject* pChaseTarget)
{
    if (mChaseTarget)
    {
        mChaseTarget->mBaseGameObjectRefCount--;
    }

    mSwarmState = BeeSwarmStates::eAttackChase_1;

    mChaseTarget = pChaseTarget;
    field_DA4_update_chase_timer = 0;

    pChaseTarget->mBaseGameObjectRefCount++;

    mChaseTargetX = pChaseTarget->mXPos;
    mChaseTargetY = pChaseTarget->mYPos;

    mAliveTimer = sGnFrame + mTotalChaseTime;
}

void BeeSwarm::VUpdate()
{
    if (gNumCamSwappers != 0)
    {
        return;
    }

    if (EventGet(kEventDeathReset))
    {
        ToFlyAwayAndDie();
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        return;
    }

    // Chase target has died
    if (mChaseTarget && mChaseTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        ToFlyAwayAndDie();

        // Oddly continue to play pain sounds even though our target is rip
    }

    // Play random bee sounds
    const s16 volMax = mSwarmState == BeeSwarmStates::eAttackChase_1 ? 24 : 16;
    const s16 volMin = mSwarmState == BeeSwarmStates::eAttackChase_1 ? 30 : 22;
    for (s32 i = 0; i < 2; i++)
    {
        if (Math_RandomRange(0, 100) < 40)
        {
            SfxPlayMono(relive::RandomSfx(relive::SoundEffects::Bees1, relive::SoundEffects::Bees2), Math_RandomRange(volMin, volMax));
        }
    }

    switch (mSwarmState)
    {
        case BeeSwarmStates::eIdle_0:
            if (!gMap.Is_Point_In_Current_Camera(
                    mCurrentLevel,
                    mCurrentPath,
                    mXPos,
                    mYPos,
                    0))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case BeeSwarmStates::eAttackChase_1:
            if (static_cast<s32>(sGnFrame) > mAliveTimer)
            {
                ToFlyAwayAndDie();
            }
            else
            {
                // Move far on X bees closer to target
                const s32 toTargetXDelta = FP_GetExponent(mChaseTarget->mXPos - mChaseTargetX);
                if (abs(toTargetXDelta) > 368)
                {
                    for (s32 i = 0; i < mCurrentBeesCount; i++)
                    {
                        field_E4_bees.bees[i].mXPos += FP_FromInteger(toTargetXDelta);
                    }
                    mXPos += FP_FromInteger(toTargetXDelta);
                }

                // Move far on  Y bees closer to target
                const s32 toTargetYDelta = FP_GetExponent(mChaseTarget->mYPos - mChaseTargetY);
                if (abs(toTargetYDelta) > 200)
                {
                    for (s32 i = 0; i < mCurrentBeesCount; i++)
                    {
                        field_E4_bees.bees[i].mYPos += FP_FromInteger(toTargetYDelta);
                    }
                    mYPos += FP_FromInteger(toTargetYDelta);
                }

                // Update target x/y to the mid of the target rect
                const PSX_RECT targetRect = mChaseTarget->VGetBoundingRect();
                mChaseTargetY = FP_FromInteger((targetRect.h + targetRect.y) / 2);
                mChaseTargetX = FP_FromInteger((targetRect.w + targetRect.x) / 2);

                if (Math_Distance(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(mChaseTargetX),
                        FP_GetExponent(mChaseTargetY))
                        < 60
                    && mChaseTarget == sActiveHero)
                {
                    gBeesNearAbe = TRUE;
                }
                else
                {
                    gBeesNearAbe = FALSE;
                }

                if (static_cast<s32>(sGnFrame) <= field_DA0_do_damage_or_pain_sound_timer)
                {
                    if (!(sGnFrame % 10) && Math_RandomRange(0, 100) < 70)
                    {
                        // Check every single object just to see if its sActiveHero (nice algorithm lads)
                        // and play pain sounds if so and in the damage rect.
                        for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
                        {
                            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
                            if (!pObjIter)
                            {
                                break;
                            }

                            const PSX_RECT obj_rect = pObjIter->VGetBoundingRect();
                            if (FP_FromInteger(obj_rect.x) <= mRectW && FP_FromInteger(obj_rect.w) >= mRectX && FP_FromInteger(obj_rect.h) >= mRectY && FP_FromInteger(obj_rect.y) <= mRectH)
                            {
                                if (pObjIter == sActiveHero && sActiveHero->mHealth > FP_FromInteger(0))
                                {
                                    const MudSounds snd = Math_RandomRange(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10;
                                    const FP pitch_val = (FP_FromInteger(1) - sActiveHero->mHealth) / FP_FromDouble(0.15);
                                    const s16 pitch = Math_RandomRange(
                                        200 * FP_GetExponent(pitch_val),
                                        200 * (FP_GetExponent(pitch_val) + 1));
                                    Mudokon_SFX(snd, 0, pitch, sActiveHero);
                                }
                            }
                        }
                    }
                }
                else
                {
                    field_DA0_do_damage_or_pain_sound_timer = sGnFrame + 30;

                    for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
                    {
                        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        // Get rect and check if they're in the damage rect
                        const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                        if (FP_FromInteger(objRect.x) <= mRectW && FP_FromInteger(objRect.w) >= mRectX && FP_FromInteger(objRect.h) >= mRectY && FP_FromInteger(objRect.y) <= mRectH)
                        {
                            // Damage them if so
                            pObjIter->VTakeDamage(this);
                        }
                    }
                }
            }
            break;

        case BeeSwarmStates::eFollowPathLines_2:
            mLine = mLine->MoveOnLine(
                &mChaseTargetX,
                &mChaseTargetY,
                mLineFollowSpeed);
            if (!mLine)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case BeeSwarmStates::eFlyAwayAndDie_3:
            if (field_D7C_pos_offset < FP_FromInteger(4))
            {
                field_D7C_pos_offset += FP_FromDouble(0.2);
            }

            if (static_cast<s32>(sGnFrame) > mAliveTimer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            break;
    }

    if (!(sGnFrame % 4) && field_DA4_update_chase_timer < static_cast<s32>(sGnFrame))
    {
        if (mSwarmState != BeeSwarmStates::eIdle_0 && mSwarmState != BeeSwarmStates::eFlyAwayAndDie_3)
        {
            for (s32 i = 0; i < gBaseAliveGameObjects->Size(); i++)
            {
                BaseAliveGameObject* pObjIter = gBaseAliveGameObjects->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter != mChaseTarget)
                {
                    if (pObjIter->field_EC) // can be chased
                    {
                        const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                        if (FP_FromInteger(objRect.x) <= mRectW && FP_FromInteger(objRect.w) >= mRectX && FP_FromInteger(objRect.h) >= mRectY && FP_FromInteger(objRect.y) <= mRectH)
                        {
                            LOG_INFO("Got new target");

                            const auto oldChaseTimer = mAliveTimer;

                            // De-ref old target
                            if (mChaseTarget)
                            {
                                mChaseTarget->mBaseGameObjectRefCount--;
                            }

                            // Set new target
                            mChaseTarget = pObjIter;
                            mChaseTarget->mBaseGameObjectRefCount++;

                            mSwarmState = BeeSwarmStates::eAttackChase_1;
                            mChaseTargetX = pObjIter->mXPos;
                            mChaseTargetY = pObjIter->mYPos;

                            mAliveTimer = sGnFrame + mTotalChaseTime;
                            field_DA4_update_chase_timer = sGnFrame + 60;

                            // ?? why not check < 0 then calc
                            if (oldChaseTimer != 0)
                            {
                                mAliveTimer = oldChaseTimer;
                            }

                            // Got a new target, stop looking
                            break;
                        }
                    }
                }
            }
        }
    }

    // Set max rect (just below max int16 else will overflow in fixed point)
    mRectX = FP_FromInteger(32767);
    mRectW = FP_FromInteger(-32767);
    mRectY = FP_FromInteger(32767);
    mRectH = FP_FromInteger(-32767);

    // Alive bees update
    for (s32 i = 0; i < mCurrentBeesCount; i++)
    {
        BeeSwarmParticle* pBee = &field_E4_bees.bees[i];

        const FP distToTargetX = mChaseTargetX - pBee->mXPos;
        const FP distToTargetY = mChaseTargetY - pBee->mYPos;

        if (field_E4_bees.bees[i].mTimer < static_cast<s32>(sGnFrame))
        {
            const FP abs_new_chase_x = FP_Abs(distToTargetX);
            const FP abs_new_chase_y = FP_Abs(distToTargetY);

            s8 tmpAng = 0;
            if (distToTargetY > FP_FromInteger(0))
            {
                if (distToTargetX >= FP_FromInteger(0))
                {
                    tmpAng = abs_new_chase_x >= abs_new_chase_y ? 240 : 208;
                }
                else
                {
                    tmpAng = abs_new_chase_x <= abs_new_chase_y ? 176 : 144;
                }
            }
            else
            {
                if (distToTargetX <= FP_FromInteger(0))
                {
                    tmpAng = abs_new_chase_x >= abs_new_chase_y ? 112 : 80;
                }
                else
                {
                    tmpAng = abs_new_chase_x <= abs_new_chase_y ? 48 : 16;
                }
            }

            auto angSpeed = tmpAng - pBee->field_8_angle;
            if (angSpeed <= 127)
            {
                if (angSpeed < -127)
                {
                    angSpeed += 255;
                }
            }
            else
            {
                angSpeed -= 255;
            }

            pBee->field_9_angle_speed = static_cast<s8>(angSpeed >> 2);
            pBee->mTimer = sGnFrame + 5;
        }

        FP xMove = {};
        if (mChaseTarget)
        {
            if (FP_Abs(distToTargetX) > FP_FromInteger(20) || mChaseTarget->mVelX != FP_FromInteger(0))
            {
                if (distToTargetX <= FP_FromInteger(0))
                {
                    xMove = -field_D78_speed;
                }
                else
                {
                    xMove = field_D78_speed;
                }
            }
            else
            {
                xMove = FP_FromInteger(0);
            }
        }
        else
        {
            xMove = FP_FromInteger(0);
        }


        const FP rnd1 = field_D7C_pos_offset + FP_FromInteger(((Math_NextRandom() & 3) + 4));
        pBee->mXPos += (rnd1 * Math_Sine(pBee->field_8_angle));

        const FP rnd4 = field_D7C_pos_offset + FP_FromInteger(((Math_NextRandom() & 3) + 4));
        pBee->mYPos += (rnd4 * Math_Cosine(pBee->field_8_angle));

        pBee->mXPos += xMove; // ??? overwrites above calc
        pBee->field_8_angle += pBee->field_9_angle_speed;

        if (pBee->mXPos < mRectX)
        {
            mRectX = pBee->mXPos;
        }
        else if (pBee->mXPos > mRectW)
        {
            mRectW = pBee->mXPos;
        }

        if (pBee->mYPos < mRectY)
        {
            mRectY = pBee->mYPos;
        }
        else if (pBee->mYPos > mRectH)
        {
            mRectH = pBee->mYPos;
        }
    }

    // Spawn bees till we hit max possible
    if (mCurrentBeesCount < mMaxBeesCount)
    {
        BeeSwarmParticle* pBee = &field_E4_bees.bees[mCurrentBeesCount];

        pBee->field_10_anim.mFlags.Set(AnimFlags::eRender);
        pBee->field_10_anim.mFlags.Set(AnimFlags::eBlending); // TODO: or higher byte

        pBee->field_10_anim.field_68_anim_ptr = &GetAnimation();
        pBee->field_10_anim.field_6C_scale = GetSpriteScale();

        pBee->field_10_anim.SetRenderLayer(Layer::eLayer_MainMenuButtonBees_38);

        pBee->mXPos = field_D68_xpos;
        pBee->mYPos = field_D6C_ypos;

        pBee->field_8_angle = Math_NextRandom();
        pBee->field_9_angle_speed = (Math_NextRandom() >> 4) - 8;

        pBee->mTimer = sGnFrame + 5;

        mCurrentBeesCount++;
    }

    mXPos = field_E4_bees.bees[0].mXPos;
    mYPos = field_E4_bees.bees[0].mYPos;
}

void BeeSwarm::ToFlyAwayAndDie()
{
    mAliveTimer = sGnFrame + 120;
    mChaseTargetY -= FP_FromInteger(240);
    mSwarmState = BeeSwarmStates::eFlyAwayAndDie_3;

    gBeesNearAbe = FALSE;

    if (mChaseTarget)
    {
        mChaseTarget->mBaseGameObjectRefCount--;
        mChaseTarget = nullptr;
    }
}

void BeeSwarm::VRender(PrimHeader** ppOt)
{
    GetAnimation().SetRenderLayer(Layer::eLayer_MainMenuButtonBees_38);
    GetAnimation().SetRGB(mRGB.r, mRGB.g, mRGB.b);
    GetAnimation().SetSpriteScale(GetSpriteScale());

    const auto campos_x_delta = pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff);
    const auto campos_y_delta = pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff);
    const auto cam_x_abs = pScreenManager->mCamPos->x + FP_FromInteger(pScreenManager->mCamXOff);
    const auto cam_y_abs = pScreenManager->mCamPos->y + FP_FromInteger(pScreenManager->mCamYOff);

    PSX_Point xy = {32767, 32767};
    PSX_Point wh = {-32767, -32767};

    bool bDontClear = 1;
    for (s32 next = 0; next < mCurrentBeesCount; next++)
    {
        auto bee = &field_E4_bees.bees[next];
        PSX_RECT out = {};
        if (bee->mXPos >= campos_x_delta && bee->mXPos <= cam_x_abs && bee->mYPos >= campos_y_delta && bee->mYPos <= cam_y_abs)
        {
            if (bDontClear)
            {
                GetAnimation().VRender(
                    FP_GetExponent(bee->mXPos - campos_x_delta),
                    FP_GetExponent(bee->mYPos - campos_y_delta),
                    ppOt,
                    0,
                    0);
                bDontClear = 0;
                GetAnimation().Get_Frame_Rect(&out);
            }
            else
            {
                bee->field_10_anim.VRender(
                    FP_GetExponent(PsxToPCX((bee->mXPos - campos_x_delta), FP_FromInteger(11))),
                    FP_GetExponent(bee->mYPos - campos_y_delta),
                    ppOt, 0, 0);
                bee->field_10_anim.GetRenderedSize(&out);
            }

            out.x = std::min(out.x, xy.x);
            out.y = std::min(out.y, xy.y);

            out.w = std::max(out.w, wh.x);
            out.h = std::max(out.h, wh.y);
        }
        if (!bDontClear)
        {

        }
    }
}

} // namespace AO
