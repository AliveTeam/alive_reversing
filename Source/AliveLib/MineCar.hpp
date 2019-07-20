#pragma once

#include "BaseAliveGameObject.hpp"
#include "Path.hpp"
#include "FunctionFwd.hpp"

struct Path_MineCar : public Path_TLV
{
    __int16 field_10_scale;
    __int16 field_12_max_damage;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_MineCar, 0x14);

class MineCar : public BaseAliveGameObject
{
public:
    EXPORT MineCar* ctor_46BC80(Path_MineCar* pTlv, int tlvInfo, int a4, int a5, int a6);

    virtual BaseGameObject* VDestructor(signed int flags) override
    {
        return vdtor_46BF50(flags);
    }

    virtual void VUpdate() override
    {
        vUpdate_46C010();
    }

    virtual void VRender(int** pOrderingTable) override
    {
        vRender_46E760(pOrderingTable);
    }

    virtual void VScreenChanged() override
    {
        vScreenChanged_46F800();
    }

    virtual void VStopAudio() override
    {
        vStopAudio_46F9C0();
    }

    virtual int VGetSaveState(BYTE* /*pSaveBuffer*/) override
    {
        // TODO
        return 0;
    }

    virtual __int16 VTakeDamage_408730(BaseGameObject* /*pFrom*/) override
    {
        // TODO
        return 0;
    }

    enum class States : __int16
    {
        eState_0_ParkedWithoutAbe = 0,
        eState_1_ParkedWithAbe = 1,
        eState_2_Moving = 2,
        eState_3_Falling = 3,
    };
private:
    EXPORT void LoadAnimation_46BF80(Animation* pAnim);

    EXPORT void vStopAudio_46F9C0();

    EXPORT void vScreenChanged_46F800();

    EXPORT MineCar* vdtor_46BF50(signed int flags);

    EXPORT void dtor_46F2A0();

    EXPORT BOOL CheckRoofCollision_46F6B0(FP hitX, FP hitY);

    EXPORT __int16 CheckFloorCollision_46F730(FP hitX, FP hitY);

    EXPORT void vUpdate_Real_46C010();

    EXPORT void vUpdate_46C010();

    EXPORT void vRender_46E760(int **pOt);

    EXPORT void Stop_46E570();

    EXPORT void Move_46E640(unsigned __int16 frameTabeOffset, FP velX, FP velY, unsigned __int16 input, __int16 unknown, char bChangeDirection);

    EXPORT __int16 IsBlocked_46F4A0(__int16 a2, int a3);

    EXPORT __int16 sub_46EA00();

    EXPORT void RunThingsOver_46F380();

    void State_0();
    void State_1();
    void State_2();
    void State_3();


private:
    int field_118_tlvInfo;
public:
    States field_11C_state;
private:
    __int16 field_11E_scale;
    __int16 field_120_max_damage;
    __int16 field_122;
    Animation field_124_anim;
public:
    __int16 field_1BC;
private:
    __int16 field_1BE;
    __int16 field_1C0;
    __int16 field_1C2_falling_counter;
    __int16 field_1C4;
    __int16 field_1C6;
    int field_1C8_frame_mod_16;
    __int16 field_1CC_spawned_path;
    __int16 field_1CE_spawned_camera;
    int field_1D0_sound_channels_mask;
    __int16 field_1D4_previous_input;
    __int16 field_1D6_continue_move_input;
};
ALIVE_ASSERT_SIZEOF(MineCar, 0x1D8);
