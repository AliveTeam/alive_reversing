#pragma once

#include "../AliveLibCommon/BaseGameObjectCommon.hpp"

void SetIFields();

void BaseGameObjectCommon::InitFlags()
{
    field_flags->Clear(Options::eListAddFailed_Bit1);
    field_flags->Clear(Options::eDead_Bit3);
    field_flags->Clear(Options::eIsBaseAnimatedWithPhysicsObj_Bit5);
    field_flags->Clear(Options::eIsBaseAliveGameObject_Bit6);
    field_flags->Clear(Options::eCanExplode_Bit7);
    field_flags->Clear(Options::eInteractive_Bit8);
    field_flags->Clear(Options::eSurviveDeathReset_Bit9);
    field_flags->Clear(Options::eUpdateDuringCamSwap_Bit10);

    //NOTE 2: Was previously commented out. Hoping it was just a matter of precaution without side effects
    field_flags->Clear(Options::eCantKill_Bit11); // NOTE: AE clears this too

    field_flags->Set(Options::eUpdatable_Bit2);
}
