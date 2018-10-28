#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

struct Abe_1BC_20_sub_object
{
    int field_0;
    char field_4;
    char field_5;
    __int16 field_6;
    char field_8;
    char field_9;
    char field_A;
    char field_B;
    __int16 field_C;
    __int16 field_E;
    int field_10_mood;
    int field_14;
    __int16 field_18;
    __int16 field_1A;
};
ALIVE_ASSERT_SIZEOF(Abe_1BC_20_sub_object, 0x1C);

EXPORT void CC Abe_SFX_457EC0(unsigned __int8 idx, __int16 volume, int pitch, class Abe *pHero);

class Abe : public BaseAliveGameObject
{
    // TODO: Vtables
public:
    EXPORT Abe* ctor_44AD10(int frameTableOffset, int a3, int a4, int a5);

    EXPORT static signed int CC CreateFromSaveState_44D4F0(char *a1);
    virtual void VDestructor(signed int flags) override;
    EXPORT void dtor_44B350(signed int flags);

    EXPORT void Knockback_44E700(__int16 a2, __int16 a3);
    EXPORT bool vsub_Kill_44BB50(BaseGameObject *otherObj); // PART OF VTABLE
    EXPORT int sub_44B7B0();
    EXPORT void Load_Basic_Resources_44D460();
    EXPORT void Free_Resources_44D420();

    EXPORT BYTE ** StateToAnimResource_44AAB0(signed int state);
public:
    __int16 field_116;
    __int16 field_118;
    __int16 field_11A;
    __int16 field_11C;
    __int16 field_11E;
    __int16 field_120;
    __int16 field_122;
    __int16 field_124;
    __int16 field_126;
    Abe_1BC_20_sub_object field_128;
    int field_144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int field_158;
    int field_15C;
    int field_160;
    int field_164;
    int field_168;
    __int16 field_16C;
    __int16 field_16E;
    int field_170;
    __int16 field_174;
    __int16 field_176;
    int field_178;
    int field_17C;
    int field_180;
    int field_184;
    int field_188;
    int field_18C;
    int field_190;
    int field_194;
    __int16 field_198;
    __int16 field_19A;
    int field_19C;
    __int16 field_1A0_door_id;
    char field_1A2_rock_or_bone_count;
    char field_1A3_throw_direction;
    int field_1A4;
    int field_1A8;
    __int16 field_1AC;
    __int16 field_1AE;
    int field_1B0_save_num;
    int field_1B4;
    int field_1B8;
};
ALIVE_ASSERT_SIZEOF(Abe, 0x1BC);

ALIVE_VAR_EXTERN(Abe *, sActiveHero_5C1B68);
ALIVE_VAR_EXTERN(Abe *, sControlledCharacter_5C1B8C);

extern int sAbeFrameOffsetTable_554B18[130];
extern const char * sAbeStateNames[];