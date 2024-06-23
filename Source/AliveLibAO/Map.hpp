#pragma once

#include "../relive_lib/MapWrapper.hpp"
#include "../relive_lib/FixedPoint.hpp"

enum class ReliveTypes : s16;
class CamResource;
struct PSX_Point;
class BinaryPath;

namespace relive
{
    class Path_TLV;
}

class Camera;

extern const CameraSwapEffects kPathChangeEffectToInternalScreenChangeEffect[10];

namespace AO {

struct PathData;
class CameraSwapper;
enum class LoadMode : s16;

struct CameraName final
{
    char_type name[8];
};
ALIVE_ASSERT_SIZEOF(CameraName, 8);

namespace CameraIds::Menu
{
    const s16 eMainMenu_1 = 1;
    const s16 eOptions_2 = 2;
    const s16 eGamespeakGamepad_3 = 3;
    const s16 eMotions_4 = 4;
    const s16 eSound_5 = 5;
    const s16 eLoad_6 = 6;
    const s16 eMotionsGamespeakGamepad_7 = 7;
    const s16 eCopyright_10 = 10;
    const s16 eLoading_21 = 21;
    const s16 eFmvSelect_30 = 30;
    const s16 eLvlSelect_31 = 31;
    const s16 eGamespeakKeyboard_33 = 33;
    const s16 eMotionsGamespeakKeyboard_37 = 37;
    const s16 eController_40 = 40;
    const s16 eControllerConfig_41 = 41;
}


class Map final : public IMap
{
public:
    Map();

    void Init(EReliveLevelIds level, s16 path, s16 camera, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);

    void Shutdown();
    void Reset();

    s16 SetActiveCam(EReliveLevelIds level, s16 path, s16 cam, CameraSwapEffects screenChangeEffect, s16 fmvBaseId, s16 forceChange);

    void ScreenChange();

    void FreePathResourceBlocks();
    BinaryPath* GetPathResourceBlockPtr(u32 pathId);
    void ClearPathResourceBlocks();

    void GoTo_Camera();

    void Loader(s16 camX, s16 camY, LoadMode loadMode, ReliveTypes typeToLoad);

    void RemoveObjectsWithPurpleLight(s16 bMakeInvisible);

    void Handle_PathTransition();

    void ScreenChange_Common();

    void Get_map_size(PSX_Point* pPoint);

    virtual void GetCurrentCamCoords(PSX_Point* pPoint) override;
    s16 GetOverlayId() override;

    static CameraSwapper* FMV_Camera_Change(CamResource& ppBits, Map* pMap, EReliveLevelIds levelId);

    void Create_FG1s();

    Camera* Create_Camera(s16 xpos, s16 ypos, s32 a4);

    void RestoreBlyData(const u8* pSaveData);

    void Load_Path_Items(Camera* pCamera, LoadMode loadMode);

    virtual relive::Path_TLV* TLV_First_Of_Type_In_Camera(ReliveTypes type, s16 camX) override;

    relive::Path_TLV* VTLV_Get_At_Of_Type(s16 xpos, s16 ypos, s16 width, s16 height, ReliveTypes typeToFind) override;

    virtual relive::Path_TLV* TLV_Get_At(relive::Path_TLV* pTlv, FP xpos, FP ypos, FP width, FP height) override;

    void ResetPathObjects(u16 pathNum);

    virtual CameraPos GetDirection(EReliveLevelIds level, s32 path, FP xpos, FP ypos) override;

    virtual CameraPos Rect_Location_Relative_To_Active_Camera(const PSX_RECT* pRect, s16 width = 0) override;

    s16 Get_Camera_World_Rect(CameraPos camIdx, PSX_RECT* pRect);

    s16 Is_Point_In_Current_Camera(EReliveLevelIds level, s32 path, FP xpos, FP ypos, s16 width) override;

    // NOTE: Global func in AE
    void Start_Sounds_For_Objects_In_Near_Cameras();

    // NOTE: Part of Path object in AE
    void Start_Sounds_For_Objects_In_Camera(CameraPos direction, s16 cam_x_idx, s16 cam_y_idx);

    // NOTE: Part of Path object in AE
    relive::Path_TLV* Get_First_TLV_For_Offsetted_Camera(s16 camX, s16 camY);

    void SaveBlyData(u8* pSaveBuffer);

    std::vector<std::unique_ptr<BinaryPath>>& GetLoadedPaths()
    {
        return mLoadedPaths;
    }

    virtual void TLV_Reset(const Guid& tlvId, s16 hiFlags = -1) override;
    virtual void TLV_Persist(const Guid& tlvId, s16 hiFlags = -1) override;
    virtual void TLV_Delete(const Guid& tlvId, s16 hiFlags = -1) override;
    void Set_TLVData(const Guid& tlvId, s16 hiFlags, s8 bSetCreated, s8 bSetDestroyed) override;


    s16 mForceLoad = 0;

    CameraSwapEffects mCameraSwapEffect = CameraSwapEffects::eInstantChange_0;
    s16 mFmvBaseId = 0;
    s16 field_1E_door = 0;
    s16 mCamIdxOnX = 0;
    s16 mCamIdxOnY = 0;
    u16 mMaxCamsX = 0;
    u16 mMaxCamsY = 0;
    FP_Point mCameraOffset = {};

    const PathData* mPathData = nullptr;
    s16 mMapChanged = 0;
    bool mFreeAllAnimAndPalts = false;
    u8* mSaveData = nullptr;

    std::vector<std::unique_ptr<class BinaryPath>> mLoadedPaths;
};


extern Map gMap;

s32 MaxGridBlocks(FP scale);

} // namespace AO

extern bool gMap_bDoPurpleLightEffect;