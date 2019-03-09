#pragma once

#include "BaseAliveGameObject.hpp"
#include "FunctionFwd.hpp"

class EvilFart : public BaseAliveGameObject
{
public:
    EXPORT EvilFart* ctor_422E30();
private:
    __int16 field_116_pad;
    __int16 field_118_bFartCountDown;
    __int16 field_11A_isNotChanting;
    __int16 field_11C_k900;
    __int16 field_11E_path;
    __int16 field_120_level;
    __int16 field_122_camera;
    __int16 field_124_bPlayerControlled;
    __int16 field_126_pad;
    int field_128_timer;
    int field_12C_timer;
    int field_130_sound_channels;
};
ALIVE_ASSERT_SIZEOF(EvilFart, 0x134);
