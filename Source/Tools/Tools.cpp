#include "stdafx.h"
#include <SDL.h>
#include "Function.hpp"
#include "LvlArchive.hpp"
#include "ResourceManager.hpp"
#include "Io.hpp"
#include "Game.hpp"
#include "DynamicArray.hpp"
#include "BaseGameObject.hpp"
#include "stdlib.hpp"
#include "FG1.hpp"
#include "Map.hpp"
#include "Path.hpp"

#if _WIN32
#include "ExportHooker.hpp"
#endif

EXPORT void Static_Inits();

bool IsAlive()
{
    return false;
}

int main(int argc, char* argv[])
{
#if _WIN32
    // So calling NOT_IMPLEMENTED() won't __debugbreak()
    ExportHooker hooker(GetModuleHandle(nullptr));
    hooker.Apply();
#endif

    Static_Inits();

    sIOSyncReads_BD2A5C = TRUE;

    IO_Init_494230();

    gBaseGameObject_list_BB47C4 = alive_new<DynamicArrayT<BaseGameObject>>();
    gBaseGameObject_list_BB47C4->ctor_40CA60(50);

    gObjList_drawables_5C1124 = alive_new<DynamicArrayT<BaseGameObject>>();
    gObjList_drawables_5C1124->ctor_40CA60(30);

    gFG1List_5D1E28 = alive_new<DynamicArrayT<FG1>>();
    gFG1List_5D1E28->ctor_40CA60(4);


    gObjList_animations_5C1A24 = alive_new<DynamicArrayT<AnimationBase>>();
    gObjList_animations_5C1A24->ctor_40CA60(30);

    pResourceManager_5C1BB0 = alive_new<ResourceManager>();
    pResourceManager_5C1BB0->ctor_464910();

    //Init_Sound_DynamicArrays_And_Others_43BDB0();

    ResourceManager::Init_49BCE0();
    
    const int typeToFind = TlvTypes::Null_76;

    for (const PathRoot& pathData : sPathData_559660.paths)
    {
        const std::string lvlName(pathData.field_18_lvl_name);
        if (lvlName == "TL")
        {
            // TL.LVL don't exist
            continue;
        }

        const std::string name = "C:\\GOG Games\\Abes Exoddus\\" + lvlName + ".LVL";
        if (!sLvlArchive_5BC520.Open_Archive_432E80(name.c_str()))
        {
            ALIVE_FATAL("Can't open LVL");
        }

        ResourceManager::LoadResourceFile_49C170(pathData.field_3A_bnd_name, nullptr);

        for (int i = 1; i <= pathData.field_1A_num_paths; ++i)
        {
            if (pathData.field_0_pBlyArrayPtr[i].field_0_blyName)
            {
                BYTE** ppRes = ResourceManager::GetLoadedResource_49C2A0(ResourceManager::Resource_Path, i, FALSE, FALSE);
                if (!ppRes)
                {
                    ALIVE_FATAL("Path resource not found");
                }

                gMap_5C3030.field_D2_cam_y_idx = 0;
                gMap_5C3030.field_D2_cam_y_idx = 0;

                Path path;
                path.ctor_4DB170();
                path.Init_4DB200(pathData.field_0_pBlyArrayPtr[i].field_4_pPathData, static_cast<LevelIds>(i), 0, 0, ppRes);
                for (int x = 0; x < path.field_6_cams_on_x; x++)
                {
                    for (int y = 0; y < path.field_8_cams_on_y; y++)
                    {
                        Path_TLV* pTlv = path.Get_First_TLV_For_Offsetted_Camera_4DB610(x, y);
                        while (pTlv)
                        {
                            if (pTlv->field_4_type == typeToFind)
                            {
                                BYTE* pRes = *ppRes;
                                CameraName* pCamName = reinterpret_cast<CameraName*>(&pRes[(x + (y * path.field_6_cams_on_x)) * sizeof(CameraName)]);

                                std::cout << "Found in LVL " << lvlName << " path number " << i  << " in camera " << std::string(pCamName->name, 8) << std::endl;
                            }
                            pTlv = Path::Next_TLV_4DB6A0(pTlv);
                        }
                    }
                }

                path.dtor_4DB1A0();
            }
        }
    }


    return 0;
}
