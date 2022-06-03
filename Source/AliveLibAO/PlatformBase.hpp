#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "BaseAliveGameObject.hpp"

namespace AO {

class Map;

class PlatformBase : public BaseAliveGameObject
{
public:
    ~PlatformBase();

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

    void AddDynamicCollision(s32 frameTableOffset, s32 maxW, s32 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo);
    void SyncCollisionLinePosition();
    void KeepThingsOnPlatform(FP xpos);

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
