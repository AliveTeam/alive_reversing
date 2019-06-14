#pragma once

#include "BaseGameObject.hpp"
#include "FunctionFwd.hpp"

struct PathLine;

enum class LevelIds : __int16;

class BaseAliveGameObject;

class Bullet : public BaseGameObject
{
public:
    EXPORT Bullet* ctor_414540(BaseAliveGameObject* pParent, __int16 type, int xpos, int ypos, int a6, int a7, int scale, __int16 a9);

public:

    __int16 field_20_type;
private:
    __int16 field_22;
    PathLine* field_24_pLine;
    int field_28_xpos;
    int field_2C_ypos;
public:
    int field_30;
private:
    int field_34;
    LevelIds field_38_level;
    __int16 field_3A_path;
    int field_3C_scale;
    BaseAliveGameObject* field_40_pParent;
    __int16 field_44;
    __int16 field_46;
};
ALIVE_ASSERT_SIZEOF(Bullet, 0x48);
