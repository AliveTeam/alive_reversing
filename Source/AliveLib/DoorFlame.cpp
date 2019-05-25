#include "stdafx.h"
#include "DoorFlame.hpp"
#include "Function.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"
#include "SwitchStates.hpp"
#include "ObjectIds.hpp"
#include "Midi.hpp"

ALIVE_VAR(1, 0x5C2C6C, DoorFlame*, pFlameControllingTheSound_5C2C6C, nullptr);

struct Class_545974_Anim
{
    FP field_0_x;
    FP field_4_y;
    int field_8_off_x;
    int field_C_off_y;
    __int16 field_10;
    __int16 field_12;
    AnimationUnknown field_14;
};

// This one seems to be random sparks that come out of the door flame, TODO rename when known for sure
class Class_545974 : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT Class_545974* ctor_45DE00(FP xpos, FP ypos)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        for (auto& anim : field_F8_anims)
        {
            SetVTable(&anim.field_14, 0x5447CC);
        }

        SetVTable(this, 0x545974);
        field_4_typeId = Types::eNone_0;

        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 312);
        Animation_Init_424E10(1672, 39, 21, ppRes, 1, 1);
        field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);

        field_DC_bApplyShadows |= 1u;
        field_20_animation.field_C_render_layer = 17;

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;
        field_410_xpos = xpos;
        field_414_ypos = ypos;

        field_CC_sprite_scale = FP_FromDouble(0.3);

        for (auto& anim : field_F8_anims)
        {
            anim.field_14.field_68_anim_ptr = &field_20_animation;

            anim.field_14.field_4_flags.Set(AnimFlags::eBit3_Render);
            anim.field_14.field_4_flags.Set(AnimFlags::eBit16_bBlending);

            anim.field_14.field_C_render_layer = field_20_animation.field_C_render_layer + Math_RandomRange_496AB0(-1, 1);
            anim.field_14.field_6C_scale = field_CC_sprite_scale;

            anim.field_0_x = field_B8_xpos;
            anim.field_4_y = field_BC_ypos;

            anim.field_8_off_x = 0;
            anim.field_C_off_y = 0;

            anim.field_10 = 0;
            anim.field_12 = Math_RandomRange_496AB0(0, 64);
        }

        field_F4_bRender = 0;
        return this;
    }

private:
    int field_E4_not_used[4];
    __int16 field_F4_bRender;
    __int16 field_F6_pad;
    Class_545974_Anim field_F8_anims[6];
    FP field_410_xpos;
    FP field_414_ypos;
};

DoorFlame* DoorFlame::ctor_45E460(Path_DoorFlame* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
    SetVTable(this, 0x5459BC);

    field_4_typeId = Types::eNone_0;
    field_F4_tlvInfo = tlvInfo;
    
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 304);
    Animation_Init_424E10(5156, 22, 21u, ppRes, 1, 1u);

    field_20_animation.field_4_flags.Set(AnimFlags::eBit15_bSemiTrans);
    field_DC_bApplyShadows |= 1u;
    field_20_animation.field_C_render_layer = 17;
    field_FA_frame_count = field_20_animation.Get_Frame_Count_40AC70();
    field_20_animation.SetFrame_409D50(Math_RandomRange_496AB0(0, field_FA_frame_count - 1));
    
    field_F8_switch_id = pTlv->field_10_id;

    if (pTlv->field_12_scale)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }

    field_B8_xpos = FP_FromInteger(pTlv->field_8_top_left.field_0_x)  + (FP_FromInteger(12) * field_CC_sprite_scale);
    field_108 = -1;
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y) + (FP_FromInteger(15) * field_CC_sprite_scale);

    if (SwitchStates_Get_466020(field_F8_switch_id))
    {
        field_20_animation.field_4_flags.Set(AnimFlags::eBit3_Render);
        field_FC_bOn = 1;
    }
    else
    {
        field_20_animation.field_4_flags.Clear(AnimFlags::eBit3_Render);
        field_FC_bOn = 0;
    }
    
    field_FE_2_random = Math_NextRandom() % 2;
    
    auto pFlameSparks = alive_new<Class_545974>();
    if (pFlameSparks)
    {
        pFlameSparks->ctor_45DE00(field_B8_xpos, field_BC_ypos);
        field_10C_flame_sparks_id = pFlameSparks->field_8_object_id;
    }
    
    return this;
}

DoorFlame* DoorFlame::vdtor_45E690(signed int flags)
{
    dtor_45E6C0();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

void DoorFlame::dtor_45E6C0()
{
    SetVTable(this, 0x5459BC);

    BaseGameObject* v2 = sObjectIds_5C1B70.Find_449CF0(field_108);
    BaseGameObject* pFlameSparks = sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id);
    
    if (v2)
    {
        v2->field_6_flags.Set(BaseGameObject::eDead);
        field_108 = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->field_6_flags.Set(BaseGameObject::eDead);
        field_10C_flame_sparks_id = -1;
    }

    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = 0;
        SND_Stop_Channels_Mask_4CA810(field_100_sounds_mask);
    }

    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);
    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

void DoorFlame::vStopAudio_45E7E0()
{
    if (pFlameControllingTheSound_5C2C6C == this)
    {
        pFlameControllingTheSound_5C2C6C = nullptr;
        SND_Stop_Channels_Mask_4CA810(field_100_sounds_mask);
        field_100_sounds_mask = 0;
    }
}

void DoorFlame::vScreenChanged_45EA90()
{
    BaseGameObject* v2 = sObjectIds_5C1B70.Find_449CF0(field_108);
    BaseGameObject* pFlameSparks = sObjectIds_5C1B70.Find_449CF0(field_10C_flame_sparks_id);

    field_6_flags.Set(BaseGameObject::eDead);
    
    if (v2)
    {
        v2->field_6_flags.Set(BaseGameObject::eDead);
        field_108 = -1;
    }

    if (pFlameSparks)
    {
        pFlameSparks->field_6_flags.Set(BaseGameObject::eDead);
        field_10C_flame_sparks_id = -1;
    }
}
