#pragma once

#include "FunctionFwd.hpp"

struct Path_TLV;
class Path;

struct TlvOffsetLevelIdPathId
{
    WORD tlvOffset;
    BYTE levelId;
    BYTE pathId;
};

union TlvItemInfoUnion
{
    DWORD all;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

using TPathFunctionFn = void(CC*)(Path_TLV*, Path*, TlvItemInfoUnion, __int16);

struct PathFunctionTable
{
    TPathFunctionFn object_funcs[256];
};

extern const PathFunctionTable kObjectFactory;
