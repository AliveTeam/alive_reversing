#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "DynamicArray.hpp"
#include "Primitives.hpp"

struct Fg1Block
{
    Poly_FT4 field_0_polys[2];
    int field_58;
    int field_5C;
    int field_60;
    __int16 field_64;
    unsigned __int16 field_66_mapped_layer;
    int field_68_array_of_height[16];
};
ALIVE_ASSERT_SIZEOF(Fg1Block, 0xA8);

struct Fg1Chunk;
enum class LevelIds : __int16;

class FG1 : public BaseGameObject
{
public:
    EXPORT FG1* ctor_499FC0(BYTE** pFg1Res);

    virtual BaseGameObject* VDestructor(signed int flags) override;
    virtual void VRender(int** pOrderingTable) override;
    virtual void VScreenChanged() override;

    EXPORT void vScreenChanged_49A520();
    EXPORT void vRender_49A3C0(int** pOt);
    EXPORT BaseGameObject* vdtor_49A1E0(signed int flags);
    EXPORT void dtor_49A540();
    EXPORT __int16 Convert_Chunk_To_Render_Block_49A210(Fg1Chunk* pChunk, Fg1Block* pBlock);

    int field_20;
    LevelIds field_24_level_id;
    __int16 field_26_path_id;
    __int16 field_28_render_block_count;
    //__int16 field_2A;
    BYTE** field_2C_ptr;
    Fg1Block* field_30_chnk_res;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x34);

ALIVE_VAR_EXTERN(DynamicArrayT<FG1>*, gFG1List_5D1E28);
