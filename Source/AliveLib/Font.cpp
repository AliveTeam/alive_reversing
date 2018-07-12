#include "stdafx.h"
#include "PauseMenu.hpp"

#include "Function.hpp"

ALIVE_ARY(1, 0x5bc5c8, byte, 32, sFont1VRAM_5BC5C8, {});
ALIVE_ARY(1, 0x5BC5D8, byte, 32, sFont2VRAM_5BC5D8, {});

ALIVE_VAR(1, 0x5c9304, char, sDisableFontFlicker_5C9304, 0);

void Font::ctor_433590(int blockSize, BYTE * palette, byte * vramBuffer)
{
    NOT_IMPLEMENTED();
}

int Font::DrawString_4337D0(int ** ot, char * text, int x, __int16 y, char abr, int bSemiTrans, int a2, int a9, char r, char g, char b, int a13, signed int scale, int a15, __int16 colorRandomRange)
{
    NOT_IMPLEMENTED();
    return 0;
}

int Font::MeasureWidth_433700(char * text)
{
    NOT_IMPLEMENTED();
    return 0;
}