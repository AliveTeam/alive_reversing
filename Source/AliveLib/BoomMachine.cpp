#include "stdafx.h"
#include "BoomMachine.hpp"
#include "Function.hpp"
#include "ObjectIds.hpp"
#include "stdlib.hpp"
#include "ThrowableArray.hpp"
#include "Game.hpp"

class GrenadeMachineNozzel : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT GrenadeMachineNozzel* ctor_4456F0(FP xpos, FP ypos, FP scale, __int16 numGrenades)
    {
        BaseAnimatedWithPhysicsGameObject_ctor_424930(0);
        SetVTable(this, 0x54557C);
        BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6009);
        Animation_Init_424E10(3700, 67, 36u, ppRes, 1, 1);

        field_20_animation.field_4_flags.Clear(AnimFlags::eBit15_bSemiTrans);
        field_DC_bApplyShadows &= ~1u;

        field_CC_sprite_scale = scale;

        field_B8_xpos = xpos;
        field_BC_ypos = ypos;

        field_FC_numGrenades = numGrenades;

        field_F4_state = 0;

        return this;
    }

private:
    int field_E4_not_used[4];
    __int16 field_F4_state;
    __int16 field_F6_pad;
    int field_F8_timer;
    __int16 field_FC_numGrenades;
    __int16 field_FE_pad;
};
ALIVE_ASSERT_SIZEOF(GrenadeMachineNozzel, 0x100);

BoomMachine* BoomMachine::ctor_445B30(Path_BoomMachine* pTlv, int tlvInfo)
{
    BaseAnimatedWithPhysicsGameObject_ctor_424930(0);

    SetVTable(this, 0x5455C4);
    field_4_typeId = Types::eGrenadeMachine_66;

    BYTE** ppRes = Add_Resource_4DC130(ResourceManager::Resource_Animation, 6008);
    Animation_Init_424E10(1736, 26, 17u, ppRes, 1, 1);
    
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

    auto pNozzel = alive_new<GrenadeMachineNozzel>();
    if (pNozzel)
    {
        pNozzel->ctor_4456F0(
            ((pTlv->field_12_nozzel_side ? -field_CC_sprite_scale : field_CC_sprite_scale) * FP_FromInteger(30)) + field_B8_xpos,
            (field_CC_sprite_scale * FP_FromInteger(-30)) + field_BC_ypos,
            field_CC_sprite_scale,
            pTlv->field_16_number_of_grenades);

        pNozzel->field_20_animation.field_4_flags.Set(AnimFlags::eBit5_FlipX, pTlv->field_12_nozzel_side & 1);
        field_F8_nozzel_id = pNozzel->field_8_object_id;
    }

    if (gpThrowableArray_5D1E2C && gpThrowableArray_5D1E2C->field_20_count)
    {
        field_FC = 1;
        field_20_animation.Set_Animation_Data_409C80(1708, nullptr);
    }
    else
    {
        field_FC = 0;
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

BOOL BoomMachine::Vsub_445DF0()
{
    return vsub_445DF0();
}

BaseGameObject* BoomMachine::Vsub_445F00()
{
    return vsub_445F00();
}

void BoomMachine::vUpdate_445F50()
{
    NOT_IMPLEMENTED();
}

void BoomMachine::vScreenChange_446020()
{
    field_6_flags.Set(BaseGameObject::eDead);
}

BOOL BoomMachine::vsub_445DF0()
{
    NOT_IMPLEMENTED();
    return 0;
}

BaseGameObject* BoomMachine::vsub_445F00()
{
    NOT_IMPLEMENTED();
    return nullptr;
}

void BoomMachine::dtor_445E40()
{
    SetVTable(this, 0x5455C4); // vTbl_GrenadeMachine_5455C4

    BaseGameObject* pObj = sObjectIds_5C1B70.Find_449CF0(field_F8_nozzel_id);
    if (pObj)
    {
        pObj->field_6_flags.Set(BaseGameObject::eDead);;
    }
    Path::TLV_Reset_4DB8E0(field_F4_tlvInfo, -1, 0, 0);

    BaseAnimatedWithPhysicsGameObject_dtor_424AD0();
}

BoomMachine* BoomMachine::vdtor_445E10(signed int flags)
{
    dtor_445E40();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
    return this;
}

