#include "PathDataExtensions.hpp"
#include "PathData.hpp"
#include "LvlArchive.hpp"
#include "LCDScreen.hpp"
#include "../AliveLibCommon/PathDataExtensionsTypes.hpp"
#include "../relive_lib/MapWrapper.hpp"

struct MudCounts final
{
    s32 mTotal = 300;
    s32 mBadEnding = 255;
    s32 mGoodEnding = 150;
};
static MudCounts sMudExtData[static_cast<u32>(LevelIds::eCredits_16) + 1][99];


s32 Path_GetTotalMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathNum].mTotal;
}

s32 Path_BadEndingMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathNum].mBadEnding;
}

s32 Path_GoodEndingMuds(EReliveLevelIds lvlId, u32 pathNum)
{
    return sMudExtData[static_cast<u32>(MapWrapper::ToAE(lvlId))][pathNum].mGoodEnding;
}

static u8* sPathExtData[static_cast<u32>(LevelIds::eCredits_16) + 1] = {};

template <typename T>
static void SetAndLog(const char_type* propertyName, T& dst, T newVal)
{
    if (dst != newVal)
    {
        LOG_INFO("Update " << propertyName << " from " << static_cast<s32>(dst) << " to " << static_cast<s32>(newVal));
        dst = newVal;
    }
}

void Path_Set_NewData_FromLvls()
{
    for (s32 lvlIdx = 0; lvlIdx < Path_Get_Paths_Count(); lvlIdx++)
    {
        if (lvlIdx == static_cast<s32>(LevelIds::eNotUsed_15))
        {
            continue;
        }

        // Open the LVL
        LvlArchive archive;
        if (archive.Open_Archive_432E80(CdLvlName(MapWrapper::FromAE(static_cast<LevelIds>(lvlIdx)))))
        {
            // Check for hard coded data replacement
            LvlFileRecord* pRec = archive.Find_File_Record_433160(Path_Get_BndName(MapWrapper::FromAE(static_cast<LevelIds>(lvlIdx))));
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
                while (pHeader->mLineType != ResourceManager::Resource_End)
                {
                    if (pHeader->mLineType == Resource_Pxtd)
                    {
                        auto pChunkData = reinterpret_cast<u8*>(&pHeader[1]);
                        auto pExt = reinterpret_cast<const PerPathExtension*>(pChunkData);
                        if (pExt->mSize != sizeof(PerPathExtension))
                        {
                            LOG_INFO("Skip " << pExt->mBlyName << " expected size " << sizeof(PerPathExtension) << " but got " << pExt->mSize);
                            pChunkData += pExt->mSize;
                        }
                        else
                        {
                            LOG_INFO("Applying " << pExt->mBlyName);

                            pChunkData += sizeof(PerPathExtension);

                            // Apply LCD Screen messages
                            auto pLCDScreenMsgs = reinterpret_cast<StringTable*>(pChunkData);
                            pChunkData = StringTable::MakeTable(pLCDScreenMsgs);
                            SetLcdMessagesForLvl(*pLCDScreenMsgs, static_cast<LevelIds>(lvlIdx), pExt->mPathId);

                            // Will be empty for AE
                            auto pHintFlyMsgs = reinterpret_cast<StringTable*>(pChunkData);
                            pChunkData = StringTable::MakeTable(pHintFlyMsgs);

                            // Apply the data
                            PathRoot& rPath = *Path_Get_PathRoot(lvlIdx);
                            rPath.field_0_pBlyArrayPtr[pExt->mPathId].field_0_blyName = pExt->mBlyName;

                            PathBlyRec& rBlyRec = rPath.field_0_pBlyArrayPtr[pExt->mPathId];
                            if (!rBlyRec.field_4_pPathData)
                            {
                                rBlyRec.field_4_pPathData = &GetPathData(lvlIdx)[pExt->mPathId];
                                rPath.field_18_num_paths++;
                            }
                            PathData& rPathData = *rBlyRec.field_4_pPathData;

                            rPathData.field_0_bLeft = 0;
                            rPathData.field_2_bRight = 0;
                            SetAndLog("top", rPathData.field_4_bTop, static_cast<s16>(pExt->mXSize * pExt->mGridWidth));
                            SetAndLog("bottom", rPathData.field_6_bBottom, static_cast<s16>(pExt->mYSize * pExt->mGridHeight));

                            SetAndLog("grid width", rPathData.field_A_grid_width, static_cast<s16>(pExt->mGridWidth));
                            SetAndLog("grid height", rPathData.field_C_grid_height, static_cast<s16>(pExt->mGridHeight));

                            SetAndLog("width", rPathData.field_E_width, static_cast<s16>(pExt->mGridWidth));
                            SetAndLog("height", rPathData.field_10_height, static_cast<s16>(pExt->mGridHeight));

                            SetAndLog("object offset", rPathData.field_12_object_offset, pExt->mObjectOffset);
                            SetAndLog("index table offset", rPathData.field_16_object_indextable_offset, pExt->mIndexTableOffset);

                            SetAndLog("abe start xpos", rPathData.field_1A_abe_start_xpos, static_cast<s16>(pExt->mAbeStartXPos));
                            SetAndLog("abe start ypos", rPathData.field_1C_abe_start_ypos, static_cast<s16>(pExt->mAbeStartYPos));

                            rPathData.field_1E_object_funcs = kObjectFactory;

                            rBlyRec.field_8_pCollisionData = &GetCollisions(lvlIdx)[pExt->mPathId];

                            CollisionInfo& rColInfo = *rBlyRec.field_8_pCollisionData;
                            rColInfo.field_4_left = 0;
                            rColInfo.field_6_right = 0;
                            SetAndLog("top", rColInfo.field_8_top, static_cast<s16>(pExt->mXSize * pExt->mGridWidth));
                            SetAndLog("bottom", rColInfo.field_A_bottom, static_cast<s16>(pExt->mYSize * pExt->mGridHeight));
                            SetAndLog("collision offset", rColInfo.field_C_collision_offset, pExt->mCollisionOffset);
                            SetAndLog("num collision items", rColInfo.field_10_num_collision_items, pExt->mNumCollisionLines);
                            SetAndLog<u32>("grid width", rColInfo.field_14_grid_width, pExt->mGridWidth);
                            SetAndLog<u32>("grid height", rColInfo.field_18_grid_height, pExt->mGridHeight);

                            if (pExt->mTotalMuds != 0)
                            {
                                SetAndLog("sTotalMuds", sMudExtData[lvlIdx][pExt->mPathId].mTotal, pExt->mTotalMuds);
                            }

                            if (pExt->mBadEndingMuds != 0)
                            {
                                SetAndLog("sBadEndingMuds", sMudExtData[lvlIdx][pExt->mPathId].mBadEnding, pExt->mBadEndingMuds);
                            }

                            if (pExt->mGoodEndingMuds)
                            {
                                SetAndLog("sGoodEndingMuds", sMudExtData[lvlIdx][pExt->mPathId].mGoodEnding, pExt->mGoodEndingMuds);
                            }

                            if (pExt->mNumMudsInPath != 0)
                            {
                                if (pExt->mNumMudsInPath != Path_GetMudsInLevel(MapWrapper::FromAE(static_cast<LevelIds>(lvlIdx)), pExt->mPathId))
                                {
                                    LOG_INFO("Set muds in lvl count to " << pExt->mNumMudsInPath);
                                    Path_SetMudsInLevel(MapWrapper::FromAE(static_cast<LevelIds>(lvlIdx)), pExt->mPathId, pExt->mNumMudsInPath);
                                }
                            }
                        }
                    }

                    // To the next chunk
                    pHeader = reinterpret_cast<ResourceManager::Header*>(reinterpret_cast<u8*>(pHeader) + pHeader->field_0_size);
                }
            }
        }
        archive.Free_433130();
    }
}
