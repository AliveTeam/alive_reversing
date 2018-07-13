#include "stdafx.h"
#include "Font.hpp"
#include "Function.hpp"

void Font_ForceLink()
{

}

ALIVE_ARY(1, 0x5bc5c8, byte, 32, sFont1VRAM_5BC5C8, {});
ALIVE_ARY(1, 0x5BC5D8, byte, 32, sFont2VRAM_5BC5D8, {});

ALIVE_VAR(1, 0x5c9304, char, sDisableFontFlicker_5C9304, 0);

EXPORT signed __int16 __cdecl Pal_Allocate_483110(PSX_RECT *a1, unsigned int paletteColorCount)
{
    NOT_IMPLEMENTED();
}

Font::Font()
{
}

Font::Font(int blockSize, BYTE * palette, byte * vramBuffer)
{
    ctor_433590(blockSize, palette, vramBuffer);
}

void Font::ctor_433590(int blockSize, BYTE * palette, byte * vramBuffer)
{
    field_34 = vramBuffer;
    Pal_Allocate_483110(&field_28_palette_rect, 0x10u);
    PSX_RECT rect = { field_28_palette_rect.x , field_28_palette_rect.y, 16, 1 };
    PSX_LoadImage16_4F5E20(&rect, palette);
    field_30_block_size = blockSize;
    field_20_resource = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_FntP, *((signed __int16 *)vramBuffer + 6), 88 * blockSize);
    field_24_resource_data = *field_20_resource;
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