#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Function.hpp"

START_NS_AO

void BaseAliveGameObject_ForceLink() {}

FP CC Grid_Scale_41FA30(FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return {};
}

ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

BaseAliveGameObject *BaseAliveGameObject::ctor_401090()
{
    ctor_417C10();
    
    SetVTable(this, 0x4BA000);

    field_10A_flags.Clear(Flags_10A::e10A_Bit1);
    field_10A_flags.Clear(Flags_10A::e10A_Bit2);
    field_10A_flags.Clear(Flags_10A::e10A_Bit3);
    field_10A_flags.Clear(Flags_10A::e10A_Bit4);
    field_10A_flags.Clear(Flags_10A::e10A_Bit5);
    field_10A_flags.Clear(Flags_10A::e10A_Bit6);

    field_F0_pTlv = nullptr;
    field_F8_pLiftPoint = nullptr;
    field_F4_pLine = nullptr;
    field_100_health = FP_FromInteger(1);
    field_106_shot = 0;
    field_108_bMotionChanged = 0;
    field_EC_oldY = 0;
    field_FC_state = 0;
    field_FE_next_state = 0;
    field_E4 = 0;
    field_E6_last_state = 0;
    field_E8_last_frame = 0;
    field_104_pending_resource_count = 0;
    gBaseAliveGameObjects_4FC8A0->Push_Back(this);
    field_6_flags.Set(Options::eIsBaseAliveGameObject_Bit6);
    return this;
}


BaseAliveGameObject* BaseAliveGameObject::dtor_401000()
{
    NOT_IMPLEMENTED();
    return this;
}

void BaseAliveGameObject::VCheckCollisionLineStillValid_401A90(int /*distance*/)
{
    NOT_IMPLEMENTED();
}

__int16 BaseAliveGameObject::MapFollowMe_401D30(__int16 /*snapToGrid*/)
{
    NOT_IMPLEMENTED();
    return 0;
}

END_NS_AO

