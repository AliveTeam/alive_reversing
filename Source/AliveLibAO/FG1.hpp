#pragma once

#include "../AliveLibCommon/Function.hpp"
#include "../relive_lib/BaseGameObject.hpp"
#include "../relive_lib/MapWrapper.hpp"

struct Fg1Chunk;
enum class EReliveLevelIds : s16;

namespace AO {

struct Fg1Block;

class FG1 final : public ::BaseGameObject
{
public:
    FG1(u8** ppRes);
    ~FG1();

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;
    virtual void VRender(PrimHeader** ppOt) override;

    void Convert_Chunk_To_Render_Block(const Fg1Chunk* pChunk, Fg1Block* pBlock);
    void Convert_Chunk_To_Render_Block_AE(const Fg1Chunk* pChunk, Fg1Block* pBlock);

    s16 field_10_cam_pos_x = 0;
    s16 field_12_cam_pos_y = 0;
    EReliveLevelIds field_14_current_level = EReliveLevelIds::eNone;
    s16 field_16_current_path = 0;
    s16 field_18_render_block_count = 0;
    u8** field_1C_ptr = nullptr;
    Fg1Block* field_20_chnk_res = nullptr;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x24);

} // namespace AO
