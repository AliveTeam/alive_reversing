#include "../AliveLibCommon/stdafx_common.h"
#include "TypesCollection.hpp"
#include "AOTlvs.hpp"
#include "AETlvs.hpp"

TypesCollection::TypesCollection()
{
    AddBasicType<BYTE>("Byte", 0, 255);
    AddBasicType<short>("UInt16", 0, 65535);

    AOTlvs::Path_Hoist::AddTypes(*this);
    //AOTlvs::Path_ContinuePoint::AddTypes(*this);

    AETlvs::Path_Hoist::AddTypes(*this);


}
