#include "stdafx_ao.h"
#include "Function.hpp"
#include "Bat.hpp"
#include "ResourceManager.hpp"
#include "Collisions.hpp"
#include "Math.hpp"
#include "Events.hpp"
#include "Game.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "CameraSwapper.hpp"
#include "stdlib.hpp"
#include "Midi.hpp"

START_NS_AO

Bat* Bat::ctor_4046E0(Path_Bat* pTlv, int tlvInfo)
{
    ctor_417C10();
    SetVTable(this, 0x4BA0E8);

    field_4_typeId = Types::eBat_6;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 63, 1, 0);
    if (!ppRes)
    {
        field_6_flags.Clear(BaseGameObject::eDrawable_Bit4);
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        return this;
    }

    Animation_Init_417FD0(6560, 48, 17, ppRes, 1);
    field_10_anim.field_C_layer = 33;

    FP hitX = {};
    FP hitY = {};
    // OG Bug fix, if bat isn't spawned on a line then we crash
    // so field_E4_pLine is checked here and in VUpdate
    sCollisions_DArray_504C6C->RayCast_40C410(
        FP_FromInteger(pTlv->field_10_top_left.field_0_x),
        FP_FromInteger(pTlv->field_10_top_left.field_2_y),
        FP_FromInteger(pTlv->field_14_bottom_right.field_0_x),
        FP_FromInteger(pTlv->field_14_bottom_right.field_2_y),
        &field_E4_pLine,
        &hitX,
        &hitY,
        0x100);

    field_F0_tlvInfo = tlvInfo;

    if (field_E4_pLine)
    {
        field_A8_xpos = FP_FromInteger(field_E4_pLine->field_0_rect.x);
        field_AC_ypos = FP_FromInteger(field_E4_pLine->field_0_rect.y);
    }

    field_EC = pTlv->field_18_ticks_before_moving;
    field_E8_speed = FP_FromRaw(pTlv->field_1A_speed << 8);

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 6;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 25;
    }

    field_F4_state = States::e0;
    field_10C = nullptr;
    field_F6_attack_duration = pTlv->field_1E_attack_duration;

    return this;
}

BaseGameObject* Bat::dtor_404870()
{
    SetVTable(this, 0x4BA0E8);
    if (field_10C)
    {
        field_10C->field_C_refCount--;
    }
    gMap_507BA8.TLV_Reset_446870(field_F0_tlvInfo, -1, 0, 0);
    return dtor_417D10();
}

BaseGameObject* Bat::VDestructor(signed int flags)
{
    return Vdtor_404FF0(flags);
}

void Bat::VScreenChanged()
{
    VScreenChanged_404FE0();
}

void Bat::VScreenChanged_404FE0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

Bat* Bat::Vdtor_404FF0(signed int flags)
{
    dtor_404870();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void Bat::FlyTo_404E50(FP xpos, FP ypos, FP* xSpeed, FP* ySpeed)
{
    const FP xd = FP_Abs(xpos - field_104);
    if (xd > FP_FromInteger(350))
    {
        field_A8_xpos += *xSpeed;
    }

    const FP yd = FP_Abs(ypos - field_108);
    if (yd > FP_FromInteger(200))
    {
        field_AC_ypos += *ySpeed;
    }

    *xSpeed = xd;
    *ySpeed = yd;

    field_104 = xpos;
    field_108 = ypos;

    *xSpeed = FP_FromInteger((Math_NextRandom() & 63) - 32) + xpos - field_A8_xpos;
    *ySpeed = FP_FromInteger((Math_NextRandom() & 31) - 8) + ypos - field_AC_ypos;

    const int ySpeedi = FP_GetExponent(*ySpeed);
    const int xSpeedi = FP_GetExponent(*xSpeed);
    const FP x_final = FP_FromInteger(Math_SquareRoot_Int_4511B0((ySpeedi * ySpeedi) + (xSpeedi * xSpeedi)));

    field_B4_velx = (FP_FromInteger(8) * *xSpeed) / x_final;
    field_B8_vely = (FP_FromInteger(8) * *ySpeed) / x_final;

    field_A8_xpos += field_B4_velx;
    field_AC_ypos += field_B8_vely;
}

void Bat::VUpdate()
{
    VUpdate_404950();
}

void Bat::VUpdate_404950()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (sNumCamSwappers_507668 != 0)
    {
        return;
    }

    FP xSpeed = {};
    FP ySpeed = {};

    switch (field_F4_state)
    {
    case States::e0:
        field_F4_state = States::e1;
        field_F8 = gnFrameCount_507670 + field_EC;
        break;

    case States::e1:
        if (static_cast<int>(gnFrameCount_507670) > field_F8)
        {
            field_F4_state = States::e2;
            field_100 = FP_FromInteger(0);
            field_10_anim.Set_Animation_Data_402A40(6608, nullptr);
        }
        break;

    case States::e2:
        if (field_100 < field_E8_speed)
        {
            field_100 += FP_FromDouble(1.8);
            if (field_100 > field_E8_speed)
            {
                field_100 = field_E8_speed;
            }
        }

        if (field_E4_pLine)
        {
            field_E4_pLine = field_E4_pLine->MoveOnLine_40CA20(&field_A8_xpos, &field_AC_ypos, field_100);
        }

        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_F4_state = States::e3;
            field_10_anim.Set_Animation_Data_402A40(6644, nullptr);
            field_F8 = gnFrameCount_507670 + Math_RandomRange_450F20(0, 90);
        }
        break;

    case States::e3:
        if (field_100 < field_E8_speed)
        {
            field_100 += FP_FromDouble(1.8);
            if (field_100 > field_E8_speed)
            {
                field_100 = field_E8_speed;
            }
        }

        if (!(field_10_anim.field_92_current_frame % 3))
        {
            SFX_Play_43AD70(Math_RandomRange_450F20(41, 42) & 0xFF, Math_RandomRange_450F20(20, 26), 0);
        }

        if (static_cast<int>(gnFrameCount_507670) > field_F8)
        {
            SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_18, 1, 1);
            field_F8 = gnFrameCount_507670 + Math_RandomRange_450F20(120, 240);
        }

        if (field_E4_pLine)
        {
            field_E4_pLine = field_E4_pLine->MoveOnLine_40CA20(&field_A8_xpos, &field_AC_ypos, field_100);
        }

        if (!field_E4_pLine)
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }

        if (!sActiveHero_507678->field_2A8_flags.Get(Flags_2A8::e2A8_Bit6_bShrivel))
        {
            for (int i = 0; i < gBaseAliveGameObjects_4FC8A0->Size(); i++)
            {
                BaseAliveGameObject* pObjIter = gBaseAliveGameObjects_4FC8A0->ItemAt(i);
                if (!pObjIter)
                {
                    break;
                }

                if (pObjIter->field_4_typeId != Types::SecurityOrb_53 && pObjIter->field_4_typeId != Types::eSlig_88 && pObjIter->field_4_typeId != Types::eSlog_89)
                {
                    PSX_RECT bObjRect = {};
                    pObjIter->VGetBoundingRect(&bObjRect, 1);

                    if (FP_GetExponent(field_A8_xpos) >= bObjRect.x &&
                        FP_GetExponent(field_A8_xpos) <= bObjRect.w &&
                        FP_GetExponent(field_AC_ypos) >= bObjRect.y &&
                        FP_GetExponent(field_AC_ypos) <= bObjRect.h &&
                        pObjIter->field_BC_sprite_scale == field_BC_sprite_scale)
                    {
                        for (int j = 0; j < gBaseGameObject_list_9F2DF0->Size(); j++)
                        {
                            BaseGameObject* pMaybeBat = gBaseGameObject_list_9F2DF0->ItemAt(j);
                            if (!pMaybeBat)
                            {
                                break;
                            }

                            if (pMaybeBat->field_4_typeId == Types::eBat_6)
                            {
                                auto pBat = static_cast<Bat*>(pMaybeBat);

                                pBat->field_10C = pObjIter;
                                pBat->field_10C->field_C_refCount++;

                                pBat->field_F4_state = States::e4;
                                pBat->field_10_anim.Set_Animation_Data_402A40(6644, nullptr);

                                pBat->field_F8 = 0;
                                pBat->field_FC = gnFrameCount_507670 + pBat->field_F6_attack_duration;

                                pBat->field_104 = pBat->field_10C->field_A8_xpos;
                                pBat->field_108 = pBat->field_10C->field_AC_ypos;
                            }
                        }
                    }
                }
            }
        }
        break;

    case States::e4:
    {
        if (field_10C->field_6_flags.Get(BaseGameObject::eDead_Bit3) || Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
        {
            field_6_flags.Set(Options::eDead_Bit3);
            return;
        }

        PSX_RECT bRect = {};
        field_10C->VGetBoundingRect(&bRect, 1);
        FlyTo_404E50(
            FP_FromInteger((bRect.w + bRect.x) / 2),
            FP_FromInteger((bRect.h + bRect.y) / 2),
            &xSpeed,
            &ySpeed);

        if (FP_Abs(xSpeed) < FP_FromInteger(10))
        {
            if (FP_Abs(ySpeed) < FP_FromInteger(20) && static_cast<int>(gnFrameCount_507670) > field_F8)
            {
                field_10C->VTakeDamage(this);
                field_F8 = gnFrameCount_507670 + 30;
                SND_SEQ_PlaySeq_4775A0(SeqId::Unknown_18, 1, 1);
            }
        }

        if (field_FC <= static_cast<int>(gnFrameCount_507670))
        {
            field_10C->field_C_refCount--;
            field_10C = nullptr;
            field_F4_state = States::e5;
        }
    }
    break;

    case States::e5:
    {
        FlyTo_404E50(field_A8_xpos, field_AC_ypos - FP_FromInteger(40), &xSpeed, &ySpeed);
        if (Event_Get_417250(kEventDeathReset_4) || Event_Get_417250(kEvent_9))
        {
            field_6_flags.Set(Options::eDead_Bit3);
        }
    }
    break;

    default:
        return;
    }
}

END_NS_AO
