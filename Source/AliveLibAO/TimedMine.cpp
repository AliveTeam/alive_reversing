#include "stdafx_ao.h"
#include "Function.hpp"
#include "TimedMine.hpp"
#include "ResourceManager.hpp"
#include "Game.hpp"
#include "stdlib.hpp"
#include "LiftPoint.hpp"
#include "BaseBomb.hpp"
#include "ScreenManager.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Collisions.hpp"

namespace AO {

const TintEntry kTimedMineTints_4C3140[3] = {
    {LevelIds_s8::eStockYards_5, 60u, 60u, 60u},
    {LevelIds_s8::eStockYardsReturn_6, 60u, 60u, 60u},
    {LevelIds_s8::eNone, 127u, 127u, 127u},
};

TimedMine::TimedMine(Path_TimedMine* pTlv, s32 tlvInfo)
    : BaseAliveGameObject()
{
    field_4_typeId = Types::eTimedMine_8;

    const AnimRecord& rec = AO::AnimRec(AnimId::TimedMine_Idle);
    u8** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, rec.mResourceId, 1, 0);
    Animation_Init_417FD0(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1);

    mFlags.Set(Options::eInteractive_Bit8);
    field_1B8_flags.Clear(TimedMine_Flags_1B8::eStickToLiftPoint_0);
    field_10C_armed = 0;

    if (pTlv->field_1C_scale == Scale_short::eHalf_1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = Layer::eLayer_BombRollingBall_Half_16;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = Layer::eLayer_BombRollingBall_35;
    }

    const AnimRecord& tickRec = AO::AnimRec(AnimId::Bomb_RedGreenTick);
    if (field_118_anim.Init(
            tickRec.mFrameTableOffset,
            gObjList_animations_505564,
            this,
            tickRec.mMaxW,
            tickRec.mMaxH,
            ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, AOResourceID::kBombflshAOResID, 1, 0),
            1,
            0,
            0))
    {
        field_118_anim.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
        field_118_anim.field_4_flags.Set(AnimFlags::eBit16_bBlending);
        field_118_anim.field_C_layer = field_10_anim.field_C_layer;
        field_118_anim.field_14_scale = field_BC_sprite_scale;
        field_118_anim.field_8_r = 128;
        field_118_anim.field_9_g = 128;
        field_118_anim.field_A_b = 128;
        field_118_anim.field_B_render_mode = TPageAbr::eBlend_1;
    }
    else
    {
        mFlags.Set(Options::eListAddFailed_Bit1);
    }

    field_10E_ticks_before_explosion = pTlv->field_1E_ticks_before_explosion;
    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y + 24);

    field_114_timer = gnFrameCount_507670;
    field_110_tlvInfo = tlvInfo;

    SetBaseAnimPaletteTint_4187C0(kTimedMineTints_4C3140, gMap.mCurrentLevel, 1005);
    field_D4_collection_rect.x = field_A8_xpos - (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
    field_D4_collection_rect.w = field_A8_xpos + (ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2));
    field_D4_collection_rect.h = field_AC_ypos;
    field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize(field_BC_sprite_scale);

    mFlags.Set(Options::eInteractive_Bit8);
    field_F8_pLiftPoint = nullptr;
}

TimedMine::~TimedMine()
{
    if (field_10C_armed != 1 || static_cast<s32>(gnFrameCount_507670) < field_114_timer)
    {
        gMap.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 1);
    }

    field_118_anim.VCleanUp();

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }

    mFlags.Clear(Options::eInteractive_Bit8);
}

void TimedMine::VScreenChanged()
{
    if (gMap.mCurrentLevel != gMap.mLevel || gMap.mCurrentPath != gMap.mPath)
    {
        mFlags.Set(Options::eDead);
    }

    if (field_10C_armed != 1)
    {
        mFlags.Set(Options::eDead);
    }
}

s16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    if (mFlags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    switch (pFrom->field_4_typeId)
    {
        case Types::eAbe_43:
        case Types::eAbilityRing_69:
        case Types::eExplosion_74:
        case Types::eShrykull_85:
        {
            mFlags.Set(BaseGameObject::eDead);
            ao_new<BaseBomb>(
                field_A8_xpos,
                field_AC_ypos,
                0,
                field_BC_sprite_scale);
            field_10C_armed = 1;
            field_114_timer = gnFrameCount_507670;
            return 1;
        }
        default:
            return 0;
    }
}

void TimedMine::VRender(PrimHeader** ppOt)
{
    if (gMap.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
    {
        field_118_anim.VRender(
            FP_GetExponent(field_A8_xpos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos - FP_GetExponent(field_BC_sprite_scale * FP_FromInteger(14)))
                           - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0);

        PSX_RECT pRect = {};
        field_118_anim.Get_Frame_Rect(&pRect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            pRect.x,
            pRect.y,
            pRect.w,
            pRect.h,
            pScreenManager_4FF7C8->field_2E_idx);
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::StickToLiftPoint()
{
    FP hitY = {};
    FP hitX = {};
    PathLine* pLine = nullptr;
    field_1B8_flags.Set(TimedMine_Flags_1B8::eStickToLiftPoint_0);
    if (sCollisions_DArray_504C6C->RayCast(
            field_A8_xpos,
            field_AC_ypos - FP_FromInteger(20),
            field_A8_xpos,
            field_AC_ypos + FP_FromInteger(20),
            &pLine,
            &hitX,
            &hitY,
            (field_BC_sprite_scale != FP_FromDouble(0.5)) ? 7 : 0x70))
    {
        if (pLine->field_8_type == eLineTypes ::eUnknown_32 ||
            pLine->field_8_type == eLineTypes::eUnknown_36)
        {
            if (ObjListPlatforms_50766C)
            {
                for (s32 i = 0; i < ObjListPlatforms_50766C->Size(); i++)
                {
                    BaseGameObject* pObj = ObjListPlatforms_50766C->ItemAt(i);
                    if (!pObj)
                    {
                        break;
                    }

                    if (pObj->field_4_typeId == Types::eLiftPoint_51)
                    {
                        PSX_RECT pObjRect = {};
                        auto pLiftPoint = static_cast<LiftPoint*>(pObj);
                        pLiftPoint->VGetBoundingRect(&pObjRect, 1);
                        if (FP_GetExponent(field_A8_xpos) > pObjRect.x && FP_GetExponent(field_A8_xpos) < pObjRect.w && FP_GetExponent(field_AC_ypos) < pObjRect.h)
                        {
                            field_F8_pLiftPoint = pLiftPoint;
                            pLiftPoint->VAdd(this);
                            field_F8_pLiftPoint->field_C_refCount++;
                            return;
                        }
                    }
                }
            }
        }
    }
}

void TimedMine::VUpdate()
{
    auto pPlatform = static_cast<LiftPoint*>(field_F8_pLiftPoint);
    if (Event_Get(kEventDeathReset_4))
    {
        mFlags.Set(BaseGameObject::eDead);
    }

    if (!field_1B8_flags.Get(TimedMine_Flags_1B8::eStickToLiftPoint_0))
    {
        StickToLiftPoint();
    }

    if (pPlatform && pPlatform->OnAnyFloor())
    {
        field_D4_collection_rect.x = field_A8_xpos - ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2);
        field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize(field_BC_sprite_scale);
        field_D4_collection_rect.w = field_A8_xpos + ScaleToGridSize(field_BC_sprite_scale) / FP_FromInteger(2);
        field_D4_collection_rect.h = field_AC_ypos;
    }
    if (field_10C_armed == 1)
    {
        if (static_cast<s32>(gnFrameCount_507670) > (field_1B4_detonation_timer + field_1B0_gnFrame_2))
        {
            field_1B0_gnFrame_2 = gnFrameCount_507670;
            auto direction = gMap.GetDirection(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos);
            SFX_Play_43AED0(SoundEffect::GreenTick_3, 50, direction);

            //~7 limits the number to multiples of 8
            if (((field_114_timer - gnFrameCount_507670) & ~7) >= 18 * 8)
            {
                field_1B4_detonation_timer = 18;
            }
            else
            {
                field_1B4_detonation_timer = (field_114_timer - gnFrameCount_507670) / 8;
            }
        }
        if (static_cast<s32>(gnFrameCount_507670) >= field_114_timer)
        {
            ao_new<BaseBomb>(
                field_A8_xpos,
                field_AC_ypos,
                0,
                field_BC_sprite_scale);

            mFlags.Set(BaseGameObject::eDead);
        }
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* /*pFrom*/)
{
    ao_new<BaseBomb>(
        field_A8_xpos,
        field_AC_ypos,
        0,
        field_BC_sprite_scale);

    mFlags.Set(BaseGameObject::eDead);
    field_10C_armed = 1;
    field_114_timer = gnFrameCount_507670;
}

void TimedMine::VOnPickUpOrSlapped()
{
    if (field_10C_armed != 1)
    {
        field_10C_armed = 1;
        if ((s32)(field_10E_ticks_before_explosion & 0xFFFC) >= 72)
        {
            field_1B4_detonation_timer = 18;
        }
        else
        {
            field_1B4_detonation_timer = field_10E_ticks_before_explosion >> 2;
        }
        field_1B0_gnFrame_2 = gnFrameCount_507670;
        field_10_anim.Set_Animation_Data(804, 0);
        field_114_timer = gnFrameCount_507670 + field_10E_ticks_before_explosion;
        field_118_anim.Set_Animation_Data(384, 0);
        SFX_Play_43AD70(SoundEffect::GreenTick_3, 0, 0);
    }
}

} // namespace AO
