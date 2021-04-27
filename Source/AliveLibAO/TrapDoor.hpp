#pragma once

#include "FunctionFwd.hpp"
#include "Map.hpp"
#include "PlatformBase.hpp"

namespace AO {

struct Path_TrapDoor : public Path_TLV
{
    s16 field_18_id;
    s16 field_1A_start_state;
    s16 field_1C_self_closing;
    s16 field_1E_scale;
    s16 field_20_dest_level;
    s16 field_22_direction;
    s16 field_24_anim_offset;
    s16 field_26_pad;
};
ALIVE_ASSERT_SIZEOF(Path_TrapDoor, 0x28);

class PathLine;

class TrapDoor : public PlatformBase
{
public:
    EXPORT TrapDoor* ctor_488010(Path_TrapDoor* pTlv, Map* pMap, s32 tlvInfo);

    void Open();

    EXPORT BaseGameObject* dtor_4882A0();

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT TrapDoor* Vdtor_4887D0(s32 flags);

    virtual PSX_RECT* VGetBoundingRect(PSX_RECT* pRect, s32 pointIdx) override;

    EXPORT PSX_RECT* VGetBoundingRect_4887B0(PSX_RECT* pRect, s32 pointIdx);

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_488740();

    EXPORT void VAdd_4886D0(BaseAliveGameObject* pObj);

    virtual void VAdd(BaseAliveGameObject* pObj) override;

    EXPORT void VRemove_4886E0(BaseAliveGameObject* pObj);

    virtual void VRemove(BaseAliveGameObject* pObj) override;

    virtual void VRender(PrimHeader** ppOt) override;

    EXPORT void VRender_4886F0(PrimHeader** ppOt);

    virtual void VUpdate() override;

    EXPORT void VUpdate_4883E0();



    s16 field_12C_flag;
    s16 field_12E_pad;
    s32 field_130_stay_open_time;
    s16 field_134_switch_idx;
    s16 field_136_state;
    s16 field_138_switch_state;
    s16 field_13A_xOff;
    s16 field_13C_set_switch_on_dead;
    s16 field_13E_pad;
    FP field_140_x;
    FP field_144_y;
    PSX_RECT field_148_bounding_rect;
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);


} // namespace AO
