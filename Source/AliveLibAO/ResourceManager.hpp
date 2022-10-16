#pragma once

#include "../relive_lib/BaseGameObject.hpp"
#include "../AliveLibCommon/Function.hpp"
#include "Psx.hpp"
#include "../AliveLibCommon/AnimResources.hpp"
#include "Factory.hpp"

namespace AO {

class BaseAliveGameObject;
class Camera;
class LoadingFile;

void Game_ShowLoadingIcon_445EB0();

// TODO Argument type is always Camera* ?
using TLoaderFn = void(CC*)(void*);

class ResourceManager final
{
public:

    struct Header final
    {
        u32 field_0_size;
        s16 field_4_ref_count;
        s16 field_6_flags;
        u32 mLineType;
        u32 field_C_id;
    };
    ALIVE_ASSERT_SIZEOF(Header, 0x10);

    struct ResourcesToLoadList_Entry final
    {
        u32 field_0_type;
        u32 field_4_res_id;
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList_Entry, 0x8);

    // TODO: Replace/combine with CompileTimeResourceList when everything is decompiled
    struct ResourcesToLoadList final
    {
        s32 field_0_count;
        ResourcesToLoadList_Entry field_4_items[1];
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList, 12);

    struct ResourceManager_FilePartRecord final
    {
        s32 field_0_type;
        s32 field_4_res_id;
        Camera* field_8_pCamera;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord, 0xc);

    class ResourceManager_FileRecord final
    {
    public:
        ResourceManager_FileRecord()
            : field_10_file_sections_dArray(10)
        {

        }

        const char_type* field_0_fileName = nullptr;
        ResourcesToLoadList* field_4_pResourcesToLoadList = nullptr;
        u32 mLineType = 0;
        u32 field_C_resourceId = 0;
        DynamicArrayT<ResourceManager_FilePartRecord> field_10_file_sections_dArray;
        LoadingFile* field_1C_pGameObjFileRec = nullptr;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord, 0x20);

    static s32 SEQ_HashName_454EA0(const char_type* seqFileName);
    static void LoadingLoop_41EAD0(s16 bShowLoadingIcon);
};

extern s16 bHideLoadingIcon_5076A0;
extern s32 loading_ticks_5076A4;
extern DynamicArrayT<ResourceManager::ResourceManager_FileRecord>* ObjList_5009E0;

} // namespace AO
