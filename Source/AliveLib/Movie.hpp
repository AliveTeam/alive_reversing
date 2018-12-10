#pragma once

#include "BaseGameObject.hpp"

struct CdlLOC;

EXPORT void CC Get_fmvs_sectors_494460(const char* fmvName1, const char* fmvName2, const char* fmvName3, DWORD* fmvSectorPos1, DWORD* fmvSectorPos2, DWORD* fmvSectorPos3);
EXPORT void IO_Init_494230();

class Movie : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT void Init_4DFF60(int a2, CdlLOC* pCdPos, __int16 bUnknown, __int16 a5, __int16 a6);
    EXPORT Movie* ctor_4DFDE0(int id, DWORD pos, __int16 a4, __int16 a5, __int16 volume);
    EXPORT void vUpdate_4E0030();
    EXPORT BaseGameObject* vdtor_4DFE80(signed int flags);
private:
    EXPORT void DeInit_4E0210();

    __int16 field_20;
    __int16 field_22_param5;
    int field_24;
    int field_28;
    BYTE** field_2C_ppRes;
    int field_30;
    int field_34;
    int field_38_param_1;
    int field_3C;
    int field_40;
    char field_44_cd_loc_min;
    char field_45_cd_loc_sec;
    char field_46;
    char field_47;
};
ALIVE_ASSERT_SIZEOF(Movie, 0x48);

ALIVE_VAR_EXTERN(int, sMovie_ref_count_BB4AE4);
