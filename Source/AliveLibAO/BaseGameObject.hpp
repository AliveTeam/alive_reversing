#pragma once

#include "FunctionFwd.hpp"
#include "BitField.hpp"
#include "DynamicArray.hpp"

void AliveLibAO_ForceLink();

EXPORT void CC Remove_Item_417350(void*);

START_NS_AO


enum class Types : __int16
{
    eNone_0 = 0,
    eAlarm_1 = 1,

    eBat_6 = 6,
    eBellHammer_27 = 27,

    eRollingBallStopperShaker_58 = 58,
    eRollingBallStopper_60 = 60,
    eEffectBase_79 = 79,
    
    eSlig_88 = 88,

    eSligSpawner_91 = 91,
    eZBall_92 = 92,
};

class BaseGameObject
{
public:
    enum Options
    {
        // TODO: Check as objects are reversed - assumed to be the same as AE for now
        eListAddFailed_Bit1 = 0x001,
        eUpdatable_Bit2 = 0x002,
        eDead_Bit3 = 0x004,
        eDrawable_Bit4 = 0x008,
        eIsBaseAnimatedWithPhysicsObj_Bit5 = 0x010,
        eIsBaseAliveGameObject_Bit6 = 0x020,
        eCanExplode_Bit7 = 0x040,
        eInteractive_Bit8 = 0x080,
        eSurviveDeathReset_Bit9 = 0x100,
        eUpdateDuringCamSwap_Bit10 = 0x200,
        eCantKill_Bit11 = 0x400
    };

    EXPORT BaseGameObject* ctor_487E10(__int16 arraySize);
    EXPORT BaseGameObject* dtor_487DF0();

    virtual BaseGameObject* VDestructor(signed int flags) = 0;

    virtual void VUpdate()
    {

    }

    virtual void VRender(int**)
    {

    }

    virtual void VScreenChanged()
    {

    }

    virtual void VUnknown()
    {

    }

public:
    Types field_4_typeId;
    BitField16<Options> field_6_flags;
    int field_8_update_delay;
    char field_C_bCanKill;
    char field_D; // pad ?
    __int16 field_E; // ??
};
ALIVE_ASSERT_SIZEOF(BaseGameObject, 0x10);

ALIVE_VAR_EXTERN(DynamicArrayT<BaseGameObject>*, gBaseGameObject_list_9F2DF0);

END_NS_AO
