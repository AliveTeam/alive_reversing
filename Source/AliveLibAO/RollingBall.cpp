#include "stdafx_ao.h"
#include "Function.hpp"
#include "RollingBall.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "RollingBallShaker.hpp"
#include "Shadow.hpp"
#include "SwitchStates.hpp"
#include "ParticleBurst.hpp"
#include "Flash.hpp"
#include "ScreenShake.hpp"
#include "Events.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Midi.hpp"

START_NS_AO

void RollingBall::VUpdate()
{
    VUpdate_457AF0();
}

RollingBall* RollingBall::Vdtor_458490(signed int flags)
{
    dtor_458230();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* RollingBall::VDestructor(signed int flags)
{
    return Vdtor_458490(flags);
}

BaseGameObject* RollingBall::dtor_458230()
{
    SetVTable(this, 0x4BC180);
    if (field_112_state)
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }

    if (field_120_pCollisionLine)
    {
        Rect_Clear_40C920(&field_120_pCollisionLine->field_0_rect);
    }

    if (field_114_pRollingBallShaker)
    {
        field_114_pRollingBallShaker->field_32_bKillMe = TRUE;
        field_114_pRollingBallShaker->field_C_refCount--;
        field_114_pRollingBallShaker = nullptr;
    }

    BYTE** pRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 0, 0);
    ResourceManager::FreeResource_455550(pRes);
    return dtor_401000();
}

RollingBall* RollingBall::ctor_4578C0(Path_RollingBall* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BC180);
    field_4_typeId = Types::eRollingBall_72;
    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 2002, 1, 0);
    Animation_Init_417FD0(
        15596,
        131,
        64,
        ppRes,
        1);

    field_10_anim.field_C_layer = 31;

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_10_anim.field_C_layer = 12;
        field_C6_scale = 0;
    }

    if (!pTlv->field_1A_roll_direction)
    {
        field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX);
    }

    field_110_release_switch_id = pTlv->field_1C_release;
    field_118_speed = FP_FromRaw(pTlv->field_1E_speed << 8);

    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        field_118_speed = -FP_FromRaw(pTlv->field_1E_speed << 8);
    }

    field_11C = FP_FromRaw(pTlv->field_20_acceleration << 8);

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    FP castX = {};
    FP castY = {};

    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos,
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(24),
        &field_F4_pLine,
        &castX,
        &castY,
        // yeah this should be simplified, WTF! 
        field_BC_sprite_scale - FP_FromDouble(0.5) != FP_FromInteger(0) ? 1 : 0x10) == 1)
    {
        field_AC_ypos = castY;
    }

    MapFollowMe_401D30(TRUE);
    field_10C_tlvInfo = tlvInfo;
    field_112_state = 0;
    field_114_pRollingBallShaker = nullptr;
    field_120_pCollisionLine = nullptr;

    field_D0_pShadow = ao_new<Shadow>();
    if (field_D0_pShadow)
    {
        field_D0_pShadow->ctor_461FB0();
    }

    // Looks strange, it just bumps the res ref count
    ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 1105, 1, 0);

    if (!SwitchStates_Get(field_110_release_switch_id))
    {
        return this;
    }

    if (gMap_507BA8.field_0_current_level == LevelIds::eForestTemple_4 && gMap_507BA8.field_2_current_path == 2)
    {
        field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
        field_A8_xpos = FP_FromInteger(2522);
        field_AC_ypos = FP_FromInteger(1300);
        field_10_anim.field_C_layer = 35;
        field_112_state = 4;
    }

    return this;
}

// Something to do with skipping gaps across camera ??
FP CC sub_418590(FP xpos, FP xvel, __int16 xMargin, WORD* pResult)
{
    const int v1 = FP_GetExponent(xpos - FP_FromInteger(256)) / 512;

    const FP v4 = xpos - FP_FromInteger(v1);
    const FP v5 = FP_FromInteger(FP_GetExponent(xpos) % v1);
    
    const int v6 = v1 % 2;

    FP result = {};
    if ((!v6 || v5 >= FP_FromInteger(xMargin - 512)) &&
         (v6 || v5 <= FP_FromInteger(xMargin + 368)))
    {
        *pResult = 0;
        result = xpos;
    }
    else if (xvel <= FP_FromInteger(0))
    {
        *pResult = 1;
        result = FP_FromInteger((v4.fpValue << 9) + xMargin + 112);
    }
    else
    {
        if (v6)
        {
            *pResult = 2;
             result = FP_FromInteger((v4.fpValue << 9) - xMargin + 768);
        }
        else
        {
            *pResult = 2;
             result = FP_FromInteger((v4.fpValue << 9) - xMargin + 1280);
        }
    }
    return result;
}

void RollingBall::VUpdate_457AF0()
{
    switch (field_112_state)
    {
    case 0:
        if (SwitchStates_Get(field_110_release_switch_id))
        {
            field_B8_vely = FP_FromDouble(2.5);
            field_112_state = 1;
            field_10_anim.Set_Animation_Data_402A40(15608, 0);
            field_114_pRollingBallShaker = ao_new<RollingBallShaker>();
            if (field_114_pRollingBallShaker)
            {
                field_114_pRollingBallShaker->ctor_4361A0();
                field_114_pRollingBallShaker->field_C_refCount++;
            }
        }
        else if (!gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0))
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
        return;

    case 1:
        if (!(field_10_anim.field_92_current_frame % 3))
        {
            SFX_Play_43AD70(static_cast<char>(Math_RandomRange_450F20(71, 72)), 0, 0);
        }
        else
        {
            SpeedUpOrDown_458410();

            PathLine* pLine = nullptr;
            FP hitX = {};
            FP hitY = {};
            if (InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromInteger(0)))
            {
                if (pLine->field_8_type && pLine->field_8_type != 4)
                {
                    return;
                }

                field_B8_vely = FP_FromInteger(0);
                field_A8_xpos = hitX;
                field_AC_ypos = hitY;
                field_F4_pLine = pLine;
                field_112_state = 2;
            }
        }
        return;

    case 2:
    {
        if (!(field_10_anim.field_92_current_frame % 3))
        {
            SFX_Play_43AD70(static_cast<char>(Math_RandomRange_450F20(71, 72)), 0, 0);
        }

        SpeedUpOrDown_458410();

        field_F4_pLine = field_F4_pLine->MoveOnLine_40CA20(
            &field_A8_xpos,
            &field_AC_ypos,
            field_B4_velx);

        WORD v39 = 0;
        sub_418590(field_A8_xpos, field_B4_velx, 50, &v39);
        if (v39 == 1 || v39 == 2)
        {
            MapFollowMe_401D30(0);
        }

        CrushThingsInTheWay_458310();

        if (Event_Get_417250(kEventDeathReset_4))
        {
            field_114_pRollingBallShaker->field_C_refCount--;
            field_114_pRollingBallShaker->field_32_bKillMe = 1;
            field_6_flags.Set(BaseGameObject::eDead_Bit3);
            field_114_pRollingBallShaker = nullptr;

        }
        else if (!field_F4_pLine)
        {
            field_112_state = 3;

            field_114_pRollingBallShaker->field_C_refCount--;
            field_114_pRollingBallShaker->field_32_bKillMe = 1;
            field_114_pRollingBallShaker = nullptr;

            field_A8_xpos += field_B4_velx;
            field_E8_LastLineYPos = field_AC_ypos;
        }
        return;
    }

    case 3:
    {
        if (WallHit_401930(FP_FromInteger(30), field_B4_velx))
        {
            auto pParticleBurst = ao_new<ParticleBurst>();
            if (pParticleBurst)
            {
                pParticleBurst->ctor_40D0F0(
                    field_A8_xpos,
                    field_AC_ypos - FP_FromInteger(30),
                    150,
                    field_BC_sprite_scale,
                    BurstType::eType_0);
            }

            auto pFlash = ao_new<Flash>();
            if (pFlash)
            {
                pFlash->ctor_41A810(39, 255, 255, 255, 1, 1u, 1);
            }

            auto pScreenShake = ao_new<ScreenShake>();
            if (pScreenShake)
            {
                pScreenShake->ctor_4624D0(0);
            }

            field_6_flags.Set(Options::eDead_Bit3);

            const  CameraPos direction = gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos);
            SFX_Play_43AED0(107u, 50, direction);

            switch (direction)
            {
            case CameraPos::eCamCurrent_0:
                SND_SEQ_Play_477760(21u, 1, 60, 60);
                break;

            case CameraPos::eCamTop_1:
            case CameraPos::eCamBottom_2:
                SND_SEQ_Play_477760(21u, 1, 40, 40);
                break;

            case CameraPos::eCamLeft_3:
                SND_SEQ_Play_477760(21u, 1, 40, 13);
                break;

            case CameraPos::eCamRight_4:
                SND_SEQ_Play_477760(21u, 1, 13, 40);
                break;

            default:
                return;
            }
            return;
        }

        if (Event_Get_417250(kEventDeathReset_4))
        {
            field_6_flags.Set(Options::eDead_Bit3);
            CrushThingsInTheWay_458310();
            return;
        }

        PathLine* pLine = nullptr;
        FP hitX = {};
        FP hitY = {};
        if (!InAirCollision_4019C0(&pLine, &hitX, &hitY, FP_FromDouble(1.8)))
        {
            if (field_AC_ypos - field_E8_LastLineYPos > FP_FromInteger(240))
            {
                if (gMap_507BA8.field_0_current_level == LevelIds::eForestTemple_4
                    && gMap_507BA8.field_2_current_path == 2
                    && !sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
                {
                    field_10_anim.field_4_flags.Clear(AnimFlags::eBit2_Animate);
                    field_A8_xpos = FP_FromInteger(2522);
                    field_AC_ypos = FP_FromInteger(1300);
                    field_10_anim.field_C_layer = 35;
                    field_112_state = 4;
                    CrushThingsInTheWay_458310();
                    return;
                }
                field_6_flags.Set(Options::eDead_Bit3);
            }
            CrushThingsInTheWay_458310();
            return;
        }

        field_AC_ypos = hitY;
        field_E8_LastLineYPos = field_AC_ypos;
        field_B8_vely = (-field_B8_vely * FP_FromDouble(0.8));

        auto pScreenShake = ao_new<ScreenShake>();
        if (pScreenShake)
        {
            pScreenShake->ctor_4624D0(0);
        }

        const CameraPos direction = gMap_507BA8.GetDirection(field_B2_lvl_number, field_B0_path_number, field_A8_xpos, field_AC_ypos);
        SFX_Play_43AED0(107u, 50, direction);

        switch (direction)
        {
        case CameraPos::eCamCurrent_0:
            SND_SEQ_Play_477760(21u, 1, 60, 60);
            CrushThingsInTheWay_458310();
            break;

        case CameraPos::eCamTop_1:
        case CameraPos::eCamBottom_2:
            SND_SEQ_Play_477760(21u, 1, 40, 40);
            CrushThingsInTheWay_458310();
            break;

        case CameraPos::eCamLeft_3:
            SND_SEQ_Play_477760(21u, 1, 40, 13);
            CrushThingsInTheWay_458310();
            break;

        case CameraPos::eCamRight_4:
            SND_SEQ_Play_477760(21u, 1, 13, 40);
            CrushThingsInTheWay_458310();
            break;

        default:
            CrushThingsInTheWay_458310();
            return;
        }
        return;
    }

    case 4:
        if (field_B2_lvl_number != gMap_507BA8.field_0_current_level ||
            field_B0_path_number != gMap_507BA8.field_2_current_path ||
            Event_Get_417250(4))
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
        return;

    default:
        return;
    }
}

void RollingBall::SpeedUpOrDown_458410()
{
    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
    {
        if (field_B4_velx > field_118_speed)
        {
            field_B4_velx -= field_11C;
            field_B8_vely = (-field_B4_velx * FP_FromDouble(0.5));
        }
    }
    else
    {
        if (field_B4_velx < field_118_speed)
        {
            field_B4_velx += field_11C;
            field_B8_vely = (field_B4_velx * FP_FromDouble(0.5));
        }
    }
}

void RollingBall::CrushThingsInTheWay_458310()
{
    for (int i = 0; i < gBaseGameObject_list_9F2DF0->Size(); i++)
    {
        BaseGameObject* pObj = gBaseGameObject_list_9F2DF0->ItemAt(i);
        if (!pObj)
        {
            break;
        }

        if (pObj->field_6_flags.Get(BaseGameObject::eIsBaseAliveGameObject_Bit6))
        {
            PSX_RECT bOurRect = {};
            VGetBoundingRect(&bOurRect, 1);

            auto pAliveObj = static_cast<BaseAliveGameObject*>(pObj);

            PSX_RECT bObjRect = {};
            pAliveObj->VGetBoundingRect(&bObjRect, 1);

            if (bOurRect.x <= bObjRect.x
                && bOurRect.w >= bObjRect.x
                && bOurRect.h >= bObjRect.y
                && bOurRect.y <= bObjRect.y)
            {
                pAliveObj->VTakeDamage(this);
            }
        }
    }
}

END_NS_AO
