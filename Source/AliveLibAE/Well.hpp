#pragma once

#include "BaseGameObject.hpp"
#include "FixedPoint.hpp"
#include "FunctionFwd.hpp"

struct Path_Well_Base;
struct Path_Well_Local;
struct Path_Well_Express;

class Well : public BaseGameObject
{
public:
    EXPORT Well* ctor_4E2BE0(Path_Well_Base* pTlv, FP xpos, FP ypos, int tlvInfo);
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
private:
    EXPORT void WellExpress_Init_4E2E00(Path_Well_Express* pTlv, FP /*xpos*/, FP ypos);
    EXPORT void WellLocal_Init_4E2CD0(Path_Well_Local* pTlv, FP /*xpos*/, FP ypos);
    EXPORT Well* vdtor_4E2CA0(signed int flags);
    EXPORT void dtor_4E3090();
    EXPORT void vScreenChanged_4E3070();
    EXPORT void vUpdate_4E2F60();
private:
    int field_20_tlvInfo;
    __int16 field_24_trigger_id;
    __int16 field_26_pad;
    FP field_28_scale;
    FP field_2C_exit_x;
    FP field_30_exit_y;
    FP field_34_leaf_xpos;
    FP field_38_leaf_ypos;
    __int16 field_3C_bEmitLeaves;
    __int16 field_3E_pad;
};
ALIVE_ASSERT_SIZEOF(Well, 0x40);
