#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class LvlArchive
{
public:
    EXPORT void OpenArchive_41BC60(int pos);

    EXPORT __int16 Free_41BEB0();

private:
    BYTE** field_0_0x2800_res;
    int field_4_cd_pos;
};
ALIVE_ASSERT_SIZEOF(LvlArchive, 0x8);

ALIVE_VAR_EXTERN(LvlArchive, sLvlArchive_4FFD60);
ALIVE_VAR_EXTERN(LvlArchive, stru_507C90);

END_NS_AO
