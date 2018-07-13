#pragma once

#include "stdlib.hpp"
#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Animation.hpp"
#include "Game.hpp"
#include "Psx.hpp"
#include "Function.hpp"
#include "ResourceManager.hpp"
#include "PsxDisplay.hpp"

void Font_ForceLink();

struct Font
{
public:
    Font();
    Font(int blockSize, BYTE *palette, byte *vramBuffer);

    EXPORT void ctor_433590(int blockSize, BYTE *palette, byte *vramBuffer);
    EXPORT int DrawString_4337D0(int **ot, char *text, int x, __int16 y, char abr, int bSemiTrans, int a2, int a9, char r, char g, char b, int a13, signed int scale, int a15, __int16 colorRandomRange);
    EXPORT int MeasureWidth_433700(char * text);
public:
    byte gap0[32];
    BYTE **field_20_resource;
    byte *field_24_resource_data;
    PSX_RECT field_28_palette_rect;
    int field_30_block_size;
    byte *field_34;
};
ALIVE_ASSERT_SIZEOF(Font, 0x38);

ALIVE_ARY_EXTERN(byte, 32, sFont1VRAM_5BC5C8);
ALIVE_ARY_EXTERN(byte, 32, sFont2VRAM_5BC5D8);

ALIVE_VAR_EXTERN(char, sDisableFontFlicker_5C9304);