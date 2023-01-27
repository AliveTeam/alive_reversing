#pragma once

#include "../relive_lib/GameObjects/BaseGameObject.hpp"
#include "../relive_lib/AddPointer.hpp"
#include "../relive_lib/Psx.hpp"

class Camera;

void Game_ShowLoadingIcon_482D80();

class ResourceManager final : public BaseGameObject
{
public:
    enum ResourceType : u32
    {
        Resource_VLC = 0x20434C56,
        Resource_End = 0x21646E45,
        Resource_CHNK = 0x4B4E4843,
        Resource_SEQp = 0x53455170,
        Resource_Pend = 0x646E6550,
        Resource_Free = 0x65657246,
        Resource_PBuf = 0x66754250,
        Resource_Path = 0x68746150,
        Resource_Animation = 0x6D696E41,
        Resource_Demo = 0x6F6D6544,
        Resource_Bits = 0x73746942,
        Resource_Palt = 0x746C6150,
        Resource_Font = 0x746E6F46,
        Resource_FntP = 0x50746E46,
        Resource_Seq = 0x20716553,
        Resource_VabHeader = 0x48424156,
        Resource_VabBody = 0x42424156,
        Resource_FG1 = 0x20314746,
        Resource_NxtP = 0x5074784E,
        Resource_Sprx = 0x78727053,
        Resource_Blood = 0x64756C42,
        Resource_Water = 0x72746157,
        Resource_DecompressionBuffer = 0x66754244,
        Resource_Rope = 0x65706F52,
        Resource_Wave = 0x65766157,
        Resource_3DGibs = 0x65444433,
        Resource_Web = 0x20626557,
        Resource_Spline = 0x6e6c7053, // Went with Spline as in ZapLine.cpp.
        Resource_Pxtd = 0x50787464,   // Added for relive path extension blocks
    };

    ResourceManager();
    ~ResourceManager();

    void LoadingLoop(s16 bShowLoadingIcon);

    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    void VScreenChanged_464EC0();

    static s32 SEQ_HashName_49BE30(const char_type* seqFileName);

private:

};

extern ResourceManager* pResourceManager;
extern s16 bHideLoadingIcon_5C1BAA;
extern s32 loading_ticks_5C1BAC;
