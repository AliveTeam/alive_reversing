#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"
#include "Path.hpp"

enum class LevelIds : s16;

struct Path_LevelLoader : public Path_TLV
{
    s16 field_10_id;
    LevelIds field_12_dest_level;
    s16 field_14_dest_path;
    s16 field_16_dest_camera;
    s16 field_18_movie_id;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LevelLoader, 0x1C);

class LevelLoader : public BaseGameObject
{
public:
    virtual BaseGameObject* VDestructor(s32 flags) override;
    virtual void VUpdate() override;

    EXPORT LevelLoader* ctor_4DD330(Path_LevelLoader* pTlv, u32 tlvInfo);

private:
    EXPORT void vUpdate_4DD400();
    EXPORT LevelLoader* vdtor_4DD3B0(s32 flags);
    EXPORT void dtor_4DD3E0();

private:
    s16 field_20_id;
    enum class States : s16
    {
        eIdle_0 = 0,
        eFadingOut_1 = 1,
        eDone_2 = 2,
    };
    States field_22_state;
    LevelIds field_24_level;
    s16 field_26_path;
    s16 field_28_camera;
    s16 field_2A_movie_id;
    u32 field_2C_tlvInfo;
    s32 field_30_fade_id;
};
ALIVE_ASSERT_SIZEOF(LevelLoader, 0x34);
