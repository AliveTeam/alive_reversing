#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_Slig;

class SligSpawner : public BaseGameObject
{
public:
    EXPORT SligSpawner* ctor_402850(Path_Slig* pTlv, int tlvInfo);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;

private:
    EXPORT void VScreenChanged_402960();
    EXPORT void VUpdate_4028A0();
    EXPORT BaseGameObject* dtor_402970();
    EXPORT SligSpawner* VDtor_4029F0(signed int flags);

private:
    int field_10_tlvInfo;
    unsigned __int16 field_14_slig_id;
    __int16 field_16_flags;
    Path_TLV field_18_tlv;
};
ALIVE_ASSERT_SIZEOF(SligSpawner, 0x30);

END_NS_AO

