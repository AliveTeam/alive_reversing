#include "stdafx.h"
#include "BoomMachine.hpp"
#include "Function.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"
#include "Events.hpp"
#include "Sfx.hpp"
#include "Throwable.hpp"
#include "Grenade.hpp"

class GrenadeMachineNozzle : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT GrenadeMachineNozzle* ctor_4456F0(FP xpos, FP ypos, FP scale, __int16 numGrenades)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x54557C);
		const AnimRecord& rec = AnimRec(AnimId::Grenade_Machine_Nozzle);
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
        Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_DC_bApplyShadows &= ~1u;

        field_CC_sprite_scale = scale;

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        field_FC_numGrenades = numGrenades;

        field_F4_state = BoomMachineStates::eInactive_0;

        return this;
    }

    EXPORT void DropGrenadeAnimation_445820()
    {
        if (field_F4_state == BoomMachineStates::eInactive_0)
        {
            field_F4_state = BoomMachineStates::eDropGrenadeAnimation_2;
            field_F8_timer = sGnFrame_5C1B84 + 10;
        }
    }

    EXPORT void AlreadyUsed_445860()
    {
        if (field_F4_state == BoomMachineStates::eInactive_0)
        {
            field_F4_state = BoomMachineStates::eAlreadyUsed_1;
            field_F8_timer = sGnFrame_5C1B84 + 10;
        }
    }

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_4457D0(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_4458A0();
    }

private:
    GrenadeMachineNozzle* vdtor_4457D0(signed int flags)
    {
        BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
        if (flags & 1)
        {
            ae_delete_free_495540(this);
        }
        return this;
    }

    EXPORT void vUpdate_4458A0()
    {
        switch (field_F4_state)
        {
        case BoomMachineStates::eAlreadyUsed_1:
            if (static_cast<int>(sGnFrame_5C1B84) > field_F8_timer)
            {
                SFX_Play_46FBA0(SoundEffect::ZPop_4, 60, -1800);
                field_F4_state = BoomMachineStates::eInactive_0;
            }
            break;

        case BoomMachineStates::eDropGrenadeAnimation_2:
            if (static_cast<int>(sGnFrame_5C1B84) > field_F8_timer)
            {
                field_F4_state = BoomMachineStates::eDropGrenade_3;
                const AnimRecord& animRec = AnimRec(AnimId::Grenade_Machine_Nozzle_Drop_Grenade);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
            }
            break;

        case BoomMachineStates::eDropGrenade_3:
            if (field_20_animation.field_4_flags.Get(AnimFlags::eBit12_ForwardLoopCompleted))
            {
                SFX_Play_46FBA0(SoundEffect::PickupItem_28, 127, -900);

                if (!gpThrowableArray_5D1E2C)
                {
                    auto pNewArray = ae_new<ThrowableArray>();
                    if (pNewArray)
                    {
                        gpThrowableArray_5D1E2C = pNewArray->ctor_49A630();
                    }
                }

                gpThrowableArray_5D1E2C->Add_49A7A0(field_FC_numGrenades);

                auto pGrenade = ae_new<Grenade>();
                if (pGrenade)
                {
                    FP directedScale = {};
                    if (field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX))
                    {
                        directedScale = -field_CC_sprite_scale;
                    }
                    else
                    {
                        directedScale = field_CC_sprite_scale;
                    }

                    pGrenade->ctor_447F70(
                        (FP_FromInteger(6) * directedScale) + field_B8_xpos,
                        (-FP_FromInteger(6) * field_CC_sprite_scale) + field_BC_ypos,
                        field_FC_numGrenades,
                        0,
                        0,
                        0);
                }

                pGrenade->VThrow_49E460((field_20_animation.field_4_flags.Get(AnimFlags::eBit5_FlipX)) != 0 ? -FP_FromDouble(0.75) : FP_FromDouble(0.75), FP_FromInteger(3));

                const AnimRecord& animRec = AnimRec(AnimId::Grenade_Machine_Nozzle);
                field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
                field_F4_state = BoomMachineStates::eInactive_0;
            }
            break;
        }
    }

private:
    BoomMachineStates field_F4_state;
    __int16 field_F6_pad;
    int field_F8_timer;
    __int16 field_FC_numGrenades;
    __int16 field_FE_pad;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzle, 0x100);

BoomMachine* BoomMachine::ctor_445B30(Path_BoomMachine* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x5455C4);
    field_4_typeId = Types::eGrenadeMachine_66;

    const AnimRecord& rec = AnimRec(AnimId::Grenade_Machine_Button_Off);
    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, rec.mResourceId);
    Animation_Init_424E10(rec.mFrameTableOffset, rec.mMaxW, rec.mMaxH, ppRes, 1, 1);

    field_DC_bApplyShadows &= ~1u;
    field_F4_tlvInfo = tlvInfo;
    field_20_animation.field_B_render_mode = 1;

    if (pTlv->field_10_scale == 1)
    {
        field_CC_sprite_scale = FP_FromDouble(0.5);
    }    
    else if (pTlv->field_10_scale == 0)
    {
        field_CC_sprite_scale = FP_FromInteger(1);
    }

    field_B8_xpos = (ScaleToGridSize_4498B0(field_CC_sprite_scale) / FP_FromInteger(2))  + FP_FromInteger(pTlv->field_8_top_left.field_0_x);
    field_BC_ypos = FP_FromInteger(pTlv->field_8_top_left.field_2_y);

    auto pNozzle = ae_new<GrenadeMachineNozzle>();
    if (pNozzle)
    {
        pNozzle->ctor_4456F0(
            ((pTlv->field_12_nozzle_side ? -field_CC_sprite_scale : field_CC_sprite_scale) * FP_FromInteger(30)) + field_B8_xpos,
            (field_CC_sprite_scale * FP_FromInteger(-30)) + field_BC_ypos,
            field_CC_sprite_scale,
            pTlv->field_16_number_of_grenades);

        pNozzle->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_nozzle_side & 1);
        field_F8_nozzle_id = pNozzle->field_8_object_id;
    }

    if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count)
    {
        field_FC_bIsButtonOn = 1;
        const AnimRecord& animRec = AnimRec(AnimId::Grenade_Machine_Button_On);
        field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
    }
    else
    {
        field_FC_bIsButtonOn = 0;
    }
    return this;
}

BaseGameObject* BoomMachine::VDestructor(signed int flags)
{
    return vdtor_445E10(flags);
}

void BoomMachine::VUpdate()
{
    vUpdate_445F50();
}

void BoomMachine::VScreenChanged()
{
    vScreenChange_446020();
}

BOOL BoomMachine::VIsButtonOn_445DF0()
{
    return vIsButtonOn_445DF0();
}

void BoomMachine::VHandleButton_445F00()
{
    vHandleButton_445F00();
}

void BoomMachine::vUpdate_445F50()
{
    if (Event_Get_422C00(kEventDeathReset))
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }

    if (!field_FC_bIsButtonOn)
    {
        if (!gpThrowableArray_5D1E2C || gpThrowableArray_5D1E2C->field_20_count == 0)
        {
            field_FC_bIsButtonOn = 1;
            const AnimRecord& animRec = AnimRec(AnimId::Grenade_Machine_Button_On);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        }
    }
    else if (field_FC_bIsButtonOn)
    {
        if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count > 0)
        {
            field_FC_bIsButtonOn = 0;
            const AnimRecord& animRec = AnimRec(AnimId::Grenade_Machine_Button_Off);
            field_20_animation.Set_Animation_Data_409C80(animRec.mFrameTableOffset, nullptr);
        }

        if (field_20_animation.field_92_current_frame == 3)
        {
            SFX_Play_46FBA0(SoundEffect::RedTick_3, 25, -1200);
        }
    }
}

void BoomMachine::vScreenChange_446020()
{
    field_6_flags.Set(BaseGameObject::eDead_Bit3);
}

BOOL BoomMachine::vIsButtonOn_445DF0()
{
    return field_FC_bIsButtonOn == 1;
}

void BoomMachine::vHandleButton_445F00()
{
    auto pNozzle = static_cast<GrenadeMachineNozzle*>(sObjectIds_5C1B70.Find_449CF0(field_F8_nozzle_id));
    if (pNozzle)
    {
        if (VIsButtonOn_445DF0())
        {
            pNozzle->DropGrenadeAnimation_445820();
        }
        else
        {
            pNozzle->AlreadyUsed_445860();
        }
    }
}

void BoomMachine::dtor_445E40()
{
    SetVTable(this, 0x5455C4); // vTbl_GrenadeMachine_5455C4

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_F8_nozzle_id);
    if (pObj)
    {
        pObj->field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BoomMachine* BoomMachine::vdtor_445E10(signed int flags)
{
    dtor_445E40();
    if (flags & 1)
    {
        ae_delete_free_495540(this);
    }
    return this;
}

