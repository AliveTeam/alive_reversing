#include "data_conversion.hpp"
#include "file_system.hpp"
#include "camera_converter.hpp"
#include "../../AliveLibAO/PathData.hpp"
#include "../../AliveLibAO/LvlArchive.hpp"
#include "../MapWrapper.hpp"

void DataConversion::ConvertData()
{
    //FileSystem fs;
    //CameraConverter cc;

    for (s32 lvlIdx = 0; lvlIdx < AO::Path_Get_Paths_Count(); lvlIdx++)
    {
        AO::LvlArchive archive;
        if (archive.OpenArchive(AO::CdLvlName(MapWrapper::FromAO(static_cast<AO::LevelIds>(lvlIdx))), 0))
        {
            // TODO: Enumerate entries

            /*
            // Check for hard coded data replacement
            AO::LvlFileRecord* pRec = archive.Find_File_Record(AO::Path_Get_BndName(MapWrapper::FromAO(static_cast<AO::LevelIds>(lvlIdx))));
            if (pRec)
            {

            }*/
        }
        else
        {
            // Fatal, missing LVL file
        }
    }
}
