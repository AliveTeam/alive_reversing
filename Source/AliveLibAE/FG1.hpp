#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../relive_lib/Primitives.hpp"
#include "../relive_lib/MapWrapper.hpp"

struct Fg1Block final
{
    Poly_FT4 field_0_polys[2];
    Layer field_66_mapped_layer;
    u32 field_68_array_of_height[16];
};
ALIVE_ASSERT_SIZEOF(Fg1Block, 0xA8);

struct Fg1Chunk;
enum class LevelIds : s16;

class FG1 final : public BaseGameObject
{
public:
    explicit FG1(Fg1Resource& pFg1Res);
    ~FG1();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override
    {
        // Empty
    }
    virtual void VScreenChanged() override;

    s16 Convert_Chunk_To_Render_Block(const Fg1Chunk* pChunk, Fg1Block* pBlock);

    s32 field_20_unused = 0;
    EReliveLevelIds field_24_level_id = EReliveLevelIds::eNone;
    s16 field_26_path_id = 0;
    s16 field_28_render_block_count = 0;
    u8** field_2C_ptr = nullptr;
    Fg1Block* field_30_chnk_res = nullptr;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x34);

ALIVE_VAR_EXTERN(DynamicArrayT<FG1>*, gFG1List_5D1E28);
