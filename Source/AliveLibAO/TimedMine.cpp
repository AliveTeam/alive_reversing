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

TintEntry stru_4C3140[3] =
{
  { 5, 60u, 60u, 60u },
  { 6, 60u, 60u, 60u },
  { -1, 127u, 127u, 127u },
};

TimedMine* TimedMine::ctor_4083F0(Path_TimedMine* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(&field_118_anim, 0x4BA2B8);
    SetVTable(this, 0x4BA2C8);
    field_4_typeId = Types::eTimedMine_8;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBombResID, 1, 0);
    Animation_Init_417FD0(792, 35, 17, ppRes, 1);

    field_6_flags.Set(Options::eInteractive_Bit8);
    field_1B8 &= ~1u;
    field_10C_armed = 0;

    if (pTlv->field_1C_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
        field_C6_scale = 0;
        field_10_anim.field_C_layer = 16;
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
        field_C6_scale = 1;
        field_10_anim.field_C_layer = 35;
    }

    if (field_118_anim.Init_402D20(
        372,
        gObjList_animations_505564,
        this,
        37,
        21,
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kBombflshResID, 1, 0),
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
        field_118_anim.field_B_render_mode = 1;
    }
    else
    {
        field_6_flags.Set(Options::eListAddFailed_Bit1);
    }

    field_10E_explode_timeout = pTlv->field_1E_ticks_before_explode;
    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x + 12);
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y + 24);

    field_114_timer = gnFrameCount_507670;
    field_110_tlvInfo = tlvInfo;

    SetBaseAnimPaletteTint_4187C0(stru_4C3140, gMap_507BA8.field_0_current_level, 1005);
    field_D4_collection_rect.x = field_A8_xpos - (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
    field_D4_collection_rect.w = field_A8_xpos + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2)); 
    field_D4_collection_rect.h = field_AC_ypos;
    field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale);

    field_6_flags.Set(Options::eInteractive_Bit8);
    field_F8_pLiftPoint = nullptr;
    return this;
}

BaseGameObject* TimedMine::dtor_408690()
{
    SetVTable(this, 0x4BA2C8);
    if (field_10C_armed != 1 || static_cast<int>(gnFrameCount_507670) < field_114_timer)
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 0);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_110_tlvInfo, -1, 0, 1);
    }

    field_118_anim.vCleanUp();

    if (field_F8_pLiftPoint)
    {
        field_F8_pLiftPoint->VRemove(this);
        field_F8_pLiftPoint->field_C_refCount--;
        field_F8_pLiftPoint = nullptr;
    }

    field_6_flags.Clear(Options::eInteractive_Bit8);
    return dtor_401000();
}

BaseGameObject* TimedMine::VDestructor(signed int flags)
{
    return Vdtor_408E10(flags);
}

TimedMine* TimedMine::Vdtor_408E10(signed int flags)
{
    dtor_408690();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void TimedMine::VScreenChanged()
{
    VScreenChanged_408DD0();
}

void TimedMine::VScreenChanged_408DD0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level ||
        gMap_507BA8.field_2_current_path != gMap_507BA8.field_C_path)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }

    if (field_10C_armed != 1)
    {
        field_6_flags.Set(Options::eDead_Bit3);
    }
}

__int16 TimedMine::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_408B90(pFrom);
}

__int16 TimedMine::VTakeDamage_408B90(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
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
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
        auto pBaseBomb = ao_new<BaseBomb>();
        if (pBaseBomb)
        {
            pBaseBomb->ctor_4173A0(
                field_A8_xpos,
                field_AC_ypos,
                0,
                field_BC_sprite_scale);
        }
        field_10C_armed = 1;
        field_114_timer = gnFrameCount_507670;
        return 1;
    }
    default:
        return 0;
    }
}

void TimedMine::VRender(int** pOt)
{
    VRender_408960(pOt);
}

void TimedMine::VRender_408960(int** ppOt)
{
    if (gMap_507BA8.Is_Point_In_Current_Camera_4449C0(
            field_B2_lvl_number,
            field_B0_path_number,
            field_A8_xpos,
            field_AC_ypos,
            0
        ))
    {
        field_118_anim.VRender_403AE0(
            FP_GetExponent(field_A8_xpos + FP_FromInteger(pScreenManager_4FF7C8->field_14_xpos) - pScreenManager_4FF7C8->field_10_pCamPos->field_0_x),
            FP_GetExponent(field_AC_ypos + FP_FromInteger(pScreenManager_4FF7C8->field_16_ypos - FP_GetExponent(field_BC_sprite_scale * FP_FromInteger(14)))
            - pScreenManager_4FF7C8->field_10_pCamPos->field_4_y),
            ppOt,
            0,
            0
        );

        PSX_RECT pRect = {};
        field_118_anim.Get_Frame_Rect_402B50(&pRect);
        pScreenManager_4FF7C8->InvalidateRect_406E40(
            pRect.x,
            pRect.y,
            pRect.w,
            pRect.h,
            pScreenManager_4FF7C8->field_2E_idx
        );
        BaseAnimatedWithPhysicsGameObject::VRender(ppOt);
    }
}

void TimedMine::StickToLiftPoint_408CA0()
{
    FP hitY = {};
    FP hitX = {};
    PathLine *pLine = nullptr;
    field_1B8 |= 1;
    if (sCollisions_DArray_504C6C->RayCast_40C410(
        field_A8_xpos,
        field_AC_ypos - FP_FromInteger(20),
        field_A8_xpos,
        field_AC_ypos + FP_FromInteger(20),
        &pLine,
        &hitX,
        &hitY,
        (field_BC_sprite_scale != FP_FromDouble(0.5)) ? 7 : 0x70))
    {
        if (pLine->field_8_type == 32 || pLine->field_8_type == 36)
        {
            if (ObjListPlatforms_50766C)
            {
                for (int i = 0; i < ObjListPlatforms_50766C->Size(); i++)
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
                        if (FP_GetExponent(field_A8_xpos) > pObjRect.x &&
                            FP_GetExponent(field_A8_xpos) < pObjRect.w &&
                            FP_GetExponent(field_AC_ypos) < pObjRect.h)
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
    VUpdate_408760();
}

void TimedMine::VUpdate_408760()
{
    auto pPlatform = static_cast<LiftPoint*>(field_F8_pLiftPoint);
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!(field_1B8 & 1))
    {
        StickToLiftPoint_408CA0();
    }

    if (pPlatform && pPlatform->OnAnyFloor())
    {
        field_D4_collection_rect.x = field_A8_xpos - ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2);
        field_D4_collection_rect.y = field_AC_ypos - ScaleToGridSize_41FA30(field_BC_sprite_scale);
        field_D4_collection_rect.w = field_A8_xpos + ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2);
        field_D4_collection_rect.h = field_AC_ypos;
    }
    if (field_10C_armed == 1)
    {
        if (static_cast<int>(gnFrameCount_507670) > (field_1B4 + field_1B0))
        {
            field_1B0 = gnFrameCount_507670;
            auto direction = gMap_507BA8.GetDirection(
                field_B2_lvl_number,
                field_B0_path_number,
                field_A8_xpos,
                field_AC_ypos);
            SFX_Play_43AED0(SoundEffect::GreenTick_3, 50, direction);

            //~7 limits the number to multiples of 8
            if (((field_114_timer - gnFrameCount_507670) & ~7) >= 18 * 8)
            {
                field_1B4 = 18;
            }
            else
            {
                field_1B4 = (field_114_timer - gnFrameCount_507670) / 8;
            }
        }
        if (static_cast<int>(gnFrameCount_507670) >= field_114_timer)
        {
            auto pBaseBomb = ao_new<BaseBomb>();
            if (pBaseBomb)
            {
                pBaseBomb->ctor_4173A0(
                    field_A8_xpos,
                    field_AC_ypos,
                    0,
                    field_BC_sprite_scale
                );
            }

            field_6_flags.Set(BaseGameObject::eDead_Bit3);
        }
    }
}

void TimedMine::VOnThrowableHit(BaseGameObject* pFrom)
{
    VOnThrowableHit_408B10(pFrom);
}

void TimedMine::VOnThrowableHit_408B10(BaseGameObject* /*pFrom*/)
{
    auto pBaseBomb = ao_new<BaseBomb>();
    if (pBaseBomb)
    {
        pBaseBomb->ctor_4173A0(
            field_A8_xpos,
            field_AC_ypos,
            0,
            field_BC_sprite_scale);
    }
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
    field_10C_armed = 1;
    field_114_timer = gnFrameCount_507670;
}

void TimedMine::VOnPickUpOrSlapped()
{
    vOnPickUpOrSlapped_408A80();
}

void TimedMine::vOnPickUpOrSlapped_408A80()
{
    if (field_10C_armed != 1)
    {
        field_10C_armed = 1;
        if ((signed int) (field_10E_explode_timeout & 0xFFFC) >= 72)
        {
            field_1B4 = 18;
        }
        else
        {
            field_1B4 = field_10E_explode_timeout >> 2;
        }
        field_1B0 = gnFrameCount_507670;
        field_10_anim.Set_Animation_Data_402A40(804, 0);
        field_114_timer = gnFrameCount_507670 + field_10E_explode_timeout;
        field_118_anim.Set_Animation_Data_402A40(384, 0);
        SFX_Play_43AD70(SoundEffect::GreenTick_3, 0, 0);
    }
}

}
