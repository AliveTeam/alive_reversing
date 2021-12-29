#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

EXPORT void CC Get_fmvs_sectors_44FEB0(const char_type* pMovieName1, const char_type* pMovieName2, const char_type* pMovieName3, u32* pMovie1Sector, u32* pMovie2Sector, u32* pMovie3Sector);

class Movie final : public BaseGameObject
{
public:
    EXPORT Movie* ctor_489C90(s32 id, s32 pos, s8 bUnknown, s32 flags, s16 volume);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_475FB0(s32 flags);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_489EA0();

    s16 field_10_flags;
    s16 field_12;
    s32 field_14;
    s32 field_18;
    u8** field_1C;
    s32 field_20;
    s32 field_24;
    s32 field_28;
    s32 field_2C;
    s32 field_30;
    s8 field_34_min;
    s8 field_35;
    s8 field_36;
    s8 field_37;
    s32 field_38;
    s16 field_3C;
    s16 field_3E;
};
ALIVE_ASSERT_SIZEOF(Movie, 0x40);

ALIVE_VAR_EXTERN(s32, sMovie_ref_count_9F309C);

} // namespace AO
