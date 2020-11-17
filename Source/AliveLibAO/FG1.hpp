#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"

namespace AO {

struct Fg1Block;
struct Fg1Chunk;

enum class LevelIds : __int16;

class FG1 : public BaseGameObject
{
public:
    EXPORT FG1* ctor_4539C0(unsigned __int8** ppRes);

    EXPORT FG1* Vdtor_453E90(signed int flags);

    virtual BaseGameObject* VDestructor(signed int flags) override;

    virtual void VUpdate() override;

    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_453DE0();

    virtual void VRender(int** ppOt) override;

    EXPORT void VRender_453D50(int** ppOt);

    EXPORT void Convert_Chunk_To_Render_Block_453BA0(const Fg1Chunk* pChunk, Fg1Block* pBlock);

    EXPORT BaseGameObject* dtor_453DF0();

    __int16 field_10_cam_pos_x;
    __int16 field_12_cam_pos_y;
    LevelIds field_14_current_level;
    __int16 field_16_current_path;
    __int16 field_18_render_block_count;
    __int16 field_1A_pad;
    BYTE** field_1C_ptr;
    Fg1Block* field_20_chnk_res;
};
ALIVE_ASSERT_SIZEOF(FG1, 0x24);

}
