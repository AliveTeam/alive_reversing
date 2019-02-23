#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_FootSwitch : public Path_TLV
{
    __int16 field_10_id;
    __int16 field_12_scale;
    __int16 field_14_action;
    __int16 field_16_trigger_by;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_FootSwitch, 0x18);

struct FootSwitch_Data
{
    int field_0_frameTableOffset;
    int field_4_frameTableOffset;
    __int16 field_8_maxH;
    __int16 field_A_frameTableOffset;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(FootSwitch_Data, 0xC);

class FootSwitch : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT FootSwitch* ctor_4DE090(Path_FootSwitch* pTlv, int tlvInfo);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

private:
    EXPORT FootSwitch* vdtor_4DE240(signed int flags);
    EXPORT void dtor_4DE670();
    EXPORT void vScreenChanged_4DE650();
    EXPORT void vUpdate_4DE270();

private:
    int field_E4_not_used[4];
    int field_F4_tlvInfo;
    __int16 field_F8_state;
    __int16 field_FA_id;
    __int16 field_FC_action;
    __int16 field_FE_trigger_by;
    int field_100_obj_id;
    __int16 field_104_bUnknown;
    __int16 field_106_bFindStander;
};
ALIVE_ASSERT_SIZEOF(FootSwitch, 0x108);
