#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_Slig;

class SligSpawner final : public BaseGameObject
{
public:
    EXPORT SligSpawner* ctor_402850(Path_Slig* pTlv, s32 tlvInfo);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    

private:
    EXPORT void VScreenChanged_402960();
    EXPORT void VUpdate_4028A0();
    EXPORT BaseGameObject* dtor_402970();
    EXPORT SligSpawner* VDtor_4029F0(s32 flags);

private:
    s32 field_10_tlvInfo;
    u16 field_14_slig_spawner_switch_id;
    s16 field_16_flags;
    Path_TLV field_18_tlv;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x30);

} // namespace AO
