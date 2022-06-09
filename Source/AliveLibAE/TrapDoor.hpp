#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "PlatformBase.hpp"
#include "Path.hpp"

struct Path_TrapDoor final : public Path_TLV
{
    s16 field_10_switch_id;
    enum StartState : s16
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
    };
    StartState field_12_start_state;
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

struct TrapDoor_State final
{
    AETypes field_0_type;
    TrapDoorState field_2_state;
    s32 field_4_open_time;
    s32 field_8_tlvInfo;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(TrapDoor_State, 0xC);

class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo);
    ~TrapDoor();

    virtual void VUpdate() override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual PSX_RECT VGetBoundingRect(s32 pointIdx) override;
    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;

    static s32 CreateFromSaveState(const u8* pData);

private:
    void Add_To_Collisions_Array();
    void Open();

private:
    s16 field_12C_unused = 0;
    s32 field_130_stay_open_time2 = 0;
    s16 field_134_switch_id = 0;
    TrapDoorState field_136_state = TrapDoorState::eClosed_0;
    s16 field_138_switch_state = 0;
    s16 field_13A_xOff = 0;
    s16 field_13C_stay_open_time = 0;
    Choice_short field_13E_self_closing = Choice_short::eNo_0;
    FP field_140_x = {};
    FP field_144_y = {};
    PSX_RECT field_148_bounding_rect = {};
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);
