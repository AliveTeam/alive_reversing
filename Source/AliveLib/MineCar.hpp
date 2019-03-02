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

    virtual void VRender(int** /*pOrderingTable*/) override
    {
        // TODO
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

    virtual __int16 VTakeDamage_408730(BaseAnimatedWithPhysicsGameObject* /*pFrom*/) override
    {
        // TODO
        return 0;
    }

private:
    EXPORT void LoadAnimation_46BF80(Animation* pAnim);

    EXPORT void vStopAudio_46F9C0();

    EXPORT void vScreenChanged_46F800();

    EXPORT MineCar* vdtor_46BF50(signed int flags);

    EXPORT void dtor_46F2A0();

    EXPORT BOOL CheckCollision_46F6B0(FP hitX, FP hitY);

    EXPORT __int16 CheckCollision_46F730(FP hitX, FP hitY);

    EXPORT void vUpdate_46C010();
private:
    __int16 field_116_pad;
    int field_118_tlvInfo;
    __int16 field_11C_state;
    __int16 field_11E_scale;
    __int16 field_120_max_damage;
    __int16 field_122;
    Animation field_124_anim;
    __int16 field_1BC;
    __int16 field_1BE;
    __int16 field_1C0;
    __int16 field_1C2;
    __int16 field_1C4;
    __int16 field_1C6;
    int field_1C8_frame_mod_16;
    __int16 field_1CC_spawned_path;
    __int16 field_1CE_spawned_camera;
    int field_1D0_sound_channels_mask;
    __int16 field_1D4_throw_item_key1;
    __int16 field_1D6_throw_item_key2;
};
ALIVE_ASSERT_SIZEOF(MineCar, 0x1D8);
