#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "Map.hpp"

namespace AO {

struct Path_Slig;

class SligSpawner final : public BaseGameObject
{
public:
    SligSpawner(Path_Slig* pTlv, s32 tlvInfo);
    ~SligSpawner();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    

private:
    void VScreenChanged_402960();
    void VUpdate_4028A0();

private:
    s32 field_10_tlvInfo;
    u16 field_14_slig_spawner_switch_id;
    s16 field_16_flags;
    Path_TLV field_18_tlv;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x30);

} // namespace AO
