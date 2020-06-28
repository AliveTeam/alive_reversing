#include "stdafx_ao.h"
#include "Function.hpp"
#include "MusicTrigger.hpp"
#include "SwitchStates.hpp"
#include "stdlib.hpp"

START_NS_AO

MusicTrigger* MusicTrigger::ctor_443A60(__int16 type, __int16 enabledBy, int id, __int16 delay)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBBC0);
    Init_443AD0(type, enabledBy, static_cast<short>(id), delay);
    field_10_tlvInfo = -1;
    return this;
}

MusicTrigger* MusicTrigger::ctor_4439F0(Path_MusicTrigger* pTlv, int tlvInfo)
{
    ctor_487E10(1);
    SetVTable(this, 0x4BBBC0);
    Init_443AD0(pTlv->field_18_type, pTlv->field_1A_enabled_by, pTlv->field_1C_id, pTlv->field_1E_timer);
    field_10_tlvInfo = tlvInfo;
    return this;
}

void MusicTrigger::Init_443AD0(__int16 type, __int16 enabledBy, unsigned __int16 id, __int16 delay)
{
    field_14_flags &= ~7u;
    field_4_typeId = Types::eNone_0;

    switch (type)
    {
    case 0:
        field_1C_music_type = MusicController::MusicTypes::eType3;
        field_18_counter = 400;
        break;

    case 1:
        field_1C_music_type = MusicController::MusicTypes::eType13;
        field_18_counter = 30;
        break;

    case 2:
        field_1C_music_type = MusicController::MusicTypes::eType16;
        field_18_counter = 30;
        break;

    case 3:
        field_1C_music_type = MusicController::MusicTypes::eType5;
        field_14_flags |= 4;
        break;

    case 4:
        field_1C_music_type = MusicController::MusicTypes::eType8;
        field_14_flags |= 4;
        break;

    case 5:
        field_1C_music_type = MusicController::MusicTypes::eType2;
        field_18_counter = delay;
        break;

    case 6:
        field_1C_music_type = MusicController::MusicTypes::eType15;
        field_18_counter = 30;
        break;

    default:
        break;
    }

    switch (enabledBy)
    {
    case 0:
        field_8_update_delay = 0;
        break;

    case 1:
        field_8_update_delay = delay;
        break;

    case 2:  // removed in AE
        field_14_flags |= 1u;
        field_1E_id = id;
        field_8_update_delay = 0;
        field_18_counter = delay;
        if (id > 1)
        {
            if (SwitchStates_Get(id))
            {
                field_6_flags.Set(BaseGameObject::eDead_Bit3);
            }
        }
        break;

    case 3: // removed in AE
        field_14_flags |= 1u;
        field_1E_id = id;
        field_8_update_delay = 0;
        field_18_counter = -1;
        break;

    default:
        return;
    }
}

BaseGameObject* MusicTrigger::dtor_443C20()
{
    SetVTable(this, 0x4BBBC0);
    if (field_14_flags & 4)
    {
        MusicController::sub_443810(0, this, 0, 0);
    }
    return dtor_487DF0();
}


void MusicTrigger::VScreenChanged()
{
    VScreenChanged_443DD0();
}

void MusicTrigger::VScreenChanged_443DD0()
{
    if (gMap_507BA8.field_0_current_level != gMap_507BA8.field_A_level)
    {
        field_6_flags.Set(BaseGameObject::eDead_Bit3);
    }
}

BaseGameObject* MusicTrigger::VDestructor(signed int flags)
{
    return Vdtor_443DF0(flags);
}

MusicTrigger* MusicTrigger::Vdtor_443DF0(signed int flags)
{
    dtor_443C20();
    if (flags & 1)
    {
        ao_delete_free_447540(this);
    }
    return this;
}

void MusicTrigger::VUpdate()
{
    VUpdate_443C90();
}

void MusicTrigger::VUpdate_443C90()
{
    NOT_IMPLEMENTED();
}

END_NS_AO
