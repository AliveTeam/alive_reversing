#pragma once

#include "../AliveLibCommon/BitField.hpp"
class BaseGameObjectCommon
{
public:
    enum Options
    {
        // bit 01 = 0x001 = BaseGameObject couldn't add to list / it unsets inverse of 0xF80A / 0b1111100000001010
        eListAddFailed_Bit1 = 0x001,

        // bit 02 = 0x002 = do update?
        eUpdatable_Bit2 = 0x002,

        // bit 03 = 0x004 = dead
        eDead_Bit3 = 0x004,

        // bit 04 = 0x008 = render
        eDrawable_Bit4 = 0x008,

        // bit 05 = 0x010 = set by BaseAnimatedWithPhysicsGameObject
        eIsBaseAnimatedWithPhysicsObj_Bit5 = 0x010,

        // bit 06 = 0x020 = set by BaseAliveGameObject
        eIsBaseAliveGameObject_Bit6 = 0x020,

        // bit 07 = 0x040 = set by ChantSuppressor::ctor_466350 / MovingBomb::ctor_46FD40 - explodable?
        eCanExplode_Bit7 = 0x040,

        // bit 08 = 0x080 = set by Uxb::ctor_4DE9A0 = pressable?
        eInteractive_Bit8 = 0x080,

        // bit 09 = 0x100 = ?
        eSurviveDeathReset_Bit9 = 0x100,

        // bit 10 = 0x200 = still update when the camera is changing
        eUpdateDuringCamSwap_Bit10 = 0x200,

        // bit 11 = 0x400 = can never be removed
        eCantKill_Bit11 = 0x400
    };

    // field members interface
    BitField16<Options> *field_flags;
    s32 *field_update_delay;
    virtual void SetIFields() = 0;

    void InitFlags();
};
