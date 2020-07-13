#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Font.hpp"
#include "Map.hpp"

START_NS_AO

struct Path_StatusBoard : public Path_TLV
{

};

class StatsSign : public BaseGameObject
{
public:
    EXPORT StatsSign* ctor_4418E0(Path_StatusBoard* pTlv, int tlvInfo);

    EXPORT BaseGameObject* dtor_4419E0();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT StatsSign* Vdtor_441C80(signed int flags);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_441C70();

    virtual void VUpdate() override;

    EXPORT void VUpdate_441A90();

    FontContext field_10_fontContext;
    AliveFont field_20_font1;
    AliveFont field_58_font2;
    AliveFont field_90_font3;
    int field_C8_tlv;
    __int16 field_CC_xpos;
    __int16 field_CE_ypos;
};
ALIVE_ASSERT_SIZEOF(StatsSign, 0xD0);

END_NS_AO

