#pragma once

#include "BaseGameObject.hpp"

struct CdlLOC;

class Movie : public BaseGameObject
{
public:
    virtual void VDestructor(signed int flags) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT void Init_4DFF60(int a2, CdlLOC* pCdPos, __int16 bUnknown, __int16 a5, __int16 a6);
    EXPORT void ctor_4DFDE0(int id, DWORD pos, __int16 a4, __int16 a5, __int16 volume);
    EXPORT void vUpdate_4E0030();
    EXPORT void vdtor_4DFE80(signed int flags);
private:
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
