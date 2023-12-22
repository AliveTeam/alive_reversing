#pragma once

#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/FixedPoint.hpp"

class BaseGameObject;
class BaseAliveGameObject;
class Camera;
struct PathData;
class BinaryPath;
enum class LevelIds : s16;
struct PSX_Point;
enum class EReliveLevelIds : s16;
enum class LoadMode : s16;
class CamResource;

namespace relive
{
    class Path_TLV;
}

struct CameraName final
{
    char_type name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

enum class LoadMode : s16;

class Map final : public IMap
{
public:
    void ScreenChange();

    void FreePathResourceBlocks();
    BinaryPath* GetPathResourceBlockPtr(u32 pathId);
    void ClearPathResourceBlocks();

    void GoTo_Camera();
    void RemoveObjectsWithPurpleLight(s16 a2);
    void Handle_PathTransition();
    void Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);
    void Shutdown();
    void Reset();
    Map();
    ~Map();

    void Get_map_size(PSX_Point* pPoint);
    void GetCurrentCamCoords(PSX_Point* pPoint);
    void Get_Abe_Spawn_Pos(PSX_Point* pPoint);
    s16 GetOverlayId() override;
    void Create_FG1s();
    virtual CameraPos Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width = 0) override;
    s16 SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);
    static BaseGameObject* FMV_Camera_Change(CamResource& ppBits, Map* pMap, EReliveLevelIds lvlId);
    Camera* Create_Camera(s16 xpos, s16 ypos, s32 a4);
    static void Load_Path_Items(Camera* pCamera, LoadMode loadMode);

    static void LoadResource(const char_type* pFileName, s32 type, s32 resourceId, LoadMode loadMode, s16 bDontLoad = 0);

    s16 Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width) override;
    virtual CameraPos GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos) override;

    s16 Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect);
    
    std::vector<std::unique_ptr<BinaryPath>>& GetLoadedPaths()
    {
        return mLoadedPaths;
    }

    void TLV_Reset(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed) override;

    relive::Path_TLV* VTLV_Get_At_Of_Type(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes typeToFind) override;

private:

    void CreateScreenTransistionForTLV(relive::Path_TLV* pTlv);
    void ScreenChange_Common();

public:

    s16 mForceLoad = 0;

    CameraSwapEffects mCameraSwapEffect = CameraSwapEffects::eInstantChange_0;
    u16 mFmvBaseId = 0;
    s16 mDoorTransition = 0;
    s16 mTeleporterTransition = 0;

    FP_Point mCameraOffset = {};
    bool mFreeAllAnimAndPalts = false;

    s16 mCamIdxOnX = 0;
    s16 mCamIdxOnY = 0;

    const PathData* mPathData = nullptr;
    bool mRestoreMapObjectStates = false;

    std::vector<std::unique_ptr<BinaryPath>> mLoadedPaths;
};

extern Map gMap;
extern bool gMap_bDoPurpleLightEffect;
extern const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect[10];

s32 MaxGridBlocks(FP scale);
