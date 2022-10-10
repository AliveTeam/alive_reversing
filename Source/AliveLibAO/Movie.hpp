#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"

namespace AO {

void Get_fmvs_sectors_44FEB0(const char_type* pMovieName1, const char_type* pMovieName2, const char_type* pMovieName3, u32* pMovie1Sector, u32* pMovie2Sector, u32* pMovie3Sector);

class Movie final : public ::BaseGameObject
{
public:
    Movie(s32 id, s32 pos, s8 bUnknown, s32 flags, s16 volume);

    virtual void VScreenChanged() override;
    virtual void VUpdate() override;

    s16 field_10_flags = 0;
    s16 field_12 = 0;
    s32 field_14 = 0;
    s32 field_18 = 0;
    u8** field_1C = nullptr;
    s32 field_20 = 0;
    s32 field_24 = 0;
    s32 field_28 = 0;
    s32 field_2C = 0;
    s32 field_30 = 0;
    s8 field_34_min = 0;
    s8 field_35 = 0;
    s8 field_36 = 0;
    s8 field_37 = 0;
    s32 field_38 = 0;
    s16 field_3C = 0;
    s16 field_3E = 0;
};
ALIVE_ASSERT_SIZEOF(Movie, 0x40);

extern s32 sMovie_ref_count_9F309C;

} // namespace AO
