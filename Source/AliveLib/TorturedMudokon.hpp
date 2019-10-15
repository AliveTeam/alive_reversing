#pragma once

#include "FunctionFwd.hpp"
#include "BaseAnimatedWithPhysicsGameObject.hpp"
#include "Path.hpp"

struct Path_TorturedMudokon : public Path_TLV
{
    __int16 field_10_speed_id;
    __int16 field_12_release_id;
};
ALIVE_ASSERT_SIZEOF_ALWAYS(Path_TorturedMudokon, 0x14);

class TorturedMudokon : public BaseAnimatedWithPhysicsGameObject
{
public:
    EXPORT TorturedMudokon* ctor_47BC60(Path_TorturedMudokon* pTlv, int tlvInfo);
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;
    virtual void VUpdate() override;
    virtual BaseGameObject* VDestructor(signed int flags) override;
private:
    EXPORT void sub_47BE60(Animation* pAnim);
    EXPORT void sub_47BEF0(Animation* pAnim);
    EXPORT void vScreenChanged_47C440();
    EXPORT void vRender_47C460(int** ppOt);
    EXPORT void dtor_47C380();
    EXPORT TorturedMudokon* vdtor_47BE30(signed int flags);
    EXPORT void vUpdate_47BF80();
private:
    Animation field_F4;
    Animation field_18C;
    BYTE** field_224_ppRes;
    int field_228;
    int field_22C;
    int field_230_tlvInfo;
    int field_234;
    __int16 field_238;
    __int16 field_23A_speed_id;
    __int16 field_23C_release_id;
    __int16 field_23E_state;
    __int16 field_240;
    __int16 field_242;
};
ALIVE_ASSERT_SIZEOF(TorturedMudokon, 0x244);
