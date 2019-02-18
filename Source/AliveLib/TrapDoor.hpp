#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

class TrapDoor : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual PSX_RECT* vGetBoundingRect_424FD0(PSX_RECT* pRect, int pointIdx) override;

    EXPORT BaseGameObject* vdtor_4DD8A0(signed int /*flags*/);
    EXPORT void vUpdate_4DDA90();
    EXPORT void vRender_4DDDD0(int **ot);
    EXPORT void vScreenChanged_4DDE40();
    EXPORT int vGetSaveState_4DE050(BYTE*);
    EXPORT PSX_RECT* vGetBoundingRect_4DD870(PSX_RECT* pRect, int /*not_used*/);

    // New virtuals
    virtual int Vsub_4DDD90(int a1);
    virtual void Vsub_4DDDB0(BaseGameObject* a1);

    EXPORT int sub_4DDD90(int /*a1*/);
    EXPORT void sub_4DDDB0(BaseGameObject* /*a1*/);
private:
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    PathLine* field_124_pTrapDoorLine;
    int field_128_pTlv;
public:
    __int16 field_12C;
private:
    __int16 field_12E;
    int field_130;
    __int16 field_134_switch_idx;
    __int16 field_136_state;
    __int16 field_138_switch_state;
    __int16 field_13A_xOff;
    __int16 field_13C;
    __int16 field_13E_set_switch_on_dead;
    int field_140;
    int field_144;
    PSX_RECT field_148_bounding_rect;
};
ALIVE_ASSERT_SIZEOF(TrapDoor, 0x150);
