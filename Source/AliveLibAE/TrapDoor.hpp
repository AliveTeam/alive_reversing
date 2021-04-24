#pragma once

#include "FunctionFwd.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

struct Path_TrapDoor : public Path_TLV
{
    s16 field_10_id;
    s16 field_12_start_state;
    Choice_short field_14_self_closing;
    Scale_short field_16_scale;
    s16 field_18_dest_level;
    XDirection_short field_1A_direction;
    s16 field_1C_xOff;
    s16 field_1E_stay_open_time;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TrapDoor, 0x20);

enum class TrapDoorState : s16
{
    eClosed_0 = 0,
    eOpening_1 = 1,
    eOpen_2 = 2,
    eClosing_3 = 3,
};

struct TrapDoor_State
{
    AETypes field_0_type;
    TrapDoorState field_2_state;
    int field_4_open_time;
    int field_8_tlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TrapDoor_State, 0xC);

class TrapDoor : public PlatformBase
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx) override;

    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;

    EXPORT TrapDoor* ctor_4DD570(Path_TrapDoor* pTlv, Map* pMap, int tlvInfo);
    EXPORT static signed int CC CreateFromSaveState_4DDED0(const BYTE* pData);

private:
    EXPORT BaseGameObject* vdtor_4DD8A0(signed int flags);
    EXPORT void vUpdate_4DDA90();
    EXPORT void vRender_4DDDD0(PrimHeader** ppOt);
    EXPORT void vScreenChanged_4DDE40();
    EXPORT int vGetSaveState_4DE050(TrapDoor_State* pState);
    EXPORT PSX_RECT* vGetBoundingRect_4DD870(PSX_RECT* pRect, int /*not_used*/);

private:

    EXPORT void vAdd_4DDD90(BaseAliveGameObject* pObj);
    EXPORT void vRemove_4DDDB0(BaseAliveGameObject* pObj);

    EXPORT void Add_To_Collisions_Array_4DDA20();

    EXPORT void Open_4DD960();

    EXPORT void dtor_4DD8D0();

private:
    s16 field_12C_unused;
    s16 field_12E_padding;
    int field_130_stay_open_time2;
    s16 field_134_switch_idx;
    TrapDoorState field_136_state;
    s16 field_138_switch_state;
    s16 field_13A_xOff;
    s16 field_13C_stay_open_time;
    Choice_short field_13E_self_closing;
    FP field_140_x;
    FP field_144_y;
    PSX_RECT field_148_bounding_rect;
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);
