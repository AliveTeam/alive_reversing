#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

namespace AO {

enum class CameraSwapEffects : s16;

class ScreenClipper;

class CameraSwapper : public BaseGameObject
{
public:
    EXPORT CameraSwapper* ctor_48C540(BYTE** ppBits, s32 movieId, s32 movieFlag, char movieFlags, s16 flags, s16 volume);

    EXPORT CameraSwapper* ctor_48C5E0(BYTE** ppBits, s32 moviePos1, s32 movieId1, s32 moviePos2, s32 movieFlag1, char movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlag2_1, s16 movieFlags2_1, s16 movieVol2);

    EXPORT CameraSwapper* ctor_48C6B0(BYTE** ppBits, s32 moviePos1, s32 movieIds1, s32 moviePos2, s32 movieId2, s32 moviePos3, s32 movieId3, char movieFlag1, s16 movieFlags1, s16 movieVol1, s16 movieFlag2, s16 movieFlags2, s16 movieVol2, s16 movieFlag3, s16 movieFlags3, s16 movieVol3);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    virtual void VScreenChanged() override;

    EXPORT CameraSwapper* Vdtor_48D380(s32 flags);

    EXPORT CameraSwapper* ctor_48C7A0(BYTE** ppBits, CameraSwapEffects changeEffect, s16 xpos, s16 ypos);

    EXPORT void Init_48C830(BYTE** ppCamRes, CameraSwapEffects changeEffect);

    EXPORT BaseGameObject* dtor_48CE00();

    virtual void VUpdate() override;

    EXPORT void VUpdate_48CEA0();

    s32 field_10_movie_pos_3;
    s32 field_14_movie_id_3;
    s32 field_18_movie_pos_2;
    s32 field_1C_movie_id_2;
    BYTE** field_20_ppCamRes;
    ScreenClipper* field_24_pSubObject;
    CameraSwapEffects field_28_changeEffect;
    s16 field_2A_current_slice;
    s16 field_2C_slices_per_tick;
    s16 field_2E_total_slices;
    s16 field_30_movie_flag_3;
    s16 field_32_movie_flags_3;
    s16 field_34_movie_vol_3;
    s16 field_36_movie_flag_2;
    s16 field_38_movie_flags_2;
    s16 field_3A_movie_vol_2;
    s16 field_3C_movie_bPutDispEnv;
    s16 field_3E_xpos_converted;
    s16 field_40_ypos_converted;
    s16 field_42_XSlices;
    s16 field_44_YSlices;
    s16 field_46_slice_width;
};
ALIVE_ASSERT_SIZEOF(CameraSwapper, 0x48);

ALIVE_VAR_EXTERN(short, sNumCamSwappers_507668); // TODO: Move to own file

}
