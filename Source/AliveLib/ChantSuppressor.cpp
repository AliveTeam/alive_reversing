#include "stdafx.h"
#include "ChantSuppressor.hpp"
#include "Midi.hpp"
#include "stdlib.hpp"
#include "Gibs.hpp"
#include "ScreenShake.hpp"
#include "Function.hpp"

class Class_544534 : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Class_544534* ctor_416390(FP xpos, FP ypos, FP scale)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x544534);
        field_4_typeId = Types::eType_22;

        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kSparksResID);
        Animation_Init_424E10(808, 19, 11, ppRes, 1, 1);
        
        field_DC_bApplyShadows &= ~1u;

        field_20_animation.field_C_render_layer = 37;
        field_20_animation.field_B_render_mode = 1;

        field_20_animation.field_A_b = 80;
        field_20_animation.field_9_g = 80;
        field_20_animation.field_8_r = 80;

        field_CC_sprite_scale = scale * (((FP_FromInteger(Math_NextRandom() % 6)) / FP_FromInteger(10)) + FP_FromDouble(0.7));
        field_FA_16_random = Math_RandomRange_496AB0(0, 16);

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        field_C4_velx = FP_FromInteger(Math_RandomRange_496AB0(-8, 8));
        field_C8_vely = FP_FromInteger(Math_RandomRange_496AB0(-6, -3));
        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_416520(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_416570();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_416720();
    }

private:

    EXPORT void vUpdate_416570()
    {
        if (field_FA_16_random > 0)
        {
            field_FA_16_random--;
        }

        if (field_FA_16_random == 0)
        {
            field_20_animation.Set_Animation_Data_409C80(808, 0);
            field_FA_16_random = -1;
        }

        field_C8_vely += FP_FromDouble(0.8);

        field_C4_velx *= FP_FromDouble(0.9);
        field_C8_vely *= FP_FromDouble(0.9);

        field_C4_velx += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);
        field_C8_vely += FP_FromInteger(Math_NextRandom() - 127) / FP_FromInteger(64);

        field_BC_ypos += field_C8_vely;
        field_B8_xpos += field_C4_velx;

        if (!gMap_5C3030.Is_Point_In_Current_Camera_4810D0(field_C2_lvl_number, field_C0_path_number, field_B8_xpos, field_BC_ypos, 0))
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    EXPORT void vScreenChanged_416720()
    {
        field_6_flags.Set(BaseGameObject::eDead);
    }

    EXPORT void dtor_416550()
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT Class_544534* vdtor_416520(signed int flags)
    {
        dtor_416550();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

private:
    int field_E4_not_used[4];
    int field_F4_not_used1;
    __int16 field_F8_not_used2;
    __int16 field_FA_16_random;
};
ALIVE_ASSERT_SIZEOF(Class_544534, 0xFC);

struct Explosion_Unknown
{
    __int16 field_0;
    __int16 field_2;
    __int16 field_4;
    __int16 field_6;
    int field_8;
};
ALIVE_ASSERT_SIZEOF(Explosion_Unknown, 0xC);

ALIVE_VAR(1, 0x5C1BB6, short, word_5C1BB6, 0);

class Explosion : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Explosion* ctor_4A1200(FP xpos, FP ypos, FP scale, __int16 bUnknown)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x546CB8);
        field_4_typeId = Types::eExplosion_109;

        if (bUnknown)
        {
            BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 372);
            Animation_Init_424E10(14108, 99, 46, ppRes, 1, 1);
        }
        else
        {
            BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 301);
            Animation_Init_424E10(51156, 202, 91, ppRes, 1, 1);
        }

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit18_IsLastFrame);
        field_20_animation.field_B_render_mode = 1;
        field_F8_scale = scale;
        field_D6_scale = scale == FP_FromInteger(1);
        field_CC_sprite_scale = scale * FP_FromInteger(2);
        field_F4_bUnknown = bUnknown;

        if (bUnknown)
        {
            field_FC = scale * FP_FromDouble(0.5);
        }
        else
        {
            field_FC = scale;
        }
        field_DC_bApplyShadows &= ~1u;
        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        auto pScreenShake = alive_new<ScreenShake>();
        if (pScreenShake)
        {
            pScreenShake->ctor_4ACF70(word_5C1BB6 ? 0 : 1, field_F4_bUnknown);
        }

        Explosion_Unknown v12 = {};
        v12.field_0 = FP_GetExponent(FP_FromInteger(-10) * field_FC);
        v12.field_4 = FP_GetExponent(FP_FromInteger(10) * field_FC);
        v12.field_2 = FP_GetExponent(FP_FromInteger(-10) * field_FC);
        v12.field_6 = FP_GetExponent(FP_FromInteger(10) * field_FC);

        DealBlastDamage_4A1BD0(&v12);

        SND_SEQ_PlaySeq_4CA960(14, 1, 1);

        return this;
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4A14C0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_4A1510();
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_4A1EE0();
    }

private:

    EXPORT void DealBlastDamage_4A1BD0(Explosion_Unknown*)
    {
        NOT_IMPLEMENTED();
    }

    EXPORT void vUpdate_4A1510()
    {
        NOT_IMPLEMENTED();
    }

    EXPORT void vScreenChanged_4A1EE0()
    {
        if (gMap_5C3030.field_22 != gMap_5C3030.Get_Path_Unknown_480710())
        {
            field_6_flags.Set(BaseGameObject::eDead);
        }
    }

    EXPORT void dtor_4A14F0()
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
    }

    EXPORT Explosion* vdtor_4A14C0(signed int flags)
    {
        dtor_4A14F0();
        if (flags & 1)
        {
            Mem_Free_495540(this);
        }
        return this;
    }

private:
    __int16 field_E4;
    __int16 field_E6;
    __int16 field_E8;
    __int16 field_EA;
    __int16 field_EC;
    __int16 field_EE;
    __int16 field_F0;
    __int16 field_F2;
    __int16 field_F4_bUnknown;
    __int16 field_F6;
    FP field_F8_scale;
    FP field_FC;
};
ALIVE_ASSERT_SIZEOF(Explosion, 0x100);


const TintEntry sChantOrbTints_55C1EC[18] =
{
    { 1u, 127u, 127u, 127u },
    { 2u, 137u, 137u, 137u },
    { 3u, 127u, 127u, 127u },
    { 4u, 127u, 127u, 127u },
    { 5u, 127u, 127u, 127u },
    { 6u, 127u, 127u, 127u },
    { 7u, 127u, 127u, 127u },
    { 8u, 127u, 127u, 127u },
    { 9u, 127u, 127u, 127u },
    { 10u, 127u, 127u, 127u },
    { 3u, 127u, 127u, 127u },
    { 12u, 127u, 127u, 127u },
    { 13u, 127u, 127u, 127u },
    { 14u, 127u, 127u, 127u },
    { -1, 127u, 127u, 127u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u },
    { 0u, 0u, 0u, 0u }
};


ChantSuppressor* ChantSuppressor::ctor_466350(Path_ChantSuppressor* pTlv, int tlvInfo)
{
    ctor_408240(0);
    SetVTable(this, 0x545F3C);
    
    field_4_typeId = Types::eAntiChant_83;

    field_6_flags.Set(BaseGameObject::eCanExplode);

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, ResourceID::kMaimGameResID);
    Animation_Init_424E10(2228, 53, 28u, ppRes, 1, 1u);
    
    SetTint_425600(sChantOrbTints_55C1EC, gMap_5C3030.sCurrentLevelId_5C3030);

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    field_118_tlvInfo = tlvInfo;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
        field_D6_scale = 0;
        field_20_animation.field_C_render_layer = 8;
    }
    else
    {
        field_CC_sprite_scale = FP_FromInteger(1);
        field_D6_scale = 1;
        field_20_animation.field_C_render_layer = 27;
    }

    field_DC_bApplyShadows |= 2u;
    field_11C_state = 0;
    field_124_sound_channels_mask = 0;

    return this;
}

ChantSuppressor* ChantSuppressor::vdtor_4664B0(char flags)
{
    dtor_4664E0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void ChantSuppressor::dtor_4664E0()
{
    SetVTable(this, 0x545F3C);

    if (field_124_sound_channels_mask)
    {
        SND_Stop_Channels_Mask_4CA810(field_124_sound_channels_mask);
    }

    if (field_10C_health > FP_FromInteger(0))
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 0);
    }
    else
    {
        Path::TLV_Reset_4DB8E0(field_118_tlvInfo, -1, 0, 1);
    }

    dtor_4080B0();
}

void ChantSuppressor::vScreenChanged_466D20()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

signed __int16 ChantSuppressor::vTakeDamage_466BB0(BaseGameObject* pFrom)
{
    if (field_6_flags.Get(BaseGameObject::eDead))
    {
        return 0;
    }

    field_6_flags.Set(BaseGameObject::eDead);
    field_10C_health = FP_FromInteger(0);

    if (pFrom->field_4_typeId == Types::eMineCar_89 || pFrom->field_4_typeId == Types::eType_104 || pFrom->field_4_typeId == Types::eType_121)
    {
        auto pExplosionMem = alive_new<Explosion>();
        if (pExplosionMem)
        {
            pExplosionMem->ctor_4A1200(
                field_B8_xpos,
                field_BC_ypos - (field_CC_sprite_scale * FP_FromInteger(5)),
                field_CC_sprite_scale,
                0);
        }

        auto pGibsMem = alive_new<Gibs>();
        if (pGibsMem)
        {
            pGibsMem->ctor_40FB40(
                5,
                field_B8_xpos,
                field_BC_ypos,
                FP_FromInteger(0),
                FP_FromInteger(0),
                field_CC_sprite_scale,
                0);
        }
    }

    return 1;
}
