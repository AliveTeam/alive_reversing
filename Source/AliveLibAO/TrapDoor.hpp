#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "Map.hpp"
#include "PlatformBase.hpp"
#include "../AliveLibAE/Path.hpp"

namespace AO {

enum class TrapDoorState : s16
{
    eClosed_0 = 0,
    eOpening_1 = 1,
    eOpen_2 = 2,
    eClosing_3 = 3,
};

struct Path_TrapDoor final : public Path_TLV
{
    s16 field_18_switch_id;
    enum StartState : s16
    {
        eOpen_0 = 0,
        eClosed_1 = 1,
    };
    StartState field_1A_start_state;
    Choice_short field_1C_self_closing;
    Scale_short field_1E_scale;
    s16 field_20_dest_level;
    XDirection_short field_22_direction;
    s16 field_24_xOff;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_TrapDoor, 0x28);

class PathLine;

class TrapDoor final : public PlatformBase
{
public:
    TrapDoor(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo);
    ~TrapDoor();

    void Open();

    virtual PSX_RECT* VGetBoundingRect(PSX_RECT* pRect, s32 pointIdx) override;
    virtual void VScreenChanged() override;
    virtual void VAdd(BaseAliveGameObject* pObj) override;
    virtual void VRemove(BaseAliveGameObject* pObj) override;
    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override;

    s16 field_12C_flag;
    s16 field_12E_pad;
    s32 field_130_stay_open_time;
    s16 field_134_switch_id;
    TrapDoorState field_136_state;
    s16 field_138_switch_state;
    s16 field_13A_xOff;
    Choice_short field_13C_self_closing;
    s16 field_13E_pad;
    FP field_140_x;
    FP field_144_y;
    PSX_RECT field_148_bounding_rect;
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);


} // namespace AO
