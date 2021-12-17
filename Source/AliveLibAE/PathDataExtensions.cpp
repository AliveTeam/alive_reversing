#include "PathDataExtensions.hpp"
#include "PathData.hpp"
#include "LvlArchive.hpp"
#include "LCDScreen.hpp"
#include "../AliveLibCommon/PathDataExtensionsTypes.hpp"

static u8* sPathExtData[static_cast<u32>(LevelIds::eCredits_16) + 1] = {};

void Path_Set_NewData_FromLvls()
{
    for (s32 lvlIdx = 0; lvlIdx < Path_Get_Paths_Count(); lvlIdx++)
    {
        // Open the LVL
        LvlArchive archive;
        if (archive.Open_Archive_432E80(CdLvlName(static_cast<LevelIds>(lvlIdx))))
        {
            // Check for hard coded data replacement
            LvlFileRecord* pRec = archive.Find_File_Record_433160(Path_Get_BndName(static_cast<LevelIds>(lvlIdx)));
            if (pRec)
            {
                // Load the file data
                delete[] sPathExtData[lvlIdx];

                u8* pTmp = new u8[pRec->field_10_num_sectors * 2048];
                archive.Read_File_4330A0(pRec, pTmp);
                
                // Keep the loaded data in scope for however long the game is running
                sPathExtData[lvlIdx] = pTmp;

                // Iterate the chunks
                ResourceManager::Header* pHeader = reinterpret_cast<ResourceManager::Header*>(pTmp);
                while (pHeader->field_8_type != ResourceManager::Resource_End)
                {
                    if (pHeader->field_8_type == Resource_Pxtd)
                    {
                        auto pChunkData = reinterpret_cast<u8*>(&pHeader[1]);
                        auto pExt = reinterpret_cast<const PerPathExtension*>(pChunkData);
                        pChunkData += sizeof(PerPathExtension);

                        // Apply led messages
                        auto pLedMsgs = reinterpret_cast<StringTable*>(pChunkData);
                        pChunkData = StringTable::MakeTable(pLedMsgs);
                        SetLcdMessagesForLvl(*pLedMsgs, static_cast<LevelIds>(lvlIdx));

                        // Will be empty for AE
                        auto pHintFlyMsgs = reinterpret_cast<StringTable*>(pChunkData);
                        pChunkData = StringTable::MakeTable(pHintFlyMsgs);

                        // Apply the data
                        PathRoot& rPath = *Path_Get_PathRoot(lvlIdx);
                        rPath.field_0_pBlyArrayPtr[pExt->mPathId].field_0_blyName = pExt->mBlyName;

                        PathBlyRec& rBlyRec = rPath.field_0_pBlyArrayPtr[pExt->mPathId];
                        PathData& rPathData = *rBlyRec.field_4_pPathData;

                        rPathData.field_0_bLeft = 0;
                        rPathData.field_2_bRight = 0;
                        rPathData.field_4_bTop = static_cast<s16>(pExt->mXSize * pExt->mGridWidth);
                        rPathData.field_6_bBottom = static_cast<s16>(pExt->mYSize * pExt->mGridHeight);

                        rPathData.field_A_grid_width = static_cast<s16>(pExt->mGridWidth);
                        rPathData.field_C_grid_height = static_cast<s16>(pExt->mGridHeight);

                        rPathData.field_E_width = static_cast<s16>(pExt->mGridWidth);
                        rPathData.field_10_height = static_cast<s16>(pExt->mGridHeight);

                        rPathData.field_12_object_offset = pExt->mObjectOffset;
                        rPathData.field_16_object_indextable_offset = pExt->mIndexTableOffset;

                        rPathData.field_1A_abe_start_xpos = 0;
                        rPathData.field_1C_abe_start_ypos = 0;

                        rPathData.field_1E_object_funcs = kObjectFactory;

                        rBlyRec.field_8_pCollisionData = &GetCollisions(lvlIdx)[pExt->mPathId];

                        CollisionInfo& rColInfo = *rBlyRec.field_8_pCollisionData;
                        rColInfo.field_0_fn_ptr = Collisions::Factory_4188A0;
                        rColInfo.field_4_left = 0;
                        rColInfo.field_6_right = 0;
                        rColInfo.field_8_top = static_cast<s16>(pExt->mXSize * pExt->mGridWidth);
                        rColInfo.field_A_bottom = static_cast<s16>(pExt->mYSize * pExt->mGridHeight);
                        rColInfo.field_C_collision_offset = pExt->mCollisionOffset;
                        rColInfo.field_10_num_collision_items = pExt->mNumCollisionLines;
                        rColInfo.field_14_grid_width = pExt->mGridWidth;
                        rColInfo.field_18_grid_height = pExt->mGridHeight;
                    }

                    // To the next chunk
                    pHeader = reinterpret_cast<ResourceManager::Header*>(reinterpret_cast<u8*>(pHeader) + pHeader->field_0_size);
                }
            }
        }
        archive.Free_433130();
    }
}
