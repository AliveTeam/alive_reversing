#include "stdafx_ao.h"
#include "Function.hpp"
#include "HoneySack.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "BeeSwarm.hpp"
#include "HoneyDrip.hpp"
#include "Events.hpp"
#include "Abe.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"

namespace AO {

HoneySack::HoneySack(Path_HoneySack* pTlv, s32 tlvInfo)
{
    mBaseGameObjectTypeId = Types::eHoneySack_45;

    const AnimRecord& hangingRec = AO::AnimRec(AnimId::HoneySack_Hanging);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, hangingRec.mResourceId, 1, 0);
    Animation_Init_417FD0(hangingRec.mFrameTableOffset, hangingRec.mMaxW, hangingRec.mMaxH, ppRes, 1);

    mBaseGameObjectFlags.Set(Options::eCanExplode_Bit7);
    field_E4_tlvInfo = tlvInfo;

    field_100_chase_ticks = pTlv->field_18_chase_ticks;
    field_10_anim.mRenderLayer = Layer::eLayer_DoorFlameRollingBallPortalClip_Half_31;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);
    field_FC_ypos2 = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_EA_bHit_ground = 0;

    if (pTlv->field_1A_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
    }

    if (pTlv->field_1_unknown)
    {
        field_AC_ypos += FP_FromInteger(pTlv->field_1_unknown);

        field_E8_state = State::eUpdateHoneySackOnGround_3;
        const AnimRecord& groundRec = AO::AnimRec(AnimId::HoneySack_OnGround);
        field_10_anim.Set_Animation_Data(groundRec.mFrameTableOffset, 0);
        field_F0_pBee = nullptr;
    }
    else
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);

        field_E8_state = State::eDripHoney_0;
        field_EC_timer = gnFrameCount_507670 + 90;

        if (!ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kWaspAOResID, 0, 0))
        {
            ResourceManager::LoadResourceFile_455270("WASP.BAN", nullptr);
        }

        field_F0_pBee = ao_new<BeeSwarm>(field_A8_xpos, field_AC_ypos, FP_FromInteger(0), 5, 0);
        if (field_F0_pBee)
        {
            field_F0_pBee->mBaseGameObjectRefCount++;
            field_F0_pBee->field_BC_sprite_scale = field_BC_sprite_scale;
        }

        field_F4_drip_target_x = FP_FromInteger(0);
        field_F8_drip_target_y = FP_FromInteger(0);

        Path_TLV* pHoneyDripTarget = gMap.TLV_First_Of_Type_In_Camera(TlvTypes::HoneyDripTarget_42, 0);
        if (pHoneyDripTarget)
        {
            field_F4_drip_target_x = FP_FromInteger(pHoneyDripTarget->field_10_top_left.field_0_x);
            field_F8_drip_target_y = FP_FromInteger(pHoneyDripTarget->field_10_top_left.field_2_y);
        }
    }
}

HoneySack::~HoneySack()
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);

    if (field_E8_state == State::eDripHoney_0)
    {
        gMap.TLV_Reset(field_E4_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset(field_E4_tlvInfo, FP_GetExponent(field_AC_ypos - field_FC_ypos2), 0, 0);
    }

    if (field_F0_pBee)
    {
        field_F0_pBee->mBaseGameObjectRefCount--;
        field_F0_pBee = nullptr;
    }
}

void HoneySack::VScreenChanged()
{
    if (gMap.mOverlayId != gMap.GetOverlayId())
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

void HoneySack::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    mBaseGameObjectFlags.Clear(Options::eCanExplode_Bit7);
    field_E8_state = State::eSetFallAnimation_1;
}

void HoneySack::VUpdate()
{
    if (Event_Get(kEventDeathReset_4))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (field_F0_pBee)
    {
        if (field_F0_pBee->mBaseGameObjectFlags.Get(BaseGameObject::eDead))
        {
            field_F0_pBee->mBaseGameObjectRefCount--;
            field_F0_pBee = nullptr;
        }
    }

    switch (field_E8_state)
    {
        case State::eDripHoney_0:
            if (static_cast<s32>(gnFrameCount_507670) > field_EC_timer)
            {
                ao_new<HoneyDrip>(field_F4_drip_target_x, field_F8_drip_target_y);

                field_EC_timer = gnFrameCount_507670 + 90;
            }
            if (!gMap.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;

        case State::eSetFallAnimation_1:
            if (static_cast<s32>(gnFrameCount_507670) > field_EC_timer - 68)
            {
                const AnimRecord& rec = AO::AnimRec(AnimId::HoneySack_Falling);
                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, 0);
                field_E8_state = State::eFallOnGround_2;
                field_B4_velx = FP_FromInteger(0);
                field_B8_vely = FP_FromInteger(0);
            }
            break;

        case State::eFallOnGround_2:
        {
            if (field_B8_vely < FP_FromInteger(18))
            {
                field_B8_vely += FP_FromInteger(1);
            }

            const FP oldY = field_AC_ypos;
            field_AC_ypos += field_B8_vely;

            if (field_F0_pBee)
            {
                field_F0_pBee->field_D70_chase_target_x = field_A8_xpos;
                field_F0_pBee->field_D74_chase_target_y = field_AC_ypos;
            }

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};

            if (sCollisions_DArray_504C6C->RayCast(
                    field_A8_xpos,
                    oldY,
                    field_A8_xpos,
                    field_AC_ypos,
                    &pLine,
                    &hitX,
                    &hitY,
                    field_BC_sprite_scale == FP_FromInteger(1) ? 0x01 : 0x10))
            {
                SFX_Play_Mono(SoundEffect::MountingElum_38, 90, 0);
                Environment_SFX_42A220(EnvironmentSfx::eHitGroundSoft_6, 90, -1000, nullptr);
                field_AC_ypos = hitY;
                field_E8_state = State::eUpdateHoneySackOnGround_3;
                const AnimRecord& rec = AO::AnimRec(AnimId::HoneySack_FallingToSmashed);
                field_10_anim.Set_Animation_Data(rec.mFrameTableOffset, 0);

                auto pNewBee = ao_new<BeeSwarm>(
                    field_A8_xpos,
                    field_AC_ypos,
                    FP_FromInteger(0),
                    24,
                    field_100_chase_ticks);
                if (pNewBee)
                {
                    pNewBee->Chase(sActiveHero_507678);
                }

                if (field_F0_pBee)
                {
                    field_F0_pBee->mBaseGameObjectRefCount--;
                    field_F0_pBee->mBaseGameObjectFlags.Set(Options::eDead);
                    field_F0_pBee = nullptr;
                }

                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mBaseGameObjectTypeId == Types::eHoney_47)
                    {
                        pObj->mBaseGameObjectFlags.Set(Options::eDead);
                        field_EA_bHit_ground = 1;
                        return;
                    }
                }
            }
        }
        break;

        case State::eUpdateHoneySackOnGround_3:
            if (!field_EA_bHit_ground)
            {
                for (s32 i = 0; i < gBaseGameObjects->Size(); i++)
                {
                    BaseGameObject* pObj = gBaseGameObjects->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->mBaseGameObjectTypeId == Types::eHoney_47)
                    {
                        pObj->mBaseGameObjectFlags.Set(Options::eDead);
                        field_EA_bHit_ground = 1;
                        break;
                    }
                }
            }

            if (!gMap.Is_Point_In_Current_Camera_4449C0(
                    field_B2_lvl_number,
                    field_B0_path_number,
                    field_A8_xpos,
                    field_AC_ypos,
                    0))
            {
                mBaseGameObjectFlags.Set(Options::eDead);
            }
            break;

        default:
            return;
    }
}

} // namespace AO
