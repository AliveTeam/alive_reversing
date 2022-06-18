#pragma once

#include "../relive_lib/BaseGameObject.hpp"

struct CdlLOC;

void Get_fmvs_sectors(const char_type* fmvName1, const char_type* fmvName2, const char_type* fmvName3, u32* fmvSectorPos1, u32* fmvSectorPos2, u32* fmvSectorPos3);
s8 DDV_Play(const char_type* pDDVName);
bool AreMovieSkippingInputsHeld();

class Movie final : public BaseGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void Init(s32 a2, CdlLOC* pCdPos, s16 bUnknown, s16 flags, s16 volume);
    Movie(s32 id, u32 pos, s32 bUnknown, s32 flags, s32 volume);

private:
    void DeInit();

    s16 field_20_unused = 0;
    s16 field_22_volume = 0;
    u8** field_2C_ppRes = nullptr;
    s32 field_38_unused = 0;
    s32 field_3C_unused = 0;
    s8 field_44_cd_loc_min = 0;
    s8 field_45_cd_loc_sec = 0;
    s8 field_46_cd_loc_sector = 0;
};
ALIVE_ASSERT_SIZEOF(Movie, 0x48);

ALIVE_VAR_EXTERN(s32, sMovie_ref_count_BB4AE4);
