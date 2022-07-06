#include "stdafx_ao.h"
#include "Function.hpp"
#include "BeeSwarm.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "BaseAliveGameObject.hpp"
#include "stdlib.hpp"
#include "Abe.hpp"
#include "Game.hpp"
#include "PsxDisplay.hpp"
#include "ScreenManager.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "CameraSwapper.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"
#include <algorithm>

#undef max
#undef min

namespace AO {

ALIVE_VAR(1, 0x5076B0, s16, gBeeInstanceCount_5076B0, 0);
ALIVE_VAR(1, 0x5076AC, s16, gBeesNearAbe_5076AC, 0);

BeeSwarm::BeeSwarm(FP xpos, FP ypos, FP speed, s32 numBees, s32 chaseTicks)
{
    mBaseGameObjectTypeId = ReliveTypes::eBeeSwarm;

    const AnimRecord rec = AO::AnimRec(AnimId::Bee_Swarm);
    u8** ppRes = ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, 16, 1, 0);
    if (gMap.mCurrentLevel != EReliveLevelIds::eForestTemple && gMap.mCurrentLevel != EReliveLevelIds::eDesertTemple)
    {
        ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, 1, 0);
    }

    s32 numBeesToUse = 0;
    gBeeInstanceCount_5076B0++;
    if (gBeeInstanceCount_5076B0 < 3)
    {
        numBeesToUse = numBees;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
        numBeesToUse = 1;
    }

    Animation_Init(AnimId::Bee_Swarm, ppRes);
    if (numBeesToUse <= 25)
    {
        field_D64_num_bees = static_cast<s16>(numBeesToUse);
    }
    else
    {
        field_D64_num_bees = 25;
    }

    field_D78_speed = speed;

    field_D70_chase_target_x = xpos;
    field_D74_chase_target_y = ypos;

    field_D84_chaseTicks = chaseTicks;
    field_D68_xpos = xpos;
    mXPos = xpos;
    field_D6C_ypos = ypos;
    field_D80_state = BeeSwarmStates::eIdle_0;
    field_D98_pChaseTarget = 0;
    field_DA4_update_chase_timer = 0;
    field_D9C_alive_timer = 0;
    field_DA0_do_damage_or_pain_sound_timer = 0;
    field_D88_rect_x = FP_FromInteger(0);
    field_D90_rect_w = FP_FromInteger(1);
    field_D8C_rect_y = FP_FromInteger(0);
    field_D94_rect_h = FP_FromInteger(1);
    field_D66_bee_count = 0;
    field_D7C_pos_offset = FP_FromInteger(0);
    mYPos = ypos;
}

BeeSwarm::~BeeSwarm()
{
    gBeeInstanceCount_5076B0--;

    gBeesNearAbe_5076AC = 0;

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID, 0, 0));

    if (gMap.mCurrentLevel != EReliveLevelIds::eForestTemple && gMap.mCurrentLevel != EReliveLevelIds::eDesertTemple)
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource(ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, 0, 0));
    }

    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->mBaseGameObjectRefCount--;
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

    if (field_D98_pChaseTarget)
    {
        if (field_D98_pChaseTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_D80_state = BeeSwarmStates::eFlyAwayAndDie_3;
            field_D74_chase_target_y -= FP_FromInteger(240);
            field_D9C_alive_timer = sGnFrame + 120;
            gBeesNearAbe_5076AC = 0;
            field_D98_pChaseTarget->mBaseGameObjectRefCount--;
            field_D98_pChaseTarget = nullptr;
        }
    }

    if (!sActiveHero || (field_D98_pChaseTarget == sActiveHero && sActiveHero->mCurrentMotion == eAbeMotions::Motion_156_DoorEnter_42D370))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }
}

void BeeSwarm::FollowLine(PathLine* pLine, FP target_x, FP target_y, FP speed)
{
    field_DA8_pLine = pLine;
    field_DAC_line_follow_speed = speed;
    field_D80_state = BeeSwarmStates::eFollowPathLines_2;
    field_D70_chase_target_x = target_x;
    field_D74_chase_target_y = target_y;
    field_D98_pChaseTarget = nullptr;
}

void BeeSwarm::Chase(BaseAliveGameObject* pChaseTarget)
{
    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->mBaseGameObjectRefCount--;
    }

    field_D80_state = BeeSwarmStates::eAttackChase_1;

    field_D98_pChaseTarget = pChaseTarget;
    field_DA4_update_chase_timer = 0;

    pChaseTarget->mBaseGameObjectRefCount++;

    field_D70_chase_target_x = pChaseTarget->mXPos;
    field_D74_chase_target_y = pChaseTarget->mYPos;

    field_D9C_alive_timer = sGnFrame + field_D84_chaseTicks;
}

void BeeSwarm::VUpdate()
{
    if (sNumCamSwappers_507668 != 0)
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
    if (field_D98_pChaseTarget && field_D98_pChaseTarget->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        ToFlyAwayAndDie();

        // Oddly continue to play pain sounds even though our target is rip
    }

    // Play random bee sounds
    const s16 volMax = field_D80_state == BeeSwarmStates::eAttackChase_1 ? 24 : 16;
    const s16 volMin = field_D80_state == BeeSwarmStates::eAttackChase_1 ? 30 : 22;
    for (s32 i = 0; i < 2; i++)
    {
        if (Math_RandomRange(0, 100) < 40)
        {
            SfxPlayMono(static_cast<SoundEffect>(Math_RandomRange(SoundEffect::Bees1_14, SoundEffect::Bees2_15)), Math_RandomRange(volMin, volMax));
        }
    }

    switch (field_D80_state)
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
            if (static_cast<s32>(sGnFrame) > field_D9C_alive_timer)
            {
                ToFlyAwayAndDie();
            }
            else
            {
                // Move far on X bees closer to target
                const s32 toTargetXDelta = FP_GetExponent(field_D98_pChaseTarget->mXPos - field_D70_chase_target_x);
                if (abs(toTargetXDelta) > 368)
                {
                    for (s32 i = 0; i < field_D66_bee_count; i++)
                    {
                        field_E4_bees.bees[i].field_0_xpos += FP_FromInteger(toTargetXDelta);
                    }
                    mXPos += FP_FromInteger(toTargetXDelta);
                }

                // Move far on  Y bees closer to target
                const s32 toTargetYDelta = FP_GetExponent(field_D98_pChaseTarget->mYPos - field_D74_chase_target_y);
                if (abs(toTargetYDelta) > 200)
                {
                    for (s32 i = 0; i < field_D66_bee_count; i++)
                    {
                        field_E4_bees.bees[i].field_4_ypos += FP_FromInteger(toTargetYDelta);
                    }
                    mYPos += FP_FromInteger(toTargetYDelta);
                }

                // Update target x/y to the mid of the target rect
                const PSX_RECT targetRect = field_D98_pChaseTarget->VGetBoundingRect();
                field_D74_chase_target_y = FP_FromInteger((targetRect.h + targetRect.y) / 2);
                field_D70_chase_target_x = FP_FromInteger((targetRect.w + targetRect.x) / 2);

                if (Math_Distance(
                        FP_GetExponent(mXPos),
                        FP_GetExponent(mYPos),
                        FP_GetExponent(field_D70_chase_target_x),
                        FP_GetExponent(field_D74_chase_target_y))
                        < 60
                    && field_D98_pChaseTarget == sActiveHero)
                {
                    gBeesNearAbe_5076AC = TRUE;
                }
                else
                {
                    gBeesNearAbe_5076AC = FALSE;
                }

                if (static_cast<s32>(sGnFrame) <= field_DA0_do_damage_or_pain_sound_timer)
                {
                    if (!(sGnFrame % 10) && Math_RandomRange(0, 100) < 70)
                    {
                        // Check every single object just to see if its sActiveHero (nice algorithm lads)
                        // and play pain sounds if so and in the damage rect.
                        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
                        {
                            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                            if (!pObjIter)
                            {
                                break;
                            }

                            const PSX_RECT obj_rect = pObjIter->VGetBoundingRect();
                            if (FP_FromInteger(obj_rect.x) <= field_D90_rect_w && FP_FromInteger(obj_rect.w) >= field_D88_rect_x && FP_FromInteger(obj_rect.h) >= field_D8C_rect_y && FP_FromInteger(obj_rect.y) <= field_D94_rect_h)
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

                    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
                    {
                        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        // Get rect and check if they're in the damage rect
                        const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                        if (FP_FromInteger(objRect.x) <= field_D90_rect_w && FP_FromInteger(objRect.w) >= field_D88_rect_x && FP_FromInteger(objRect.h) >= field_D8C_rect_y && FP_FromInteger(objRect.y) <= field_D94_rect_h)
                        {
                            // Damage them if so
                            pObjIter->VTakeDamage(this);
                        }
                    }
                }
            }
            break;

        case BeeSwarmStates::eFollowPathLines_2:
            field_DA8_pLine = field_DA8_pLine->MoveOnLine(
                &field_D70_chase_target_x,
                &field_D74_chase_target_y,
                field_DAC_line_follow_speed);
            if (!field_DA8_pLine)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        case BeeSwarmStates::eFlyAwayAndDie_3:
            if (field_D7C_pos_offset < FP_FromInteger(4))
            {
                field_D7C_pos_offset += FP_FromDouble(0.2);
            }

            if (static_cast<s32>(sGnFrame) > field_D9C_alive_timer)
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            break;
    }

    if (!(sGnFrame % 4) && field_DA4_update_chase_timer < static_cast<s32>(sGnFrame))
    {
        if (field_D80_state != BeeSwarmStates::eIdle_0 && field_D80_state != BeeSwarmStates::eFlyAwayAndDie_3)
        {
            for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
            {
                BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter != field_D98_pChaseTarget)
                {
                    if (pObjIter->field_EC) // can be chased
                    {
                        const PSX_RECT objRect = pObjIter->VGetBoundingRect();
                        if (FP_FromInteger(objRect.x) <= field_D90_rect_w && FP_FromInteger(objRect.w) >= field_D88_rect_x && FP_FromInteger(objRect.h) >= field_D8C_rect_y && FP_FromInteger(objRect.y) <= field_D94_rect_h)
                        {
                            LOG_INFO("Got new target");

                            const auto oldChaseTimer = field_D9C_alive_timer;

                            // De-ref old target
                            if (field_D98_pChaseTarget)
                            {
                                field_D98_pChaseTarget->mBaseGameObjectRefCount--;
                            }

                            // Set new target
                            field_D98_pChaseTarget = pObjIter;
                            field_D98_pChaseTarget->mBaseGameObjectRefCount++;

                            field_D80_state = BeeSwarmStates::eAttackChase_1;
                            field_D70_chase_target_x = pObjIter->mXPos;
                            field_D74_chase_target_y = pObjIter->mYPos;

                            field_D9C_alive_timer = sGnFrame + field_D84_chaseTicks;
                            field_DA4_update_chase_timer = sGnFrame + 60;

                            // ?? why not check < 0 then calc
                            if (oldChaseTimer != 0)
                            {
                                field_D9C_alive_timer = oldChaseTimer;
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
    field_D88_rect_x = FP_FromInteger(32767);
    field_D90_rect_w = FP_FromInteger(-32767);
    field_D8C_rect_y = FP_FromInteger(32767);
    field_D94_rect_h = FP_FromInteger(-32767);

    // Alive bees update
    for (s32 i = 0; i < field_D66_bee_count; i++)
    {
        BeeSwarmParticle* pBee = &field_E4_bees.bees[i];

        const FP distToTargetX = field_D70_chase_target_x - pBee->field_0_xpos;
        const FP distToTargetY = field_D74_chase_target_y - pBee->field_4_ypos;

        if (field_E4_bees.bees[i].field_C_timer < static_cast<s32>(sGnFrame))
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
            pBee->field_C_timer = sGnFrame + 5;
        }

        FP xMove = {};
        if (field_D98_pChaseTarget)
        {
            if (FP_Abs(distToTargetX) > FP_FromInteger(20) || field_D98_pChaseTarget->mVelX != FP_FromInteger(0))
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
        pBee->field_0_xpos += (rnd1 * Math_Sine_451110(pBee->field_8_angle));

        const FP rnd4 = field_D7C_pos_offset + FP_FromInteger(((Math_NextRandom() & 3) + 4));
        pBee->field_4_ypos += (rnd4 * Math_Cosine_4510A0(pBee->field_8_angle));

        pBee->field_0_xpos += xMove; // ??? overwrites above calc
        pBee->field_8_angle += pBee->field_9_angle_speed;

        if (pBee->field_0_xpos < field_D88_rect_x)
        {
            field_D88_rect_x = pBee->field_0_xpos;
        }
        else if (pBee->field_0_xpos > field_D90_rect_w)
        {
            field_D90_rect_w = pBee->field_0_xpos;
        }

        if (pBee->field_4_ypos < field_D8C_rect_y)
        {
            field_D8C_rect_y = pBee->field_4_ypos;
        }
        else if (pBee->field_4_ypos > field_D94_rect_h)
        {
            field_D94_rect_h = pBee->field_4_ypos;
        }
    }

    // Spawn bees till we hit max possible
    if (field_D66_bee_count < field_D64_num_bees)
    {
        BeeSwarmParticle* pBee = &field_E4_bees.bees[field_D66_bee_count];

        pBee->field_10_anim.mFlags.Set(AnimFlags::eBit3_Render);
        pBee->field_10_anim.mFlags.Set(AnimFlags::eBit16_bBlending); // TODO: or higher byte

        pBee->field_10_anim.field_68_anim_ptr = &mAnim;
        pBee->field_10_anim.field_6C_scale = mSpriteScale;

        pBee->field_10_anim.mRenderLayer = Layer::eLayer_MainMenuButtonBees_38;

        pBee->field_0_xpos = field_D68_xpos;
        pBee->field_4_ypos = field_D6C_ypos;

        pBee->field_8_angle = Math_NextRandom();
        pBee->field_9_angle_speed = (Math_NextRandom() >> 4) - 8;

        pBee->field_C_timer = sGnFrame + 5;

        field_D66_bee_count++;
    }

    mXPos = field_E4_bees.bees[0].field_0_xpos;
    mYPos = field_E4_bees.bees[0].field_4_ypos;
}

void BeeSwarm::ToFlyAwayAndDie()
{
    field_D9C_alive_timer = sGnFrame + 120;
    field_D74_chase_target_y -= FP_FromInteger(240);
    field_D80_state = BeeSwarmStates::eFlyAwayAndDie_3;

    gBeesNearAbe_5076AC = FALSE;

    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->mBaseGameObjectRefCount--;
        field_D98_pChaseTarget = nullptr;
    }
}

void BeeSwarm::VRender(PrimHeader** ppOt)
{
    mAnim.mRenderLayer = Layer::eLayer_MainMenuButtonBees_38;
    mAnim.mRed = static_cast<u8>(mRGB.r);
    mAnim.mGreen = static_cast<u8>(mRGB.g);
    mAnim.mBlue = static_cast<u8>(mRGB.b);
    mAnim.field_14_scale = mSpriteScale;

    const auto campos_x_delta = pScreenManager->mCamPos->x - FP_FromInteger(pScreenManager->mCamXOff);
    const auto campos_y_delta = pScreenManager->mCamPos->y - FP_FromInteger(pScreenManager->mCamYOff);
    const auto cam_x_abs = pScreenManager->mCamPos->x + FP_FromInteger(pScreenManager->mCamXOff);
    const auto cam_y_abs = pScreenManager->mCamPos->y + FP_FromInteger(pScreenManager->mCamYOff);

    PSX_Point xy = {32767, 32767};
    PSX_Point wh = {-32767, -32767};

    bool bDontClear = 1;
    for (s32 next = 0; next < field_D66_bee_count; next++)
    {
        auto bee = &field_E4_bees.bees[next];
        PSX_RECT out = {};
        if (bee->field_0_xpos >= campos_x_delta && bee->field_0_xpos <= cam_x_abs && bee->field_4_ypos >= campos_y_delta && bee->field_4_ypos <= cam_y_abs)
        {
            if (bDontClear)
            {
                mAnim.VRender(
                    FP_GetExponent(bee->field_0_xpos - campos_x_delta),
                    FP_GetExponent(bee->field_4_ypos - campos_y_delta),
                    ppOt,
                    0,
                    0);
                bDontClear = 0;
                mAnim.Get_Frame_Rect(&out);
            }
            else
            {
                bee->field_10_anim.VRender(
                    FP_GetExponent(PsxToPCX((bee->field_0_xpos - campos_x_delta), FP_FromInteger(11))),
                    FP_GetExponent(bee->field_4_ypos - campos_y_delta),
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
            pScreenManager->InvalidateRectCurrentIdx(
                out.x,
                out.y,
                out.w,
                out.h);
        }
    }
}

} // namespace AO
