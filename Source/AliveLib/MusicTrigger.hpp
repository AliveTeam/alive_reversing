#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Path.hpp"

struct Path_MusicTrigger : public Path_TLV
{
    __int16 field_10_type; // TODO: Enum
    short field_12_enabled_by; // TODO: Enum
    __int16 field_14_timer;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MusicTrigger, 0x18);

class MusicTrigger : public BaseGameObject
{
public:
    EXPORT BaseGameObject* ctor_47FE40(Path_MusicTrigger* pTlv, DWORD tlvInfo);
    EXPORT MusicTrigger* ctor_47FF10(__int16 type, __int16 enabledBy, int /*not_used*/, __int16 delay);
    EXPORT void Init_47FFB0(__int16 type, __int16 enabledBy, __int16 delay);
    EXPORT BaseGameObject* vdtor_47FEE0(signed int flags);
    EXPORT void dtor_4800C0();
    EXPORT void vScreenChange_4802A0();
    EXPORT void vUpdate_480140();

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    int field_20_tlvInfo;
    __int16 field_24_flags; // TODO: Recover flags
    __int16 field_26_music_type; // TODO: Enum
    int field_28_counter;
    PSX_Point field_2C_tl;
    PSX_Point field_30_br;
};
ALIVE_ASSERT_SIZEOF(MusicTrigger, 0x34);
