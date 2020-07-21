#include "stdafx_ao.h"
#include "Function.hpp"
#include "BoomMachine.hpp"
#include "ResourceManager.hpp"
#include "stdlib.hpp"
#include "Events.hpp"
#include "BaseAliveGameObject.hpp"
#include "Sfx.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"

START_NS_AO

class GrenadeMachineNozzel : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override
    {
        VUpdate_41E220();
    }

    EXPORT void VUpdate_41E220()
    {
        NOT_IMPLEMENTED();
    }

    virtual void VScreenChanged() override
    {
        // Empty
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        dtor_417D10();
        if (flags & 1)
        {
            ao_delete_free_447540(this);
        }
        return this;
    }

    int field_D4[4];
    __int16 field_E4_state;
    __int16 field_E6;
    int field_E8;
    __int16 field_EC_num_grenades;
    __int16 field_EE_pad;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzel, 0xF0);

void BoomMachine::Vsub_41E6F0()
{
    if (Vsub_41E840())
    {
        if (field_EC_pNozzel->field_E4_state == 0)
        {
            field_EC_pNozzel->field_E4_state = 2;
            field_EC_pNozzel->field_E8 = gnFrameCount_507670 + 10;
        }
    }
    else
    {
        if (field_EC_pNozzel->field_E4_state == 0)
        {
            field_EC_pNozzel->field_E4_state = 1;
            field_EC_pNozzel->field_E8 = gnFrameCount_507670 + 10;
        }
    }
}

BOOL BoomMachine::Vsub_41E840()
{
    return field_E8_state == 1;
}

void BoomMachine::VUpdate_41E750()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_E8_state == 0)
    {
        if (!gpThrowableArray_50E26C || gpThrowableArray_50E26C->field_10_count == 0)
        {
            field_E8_state = 1;
            field_10_anim.Set_Animation_Data_402A40(1572, nullptr);
        }
    }
    else if (field_E8_state == 1)
    {
        if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count > 0)
        {
            field_E8_state = 0;
            field_10_anim.Set_Animation_Data_402A40(1600, nullptr);
        }

        if (field_10_anim.field_92_current_frame == 3)
        {
            SFX_Play_43AE60(4u, 25, -1200, 0);
        }
    }
}

BaseGameObject* BoomMachine::dtor_41E670()
{
    SetVTable(this, 0x4BB008);

    if (field_EC_pNozzel)
    {
        field_EC_pNozzel->field_C_refCount--;
        field_EC_pNozzel->field_6_flags.Set(Options::eDead_Bit3);
    }

    gMap_507BA8.TLV_Reset_446870(field_E4_tlvInfo, -1, 0, 0);
    SetVTable(this, 0x4BB048);
    return dtor_417D10();
}

BoomMachine* BoomMachine::ctor_41E420(Path_BoomMachine* pTlv, int tlvInfo)
{
    ctor_417C10();

    SetVTable(this, 0x4BB008);
    field_4_typeId = Types::eGrenadeMachine_41;

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6008, 1, 0);
    Animation_Init_417FD0(1600, 26, 17, ppRes, 1);

    field_CC_bApplyShadows &= ~1u;
    field_E4_tlvInfo = tlvInfo;
    field_10_anim.field_B_render_mode = 1;

    if (pTlv->field_18_scale == 1)
    {
        field_BC_sprite_scale = FP_FromDouble(0.5);
    }
    else
    {
        field_BC_sprite_scale = FP_FromInteger(1);
    }

    field_A8_xpos = FP_FromInteger(pTlv->field_C_sound_pos.field_0_x) + (ScaleToGridSize_41FA30(field_BC_sprite_scale) / FP_FromInteger(2));
    field_AC_ypos = FP_FromInteger(pTlv->field_C_sound_pos.field_2_y);

    auto pNozzel = ao_new<GrenadeMachineNozzel>();
    if (pNozzel)
    {
        FP directedScale = field_BC_sprite_scale;
        if (pTlv->field_1A_nozzel_side)
        {
            directedScale = -directedScale;
        }

        pNozzel->ctor_417C10();
        SetVTable(pNozzel, 0x4BAFD0);

        BYTE** ppNozzelRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, 6009, 1, 0);
        pNozzel->Animation_Init_417FD0(3616, 66, 36, ppNozzelRes, 1);

        pNozzel->field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pNozzel->field_BC_sprite_scale = field_BC_sprite_scale;
        pNozzel->field_CC_bApplyShadows &= ~1u;
        pNozzel->field_E4_state = 0;
        pNozzel->field_A8_xpos = field_A8_xpos + (directedScale * FP_FromInteger(30));
        pNozzel->field_AC_ypos = field_AC_ypos + (field_BC_sprite_scale * FP_FromInteger(-30));
        pNozzel->field_EC_num_grenades = static_cast<short>(pTlv->field_1E_number_of_grenades);
    }

    pNozzel->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_nozzel_side & 1);

    pNozzel->field_C_refCount++;
    field_EC_pNozzel = pNozzel;

    if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count)
    {
        field_E8_state = 1;
        field_10_anim.Set_Animation_Data_402A40(1572, 0);
    }
    else
    {
        field_E8_state = 0;
    }

    field_C8 = 0;
    return this;
}

BaseGameObject* BoomMachine::VDestructor(signed int flags)
{
    dtor_417D10();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

BoomMachine* BoomMachine::Vdtor_41E850(signed int flags)
{
    dtor_41E670();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void BoomMachine::VScreenChanged()
{
    VScreenChanged_41E7F0();
}

void BoomMachine::VScreenChanged_41E7F0()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

END_NS_AO
