#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

class PlatformBase : public BaseAliveGameObject
{
public:
    PlatformBase();
    ~PlatformBase();

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

    // TODO: New virtuals

protected:
    EXPORT void AddDynamicCollision_4971C0(s32 frameTableOffset, s32 maxW, u16 maxH, u8** ppAnimData, Path_TLV* pTlv, Map* pMap, s32 tlvInfo);

    EXPORT void SyncCollisionLinePosition_4974E0();
    EXPORT void vRemoveCount_4975E0(BaseAliveGameObject* pObj);

    EXPORT void vAddCount_4975B0(BaseAliveGameObject* pObj);

protected:
    s32 field_118_count;
    s16 field_11C_x_offset;
    s16 field_11E_width_offset;
    s16 field_120_y_offset;
    s16 field_122_height_offset;
    PathLine* field_124_pCollisionLine;
    s32 field_128_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);
