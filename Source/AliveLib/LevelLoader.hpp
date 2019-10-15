#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Path.hpp"

enum class LevelIds : __int16;

struct Path_LevelLoader : public Path_TLV
{
    __int16 field_10_id;
    LevelIds field_12_dest_level;
    __int16 field_14_dest_path;
    __int16 field_16_dest_camera;
    __int16 field_18_movie;
    //__int16 field_1A; // pad
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LevelLoader, 0x1C);

class LevelLoader : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VUpdate() override;

    EXPORT LevelLoader* ctor_4DD330(Path_LevelLoader* pTlv, DWORD tlvInfo);
private:
    EXPORT void vUpdate_4DD400();
    EXPORT LevelLoader* vdtor_4DD3B0(signed int flags);
    EXPORT void dtor_4DD3E0();
private:
    __int16 field_20_id;
    enum class States : __int16
    {
        eIdle_0 = 0,
        eFadingOut_1 = 1,
        eDone_2 = 2,
    };
    States field_22_state;
    LevelIds field_24_level;
    __int16 field_26_path;
    __int16 field_28_camera;
    __int16 field_2A_movie;
    DWORD field_2C_tlvInfo;
    int field_30_fade_id;
};
ALIVE_ASSERT_SIZEOF(LevelLoader, 0x34);
