#pragma once

#include "FunctionFwd.hpp"

START_NS_AO

class BaseAliveGameObject;
class Camera;

EXPORT void CC Game_ShowLoadingIcon_445EB0();

class ResourceManager
{
public:
    enum ResourceType : DWORD
    {
        Resource_Animation = 0x6D696E41,
        Resource_Font = 0x746E6F46,
        Resource_Path = 0x68746150,
        Resource_Palt = 0x746C6150,
    };

    static EXPORT void CC CancelPendingResourcesFor_41EA60(BaseAliveGameObject* pObj);

    static EXPORT void CC Reclaim_Memory_455660(DWORD sizeToReclaim);

    static EXPORT __int16 CC FreeResource_455550(BYTE** ppRes);

    static EXPORT void CC LoadResource_446C90(const char* pFileName, int type, int resourceId, __int16 loadMode, __int16 bDontLoad = FALSE);

    static EXPORT BYTE** CC GetLoadedResource_4554F0(int type, int resourceId, __int16 addUseCount, __int16 bLock);

    static EXPORT void CC LoadingLoop_41EAD0(__int16 bShowLoadingIcon);

    static EXPORT __int16 CC LoadResourceFile_455270(const char* filename, Camera* pCam, int allocMethod);

    static EXPORT void CC Free_Resource_Of_Type_455810(int type);

    static EXPORT void CC Free_Resources_For_Camera_447170(Camera* pCamera);
};

ALIVE_VAR_EXTERN(short, bHideLoadingIcon_5076A0);
ALIVE_VAR_EXTERN(int, loading_ticks_5076A4);


END_NS_AO
