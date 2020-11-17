#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Map.hpp"
#include "MusicController.hpp"

namespace AO {

struct Path_MusicTrigger : public Path_TLV
{
    __int16 field_18_type;
    __int16 field_1A_enabled_by;
    __int16 field_1C_id;
    __int16 field_1E_timer;
};
ALIVE_ASSERT_SIZEOF(Path_MusicTrigger, 0x20);


class MusicTrigger : public BaseGameObject
{
public:
    EXPORT MusicTrigger* ctor_443A60(__int16 type, __int16 enabledBy, int id, __int16 delay);

    EXPORT MusicTrigger* ctor_4439F0(Path_MusicTrigger* pTlv, int tlvInfo);

    EXPORT void Init_443AD0(__int16 type, __int16 enabledBy, unsigned __int16 id, __int16 delay);

    EXPORT BaseGameObject* dtor_443C20();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_443DD0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MusicTrigger* Vdtor_443DF0(signed int flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_443C90();

    int field_10_tlvInfo;
    __int16 field_14_flags;
    __int16 field_16;
    int field_18_counter;
    MusicController::MusicTypes field_1C_music_type;
    unsigned __int16 field_1E_id;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x20);

}

