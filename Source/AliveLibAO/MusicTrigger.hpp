#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Map.hpp"
#include "MusicController.hpp"

namespace AO {

struct Path_MusicTrigger final : public Path_TLV
{
    s16 field_18_type;
    s16 field_1A_enabled_by;
    s16 field_1C_id;
    s16 field_1E_timer;
};
ALIVE_ASSERT_SIZEOF(Path_MusicTrigger, 0x20);


class MusicTrigger final : public BaseGameObject
{
public:
    EXPORT MusicTrigger* ctor_443A60(s16 type, s16 enabledBy, s32 id, s16 delay);

    EXPORT MusicTrigger* ctor_4439F0(Path_MusicTrigger* pTlv, s32 tlvInfo);

    EXPORT void Init_443AD0(s16 type, s16 enabledBy, u16 id, s16 delay);

    EXPORT BaseGameObject* dtor_443C20();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_443DD0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT MusicTrigger* Vdtor_443DF0(s32 flags);

    virtual void VUpdate() override;

    EXPORT void VUpdate_443C90();

    s32 field_10_tlvInfo;
    s16 field_14_flags;
    s16 field_16;
    s32 field_18_counter;
    MusicController::MusicTypes field_1C_music_type;
    u16 field_1E_id;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x20);

} // namespace AO
