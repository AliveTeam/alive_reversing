#pragma once

#include "FunctionFwd.hpp"
#include "PlatformBase.hpp"

class TrapDoor : public PlatformBase
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx) override;

    EXPORT BaseGameObject* vdtor_4DD8A0(signed int flags);
    EXPORT void vUpdate_4DDA90();
    EXPORT void vRender_4DDDD0(int **ot);
    EXPORT void vScreenChanged_4DDE40();
    EXPORT int vGetSaveState_4DE050(BYTE*);
    EXPORT PSX_RECT* vGetBoundingRect_4DD870(PSX_RECT* pRect, int /*not_used*/);

    virtual void VAdd(BaseAliveGameObject* pObj) override;

    virtual void VRemove(BaseAliveGameObject* pObj) override;
private:

    EXPORT void vAdd_4DDD90(BaseAliveGameObject* pObj);
    EXPORT void vRemove_4DDDB0(BaseAliveGameObject* pObj);

    EXPORT void Add_To_Collisions_Array_4DDA20();

    EXPORT void Open_4DD960();

    EXPORT void dtor_4DD8D0();

private:
    __int16 field_12C;
    __int16 field_12E;
    int field_130_stay_open_time2;
    __int16 field_134_switch_idx;
    __int16 field_136_state;
    __int16 field_138_switch_state;
    __int16 field_13A_xOff;
    __int16 field_13C_stay_open_time;
    __int16 field_13E_set_switch_on_dead;
    FP field_140_x;
    FP field_144_y;
    PSX_RECT field_148_bounding_rect;
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);
