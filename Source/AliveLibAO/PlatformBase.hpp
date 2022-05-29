#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class Map;

class PlatformBase : public BaseAliveGameObject
{
public:
    virtual void VAdd(BaseAliveGameObject* pObj);

    virtual void VRemove(BaseAliveGameObject* pObj);

    EXPORT void VAdd_4515D0(BaseAliveGameObject* pObj);

    EXPORT void VRemove_451680(BaseAliveGameObject* pObj);

    EXPORT void AddDynamicCollision_4512C0(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo);

    ~PlatformBase();

    EXPORT void SyncCollisionLinePosition_451540();

    EXPORT void KeepThingsOnPlatform_451690(FP xpos);

    s32 field_10C;
    s32 field_110;
    s32 field_114_count;
    s16 field_118_x_offset;
    s16 field_11A_width_offset;
    s16 field_11C_y_offset;
    s16 field_11E_height_offset;
    PathLine* field_120_pCollisionLine;
    Map* field_124_pMap;
    s32 field_128_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);

} // namespace AO
