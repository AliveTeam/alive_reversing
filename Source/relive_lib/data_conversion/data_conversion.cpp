#include "data_conversion.hpp"
#include "file_system.hpp"
#include "camera_converter.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/LvlArchive.hpp"
#include "../MapWrapper.hpp"
#include <vector>



void DataConversion::ConvertData()
{
    //FileSystem fs;
    //CameraConverter cc;

    std::vector<u8> fileBuffer;
    for (s32 lvlIdx = 0; lvlIdx < AO::Path_Get_Paths_Count(); lvlIdx++)
    {
        if (lvlIdx == static_cast<s32>(AO::LevelIds::eRemoved_7) || lvlIdx == static_cast<s32>(AO::LevelIds::eRemoved_11))
        {
            continue;
        }

        AO::LvlArchive archive;
        if (archive.OpenArchive(AO::CdLvlName(MapWrapper::FromAO(static_cast<AO::LevelIds>(lvlIdx))), 0))
        {
            for (u32 i = 0; i < archive.FileCount(); i++)
            {
                auto pFileRec = archive.FileAt(i);
                if (pFileRec->field_0_file_name)
                {
                    std::string name(pFileRec->field_0_file_name, strnlen(pFileRec->field_0_file_name, 12));

                    fileBuffer.resize(pFileRec->field_10_num_sectors * 2048);
                    archive.Read_File(pFileRec, fileBuffer.data());
                    fileBuffer.resize(pFileRec->field_14_file_size);

                    //LOG_INFO(name);
                }
            }

        }
        else
        {
            // Fatal, missing LVL file
        }
    }
}
