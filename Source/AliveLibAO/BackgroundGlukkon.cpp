#include "stdafx_ao.h"
#include "Function.hpp"
#include "BackgroundGlukkon.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Explosion.hpp"
#include "Math.hpp"
#include "Sfx.hpp"
#include "Abe.hpp"
#include "Events.hpp"
#include "Game.hpp"

namespace AO {

void BackgroundGlukkon::VScreenChanged()
{
    VScreenChanged_41E0E0();
}

BackgroundGlukkon* BackgroundGlukkon::Vdtor_41E0F0(signed int flags)
{
    dtor_41DCE0();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BaseGameObject* BackgroundGlukkon::VDestructor(signed int flags)
{
    return Vdtor_41E0F0(flags);
}

BaseGameObject* BackgroundGlukkon::dtor_41DCE0()
{
    SetVTable(this, 0x4BAF70);

    if (field_100_health <= FP_FromInteger(0))
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 1);
    }
    else
    {
        gMap_507BA8.TLV_Reset_446870(field_10C_tlvInfo, -1, 0, 0);
    }

    return dtor_401000();
}

BackgroundGlukkon* BackgroundGlukkon::ctor_41DBD0(Path_Glukkon* pTlv, int tlvInfo)
{
    ctor_401090();
    SetVTable(this, 0x4BAF70);

    field_4_typeId = Types::eBackgroundGlukkon_42;

    Animation_Init_417FD0(
        46096,
        68,
        60,
        ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 800, 1, 0),
        1);

    field_10C_tlvInfo = tlvInfo;

    field_A8_xpos = FP_FromInteger(pTlv->field_10_top_left.field_0_x);
    field_AC_ypos = FP_FromInteger(pTlv->field_10_top_left.field_2_y);

    field_BC_sprite_scale = FP_FromInteger(pTlv->field_18_scale_percent) / FP_FromInteger(100);

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Palt, pTlv->field_1A_pal_id, 0, 0);
    if (ppRes)
    {
        field_10_anim.LoadPal_403090(ppRes, 0);
    }

    field_120_target_id = pTlv->field_1C_target_id;
    field_11C_voice_adjust = pTlv->field_1E_voice_adjust;
    field_110_state = 0;
    return this;
}

void BackgroundGlukkon::VScreenChanged_41E0E0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

__int16 BackgroundGlukkon::VTakeDamage(BaseGameObject* pFrom)
{
    return VTakeDamage_41DF80(pFrom);
}

__int16 BackgroundGlukkon::VTakeDamage_41DF80(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead_Bit3))
    {
        return 0;
    }

    if (pFrom->field_4_typeId == Types::eShrykull_85)
    {
        field_10_anim.Set_Animation_Data_402A40(46232, 0);
        const auto rndVol = Math_RandomRange_450F20(110, 127);
        const auto rndPitch = (75 * (Math_NextRandom() % 4)) + 200;

        if (Math_NextRandom() >= 128u)
        {
            SFX_Play_43AE60(SoundEffect::Empty_106, rndVol, rndPitch, 0);
        }
        else
        {
            SFX_Play_43AE60(SoundEffect::Empty_105, rndVol, rndPitch, 0);
        }

        field_10_anim.Set_Animation_Data_402A40(46232, 0);
        field_110_state = 7;
    }
    else if (pFrom->field_4_typeId == Types::eElectrocute_103 && field_100_health > FP_FromInteger(0))
    {
        field_100_health = FP_FromInteger(0);
        auto pExplosion = ao_new<Explosion>();
        if (pExplosion)
        {
            pExplosion->ctor_458B80(
                field_A8_xpos,
                field_AC_ypos - (field_BC_sprite_scale * FP_FromInteger(40)),
                field_BC_sprite_scale);
        }
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    return 1;
}

void BackgroundGlukkon::VUpdate()
{
    VUpdate_41DD60();
}

void BackgroundGlukkon::VUpdate_41DD60()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    switch (field_110_state)
    {
    case 0:
        field_110_state = 1;
        field_118_timer1 = gnFrameCount_507670 + Math_RandomRange_450F20(20, 40);
        break;

    case 1:
        field_110_state = 2;
        field_114_timer2 = gnFrameCount_507670 + Math_RandomRange_450F20(12, 20);
        break;

    case 2:
        if (static_cast<int>(gnFrameCount_507670) > field_114_timer2)
        {
            const auto rndVol = Math_RandomRange_450F20(110, 127);
            const auto rndPitch =  ((Math_NextRandom() % 4) * 128) + 200;

            switch (Math_NextRandom() % 5)
            {
            case 0:
                if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
                {
                    field_10_anim.Set_Animation_Data_402A40(46272, 0);
                    SFX_Play_43AE60(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                }
                else
                {
                    field_10_anim.Set_Animation_Data_402A40(46128, 0);
                    SFX_Play_43AE60(SoundEffect::GlukkonKillHim1_101, rndVol, rndPitch, 0);
                }
                field_110_state = 3;
                break;

            case 1:
                if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
                {
                    field_10_anim.Set_Animation_Data_402A40(46272, 0);
                    SFX_Play_43AE60(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                }
                else
                {
                    field_10_anim.Set_Animation_Data_402A40(46180, 0);
                    SFX_Play_43AE60(SoundEffect::GlukkonKillHim2_102, rndVol, rndPitch, 0);
                }
                field_110_state = 3;
                break;

            case 2:
                if (sActiveHero_507678->field_100_health <= FP_FromInteger(0))
                {
                    field_10_anim.Set_Animation_Data_402A40(46272, 0);
                    SFX_Play_43AE60(SoundEffect::GlukkonLaugh1_103, rndVol, rndPitch, 0);
                }
                else
                {
                    field_10_anim.Set_Animation_Data_402A40(46128, 0);
                    SFX_Play_43AE60(SoundEffect::Empty_105, rndVol, rndPitch, 0);
                }
                field_110_state = 3;
                break;

            case 3:
                if (sActiveHero_507678->field_100_health > FP_FromInteger(0))
                {
                    field_10_anim.Set_Animation_Data_402A40(46180, 0);
                    SFX_Play_43AE60(SoundEffect::Empty_106, rndVol, rndPitch, 0);
                }
                field_110_state = 3;
                break;

            case 4:
                return;

            default:
                field_110_state = 3;
                break;
            }
        }
        break;

    case 3:
        if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
        {
            field_10_anim.Set_Animation_Data_402A40(46096, 0);
            field_110_state = 1;
        }
        break;

    default:
        return;
    }
}

}
