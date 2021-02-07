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
#include "Grenade.hpp"

namespace AO {

class GrenadeMachineNozzle : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual void VUpdate() override
    {
        VUpdate_41E220();
    }

    EXPORT void VUpdate_41E220()
    {
        switch (field_E4_state)
        {
        case BoomMachineStates::eAlreadyUsed_1:
            if (static_cast<int>(gnFrameCount_507670) > field_E8_timer)
            {
                SFX_Play_43AE60(SoundEffect::ZPop_5, 60, -1800, 0);
                field_E4_state = BoomMachineStates::eInactive_0;
            }
            break;

        case BoomMachineStates::eDropGrenadeAnimation_2:
            if (static_cast<int>(gnFrameCount_507670) > field_E8_timer)
            {
                field_E4_state = BoomMachineStates::eDropGrenade_3;
                field_10_anim.Set_Animation_Data_402A40(3588, 0);
            }
            break;

        case BoomMachineStates::eDropGrenade_3:
            if (field_10_anim.field_4_flags.Get(AnimFlags::eBit18_IsLastFrame))
            {
                SFX_Play_43AE60(SoundEffect::PickupItem_33, 127, -900, 0);
                if (!gpThrowableArray_50E26C)
                {
                    gpThrowableArray_50E26C = ao_new<ThrowableArray>();
                    if (gpThrowableArray_50E26C)
                    {
                        gpThrowableArray_50E26C->ctor_453EE0();
                    }
                }

                gpThrowableArray_50E26C->Add_453F70(field_EC_num_grenades);

                auto pNewNade = ao_new<Grenade>();
                if (pNewNade)
                {
                    FP directedScale = {};
                    if (field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedScale = -field_BC_sprite_scale;
                    }
                    else
                    {
                        directedScale = field_BC_sprite_scale;
                    }
       
                    pNewNade->ctor_41EBD0(
                        field_A8_xpos + (FP_FromInteger(6) * directedScale),
                        field_AC_ypos + (-FP_FromInteger(6) * directedScale),
                        field_EC_num_grenades);
                }

                pNewNade->VThrow(field_10_anim.field_4_flags.Get(AnimFlags::eBit5_FlipX) ? FP_FromDouble(-0.75) : FP_FromDouble(0.75), FP_FromInteger(3));

                field_10_anim.Set_Animation_Data_402A40(3616, 0);
                field_E4_state = BoomMachineStates::eInactive_0;
            }
            break;
        default:
            return;
        }
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

    int field_D4_padding[4];
    BoomMachineStates field_E4_state;
    __int16 field_E6_padding;
    int field_E8_timer;
    __int16 field_EC_num_grenades;
    __int16 field_EE_pad;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzle, 0xF0);

void BoomMachine::VHandleButton()
{
    VHandleButton_41E6F0();
}

void BoomMachine::VHandleButton_41E6F0()
{
    if (VIsButtonOn_41E840())
    {
        if (field_EC_pNozzle->field_E4_state == BoomMachineStates::eInactive_0)
        {
            field_EC_pNozzle->field_E4_state = BoomMachineStates::eDropGrenadeAnimation_2;
            field_EC_pNozzle->field_E8_timer = gnFrameCount_507670 + 10;
        }
    }
    else
    {
        if (field_EC_pNozzle->field_E4_state == BoomMachineStates::eInactive_0)
        {
            field_EC_pNozzle->field_E4_state = BoomMachineStates::eAlreadyUsed_1;
            field_EC_pNozzle->field_E8_timer = gnFrameCount_507670 + 10;
        }
    }
}

BOOL BoomMachine::VIsButtonOn()
{
    return VIsButtonOn_41E840();
}

BOOL BoomMachine::VIsButtonOn_41E840()
{
    return field_E8_bIsButtonOn == 1;
}

void BoomMachine::VUpdate()
{
    VUpdate_41E750();
}

void BoomMachine::VUpdate_41E750()
{
    if (Event_Get_417250(kEventDeathReset_4))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (field_E8_bIsButtonOn == 0)
    {
        if (!gpThrowableArray_50E26C || gpThrowableArray_50E26C->field_10_count == 0)
        {
            field_E8_bIsButtonOn = 1;
            field_10_anim.Set_Animation_Data_402A40(1572, nullptr);
        }
    }
    else if (field_E8_bIsButtonOn == 1)
    {
        if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count > 0)
        {
            field_E8_bIsButtonOn = 0;
            field_10_anim.Set_Animation_Data_402A40(1600, nullptr);
        }

        if (field_10_anim.field_92_current_frame == 3)
        {
            SFX_Play_43AE60(SoundEffect::RedTick_4, 25, -1200, 0);
        }
    }
}

BaseGameObject* BoomMachine::dtor_41E670()
{
    SetVTable(this, 0x4BB008);

    if (field_EC_pNozzle)
    {
        field_EC_pNozzle->field_C_refCount--;
        field_EC_pNozzle->field_6_flags.Set(Options::eDead_Bit3);
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

    BYTE** ppRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kR1buttonResID, 1, 0);
    Animation_Init_417FD0(1600, 26, 17, ppRes, 1);

    field_CC_bApplyShadows &= ~1u;
    field_E4_tlvInfo = tlvInfo;
    field_10_anim.field_B_render_mode = TPageAbr::eBlend_1;

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

    auto pNozzle = ao_new<GrenadeMachineNozzle>();
    if (pNozzle)
    {
        FP directedScale = field_BC_sprite_scale;
        if (pTlv->field_1A_nozzle_side)
        {
            directedScale = -directedScale;
        }

        pNozzle->ctor_417C10();
        SetVTable(pNozzle, 0x4BAFD0);

        BYTE** ppNozzleRes = ResourceManager::GetLoadedResource_4554F0(ResourceManager::Resource_Animation, ResourceID::kR1bpipeResID, 1, 0);
        pNozzle->Animation_Init_417FD0(3616, 66, 36, ppNozzleRes, 1);

        pNozzle->field_10_anim.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        pNozzle->field_BC_sprite_scale = field_BC_sprite_scale;
        pNozzle->field_CC_bApplyShadows &= ~1u;
        pNozzle->field_E4_state = BoomMachineStates::eInactive_0;
        pNozzle->field_A8_xpos = field_A8_xpos + (directedScale * FP_FromInteger(30));
        pNozzle->field_AC_ypos = field_AC_ypos + (field_BC_sprite_scale * FP_FromInteger(-30));
        pNozzle->field_EC_num_grenades = static_cast<short>(pTlv->field_1E_number_of_grenades);
    }

    pNozzle->field_10_anim.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_1A_nozzle_side & 1);

    pNozzle->field_C_refCount++;
    field_EC_pNozzle = pNozzle;

    if (gpThrowableArray_50E26C && gpThrowableArray_50E26C->field_10_count)
    {
        field_E8_bIsButtonOn = 1;
        field_10_anim.Set_Animation_Data_402A40(1572, 0);
    }
    else
    {
        field_E8_bIsButtonOn = 0;
    }

    field_C8_yOffset = 0;
    return this;
}

BaseGameObject* BoomMachine::VDestructor(signed int flags)
{
    return Vdtor_41E850(flags);
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

}
