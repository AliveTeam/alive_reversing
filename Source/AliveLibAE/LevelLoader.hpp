#pragma once

#include "BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Path.hpp"

enum class LevelIds : s16;

struct Path_LevelLoader final : public Path_TLV
{
    s16 field_10_switch_id;
    LevelIds field_12_dest_level;
    s16 field_14_dest_path;
    s16 field_16_dest_camera;
    s16 field_18_movie_id;
    s16 field_1A_pad;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_LevelLoader, 0x1C);

class LevelLoader final : public BaseGameObject
{
public:
    
    virtual void VUpdate() override;

    LevelLoader(Path_LevelLoader* pTlv, u32 tlvInfo);

private:
    void vUpdate_4DD400();

private:
    s16 field_20_switch_id;
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
