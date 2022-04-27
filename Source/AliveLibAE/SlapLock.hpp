#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"

struct Path_SlapLock final : public Path_TLV
{
    Scale_short field_10_scale;
    s16 field_12_target_tomb_id1;
    s16 field_14_target_tomb_id2;
    Choice_short field_16_bPersistant;
    Choice_short field_18_has_ghost;
    Choice_short field_1A_give_invisibility_powerup;
    s16 field_1C_invisibility_duration;
    s16 field_1E_toggle_switch_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_SlapLock, 0x20);

enum class SlapLockStates : s16
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

struct SlapLock_State final
{
    AETypes field_0_type;
    s16 field_2_render;
    s32 field_4_tlvInfo;
    s8 field_8_tlv_state;
    s8 field_9_padding;
    SlapLockStates field_A_state;
    s32 field_C_timer1;
    s32 field_10_obj_id;
    s32 field_14_timer2;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(SlapLock_State, 0x18);

class SlapLock final : public BaseAliveGameObject
{
public:
    EXPORT SlapLock* ctor_43DC80(Path_SlapLock* pTlv, s32 tlvInfo);

    EXPORT void dtor_43DF00();
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VScreenChanged() override;
    virtual s32 VGetSaveState(u8* pSaveBuffer) override;
    virtual s16 VTakeDamage_408730(BaseGameObject* pFrom) override;

    EXPORT static s32 CC CreateFromSaveState_43EA00(const u8* pBuffer);

private:
    EXPORT SlapLock* vdtor_43DED0(s32 flags);
    EXPORT void vScreenChanged_43E840();
    EXPORT void GiveInvisibility_43E880();
    EXPORT s32 vGetSaveState_43EB30(SlapLock_State* pState);
    EXPORT void vUpdate_43DF90();
    EXPORT void GiveInvisibilityPowerUp_43E910();
    EXPORT s16 vTakeDamage_43E5D0(BaseGameObject* pFrom);

private:
    Path_SlapLock* field_118_pTlv;
    s32 field_11C_tlvInfo;
    SlapLockStates field_120_state;
    s16 field_122_padding;
    s32 field_124_timer1;
    FP field_128_midX;
    FP field_12C_midY;
    Choice_short field_130_has_ghost;
    s16 field_132_padding;
    s32 field_134_id;
    s32 field_138_possesion_flicker_id;
    s32 field_13C_timer2;
};
ALIVE_ASSERT_SIZEOF(SlapLock, 0x140);
