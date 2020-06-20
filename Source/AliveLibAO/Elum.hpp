#pragma once
#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"
#include "Factory.hpp"

START_NS_AO;

class Elum : public BaseAliveGameObject
{
public:
    EXPORT BaseGameObject* Vdtor_411710(signed int flags);
    EXPORT void VUpdate_4102A0();
    EXPORT void VRender_410E40(int** pOrderingTable);
    EXPORT void vScreenChange_411340();

    EXPORT static void Spawn_410E90(TlvItemInfoUnion);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    __int16 field_10C;
    __int16 field_10E;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    __int16 field_128_idx;
    __int16 field_12A_state;
    __int16 field_12C;
    __int16 field_12E;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    __int16 field_140;
    __int16 field_142;
    __int16 field_144;
    __int16 field_146;
    __int16 field_148;
    __int16 field_14A;
    __int16 field_14C;
    __int16 field_14E;
    int field_150;
    __int16 field_154;
    __int16 field_156;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    __int16 field_16C;
    __int16 field_16E;
    __int16 field_170;
    __int16 field_172;
    BYTE **field_174_resources[31];
    int field_1F0;
};
ALIVE_ASSERT_SIZEOF(Elum, 0x1F4);

ALIVE_VAR_EXTERN(Elum*, gElum_507680);

END_NS_AO;
