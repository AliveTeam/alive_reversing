#include "stdafx.h"
#include "TimedMine.hpp"
#include "UXB.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "DebugHelpers.hpp"
#include "StringFormatters.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "VRam.hpp"
#include "Game.hpp"
#include "ScreenManager.hpp"
#include "Sound/Midi.hpp"
#include "Abe.hpp"
#include "ObjectIds.hpp"
#include "BaseBomb.hpp"
#include "LiftPoint.hpp"
#include "Grid.hpp"

static TintEntry sTimedMineTint_550EB8[1] = {{LevelIds_s8::eNone, 127u, 127u, 127u}};


TimedMine::TimedMine(Path_TimedMine* pPath, TlvItemInfoUnion tlv)
    : BaseAliveGameObject(0)
{
    SetType(AETypes::eTimedMine_or_MovingBomb_10);

    const AnimRecord& rec = AnimRec(AnimId::TimedMine_Idle);
    u8** ppRes = Add_Resource(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);

    field_1C4_flags.Clear(TimedMine_Flags_1C4::eStickToLiftPoint_0);

    field_118_armed = 0;

    if (pPath->field_14_scale == Scale_short::eHalf_1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_Half_16;
    }
    else
    {
        field_CC_sprite_scale = FP_FromDouble(1.0);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = Layer::eLayer_BombMineCar_35;
    }

    InitBlinkAnimation(&field_124_animation);

    field_11A_ticks_before_explosion = pPath->field_16_ticks_before_explosion;

    field_B8_xpos = FP_FromInteger((pPath->field_8_top_left.field_0_x + pPath->field_C_bottom_right.field_0_x) / 2);
    field_BC_ypos = FP_FromInteger(pPath->field_8_top_left.field_2_y);

    FP hitY = {};
    FP hitX = {};

    if (sCollisions_DArray_5C1128->Raycast(
            field_B8_xpos,
            field_BC_ypos,
            field_B8_xpos,
            field_BC_ypos + FP_FromInteger(24),
            &field_100_pCollisionLine,
            &hitX,
            &hitY,
            field_D6_scale != 0 ? 1 : 0x10))
    {
        field_BC_ypos = hitY;
    }

    field_11C_tlv = tlv.all;
    field_120_gnframe = sGnFrame_5C1B84;
    SetBaseAnimPaletteTint(sTimedMineTint_550EB8, gMap.mCurrentLevel, kBombResID);

    const FP gridSnap = ScaleToGridSize(field_CC_sprite_scale);
    mBaseGameObjectFlags.Set(Options::eInteractive_Bit8);
    field_DC_bApplyShadows |= 2u;

    field_E4_collection_rect.x = field_B8_xpos - (gridSnap / FP_FromDouble(2.0));
    field_E4_collection_rect.y = field_BC_ypos - gridSnap;
    field_E4_collection_rect.w = (gridSnap / FP_FromDouble(2.0)) + field_B8_xpos;
    field_E4_collection_rect.h = field_BC_ypos;

    field_110_id = -1;
}

void TimedMine::VUpdate()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(field_110_id));
    if (Event_Get(kEventDeathReset))
    {
        mBaseGameObjectFlags.Set(Options::eDead);
    }

    if (!field_1C4_flags.Get(TimedMine_Flags_1C4::eStickToLiftPoint_0))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->vOnAnyFloor())
    {
        field_E4_collection_rect.x = field_B8_xpos - (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2));
        field_E4_collection_rect.w = (ScaleToGridSize(field_CC_sprite_scale) / FP_FromInteger(2)) + field_B8_xpos;
        field_E4_collection_rect.h = field_BC_ypos;
        field_E4_collection_rect.y = field_BC_ypos - ScaleToGridSize(field_CC_sprite_scale);
    }

    if (field_118_armed == 1)
    {
        if (sGnFrame_5C1B84 > field_1BC_gnFrame_2 + field_1C0_detonation_timer)
        {
            field_1BC_gnFrame_2 = sGnFrame_5C1B84;
            const CameraPos soundDir = gMap.GetDirection_4811A0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos);
            SFX_Play_Camera(SoundEffect::GreenTick_2, 50, soundDir);

            // TODO: Modulus ?
            if (((field_120_gnframe - sGnFrame_5C1B84) & 0xFFFFFFF8) >= 144)
            {
                field_1C0_detonation_timer = 18;
            }
            else
            {
                field_1C0_detonation_timer = (field_120_gnframe - sGnFrame_5C1B84) >> 3;
            }
        }

        if (sGnFrame_5C1B84 >= field_120_gnframe)
        {
            ae_new<BaseBomb>(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
            mBaseGameObjectFlags.Set(Options::eDead);
        }
    }
}

void TimedMine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4810D0(
            field_C2_lvl_number,
            field_C0_path_number,
            field_B8_xpos,
            field_BC_ypos,
            0))
    {
        field_124_animation.VRender(
            FP_GetExponent((field_B8_xpos - pScreenManager_5BB5F4->field_20_pCamPos->field_0_x)),
            FP_GetExponent((field_BC_ypos - pScreenManager_5BB5F4->field_20_pCamPos->field_4_y - FP_NoFractional(field_CC_sprite_scale * FP_FromDouble(14)))),
            ppOt,
            0,
            0);

        PSX_RECT frameRect = {};
        field_124_animation.Get_Frame_Rect(&frameRect);

        pScreenManager_5BB5F4->InvalidateRect_40EC90(
            frameRect.x,
            frameRect.y,
            frameRect.w,
            frameRect.h,
            pScreenManager_5BB5F4->field_3A_idx);

        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::InitBlinkAnimation(Animation* pAnimation)
{
    const AnimRecord& tickRec = AnimRec(AnimId::Bomb_RedGreenTick);
    if (pAnimation->Init(tickRec.mFrameTableOffset, gObjList_animations_5C1A24, this, tickRec.mMaxW, tickRec.mMaxH, Add_Resource(ResourceManager::Resource_Animation, tickRec.mResourceId), 1, 0, 0))
    {
        pAnimation->field_C_render_layer = field_20_animation.field_C_render_layer;
        pAnimation->field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        pAnimation->field_4_flags.Set(AnimFlags::eBit16_bBlending);
        pAnimation->field_14_scale = field_CC_sprite_scale;
        pAnimation->field_8_r = 128;
        pAnimation->field_9_g = 128;
        pAnimation->field_A_b = 128;
        pAnimation->field_B_render_mode = TPageAbr::eBlend_1;
    }
    else
    {
        mBaseGameObjectFlags.Set(Options::eListAddFailed_Bit1);
    }
}

void TimedMine::StickToLiftPoint()
{
    FP hitX = {};
    FP hitY = {};
    PathLine* pLine = nullptr;
    if (sCollisions_DArray_5C1128->Raycast(
            field_B8_xpos,
            field_BC_ypos - FP_FromInteger(20),
            field_B8_xpos, field_BC_ypos + FP_FromInteger(20),
            &pLine, &hitX, &hitY,
            (field_D6_scale == 1) ? 15 : 240))
    {
        if (pLine->field_8_type == eLineTypes::eUnknown_32 ||
            pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            if (ObjList_5C1B78)
            {
                for (s32 i = 0; i < ObjList_5C1B78->Size(); i++)
                {
                    BaseGameObject* pObj = ObjList_5C1B78->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->Type() == AETypes::eLiftPoint_78)
                    {
                        PSX_RECT bRect = {};
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        pLiftPoint->VGetBoundingRect(&bRect, 1);
                        if (FP_GetExponent(field_B8_xpos) > bRect.x && FP_GetExponent(field_B8_xpos) < bRect.w && FP_GetExponent(field_BC_ypos) < bRect.h)
                        {
                            pLiftPoint->VAdd(this);
                            field_110_id = pObj->field_8_object_id;
                            return;
                        }
                    }
                }
            }
        }
    }
}

TimedMine::~TimedMine()
{
    auto pPlatform = static_cast<LiftPoint*>(sObjectIds.Find_Impl(field_110_id));
    if (field_118_armed != 1 || sGnFrame_5C1B84 < field_120_gnframe)
    {
        Path::TLV_Reset(field_11C_tlv, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset(field_11C_tlv, -1, 0, 1);
    }

    field_124_animation.VCleanUp();

    if (pPlatform)
    {
        pPlatform->VRemove(this);
        field_110_id = -1;
    }

    mBaseGameObjectFlags.Clear(BaseGameObject::eInteractive_Bit8);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
    if (field_118_armed != 1)
    {
        mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mBaseGameObjectFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->Type())
    {
        case AETypes::eAbe_69:
        case AETypes::eAbilityRing_104:
        case AETypes::eExplosion_109:
        case AETypes::eShrykull_121:
        {
            mBaseGameObjectFlags.Set(BaseGameObject::eDead);
            ae_new<BaseBomb>(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
            field_118_armed = 1;
            field_120_gnframe = sGnFrame_5C1B84;
            return 1;
        }

        default:
            return 0;
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pHitBy*/)
{
    ae_new<BaseBomb>(field_B8_xpos, field_BC_ypos, 0, field_CC_sprite_scale);
    
    field_118_armed = 1;
    mBaseGameObjectFlags.Set(BaseGameObject::eDead);
    field_120_gnframe = sGnFrame_5C1B84;
}

void TimedMine::VOnPickUpOrSlapped()
{
    if (field_118_armed != 1)
    {
        field_118_armed = 1;
        if ((s32)(field_11A_ticks_before_explosion & 0xFFFC) >= 72)
        {
            field_1C0_detonation_timer = 18;
        }
        else
        {
            field_1C0_detonation_timer = field_11A_ticks_before_explosion >> 2;
        }
        field_1BC_gnFrame_2 = sGnFrame_5C1B84;
        const AnimRecord& animRec = AnimRec(AnimId::TimedMine_Activated);
        field_20_animation.Set_Animation_Data(animRec.mFrameTableOffset, nullptr);
        const AnimRecord& flashRec = AnimRec(AnimId::Bomb_Flash);
        field_120_gnframe = sGnFrame_5C1B84 + field_11A_ticks_before_explosion;
        field_124_animation.Set_Animation_Data(flashRec.mFrameTableOffset, 0);
        SFX_Play_Mono(SoundEffect::GreenTick_2, 0);
    }
}
