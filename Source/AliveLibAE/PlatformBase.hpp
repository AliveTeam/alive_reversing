#pragma once

#include "FunctionFwd.hpp"
#include "BaseAliveGameObject.hpp"

class PlatformBase : public BaseAliveGameObject
{
public:
    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VAdd(BaseAliveGameObject* pObj);
    virtual void VRemove(BaseAliveGameObject* pObj);

    // TODO: New virtuals

protected:
    EXPORT void AddDynamicCollision_4971C0(int maxW, int maxH, unsigned __int16 frameTableOffset, BYTE** ppAnimData, Path_TLV* pTlv, Map* pMap, int tlvInfo);

    EXPORT void dtor_4973E0();

    EXPORT void SyncCollisionLinePosition_4974E0();
    EXPORT void vRemoveCount_4975E0(BaseAliveGameObject* pObj);

    EXPORT void vAddCount_4975B0(BaseAliveGameObject* pObj);
private:
    EXPORT PlatformBase* vdtor_4974B0(signed int flags);

protected:
    int field_118_count;
    __int16 field_11C_x_offset;
    __int16 field_11E_width_offset;
    __int16 field_120_y_offset;
    __int16 field_122_height_offset;
    PathLine* field_124_pCollisionLine;
    int field_128_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);
