#pragma once

#include "BaseGameObject.hpp"

struct CdlLOC;

EXPORT void CC Get_fmvs_sectors_494460(const s8* fmvName1, const s8* fmvName2, const s8* fmvName3, u32* fmvSectorPos1, u32* fmvSectorPos2, u32* fmvSectorPos3);
EXPORT s8 CC DDV_Play_493210(const s8* pDDVName);
bool AreMovieSkippingInputsHeld();

class Movie : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT void Init_4DFF60(s32 a2, CdlLOC* pCdPos, s16 bUnknown, s16 flags, s16 volume);
    EXPORT Movie* ctor_4DFDE0(s32 id, u32 pos, s16 bUnknown, s16 flags, s16 volume);
    EXPORT void vUpdate_4E0030();
    EXPORT BaseGameObject* vdtor_4DFE80(s32 flags);

private:
    EXPORT void DeInit_4E0210();

    s16 field_20_unused;
    s16 field_22_volume;
    s32 field_24_padding;
    s32 field_28_padding;
    u8** field_2C_ppRes;
    s32 field_30_padding;
    s32 field_34_padding;
    s32 field_38_unused;
    s32 field_3C_unused;
    s32 field_40_padding;
    s8 field_44_cd_loc_min;
    s8 field_45_cd_loc_sec;
    s8 field_46_cd_loc_sector;
    s8 field_47_padding;
};
ALIVE_ASSERT_SIZEOF(Movie, 0x48);

ALIVE_VAR_EXTERN(s32, sMovie_ref_count_BB4AE4);
