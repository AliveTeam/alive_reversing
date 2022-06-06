#include "stdafx.h"
#include "Slurg.hpp"
#include "Function.hpp"
#include "Shadow.hpp"
#include "Collisions.hpp"
#include "Blood.hpp"
#include "Sfx.hpp"
#include "Events.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "PathData.hpp"

ALIVE_VAR(1, 0x5C1C08, u16, sSlurg_Step_Watch_Points_Idx_5C1C08, 0);
ALIVE_ARY(1, 0x5BD4DC, s8, 2, sSlurg_Step_Watch_Points_Count_5BD4DC, {});
ALIVE_ARY(1, 0x5C1B28, Slurg_Step_Watch_Points, 2, sSlurg_Step_Watch_Points_5C1B28, {});

void Slurg::Clear_Slurg_Step_Watch_Points()
{
    sSlurg_Step_Watch_Points_Idx_5C1C08 = !sSlurg_Step_Watch_Points_Idx_5C1C08;
    sSlurg_Step_Watch_Points_Count_5BD4DC[sSlurg_Step_Watch_Points_Idx_5C1C08] = 0;

    for (auto& point : sSlurg_Step_Watch_Points_5C1B28[sSlurg_Step_Watch_Points_Idx_5C1C08].field_0_points)
    {
        point = {};
    }
}

TintEntry sSlurgTints_560BCC[18] = {
    {EReliveLevelIds::eMines, 102u, 127u, 118u},
    {EReliveLevelIds::eNecrum, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks, 102u, 127u, 118u},
    {EReliveLevelIds::eMudancheeVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery, 102u, 127u, 118u},
    {EReliveLevelIds::eBrewery_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eMudomoVault_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eFeeCoDepot_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBarracks_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eBonewerkz_Ender, 102u, 127u, 118u},
    {EReliveLevelIds::eNone, 102u, 127u, 118u}};

Slurg::Slurg(Path_Slurg* pTlv, u32 tlvInfo)
    : BaseAliveGameObject(0)
{
    field_128_pTlv = pTlv;

    field_11C_state = Slurg_States::eMoving_0;

    const AnimRecord& rec = AnimRec(AnimId::Slurg_Move);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mBaseGameObjectFlags.Set(BaseGameObject::eCanExplode_Bit7);
    SetType(ReliveTypes::eSlurg);

    mBaseAnimatedWithPhysicsGameObject_XPos = FP_FromInteger((pTlv->field_8_top_left.field_0_x + pTlv->field_C_bottom_right.field_0_x) / 2);
    mBaseAnimatedWithPhysicsGameObject_YPos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_12C_tlvInfo = tlvInfo;

    if (pTlv->field_10_slurg_data.field_4_scale == Scale_short::eHalf_1)
    {
        field_130_scale = FP_FromDouble(0.5);
        field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_Half_14;
        field_D6_scale = 0;
    }
    else if (pTlv->field_10_slurg_data.field_4_scale == Scale_short::eFull_0)
    {
        field_130_scale = FP_FromInteger(1);
        field_20_animation.mRenderLayer = Layer::eLayer_SligGreeterFarts_33;
        field_D6_scale = 1;
    }

    field_11E_moving_timer = pTlv->field_10_slurg_data.field_0_moving_timer;
    field_120_delay_random = pTlv->field_10_slurg_data.field_0_moving_timer;

    SetTint(&sSlurgTints_560BCC[0], gMap.mCurrentLevel);

    FP hitX = {};
    FP hitY = {};
    if (sCollisions_DArray_5C1128->Raycast(
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos + FP_FromInteger(24),
            &field_124_pLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 16)
        == 1)
    {
        mBaseAnimatedWithPhysicsGameObject_YPos = hitY;
    }

    field_11A_switch_id = pTlv->field_10_slurg_data.field_6_switch_id;

    field_118_flags.Clear();

    if (pTlv->field_10_slurg_data.field_2_start_direction == XDirection_short::eRight_1)
    {
        field_118_flags.Set(SlurgFlags::Bit1_Direction);
    }

    VStackOnObjectsOfType(ReliveTypes::eSlurg);
    mApplyShadows |= 2u;
    mShadow = ae_new<Shadow>();
}

s32 Slurg::CreateFromSaveState(const u8* pData)
{
    auto pState = reinterpret_cast<const Slurg_State*>(pData);
    auto pTlv = static_cast<Path_Slurg*>(sPath_dword_BB47C0->TLV_From_Offset_Lvl_Cam(pState->field_24_tlvInfo));

    if (!ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Animation, AEResourceID::kSlurgResID, FALSE, FALSE))
    {
        ResourceManager::LoadResourceFile_49C170("SLURG.BAN", nullptr);
    }

    auto pSlurg = ae_new<Slurg>(pTlv, pState->field_24_tlvInfo);

    pSlurg->mBaseAnimatedWithPhysicsGameObject_XPos = pState->field_4_xpos;
    pSlurg->mBaseAnimatedWithPhysicsGameObject_YPos = pState->field_8_ypos;
    pSlurg->field_C4_velx = pState->field_C_velx;
    pSlurg->field_20_animation.mFrameChangeCounter = pState->field_1A_anim_frame_change_counter;

    // OG BUG: This wasn't restored
    pSlurg->field_20_animation.field_92_current_frame = pState->field_18_anim_current_frame;
    pSlurg->field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX, pState->field_14_flipX & 1);
    pSlurg->field_20_animation.mAnimFlags.Set(AnimFlags::eBit3_Render, pState->field_1C_bRender & 1);

    pSlurg->mBaseGameObjectFlags.Set(BaseGameObject::eDrawable_Bit4, pState->field_1D_bDrawable & 1);

    if (IsLastFrame(&pSlurg->field_20_animation))
    {
        pSlurg->field_20_animation.mAnimFlags.Set(AnimFlags::eBit18_IsLastFrame);
    }

    pSlurg->field_11C_state = pState->field_28_state;

    pSlurg->field_118_flags.Set(SlurgFlags::Bit1_Direction, pState->field_2A_flags.Get(SlurgFlags::Bit1_Direction));
    pSlurg->field_118_flags.Set(SlurgFlags::Bit2_StartToMove, pState->field_2A_flags.Get(SlurgFlags::Bit2_StartToMove));
    return sizeof(Slurg_State);
}

Slurg::~Slurg()
{
    if (field_12C_tlvInfo == -1)
    {
        Path::TLV_Reset(0xFFFFFFFF, -1, 0, field_11C_state == Slurg_States::eBurst_2);
    }
}

void Slurg::Burst()
{
    field_11C_state = Slurg_States::eBurst_2;
    const AnimRecord& animRec = AnimRec(AnimId::Slurg_Burst);
    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);

    ae_new<Blood>(mBaseAnimatedWithPhysicsGameObject_XPos,
                                mBaseAnimatedWithPhysicsGameObject_YPos,
                                FP_FromInteger(0),
                                FP_FromInteger(5),
                                field_130_scale,
                                20);

    Event_Broadcast(kEventLoudNoise, this);
    SFX_Play_Mono(SoundEffect::SlurgKill_89, 127, field_130_scale);

    if (field_11A_switch_id > 1)
    {
        SwitchStates_Add(field_11A_switch_id, 1);
    }
}

void Slurg::VUpdate()
{
    const FP oldXPos = mBaseAnimatedWithPhysicsGameObject_XPos;
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }

    if (field_11E_moving_timer == 0)
    {
        field_11E_moving_timer = Math_RandomRange(field_120_delay_random, field_120_delay_random + 20);
        field_11C_state = Slurg_States::eStopped_1;
        const AnimRecord& animRec = AnimRec(AnimId::Slurg_Turn_Around);
        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
    }

    PSX_RECT bRect = {};
    VGetBoundingRect(&bRect, 1);

    if (field_11C_state != Slurg_States::eBurst_2)
    {
        const s32 idx = sSlurg_Step_Watch_Points_Idx_5C1C08 == 0;
        const s32 max_count = sSlurg_Step_Watch_Points_Count_5BD4DC[idx];
        for (s32 i = 0; i < max_count; i++)
        {
            const Slurg_Step_Watch_Point* pPoint = &sSlurg_Step_Watch_Points_5C1B28[idx].field_0_points[i];
            if (pPoint->field_0_xPos > bRect.x - 2 && pPoint->field_0_xPos < bRect.w + 2 && pPoint->field_2_yPos > bRect.y - 4 && pPoint->field_2_yPos < bRect.h + 4)
            {
                Burst();
                break;
            }
        }
    }

    switch (field_11C_state)
    {
        case Slurg_States::eMoving_0:
            field_C4_velx = FP_FromInteger(1);
            field_11E_moving_timer--;
            if (field_118_flags.Get(SlurgFlags::Bit1_Direction))
            {
                field_C4_velx = -FP_FromInteger(1);
            }

            field_118_flags.Toggle(SlurgFlags::Bit2_StartToMove);

            if (field_118_flags.Get(SlurgFlags::Bit2_StartToMove))
            {
                mBaseAnimatedWithPhysicsGameObject_XPos += field_C4_velx;
            }
            break;

        case Slurg_States::eStopped_1:
            field_C4_velx = FP_FromInteger(0);
            if (field_20_animation.field_92_current_frame == 0
                && gMap.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, 0))
            {
                SFX_Play_Mono(SoundEffect::SlurgStop_90, 0);
            }

            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                field_11C_state = Slurg_States::eMoving_0;
                const AnimRecord& animRec = AnimRec(AnimId::Slurg_Move);
                field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
            }
            break;

        case Slurg_States::eBurst_2:
            if (field_20_animation.mAnimFlags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            }
            break;

        default:
            break;
    }

    if (oldXPos != mBaseAnimatedWithPhysicsGameObject_XPos)
    {
        field_128_pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(
            nullptr,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos,
            mBaseAnimatedWithPhysicsGameObject_XPos,
            mBaseAnimatedWithPhysicsGameObject_YPos);

        VOn_TLV_Collision(field_128_pTlv);
    }
}

s16 Slurg::VTakeDamage(BaseGameObject* pFrom)
{
    // Slurgs are tough little dudes, only Paramites can smack 'em up.
    if (pFrom->Type() == ReliveTypes::eParamite)
    {
        Burst();
        return 1;
    }

    return 0;
}

void Slurg::VOn_TLV_Collision(Path_TLV* pTlv)
{
    while (pTlv)
    {
        if (pTlv->field_4_type == TlvTypes::ScrabLeftBound_43)
        {
            if (field_118_flags.Get(SlurgFlags::Bit1_Direction))
            {
                GoLeft();
            }
        }
        else if (pTlv->field_4_type == TlvTypes::ScrabRightBound_44)
        {
            if (!field_118_flags.Get(SlurgFlags::Bit1_Direction))
            {
                GoRight();
            }
        }
        pTlv = sPath_dword_BB47C0->TLV_Get_At_4DB290(pTlv, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos, mBaseAnimatedWithPhysicsGameObject_XPos, mBaseAnimatedWithPhysicsGameObject_YPos);
    }

    if (field_118_flags.Get(SlurgFlags::Bit1_Direction))
    {
        if (WallHit(field_130_scale * FP_FromInteger(8), -(field_130_scale * FP_FromInteger(6))) || Check_IsOnEndOfLine(1, 1))
        {
            GoLeft();
        }
    }
    else
    {
        if (WallHit(field_130_scale * FP_FromInteger(8), field_130_scale * FP_FromInteger(6)) || Check_IsOnEndOfLine(0, 1))
        {
            GoRight();
        }
    }
}

s32 Slurg::VGetSaveState(u8* pSaveBuffer)
{
    if (mBaseAliveGameObjectFlags.Get(Flags_114::e114_Bit7_Electrocuted))
    {
        return 0;
    }

    auto pState = reinterpret_cast<Slurg_State*>(pSaveBuffer);

    pState->field_0_type = AETypes::eSlurg_129;
    pState->field_4_xpos = mBaseAnimatedWithPhysicsGameObject_XPos;
    pState->field_8_ypos = mBaseAnimatedWithPhysicsGameObject_YPos;
    pState->field_C_velx = field_C4_velx;
    pState->field_10_scale = field_130_scale;
    pState->field_14_flipX = field_20_animation.mAnimFlags.Get(AnimFlags::eBit5_FlipX);
    pState->field_16_current_motion = mCurrentMotion;
    pState->field_18_anim_current_frame = field_20_animation.field_92_current_frame;
    pState->field_1A_anim_frame_change_counter = field_20_animation.mFrameChangeCounter;
    pState->field_1D_bDrawable = mBaseGameObjectFlags.Get(BaseGameObject::eDrawable_Bit4);
    pState->field_1C_bRender = field_20_animation.mAnimFlags.Get(AnimFlags::eBit3_Render);
    pState->field_20_frame_table_offset = field_20_animation.field_18_frame_table_offset;
    pState->field_24_tlvInfo = field_12C_tlvInfo;
    pState->field_28_state = field_11C_state;
    pState->field_2A_flags.Set(SlurgFlags::Bit1_Direction, field_118_flags.Get(SlurgFlags::Bit1_Direction));
    pState->field_2A_flags.Set(SlurgFlags::Bit2_StartToMove, field_118_flags.Get(SlurgFlags::Bit2_StartToMove));
    return sizeof(Slurg_State);
}

void Slurg::GoLeft()
{
    field_20_animation.mAnimFlags.Clear(AnimFlags::eBit5_FlipX);
    field_118_flags.Clear(SlurgFlags::Bit1_Direction);

    field_11C_state = Slurg_States::eStopped_1;
    const AnimRecord& animRec = AnimRec(AnimId::Slurg_Turn_Around);
    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
}

void Slurg::GoRight()
{
    field_20_animation.mAnimFlags.Set(AnimFlags::eBit5_FlipX);
    field_118_flags.Set(SlurgFlags::Bit1_Direction);

    field_11C_state = Slurg_States::eStopped_1;
    const AnimRecord& animRec = AnimRec(AnimId::Slurg_Turn_Around);
    field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
}
