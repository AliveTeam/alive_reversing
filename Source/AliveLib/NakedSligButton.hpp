#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

enum class SwitchOp : __int16;

struct Path_NakedSligButton : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_id;
    SwitchOp field_14_id_action;
    __int16 field_16_on_sound;
    __int16 field_18_off_sound;
    __int16 field_1A_sound_direction;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_NakedSligButton, 0x1C);

class NakedSligButton : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT NakedSligButton* ctor_4148F0(Path_NakedSligButton* pTlv, int tlvInfo);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    EXPORT void UseButton_414C60();
private:
    EXPORT NakedSligButton* vdtor_414A60(signed int flags);
    EXPORT void dtor_414A90();
    EXPORT void vUpdate_414B20();
private:
    int field_F4_tlvInfo;
    __int16 field_F8_id;
    SwitchOp field_FA_id_action;
    __int16 field_FC_on_sound;
    __int16 field_FE_off_sound;
    __int16 field_100_sound_direction;
    __int16 field_102_in_use;
};
ALIVE_ASSERT_SIZEOF(NakedSligButton, 0x104);
