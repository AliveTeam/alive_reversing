#pragma once

#include "FunctionFwd.hpp"
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

    EXPORT void AddDynamicCollision_4512C0(int maxW, int maxH, int frameTableOffset, BYTE** ppAnimData, Path_TLV* pTlv, Map* pMap, int tlvInfo);

    EXPORT BaseGameObject* dtor_451490();

    EXPORT void SyncCollisionLinePosition_451540();

    EXPORT void KeepThingsOnPlatform_451690(FP xpos);


    EXPORT BaseGameObject* Vdtor_4516F0(signed int flags);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    int field_10C;
    int field_110;
    int field_114_count;
    s16 field_118_x_offset;
    s16 field_11A_width_offset;
    s16 field_11C_y_offset;
    s16 field_11E_height_offset;
    PathLine* field_120_pCollisionLine;
    Map* field_124_pMap;
    int field_128_tlvInfo;
};
ALIVE_ASSERT_SIZEOF(PlatformBase, 0x12C);

}

