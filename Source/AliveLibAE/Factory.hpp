#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/data_conversion/relive_tlvs.hpp"

class Path;

enum class LoadMode : s16
{
    ConstructObject_0 = 0,
    LoadResourceFromList_1 = 1,
    LoadResource_2 = 2
};

void ConstructTLVObject(relive::Path_TLV* pTlv, Path* pPath, const Guid& tlvId, LoadMode loadMode);
