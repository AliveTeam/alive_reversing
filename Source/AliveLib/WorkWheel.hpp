#pragma once

#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

enum class WheelStates : __int16
{
    eIdle = 0,
    eTurning = 1,
};


struct WorkWheel_SaveState
{
    __int16 field_0_id;
    // pad
    int field_4_tlvInfo;
    __int16 field_8_snd_counter;
    // pad
    WheelStates field_C_state;
    __int16 field_E; // not used ?
};
ALIVE_ASSERT_SIZEOF_ALWAYS(WorkWheel_SaveState, 0x10);

struct Path_Wheel : public Path_TLV
{
    constexpr static int kType = 79;
    __int16 field_10_scale;
    __int16 field_12_id;
    __int16 field_14_duration;
    __int16 field_16_off_time;
    __int16 field_18_off_when_stopped;
    // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_Wheel, 0x1C);

class WorkWheel : public BaseAnimatedWithPhysicsGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual void VStartTurning();
    virtual void VStopTurning(__int16 bResetSwitch);

    EXPORT WorkWheel* ctor_4E35D0(Path_Wheel* pTlv, int tlvInfo);
private:
    EXPORT void dtor_4E3850();
    EXPORT WorkWheel* vdtor_4E3820(signed int flags);
public:
    EXPORT static int CC CreateFromSaveState_4E3B10(const BYTE* pState);
private:
    EXPORT int vGetSaveState_4E3C40(WorkWheel_SaveState* pState);
    EXPORT void vUpdate_4E38E0();
    EXPORT void vScreenChanged_4E3AD0();
    EXPORT void vStartTurning_4E3A20();
    EXPORT void vStopTurning_4E3A60(__int16 bResetSwitch);
private:
    int field_F4_tlv_info;
    __int16 field_F8_id;
    __int16 field_FA;
    WheelStates field_FC_state;
    __int16 field_FE_duration;
    unsigned __int16 field_100_on_counter;
    __int16 field_102_off_time;
    __int16 field_104_off_when_stopped;
    //__int16 field_106;
};
ALIVE_ASSERT_SIZEOF(WorkWheel, 0x108);
