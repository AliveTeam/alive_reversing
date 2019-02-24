#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "Factory.hpp"
#include "Animation.hpp"

void TimedMine_ForceLink();

struct Path_TimedMine
{
    Path_TLV field_0_mBase;
    __int16 field_10_id;
    __int16 field_12_state;
    __int16 field_14_scale;
    __int16 field_16_timeout;
    int field_18_disabled_resources;
};
ALIVE_ASSERT_SIZEOF(Path_TimedMine, 0x1C);

enum TimedMine_Flags_1C4
{
    e1C4_Bit0 = 0x1,
    e1C4_Bit1 = 0x2,
};

class TimedMine : public BaseAliveGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;

    EXPORT TimedMine* ctor_410600(Path_TimedMine *pPath, TlvItemInfoUnion tlv);
    EXPORT void Update_410A80();
    EXPORT void Render_410CD0(int ** pOt);

    EXPORT void InitBlinkAnimation_4108E0(Animation *pAnim);
    EXPORT void sub_411100();
public:
    BYTE gap116[2];
    WORD field_118_armed;
    WORD field_11A_explode_timeout;
    DWORD field_11C_tlv;
    unsigned int field_120_gnframe;
    Animation field_124_animation;
    unsigned int field_1BC;
    unsigned int field_1C0;
    BitField32<TimedMine_Flags_1C4> field_1C4_flags;
};
ALIVE_ASSERT_SIZEOF(TimedMine, 0x1C8);