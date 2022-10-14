#pragma once

#include "../relive_lib/BaseGameObject.hpp"

struct CdlLOC;

void Get_fmvs_sectors(const char_type* fmvName1, const char_type* fmvName2, const char_type* fmvName3);
s8 DDV_Play(const char_type* pDDVName);
bool AreMovieSkippingInputsHeld();

class Movie final : public BaseGameObject
{
public:
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void Init(s16 flags, s16 volume);
    Movie(s32 flags, s32 volume);

private:
    void DeInit();

    s16 field_22_volume = 0;
    u8** field_2C_ppRes = nullptr;
};

extern s32 sMovie_ref_count_BB4AE4;
