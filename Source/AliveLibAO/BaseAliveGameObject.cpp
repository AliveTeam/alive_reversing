#include "stdafx_ao.h"
#include "BaseAliveGameObject.hpp"
#include "Function.hpp"
#include "Map.hpp"
#include "Abe.hpp"

START_NS_AO

void BaseAliveGameObject_ForceLink() {}

FP CC ScaleToGridSize_41FA30(FP /*scale*/)
{
    NOT_IMPLEMENTED();
    return {};
}

ALIVE_VAR(1, 0x4FC8A0, DynamicArrayT<BaseAliveGameObject>*, gBaseAliveGameObjects_4FC8A0, nullptr);

BaseAliveGameObject *BaseAliveGameObject::ctor_401090()
{
    ctor_417C10();
    
    SetVTable(this, 0x4BA000);

    field_10A_flags.Clear(Flags_10A::e10A_Bit1_Can_Be_Possessed);
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
    field_FC_current_motion = 0;
    field_FE_next_state = 0;
    field_E4 = 0;
    field_E6_last_state = 0;
    field_E8_LastLineYPos = FP_FromInteger(0);
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

void BaseAliveGameObject::SetActiveCameraDelayedFromDir_401C90()
{
    if (sControlledCharacter_50767C == this)
    {
        switch (Is_In_Current_Camera_417CC0())
        {
        case CameraPos::eCamTop_1:
            if (field_B8_vely < FP_FromInteger(0))
            {
                gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapTop_2, this, -1);
            }
            break;

        case CameraPos::eCamBottom_2:
            if (field_B8_vely > FP_FromInteger(0))
            {
                gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapBottom_3, this, -1);
            }
            break;

        case CameraPos::eCamLeft_3:
            if (field_B4_velx < FP_FromInteger(0))
            {
                gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapLeft_0, this, -1);
            }
            break;

        case CameraPos::eCamRight_4:
            if (field_B4_velx > FP_FromInteger(0))
            {
                gMap_507BA8.SetActiveCameraDelayed_444CA0(Map::MapDirections::eMapRight_1, this, -1);
            }
            break;

        default:
            return;
        }
    }
}

END_NS_AO
