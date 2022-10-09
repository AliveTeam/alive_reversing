#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/DynamicArray.hpp"
#include "../AliveLibCommon/Primitives_common.hpp"
#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/Layer.hpp"

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
    FG1(Fg1Resource& pFg1Res, CamResource& camRes);
    ~FG1();

    virtual void VRender(PrimHeader** ppOt) override;
    virtual void VUpdate() override
    {
        // Empty
    }
    virtual void VScreenChanged() override;

    s16 Convert_Chunk_To_Render_Block(const Fg1Chunk* pChunk, Fg1Block* pBlock);

    EReliveLevelIds field_24_level_id = EReliveLevelIds::eNone;
    s16 field_26_path_id = 0;
    Poly_FT4 mPolys[4] = {};
    Fg1Resource mFG1Res;
    CamResource mCamRes;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x34);

ALIVE_VAR_EXTERN(DynamicArrayT<FG1>*, gFG1List_5D1E28);
