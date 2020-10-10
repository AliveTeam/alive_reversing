#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

START_NS_AO

enum class CameraSwapEffects : __int16;

class ScreenClipper;

class CameraSwapper : public BaseGameObject
{
public:
    EXPORT CameraSwapper* ctor_48C540(BYTE** ppBits, int movieId, int movieFlag, char movieFlags, __int16 flags, __int16 volume);

    EXPORT CameraSwapper* ctor_48C5E0(BYTE** ppBits, int moviePos1, int movieId1, int moviePos2, int movieFlag1, char movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlag2_1, __int16 movieFlags2_1, __int16 movieVol2);

    EXPORT CameraSwapper* ctor_48C6B0(BYTE** ppBits, int moviePos1, int movieIds1, int moviePos2, int movieId2, int moviePos3, int movieId3, char movieFlag1, __int16 movieFlags1, __int16 movieVol1, __int16 movieFlag2, __int16 movieFlags2, __int16 movieVol2, __int16 movieFlag3, __int16 movieFlags3, __int16 movieVol3);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VScreenChanged() override;

    EXPORT CameraSwapper* Vdtor_48D380(signed int flags);

    EXPORT CameraSwapper* ctor_48C7A0(BYTE** ppBits, CameraSwapEffects changeEffect, __int16 xpos, __int16 ypos);

    EXPORT void Init_48C830(BYTE** ppCamRes, CameraSwapEffects changeEffect);

    EXPORT BaseGameObject* dtor_48CE00();

    virtual void VUpdate() override;

    EXPORT void VUpdate_48CEA0();

    int field_10_movie_pos_3;
    int field_14_movie_id_3;
    int field_18_movie_pos_2;
    int field_1C_movie_id_2;
    BYTE** field_20_ppCamRes;
    ScreenClipper* field_24_pSubObject;
    __int16 field_28_changeEffect;
    __int16 field_2A_current_slice;
    __int16 field_2C_slices_per_tick;
    __int16 field_2E_total_slices;
    __int16 field_30_movie_flag_3;
    __int16 field_32_movie_flags_3;
    __int16 field_34_movie_vol_3;
    __int16 field_36_movie_flag_2;
    __int16 field_38_movie_flags_2;
    __int16 field_3A_movie_vol_2;
    __int16 field_3C_movie_bPutDispEnv;
    __int16 field_3E_xpos_converted;
    __int16 field_40_ypos_converted;
    __int16 field_42_XSlices;
    __int16 field_44_YSlices;
    __int16 field_46_slice_width;
};
ALIVE_ASSERT_SIZEOF(CameraSwapper, 0x48);

ALIVE_VAR_EXTERN(short, sNumCamSwappers_507668); // TODO: Move to own file

END_NS_AO
