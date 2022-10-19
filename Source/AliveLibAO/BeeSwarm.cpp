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

BeeSwarm* BeeSwarm::ctor_47FC60(FP xpos, FP ypos, FP speed, s16 numBees, s32 chaseTicks)
{
    ctor_417C10();

    for (auto& bee : field_E4_bees.bees)
    {
        SetVTable(&bee.field_10_anim, 0x4BA470);
    }

    SetVTable(this, 0x4BCEB0);
    field_4_typeId = Types::eBeeSwarm_95;

    const AnimRecord& rec = AO::AnimRec(AnimId::Bee_Swarm);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 16, 1, 0);
    if (gMap_507BA8.field_0_current_level != LevelIds::eForestTemple_4 && gMap_507BA8.field_0_current_level != LevelIds::eDesertTemple_9)
    {
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, 1, 0);
    }

    s16 numBeesToUse = 0;
    gBeeInstanceCount_5076B0++;
    if (gBeeInstanceCount_5076B0 < 3)
    {
        numBeesToUse = numBees;
    }
    else
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        numBeesToUse = 1;
    }

    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);
    if (numBeesToUse <= 25)
    {
        field_D64_num_bees = numBeesToUse;
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
    field_A8_xpos = xpos;
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
    field_AC_ypos = ypos;
    return this;
}

BaseGameObject* BeeSwarm::dtor_47FDF0()
{
    SetVTable(this, 0x4BCEB0);

    gBeeInstanceCount_5076B0--;

    gBeesNearAbe_5076AC = 0;

    ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kAbewaspAOResID, 0, 0));

    if (gMap_507BA8.field_0_current_level != LevelIds::eForestTemple_4 && gMap_507BA8.field_0_current_level != LevelIds::eDesertTemple_9)
    {
        ResourceManager::FreeResource_455550(ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kElmWaspAOResID_204, 0, 0));
    }

    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->field_C_refCount--;
    }

    return dtor_417D10();
}

BaseGameObject* BeeSwarm::VDestructor(s32 flags)
{
    return Vdtor_480E00(flags);
}

BeeSwarm* BeeSwarm::Vdtor_480E00(s32 flags)
{
    dtor_47FDF0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BeeSwarm::VScreenChanged()
{
    VScreenChange_480D40();
}

void BeeSwarm::VScreenChange_480D40()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level
        || gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_D98_pChaseTarget)
    {
        if (field_D98_pChaseTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
        {
            field_D80_state = BeeSwarmStates::eFlyAwayAndDie_3;
            field_D74_chase_target_y -= FP_FromInteger(240);
            field_D9C_alive_timer = gnFrameCount_507670 + 120;
            gBeesNearAbe_5076AC = 0;
            field_D98_pChaseTarget->field_C_refCount--;
            field_D98_pChaseTarget = nullptr;
        }
    }

    if (!sActiveHero_507678 || (field_D98_pChaseTarget == sActiveHero_507678 && sActiveHero_507678->field_FC_current_motion == eAbeMotions::Motion_156_DoorEnter_42D370))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

void BeeSwarm::FollowLine_47FF10(PathLine* pLine, FP target_x, FP target_y, FP speed)
{
    field_DA8_pLine = pLine;
    field_DAC_line_follow_speed = speed;
    field_D80_state = BeeSwarmStates::eFollowPathLines_2;
    field_D70_chase_target_x = target_x;
    field_D74_chase_target_y = target_y;
    field_D98_pChaseTarget = nullptr;
}

void BeeSwarm::Chase_47FEB0(BaseAliveGameObject* pChaseTarget)
{
    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->field_C_refCount--;
    }

    field_D80_state = BeeSwarmStates::eAttackChase_1;

    field_D98_pChaseTarget = pChaseTarget;
    field_DA4_update_chase_timer = 0;

    pChaseTarget->field_C_refCount++;

    field_D70_chase_target_x = pChaseTarget->field_A8_xpos;
    field_D74_chase_target_y = pChaseTarget->field_AC_ypos;

    field_D9C_alive_timer = gnFrameCount_507670 + field_D84_chaseTicks;
}

void BeeSwarm::VUpdate()
{
    VUpdate_47FF50();
}

void BeeSwarm::VUpdate_47FF50()
{
    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
    {
        ToFlyAwayAndDie();
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return;
    }

    // Chase target has died
    if (field_D98_pChaseTarget && field_D98_pChaseTarget->field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        ToFlyAwayAndDie();

        // Oddly continue to play pain sounds even though our target is rip
    }

    // Play random bee sounds
    const s16 volMax = field_D80_state == BeeSwarmStates::eAttackChase_1 ? 24 : 16;
    const s16 volMin = field_D80_state == BeeSwarmStates::eAttackChase_1 ? 30 : 22;
    for (s32 i = 0; i < 2; i++)
    {
        if (Math_RandomRange_450F20(0, 100) < 40)
        {
            SFX_Play_43AD70(static_cast<SoundEffect>(Math_RandomRange_450F20(SoundEffect::Bees1_14, SoundEffect::Bees2_15)), Math_RandomRange_450F20(volMin, volMax));
        }
    }

    switch (field_D80_state)
    {
        case BeeSwarmStates::eIdle_0:
            if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        case BeeSwarmStates::eAttackChase_1:
            if (static_cast<s32>(gnFrameCount_507670) > field_D9C_alive_timer)
            {
                ToFlyAwayAndDie();
            }
            else
            {
                // Move far on X bees closer to target
                const s32 toTargetXDelta = FP_GetExponent(field_D98_pChaseTarget->field_A8_xpos - field_D70_chase_target_x);
                if (abs(toTargetXDelta) > 368)
                {
                    for (s32 i = 0; i < field_D66_bee_count; i++)
                    {
                        field_E4_bees.bees[i].field_0_xpos += FP_FromInteger(toTargetXDelta);
                    }
                    field_A8_xpos += FP_FromInteger(toTargetXDelta);
                }

                // Move far on  Y bees closer to target
                const s32 toTargetYDelta = FP_GetExponent(field_D98_pChaseTarget->field_AC_ypos - field_D74_chase_target_y);
                if (abs(toTargetYDelta) > 200)
                {
                    for (s32 i = 0; i < field_D66_bee_count; i++)
                    {
                        field_E4_bees.bees[i].field_4_ypos += FP_FromInteger(toTargetYDelta);
                    }
                    field_AC_ypos += FP_FromInteger(toTargetYDelta);
                }

                // Update target x/y to the mid of the target rect
                PSX_RECT targetRect = {};
                field_D98_pChaseTarget->VGetBoundingRect(&targetRect, 1);
                field_D74_chase_target_y = FP_FromInteger((targetRect.h + targetRect.y) / 2);
                field_D70_chase_target_x = FP_FromInteger((targetRect.w + targetRect.x) / 2);

                if (Math_Distance_451270(
                        FP_GetExponent(field_A8_xpos),
                        FP_GetExponent(field_AC_ypos),
                        FP_GetExponent(field_D70_chase_target_x),
                        FP_GetExponent(field_D74_chase_target_y))
                        < 60
                    && field_D98_pChaseTarget == sActiveHero_507678)
                {
                    gBeesNearAbe_5076AC = TRUE;
                }
                else
                {
                    gBeesNearAbe_5076AC = FALSE;
                }

                if (static_cast<s32>(gnFrameCount_507670) <= field_DA0_do_damage_or_pain_sound_timer)
                {
                    if (!(gnFrameCount_507670 % 10) && Math_RandomRange_450F20(0, 100) < 70)
                    {
                        // Check every single object just to see if its sActiveHero_507678 (nice algorithm lads)
                        // and play pain sounds if so and in the damage rect.
                        for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
                        {
                            BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                            if (!pObjIter)
                            {
                                break;
                            }

                            PSX_RECT obj_rect = {};
                            pObjIter->VGetBoundingRect(&obj_rect, 1);

                            if (FP_FromInteger(obj_rect.x) <= field_D90_rect_w && FP_FromInteger(obj_rect.w) >= field_D88_rect_x && FP_FromInteger(obj_rect.h) >= field_D8C_rect_y && FP_FromInteger(obj_rect.y) <= field_D94_rect_h)
                            {
                                if (pObjIter == sActiveHero_507678 && sActiveHero_507678->field_100_health > FP_FromInteger(0))
                                {
                                    const MudSounds snd = Math_RandomRange_450F20(0, 127) >= 64 ? MudSounds::eBeesStruggle_18 : MudSounds::eKnockbackOuch_10;
                                    const FP pitch_val = (FP_FromInteger(1) - sActiveHero_507678->field_100_health) / FP_FromDouble(0.15);
                                    const s16 pitch = Math_RandomRange_450F20(
                                        200 * FP_GetExponent(pitch_val),
                                        200 * (FP_GetExponent(pitch_val) + 1));
                                    Mudokon_SFX_42A4D0(snd, 0, pitch, sActiveHero_507678);
                                }
                            }
                        }
                    }
                }
                else
                {
                    field_DA0_do_damage_or_pain_sound_timer = gnFrameCount_507670 + 30;

                    for (s32 i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
                    {
                        BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                        if (!pObjIter)
                        {
                            break;
                        }

                        // Get rect and check if they're in the damage rect
                        PSX_RECT objRect = {};
                        pObjIter->VGetBoundingRect(&objRect, 1);

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
            field_DA8_pLine = field_DA8_pLine->MoveOnLine_40CA20(
                &field_D70_chase_target_x,
                &field_D74_chase_target_y,
                field_DAC_line_follow_speed);
            if (!field_DA8_pLine)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        case BeeSwarmStates::eFlyAwayAndDie_3:
            if (field_D7C_pos_offset < FP_FromInteger(4))
            {
                field_D7C_pos_offset += FP_FromDouble(0.2);
            }

            if (static_cast<s32>(gnFrameCount_507670) > field_D9C_alive_timer)
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
            break;

        default:
            break;
    }

    if (!(gnFrameCount_507670 % 4) && field_DA4_update_chase_timer < static_cast<s32>(gnFrameCount_507670))
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
                        PSX_RECT objRect = {};
                        pObjIter->VGetBoundingRect(&objRect, 1);

                        if (FP_FromInteger(objRect.x) <= field_D90_rect_w && FP_FromInteger(objRect.w) >= field_D88_rect_x && FP_FromInteger(objRect.h) >= field_D8C_rect_y && FP_FromInteger(objRect.y) <= field_D94_rect_h)
                        {
                            LOG_INFO("Got new target");

                            const auto oldChaseTimer = field_D9C_alive_timer;

                            // De-ref old target
                            if (field_D98_pChaseTarget)
                            {
                                field_D98_pChaseTarget->field_C_refCount--;
                            }

                            // Set new target
                            field_D98_pChaseTarget = pObjIter;
                            field_D98_pChaseTarget->field_C_refCount++;

                            field_D80_state = BeeSwarmStates::eAttackChase_1;
                            field_D70_chase_target_x = pObjIter->field_A8_xpos;
                            field_D74_chase_target_y = pObjIter->field_AC_ypos;

                            field_D9C_alive_timer = gnFrameCount_507670 + field_D84_chaseTicks;
                            field_DA4_update_chase_timer = gnFrameCount_507670 + 60;

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

        if (field_E4_bees.bees[i].field_C_timer < static_cast<s32>(gnFrameCount_507670))
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
            pBee->field_C_timer = gnFrameCount_507670 + 5;
        }

        FP xMove = {};
        if (field_D98_pChaseTarget)
        {
            if (FP_Abs(distToTargetX) > FP_FromInteger(20) || field_D98_pChaseTarget->field_B4_velx != FP_FromInteger(0))
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

        pBee->field_10_anim.field_4_flags.Set(AnimFlags::eBit3_Render);
        pBee->field_10_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending); // TODO: or higher byte

        pBee->field_10_anim.field_68_anim_ptr = &field_10_anim;
        pBee->field_10_anim.field_6C_scale = field_BC_sprite_scale;

        pBee->field_10_anim.field_C_layer = Layer::eLayer_MainMenuButtonBees_38;

        pBee->field_0_xpos = field_D68_xpos;
        pBee->field_4_ypos = field_D6C_ypos;

        pBee->field_8_angle = Math_NextRandom();
        pBee->field_9_angle_speed = (Math_NextRandom() >> 4) - 8;

        pBee->field_C_timer = gnFrameCount_507670 + 5;

        field_D66_bee_count++;
    }

    field_A8_xpos = field_E4_bees.bees[0].field_0_xpos;
    field_AC_ypos = field_E4_bees.bees[0].field_4_ypos;
}

void BeeSwarm::ToFlyAwayAndDie()
{
    field_D9C_alive_timer = gnFrameCount_507670 + 120;
    field_D74_chase_target_y -= FP_FromInteger(240);
    field_D80_state = BeeSwarmStates::eFlyAwayAndDie_3;

    gBeesNearAbe_5076AC = FALSE;

    if (field_D98_pChaseTarget)
    {
        field_D98_pChaseTarget->field_C_refCount--;
        field_D98_pChaseTarget = nullptr;
    }
}

void BeeSwarm::VRender(PrimHeader** ppOt)
{
    VRender_480AC0(ppOt);
}

void BeeSwarm::VRender_480AC0(PrimHeader** ppOt)
{
    field_10_anim.field_C_layer = Layer::eLayer_MainMenuButtonBees_38;
    field_10_anim.field_8_r = static_cast<u8>(field_C0_r);
    field_10_anim.field_9_g = static_cast<u8>(field_C2_g);
    field_10_anim.field_A_b = static_cast<u8>(field_C4_b);
    field_10_anim.field_14_scale = field_BC_sprite_scale;

    const auto campos_x_delta = pScreenManager_4FF7C8->field_10_pCamPos->field_0_x - FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);
    const auto campos_y_delta = pScreenManager_4FF7C8->field_10_pCamPos->field_4_y - FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos);
    const auto cam_x_abs = pScreenManager_4FF7C8->field_10_pCamPos->field_0_x + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos);
    const auto cam_y_abs = pScreenManager_4FF7C8->field_10_pCamPos->field_4_y + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos);

    PSX_Point xy = {32767, 32767};
    PSX_Point wh = {-32767, -32767};

    Bool32 bDontClear = 1;
    for (s32 next = 0; next < field_D66_bee_count; next++)
    {
        auto bee = &field_E4_bees.bees[next];
        PSX_RECT out = {};
        if (bee->field_0_xpos >= campos_x_delta && bee->field_0_xpos <= cam_x_abs && bee->field_4_ypos >= campos_y_delta && bee->field_4_ypos <= cam_y_abs)
        {
            if (bDontClear)
            {
                field_10_anim.vRender(
                    FP_GetExponent(bee->field_0_xpos - campos_x_delta),
                    FP_GetExponent(bee->field_4_ypos - campos_y_delta),
                    ppOt,
                    0,
                    0);
                bDontClear = 0;
                field_10_anim.Get_Frame_Rect_402B50(&out);
            }
            else
            {
                bee->field_10_anim.VRender2(
                    FP_GetExponent(PsxToPCX((bee->field_0_xpos - campos_x_delta), FP_FromInteger(11))),
                    FP_GetExponent(bee->field_4_ypos - campos_y_delta),
                    ppOt);
                bee->field_10_anim.GetRenderedSize_404220(&out);
            }

            out.x = std::min(out.x, xy.field_0_x);
            out.y = std::min(out.y, xy.field_2_y);

            out.w = std::max(out.w, wh.field_0_x);
            out.h = std::max(out.h, wh.field_2_y);
        }
        if (!bDontClear)
        {
            pScreenManager_4FF7C8->InvalidateRect_406E40(
                out.x,
                out.y,
                out.w,
                out.h,
                pScreenManager_4FF7C8->field_2E_idx);
        }
    }
}

} // namespace AO
