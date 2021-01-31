#pragma once

#include "FunctionFwd.hpp"

namespace AO {

struct Path_TLV;
class Map;

struct TlvOffsetLevelIdPathId
{
    WORD tlvOffset;
    BYTE levelId;
    BYTE pathId;
};

struct TlvOffsetCombinedLevelIdPathId
{
    WORD tlvOffset;
    WORD levelIdPathId;
};

union TlvItemInfoUnion
{
    DWORD all;
    TlvOffsetCombinedLevelIdPathId combined;
    TlvOffsetLevelIdPathId parts;
};
ALIVE_ASSERT_SIZEOF(TlvItemInfoUnion, 4);

enum class LoadMode : __int16
{
    Mode_0 = 0,
    Mode_1 = 1,
    Mode_2 = 2
};

using TPathFunctionFn = void(CC*)(Path_TLV*, Map*, TlvItemInfoUnion, LoadMode);

struct PathFunctionTable
{
    TPathFunctionFn object_funcs[256];
};

extern const PathFunctionTable kObjectFactory;

}
