#include "stdafx.h"
#include "FG1.hpp"
#include "Function.hpp"
#include "stdlib.hpp"
#include "ResourceManager.hpp"
#include "Map.hpp"
#include "ScreenManager.hpp"

ALIVE_VAR(1, 0x5D1E28, DynamicArrayT<FG1>*, gFG1List_5D1E28, nullptr);

enum eChunkTypes
{
    ePartialChunk = 0,
    eFullChunk = 0xFFFE,
    eEndCompressedData = 0xFFFC,
    eStartCompressedData = 0xFFFD,
    eEndChunk = 0xFFFF
};

struct Fg1Chunk
{
    unsigned __int16 field_0_type;
    unsigned __int16 field_2_layer;
    __int16 field_4_xpos;
    __int16 field_6_ypos;
    unsigned __int16 field_8_width;
    unsigned __int16 field_A_height;
};
ALIVE_ASSERT_SIZEOF(Fg1Chunk, 0xC);

struct FG1ResourceBlockHeader
{
    DWORD mCount;
    Fg1Chunk mChunks;
};

void FG1::ctor_499FC0(BYTE** pFG1Res)
{
    BaseGameObject_ctor_4DBFA0(1, 0);

    SetVTable(this, 0x5469C8); // vTbl_FG1_5469C8

    field_6_flags.Set(BaseGameObject::eUpdatableExtra);
    field_6_flags.Set(BaseGameObject::eBit08);
    field_6_flags.Set(BaseGameObject::eDrawable);

    field_4_typeId = Types::eFG1;

    field_24_level_id = gMap_5C3030.sCurrentLevelId_5C3030;
    field_26_path_id = gMap_5C3030.sCurrentPathId_5C3032;

    gFG1List_5D1E28->Push_Back(this);
    
    // Cast to the actual FG1 resource block format
    FG1ResourceBlockHeader* pHeader = reinterpret_cast<FG1ResourceBlockHeader*>(*pFG1Res);

    // So we can extract out the count of chunks and allocate a resource for it
    field_20 = 0;
    field_28_render_block_count = static_cast<short>(pHeader->mCount);
    field_2C_ptr = ResourceManager::Allocate_New_Locked_Resource_49BF40(ResourceManager::Resource_CHNK, 0, pHeader->mCount * sizeof(Fg1Block));
    field_30_chnk_res = reinterpret_cast<Fg1Block*>(*field_2C_ptr);
    
    // And take a pointer to the first chunk to iterate them
    Fg1Chunk* pChunkIter = &pHeader->mChunks;
    int render_block_idx = 0;
    for (;;) // Exit when we hit the end chunk
    {
        switch (pChunkIter->field_0_type)
        {
        case ePartialChunk:
        {
            // Convert the raw FG1 data from disk to internal render format
            Fg1Block* pRenderBlock = &field_30_chnk_res[render_block_idx++];
            Convert_Chunk_To_Render_Block_49A210(pChunkIter, pRenderBlock);

            // Skip to the next block - a bit more tricky as we must skip the bit field array thats used for the transparent pixels
            BYTE* pNextChunk = reinterpret_cast<BYTE*>(pChunkIter) + ((pChunkIter->field_A_height * sizeof(DWORD)) + sizeof(Fg1Chunk));
            pChunkIter = reinterpret_cast<Fg1Chunk*>(pNextChunk);
        }
        break;

        case eFullChunk:
        {
            // Redraw a full solid block of the background "on top"
            pScreenManager_5BB5F4->InvalidateRect_40EC50(
                pChunkIter->field_4_xpos,
                pChunkIter->field_6_ypos,
                pChunkIter->field_8_width + pChunkIter->field_4_xpos - 1,
                pChunkIter->field_A_height + pChunkIter->field_6_ypos - 1,
                pChunkIter->field_2_layer);

            // Move to the next FG1 data from disk
            pChunkIter++;
        }
        break;

        case eStartCompressedData:
        {
            ALIVE_FATAL("eStartCompressedData is not in any AE data, impossible!")
        }
        break;

        case eEndCompressedData:
        {
            ALIVE_FATAL("eEndCompressedData is not in any AE data, impossible!")
        }
        break;

        case eEndChunk:
        {
            return;
        }

        default:
            ALIVE_FATAL("Unknown FG1 block type");
        }
    }
}

void FG1::VDestructor(signed int flags)
{
    vdtor_49A1E0(flags);
}

void FG1::vdtor_49A1E0(signed int flags)
{
    dtor_49A540();
    if (flags & 1)
    {
        Mem_Free_495540(this);
    }
}

void FG1::dtor_49A540()
{
    SetVTable(this, 0x5469C8); // vTbl_FG1_5469C8
    gFG1List_5D1E28->Remove_Item(this);
    ResourceManager::FreeResource_49C330(field_2C_ptr);
    BaseGameObject_dtor_4DBEC0();
}

signed __int16 __stdcall FG1::Convert_Chunk_To_Render_Block_49A210(Fg1Chunk* /*pChunk*/, Fg1Block* /*pBlock*/)
{
    NOT_IMPLEMENTED();
    return 1;
}