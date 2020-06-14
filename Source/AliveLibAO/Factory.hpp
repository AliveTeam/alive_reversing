#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

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


using TPathFunctionFn = void(CC*)(Path_TLV*, Map*, TlvItemInfoUnion, __int16);

struct PathFunctionTable
{
    TPathFunctionFn object_funcs[256];
};

extern const PathFunctionTable kObjectFactory;

END_NS_AO
