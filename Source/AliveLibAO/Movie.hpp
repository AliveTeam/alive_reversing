#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

EXPORT void CC Get_fmvs_sectors_44FEB0(const char* pMovieName1, const char* pMovieName2, const char* pMovieName3, DWORD* pMovie1Sector, DWORD* pMovie2Sector, DWORD* pMovie3Sector);

class Movie : public BaseGameObject
{
public:
    EXPORT Movie* ctor_489C90(int id, int pos, char bUnknown, int flags, s16 volume);

    virtual BaseGameObject* VDestructor(s32 flags) override;

    EXPORT BaseGameObject* Vdtor_475FB0(s32 flags);

    virtual void VScreenChanged() override;

    virtual void VUpdate() override;

    EXPORT void VUpdate_489EA0();

    s16 field_10_flags;
    s16 field_12;
    int field_14;
    int field_18;
    BYTE** field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    char field_34_min;
    char field_35;
    char field_36;
    char field_37;
    int field_38;
    s16 field_3C;
    s16 field_3E;
};
ALIVE_ASSERT_SIZEOF(Movie, 0x40);

ALIVE_VAR_EXTERN(int, sMovie_ref_count_9F309C);

}
