#include "asset_meta.hpp"

#include <algorithm>

struct CustomRenderSpriteFormat
{
    int magic;
    int x;
    int y;
    char r;
    char g;
    char b;
    float scale;
    bool flip;
    void* origPtr;
    int resource_id;
    int frametable_offset;
    int frame;
};

std::string StringToLowerCase(std::string text);