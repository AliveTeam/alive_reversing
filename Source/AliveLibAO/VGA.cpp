#include "stdafx_ao.h"
#include "VGA.hpp"
#include "Function.hpp"
#include "../AliveLibAE/VGA.hpp"

namespace AO {

s32 VGA_DisplaySet_490230(u16 width, u16 height, s8 bpp, u8 backbufferCount)
{
    return VGA_DisplaySet_4F32C0(width, height, bpp, backbufferCount);
}

void VGA_Shutdown_4900E0()
{
    VGA_Shutdown_4F3170();
}

} // namespace AO
