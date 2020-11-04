#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

START_NS_AO

class BaseGameObject;
class BaseAliveGameObject;

enum class SeqId : __int16;
enum class LevelIds : __int16;

class MusicController : public BaseGameObject
{
public:
    EXPORT static __int16 CC Create_4436C0();

    enum class MusicTypes : __int16
    {
        eTypeNull = -1,
        eType0 = 0,
        eType1 = 1,
        eType2 = 2,
        eType3 = 3,
        eType4 = 4,
        eType5 = 5,
        eType6 = 6,
        eType7 = 7,
        eType8 = 8,
        eType9 = 9,
        eType10 = 10,
        eType11 = 11, //death jingle?
        eType12 = 12,
        eType13 = 13,
        eType14 = 14,
        eType15 = 15,
        eType16 = 16,
    };

    MusicController* ctor_442930();

    EXPORT BaseGameObject* dtor_4429B0();

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_443450();

    virtual void VUpdate() override;

    EXPORT void VUpdate_443300();

    virtual BaseGameObject* VDestructor(signed int flags) override;

    EXPORT MusicController* Vdtor_4439D0(signed int flags);

    EXPORT static void CC sub_443810(MusicTypes a1, BaseGameObject* a2, __int16 a3, __int16 a4);

    EXPORT static __int16 CC sub_443840(SeqId * seq1, SeqId * seq2, WORD* seqTime);

    EXPORT void sub_442A10();

    static EXPORT void Shutdown_4437E0();

    static EXPORT void CC EnableMusic_443900(__int16 bEnable);

    static EXPORT int CC OnRootCounter_4437D0();

    EXPORT void sub_443460(signed __int16 a2, int a3, __int16 a4, __int16 a5);

    EXPORT void sub_442C20();

    EXPORT void sub_442AC0();

    __int16 field_10;
    __int16 field_12;
    __int16 field_14;
    __int16 field_16;
    LevelIds field_18_level;
    __int16 field_1A;
    BaseAliveGameObject* field_1C_pObj;
    __int16 field_20;
    __int16 field_22;
    __int16 field_24;
    SeqId field_26_seq;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    SeqId field_38_seq;
    __int16 field_3A;
    int field_3C;
    int field_40;
    __int16 field_44;
    __int16 field_46;
    __int16 field_48_state;
    __int16 field_4A;
    __int16 field_4C;
    __int16 field_4E;
    int field_50;
};
ALIVE_ASSERT_SIZEOF(MusicController, 0x54);

END_NS_AO

