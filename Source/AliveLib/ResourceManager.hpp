#pragma once

#include "FunctionFwd.hpp"
#include "BaseGameObject.hpp"
#include "Psx.hpp"

class Camera;

EXPORT void CC Game_ShowLoadingIcon_482D80();

class ResourceManager : public BaseGameObject
{
public:
    enum ResourceType : DWORD
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
    };

    enum ResourceHeaderFlags : __int16
    {
        eLocked = 0x1,
        eOnlyAHeader = 0x2,
        eNeverFree = 0x4
    };

    struct Header
    {
        DWORD field_0_size;
        __int16 field_4_ref_count;
        __int16 field_6_flags;
        DWORD field_8_type;
        DWORD field_C_id;
    };
    ALIVE_ASSERT_SIZEOF(Header, 0x10);

    class RawHandle
    {
    public:
        RawHandle() : mResource(nullptr) { }
        explicit RawHandle(BYTE* res) : mResource(res) {}
        Header* GetHeader() { return reinterpret_cast<Header*>((mResource - sizeof(Header))); }
        bool Valid() const { return mResource != nullptr; }
    private:
        BYTE* mResource = nullptr;
    };

    class BaseHandle
    {
    public:
        BaseHandle() : mResource(nullptr) { }
        explicit BaseHandle(BYTE** res) : mResource(res) {}
        Header* GetHeader() { return reinterpret_cast<Header*>((*mResource - sizeof(Header))); }
        RawHandle ToRawHandle() { return RawHandle(*mResource); }
        bool Valid() const { return mResource != nullptr; }
        void Clear() { mResource = nullptr; }
        BYTE** RawResource() { return mResource; }
    protected:
        BYTE** mResource;
    };

    template<class T>
    class Handle : public BaseHandle
    {
    public:
        using BaseHandle::BaseHandle;

        T operator ->()
        {
            // TODO: Can assert on GetHeader()->field_8_type matching T
            return reinterpret_cast<T>(*mResource);
        }

        T operator & ()
        {
            return reinterpret_cast<T>(*mResource);
        }
    };

    struct ResourcesToLoadList_Entry
    {
        DWORD field_0_type;
        DWORD field_4_res_id;
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList_Entry, 0x8);

    struct ResourcesToLoadList
    {
        int field_0_count;
        ResourcesToLoadList_Entry field_4_items[1];
    };
    ALIVE_ASSERT_SIZEOF(ResourcesToLoadList, 12);


    using TLoaderFn = std::add_pointer<void CC(Camera*)>::type;

    struct ResourceManager_FilePartRecord_18
    {
        DWORD field_0_type;
        DWORD field_4_id;
        Camera *field_8_pCamera;
        Camera* field_C_fn_arg_pCamera;
        TLoaderFn field_10_pFn;
        __int16 field_14_bAddUseCount;
        __int16 field_16;
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FilePartRecord_18, 0x18);

    struct ResourceManager_FileRecord_1C
    {
        char* field_0_fileName;
        ResourcesToLoadList* field_4_pResourcesToLoadList;
        DWORD field_8_type;
        DWORD field_C_id;
        DynamicArrayT<ResourceManager_FilePartRecord_18> field_10_file_sections_dArray;

        EXPORT void dtor_464EA0();
    };
    ALIVE_ASSERT_SIZEOF(ResourceManager_FileRecord_1C, 0x1C);

    EXPORT void Ctor_464910();
    EXPORT void dtor_4649B0(signed int flags);
    EXPORT void dtor_4649E0();
    EXPORT void vLoadFile_StateMachine_464A70();
    EXPORT void OnResourceLoaded_464CE0();
    EXPORT void LoadResource_464EE0(const char* pFileItem, DWORD type, DWORD resourceID, Camera* pCamera, Camera* pFnArg, TLoaderFn pFn, __int16 bAddUseCount);
    EXPORT void LoadResourcesFromList_465150(const char* pFileName, ResourcesToLoadList* pTypeAndIdList, Camera* pCamera, Camera* pFnArg, TLoaderFn pFn, __int16 addUseCount);
    EXPORT void LoadResourceFile_465460(const char* filename, Camera* pCam, Camera* a4, TLoaderFn pFn, __int16 a6);
    EXPORT void LoadingLoop_465590(__int16 bShowLoadingIcon);
    EXPORT void Shutdown_465610();
    EXPORT void Free_Resources_For_Camera_4656F0(const Camera* pCamera);

    virtual void VDestructor(signed int) override;
    virtual void VUpdate() override;
    virtual void VScreenChanged() override;

    EXPORT void VScreenChanged_464EC0();

    ResourceManager();

    struct ResourceHeapItem
    {
        BYTE* field_0_ptr;
        ResourceHeapItem* field_4_pNext;
    };
    ALIVE_ASSERT_SIZEOF(ResourceHeapItem, 0x8);

    enum BlockAllocMethod : int
    {
        eFirstMatching = 0,
        eNearestMatching = 1,
        eLastMatching = 2
    };

    EXPORT static void CC Init_49BCE0();
    EXPORT static ResourceHeapItem* CC Push_List_Item_49BD70();
    EXPORT static void CC Pop_List_Item_49BD90(ResourceHeapItem* pListItem);
    EXPORT static ResourceHeapItem* CC Split_block_49BDC0(ResourceHeapItem* pItem, int size);
    EXPORT static int CC SEQ_HashName_49BE30(const char* seqFileName);
    static BYTE** Alloc_New_Resource_Impl(DWORD type, DWORD id, DWORD size, bool locked, ResourceManager::BlockAllocMethod allocType);
    EXPORT static BYTE** CC Alloc_New_Resource_49BED0(DWORD type, DWORD id, DWORD size);
    EXPORT static BYTE** CC Allocate_New_Locked_Resource_49BF40(DWORD type, DWORD id, DWORD size);
    EXPORT static BYTE** CC Allocate_New_Block_49BFB0(int sizeBytes, BlockAllocMethod allocMethod);
    EXPORT static int CC LoadResourceFile_49C130(const char* filename, TLoaderFn pFn, Camera* a4, Camera* pCamera);
    EXPORT static signed __int16 CC LoadResourceFile_49C170(const char* pFileName, Camera* pCamera);
    EXPORT static signed __int16 CC Move_Resources_To_DArray_49C1C0(BYTE** ppRes, DynamicArray* pArray);
    EXPORT static BYTE** CC GetLoadedResource_49C2A0(DWORD type, DWORD resourceID, unsigned __int16 addUseCount, unsigned __int16 bLock);
    EXPORT static void CC Inc_Ref_Count_49C310(BYTE **ppRes);
    EXPORT static signed __int16 CC FreeResource_49C330(BYTE** handle);
    EXPORT static signed __int16 CC FreeResource_Impl_49C360(BYTE* handle);
    EXPORT static Header* CC Get_Header_49C410(BYTE** ppRes);
    EXPORT static void CC Reclaim_Memory_49C470(unsigned int size);
    EXPORT static void CC Increment_Pending_Count_49C5F0();
    EXPORT static void CC Decrement_Pending_Count_49C610();
    EXPORT static void CC Set_Header_Flags_49C650(BYTE** ppRes, __int16 flags);
    EXPORT static void CC Free_Resource_Of_Type_49C6B0(DWORD type);
    EXPORT static void CC NoEffect_49C700();

    // Helper to avoid casting raw types
    template<class T>
    static Handle<T> CC Allocate_New_Block_49BFB0_T(int sizeBytes, BlockAllocMethod allocMethod)
    {
        BYTE** block = Allocate_New_Block_49BFB0(sizeBytes, allocMethod);
        return Handle<T>(block);
    }

private:

    enum LoadingStates : __int16
    {
        State_Wait_For_Load_Request = 0,
        State_Allocate_Memory_For_File = 1,
        State_Seek_To_File = 2,
        State_Read_Sectors_ASync = 3,
        State_Wait_For_Read_Complete = 4,
        State_File_Read_Completed = 5,
        State_Load_Completed = 6
    };

    DynamicArrayT<ResourceManager_FileRecord_1C> field_20_files_pending_loading;
    ResourceManager_FileRecord_1C* field_2C_pFileItem;
    int field_30_start_sector;
    int field_34_num_sectors;
    BYTE** field_38_ppRes;
    Header* field_3C_pLoadingHeader;
    char field_40_seek_attempts;
    LoadingStates field_42_state;
    CdlLOC field_44_cdLoc;
    DynamicArrayT<BYTE*> field_48_dArray;
};
ALIVE_ASSERT_SIZEOF(ResourceManager, 0x54);

ALIVE_VAR_EXTERN(ResourceManager*, pResourceManager_5C1BB0);
ALIVE_VAR_EXTERN(DWORD, sManagedMemoryUsedSize_AB4A04);
ALIVE_VAR_EXTERN(DWORD, sPeakedManagedMemUsage_AB4A08);
ALIVE_VAR_EXTERN(short, sbLoadingInProgress_5C1B96);
ALIVE_VAR_EXTERN(short, word_5C1BAA);
ALIVE_VAR_EXTERN(int, dword_5C1BAC);

enum ResourceID
{
    kAbebasicResID = 0xA,
    kAbepullResID = 0xB,
    kAbepickResID = 0xC,
    kAbebombResID = 0xD,
    kAbethrowResID = 0xE,
    kAbewaspResID = 0x10,
    kAberingResID = 0x11,
    kAbesmashResID = 0x13,
    kAbefallResID = 0x14,
    kAbestoneResID = 0x15,
    kAbecrushResID = 0x16,
    kAbeblowResID = 0x19,
    kAbeknbkResID = 0x1A,
    kAbeknfdResID = 0x1B,
    kAbeknokzResID = 0x1C,
    kAbeyawnResID = 0x1F,
    kAberubResID = 0x20,
    kAbesizeResID = 0x21,
    kAbebordResID = 0x22,
    kAbeeyesResID = 0x23,
    kAbebuttResID = 0x24,
    kAbenutsResID = 0x25,
    kAbepnlkResID = 0x26,
    kAbewizzResID = 0x27,
    kAbetalkResID = 0x28,
    kAbehoistResID = 0x2A,
    kAbeedgeResID = 0x2B,
    kAbeswingResID = 0x2C,
    kAbedoorResID = 0x2D,
    kAbewellResID = 0x2F,
    kAbeommResID = 0x30,
    kAbeliftResID = 0x35,
    kAbebsic1ResID = 0x37,
    kDovbasicResID = 0x3C,
    kWaspResID = 0x3D,
    kHintflyResID = 0x3E,
    kBatBasicResID = 0x3F,
    kFireFlyResID = 0x40,
    kDovshootResID = 0x42,
    kAbemorphResID = 0x75,
    kAbegasResID = 0x76,
    kShrmorphResID = 0x79,
    kAbespeakResID = 0x82,
    kAbeintroResID = 0x83,
    kAbespek2ResID = 0x84,
    kHappyiconResID = 0xC9,
    kAngryiconResID = 0xCA,
    kNormaliconResID = 0xCB,
    kBgexpldResID = 0x12C,
    kExplo2ResID = 0x12D,
    kAlrmFlarResID = 0x12E,
    kDustResID = 0x12F,
    kHubFireResID = 0x130,
    kVaporResID = 0x131,
    kSlurgResID = 0x132,
    kGreeterResID = 0x133,
    kOmmflareResID = 0x138,
    kPortalTerminator = 0x139,
    kSparksResID = 0x13A,
    kSpotliteResID = 0x13C,
    kSkiddustResID = 0x13D,
    kSmokeResID = 0x13E,
    kBigflashResID = 0x13F,
    kWorkwhelResID = 0x140,
    kHoneyResID = 0x151,
    kWellLeafResID = 0x155,
    kSquibResID = 0x157,
    kZflashResID = 0x158,
    kFxflareResID = 0x15C,
    kDeathFlareResID = 0x15D,
    kAberockResID = 0x15E,
    kPortliteResID = 0x15F,
    kPortlitResID = 0x161,
    kSquibSmokeResID = 0x162,
    kSplineResID = 0x163,
    kDrpRockResID = 0x165,
    kStickResID = 0x166,
    kShellResID = 0x168,
    kGlowResID = 0x169,
    kDoaexploResID = 0x16B,
    kMetalResID = 0x16D,
    kBloodropResID = 0x16E,
    kOptflareResID = 0x16F,
    kWdropResID = 0x170,
    kSplashResID = 0x171,
    kAntResID = 0x172,
    kDanliteResID = 0x173,
    kSmallExplo2ResID = 0x174,
    kSlgbasicResID = 0x19C,
    kSlgsleepResID = 0x19D,
    kSlgknbkResID = 0x19E,
    kSlgedgeResID = 0x19F,
    kSlgsmashResID = 0x1A0,
    kSlgzshotResID = 0x1A1,
    kSlgknfdResID = 0x1A2,
    kSlgleverResID = 0x1A3,
    kSlgliftResID = 0x1A4,
    kSligBlowResID = 0x1A7,
    kSlgbeatResID = 0x1AA,
    kFlySligResID = 0x1C2,
    kMudscrubResID = 0x1FE,
    kMudchslResID = 0x1FF,
    kMudidleResID = 0x200,
    kMudoduckResID = 0x202,
    kAbeworkResID = 0x203,
    kMudltusResID = 0x204,
    kMudbtlnkResID = 0x205,
    kMudangryResID = 0x212,
    kMudsadResID = 0x213,
    kMudwiredResID = 0x214,
    kMudblindResID = 0x215,
    kMudsickResID = 0x216,
    kMudombieResID = 0x226,
    kDogbasicResID = 0x23A,
    kDogrstnResID = 0x23B,
    kDogattkResID = 0x23C,
    kDogknfdResID = 0x23D,
    kDogidleResID = 0x23E,
    kSlogBlowResID = 0x240,
    kArjbasicResID = 0x258,
    kArjeatResID = 0x259,
    kArjclimbResID = 0x25C,
    kArjpumpResID = 0x25D,
    kArjscrchResID = 0x261,
    kArjwaspResID = 0x262,
    kArjjumpResID = 0x263,
    kArjponceResID = 0x266,
    kArjfalrkResID = 0x267,
    kArsbasicResID = 0x2BC,
    kArseatResID = 0x2BD,
    kArsprceResID = 0x2BE,
    kArsskwrResID = 0x2BF,
    kArswhirlResID = 0x2C0,
    kArshowlResID = 0x2C1,
    kArsdanceResID = 0x2C2,
    kArssackResID = 0x2C3,
    kArsdeadResID = 0x2C4,
    kArsroarResID = 0x2C5,
    kArsgrwlResID = 0x2C6,
    kArschewResID = 0x2C7,
    kArscrshResID = 0x2C9,
    kGlkbasicResID = 0x320,
    kGlukredResID = 0x339,
    kGlukgrenResID = 0x33A,
    kGlukblueResID = 0x33B,
    kGlukaquaResID = 0x33C,
    kFleechResID = 0x384,
    kRopesResID = 0x3E8,
    kPulleyResID = 0x3E9,
    kP2c2bagResID = 0x3EA,
    kP5c1wlkResID = 0x3EB,
    kP6c1trapResID = 0x3EC,
    kBombResID = 0x3ED,
    kGrenflshResID = 0x3EE,
    kP4c5hiveResID = 0x3F0,
    kSwitchResID = 0x3F1,
    kBombflshResID = 0x3F3,
    kP2c3HiveResID = 0x3F5,
    kPullringResID = 0x3F6,
    kP6c4tubeResID = 0x3F9,
    kP7c1spikResID = 0x3FA,
    kHammerResID = 0x3FB,
    kCrystalResID = 0x406,
    kLiftResID = 0x408,
    kLandmineResID = 0x40C,
    kTbombResID = 0x40D,
    kBGStarResID = 0x40F,
    kMineflshResID = 0x410,
    kP1c3tubeResID = 0x41A,
    kDebrisID00 = 0x451,
    kTubeResID = 0x4B0,
    kF2zballResID = 0x7D1,
    kF2stnbalResID = 0x7D2,
    kF2stnsckResID = 0x7D3,
    kChimeResID = 0x7D5,
    kF2eyeorbResID = 0x7D8,
    kMaimGameResID = 0x7D6,
    kF2rockResID = 0x7D7,
    kPupilResID = 0x7D9,
    kTriggerResID = 0x7DA,
    kF2p3dorResID = 0x7DC,
    kFlintLockResID = 0x7E1,
    kRockdoorResID = 0x7E2,
    kHubdoorResID = 0x7E3,
    kSlamResID = 0x7E4,
    kGourdResID = 0x7EC,
    kMeatResID = 0x7E7,
    kF2ChimeBallResID = 0x7E8,
    kF2ChimeRackResID = 0x7E9,
    kWebResID = 0x7F2,
    kRockShadowResID = 0x7F3,
    kTestanimResID = 0x7D0,
    kMovebombResID = 0xBBE,
    kD2elumResID = 0xFA2,
    kElecwallResID = 0x1770,
    kMotionResID = 0x1771,
    kMflareResID = 0x1772,
    kMeatsawResID = 0x1773,
    kGrenadeResID = 0x1775,
    kR1sdoorResID = 0x1776,
    kR1buttonResID = 0x1778,
    kR1bpipeResID = 0x1779,
    kTwnkleResID = 0x1B64,
    kControlsResID = 0x1F40,
    kDoorResID = 0x1F41,
    kHighliteResID = 0x1F42,
    kHighlitePalResID = 0x1F43,
    kLoadingResID = 0x1F4A,
};
