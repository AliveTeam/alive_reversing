#pragma once

#include "FunctionFwd.hpp"
#include "bmp.hpp"

void VGA_ForceLink();

EXPORT void CC VGA_Shutdown_4F3170();

ALIVE_VAR_EXTERN(Bitmap, sVGA_Bmp1_BD2A20);
