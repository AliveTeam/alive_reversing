#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_SlapLock : public Path_TLV
{
    Scale_short field_10_scale;
    __int16 field_12_target_tomb_id1;
    __int16 field_14_target_tomb_id2;
    Choice_short field_16_bPersistant;
    Choice_short field_18_has_ghost;
    Choice_short field_1A_give_invisibility_powerup;
    __int16 field_1C_invisibility_id;
    __int16 field_1E_option_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlapLock, 0x20);

enum class SlapLockStates : __int16
{
    eShaking_0 = 0,
    eIdle_1 = 1,
    eSlapped_2 = 2,
    eBroken_3 = 3,
    eEmitInvisibilityPowerupRing_4 = 4,
    eFlickerHero_5 = 5,
    eGiveInvisibilityFromFlicker_6 = 6,
    eGiveInvisibility_7 = 7,
};

struct SlapLock_State
{
    AETypes field_0_type;
    __int16 field_2_render;
    int field_4_tlvInfo;
    char field_8_tlv_state;
    char field_9_padding;
    SlapLockStates field_A_state;
    int field_C_timer1;
    int field_10_obj_id;
    int field_14_timer2;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLock_State, 0x18);

class SlapLock : public BaseAliveGameObject
{
public:
    EXPORT SlapLock* ctor_43DC80(Path_SlapLock* pTlv, int tlvInfo);

    EXPORT void dtor_43DF00();
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VScreenChanged() override;
    virtual int VGetSaveState(BYTE* pSaveBuffer) override;
    virtual __int16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    EXPORT static int CC CreateFromSaveState_43EA00(const BYTE* pBuffer);

private:
    EXPORT SlapLock* vdtor_43DED0(signed int flags);
    EXPORT void vScreenChanged_43E840();
    EXPORT void GiveInvisibility_43E880();
    EXPORT signed int vGetSaveState_43EB30(SlapLock_State* pState);
    EXPORT void vUpdate_43DF90();
    EXPORT void GiveInvisibilityPowerUp_43E910();
    EXPORT __int16 vTakeDamage_43E5D0(BaseGameObject* pFrom);

private:
    Path_SlapLock* field_118_pTlv;
    int field_11C_tlvInfo;
    SlapLockStates field_120_state;
    __int16 field_122_padding;
    int field_124_timer1;
    FP field_128_midX;
    FP field_12C_midY;
    Choice_short field_130_has_ghost;
    __int16 field_132_padding;
    int field_134_id;
    int field_138_possesion_flicker_id;
    int field_13C_timer2;
};
ALIVE_ASSERT_SIZEOF(SlapLock, 0x140);
