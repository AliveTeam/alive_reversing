#pragma once

#include "../AliveLibCommon/FunctionFwd.hpp"


struct SoundBlockInfo;
struct OpenSeqHandle;
struct SfxDefinition;
class Camera;

struct SeqIds final
{
    s16 ids[16];
};

class IMidiVars
{
public:
    ~IMidiVars()
    { }
    virtual SeqIds& sSeq_Ids_word() = 0;
    virtual u16& sSnd_ReloadAbeResources() = 0;
    virtual OpenSeqHandle*& sSeqDataTable() = 0;
    virtual s16& sSeqsPlaying_count_word() = 0;
    virtual SoundBlockInfo*& sLastLoadedSoundBlockInfo() = 0;
    virtual s16& sSFXPitchVariationEnabled() = 0;
    virtual s16& sNeedToHashSeqNames() = 0;
    virtual SoundBlockInfo& sMonkVh_Vb() = 0;
    virtual s32 MidiTableSize() = 0;

    // Res manager wrapper
    virtual s16 FreeResource_Impl(u8* handle) = 0;
    virtual u8** GetLoadedResource(u32 type, u32 resourceID, u16 addUseCount, u16 bLock) = 0;
    virtual s16 FreeResource(u8** handle) = 0;
    virtual u8** Allocate_New_Locked_Resource(u32 type, u32 id, u32 size) = 0;
    virtual void LoadingLoop(s16 bShowLoadingIcon) = 0;
    virtual void Reclaim_Memory(u32 size) = 0;
    virtual u8** Alloc_New_Resource(u32 type, u32 id, u32 size) = 0;
    virtual s16 LoadResourceFile(const char_type* pFileName, Camera* pCamera) = 0;
};

EXPORT IMidiVars* GetMidiVars();
EXPORT void SetMidiApiVars(IMidiVars* pVars);

using TReclaimMemoryFn = void(CC*)(u32);
using TLoadResourceFileFn = s16(CC*)(const s8*, Camera*);
using TGetLoadedResourceFn = u8**(CC*) (u32, u32, u16, u16);
using TSNDRestart = void(CC*)();
using TSNDStopAll = void(CC*)();

// So AO can redirect SND_StopAll_4CB060 to its own func when called from SYS_ funcs
EXPORT void SND_StopAll_SetCallBack(TSNDStopAll cb);

// So AO can redirect SND_Restart_4CB0E0 to its own func when called from SYS_ funcs
EXPORT void SND_Restart_SetCallBack(TSNDRestart cb);


EXPORT void CC SND_Load_Seqs_Impl(OpenSeqHandle* pSeqTable, const char_type* bsqFileName);

EXPORT void SND_Stop_All_Seqs_4CA850();

EXPORT void CC SND_StopAll_4CB060();
EXPORT void CC SND_Init_4CA1F0();
EXPORT void CC SND_Shutdown_4CA280();
EXPORT void CC SND_Stop_Channels_Mask_4CA810(u32 bitMask);
EXPORT void SND_Reset_4C9FB0();
EXPORT void CC SND_Load_VABS_4CA350(SoundBlockInfo* pSoundBlockInfo, s32 reverb);
EXPORT void CC SND_Load_Seqs_4CAED0(OpenSeqHandle* pSeqTable, const char_type* bsqFileName);
EXPORT void CC SND_SEQ_Stop_4CAE60(u16 idx);
EXPORT s8 CC SND_Seq_Table_Valid_4CAFE0();
EXPORT s16 CC SND_SEQ_PlaySeq_4CA960(u16 idx, s16 repeatCount, s16 bDontStop);
EXPORT void CC SND_SEQ_SetVol_4CAD20(s32 idx, s16 volLeft, s16 volRight);
EXPORT s16 CC SND_SEQ_Play_4CAB10(u16 idx, s16 repeatCount, s16 volLeft, s16 volRight);
EXPORT s32 CC SND_SsIsEos_DeInlined_4CACD0(u16 idx);
EXPORT s32 CC SFX_SfxDefinition_Play_4CA700(const SfxDefinition* sfxDef, s16 volLeft, s16 volRight, s16 pitch_min, s16 pitch_max, s32 pan);
EXPORT s32 CC SFX_SfxDefinition_Play_4CA420(const SfxDefinition* sfxDef, s16 volume, s16 pitch_min, s16 pitch_max);

enum SeqId
{
    MainMenuAmbient_0 = 0,
    MinesAmbient_1 = 1,
    MudomoAmbient_2 = 2,
    MudancheeAmbient_3 = 3,
    FeecoAmbient_4 = 4,
    BreweryAmbient_5 = 5,
    BonewerkzAmbient_6 = 6,
    BarracksAmbient_7 = 7,
    NecrumAmbient1_8 = 8,
    HitBottomOfDeathPit_9 = 9,
    MudokonChant1_10 = 10,
    MudokonChant_11 = 11,
    Scratching_12 = 12,
    Empty_13 = 13,
    Explosion1_14 = 14,
    Explosion2_15 = 15,
    AbeOops_16 = 16,
    NecrumAmbient2_17 = 17,
    AbeStopIt_18 = 18,
    WheelSqueak_19 = 19,
    MainMenuParamiteAttack_20 = 20,
    WalkingSligOffscreen_21 = 21,
    SleepingSligOffscreen_22 = 22,
    AwakeSlogOffscreen_23 = 23,
    SleepingSlogOffscreen_24 = 24,
    ParamiteNearby_25 = 25,
    SleepingFleechOffscreen_26 = 26,
    ToCalm1_27 = 27, //Unused, from Oddysee
    ToCalm2_28 = 28, //Unused, from Oddysee
    DeathDrums_29 = 29,
    AbeDeathDrums_30 = 30,
    SaveTriggerMusic_31 = 31,
    SecretMusic_32 = 32,

    //Tension1 / Chase1 are used for Sacred Beasts and Sligs
    //Tension2 / Chase2 are used for Fleeches and Slogs
    //SafeTension is used when Slogs jump up for Abe

    NecrumCalm3_33 = 33,
    NecrumCalm2_34 = 34,
    NecrumCalm1_35 = 35,
    NecrumTension1_36 = 36,
    NecrumPossess_37 = 37,
    NecrumChase1_38 = 38,
    NecrumTension2_39 = 39,
    NecrumChase2_40 = 40,
    NecrumSafeTension_41 = 41, //Unused - no slogs

    MudomoCalm3_42 = 42,
    MudomoCalm2_43 = 43,
    MudomoCalm1_44 = 44,
    MudomoTension1_45 = 45,
    MudomoChase1_46 = 46,
    MudomoPossess_47 = 47,
    MudomoTension2_48 = 48,
    MudomoChase2_49 = 49,
    MudomoSafeTension_50 = 50, //Unused - no slogs

    MudomoEnderCalm_51 = 51,
    MudomoEnderTension1_52 = 52,
    MudomoEnderPossess_53 = 53,
    MudomoEnderChase1_54 = 54,
    MudomoEnderTension2_55 = 55,
    MudomoEnderChase2_56 = 56,
    MudomoEnderSafeTension_57 = 57, //Unused - no slogs

    MudancheeCalm3_58 = 58,
    MudancheeCalm2_59 = 59,
    MudancheeCalm1_60 = 60,
    MudancheeTension1_61 = 61,
    MudancheeChase1_62 = 62,
    MudancheePossess_63 = 63,
    MudancheeTension2_64 = 64,
    MudancheeChase2_65 = 65,
    MudancheeSafeTension_66 = 66, //Unused - no slogs

    MudancheeEnderCalm_67 = 67,
    MudancheeEnderTension1_68 = 68,
    MudancheePossess_69 = 69,
    MudancheeEnderChase1_70 = 70,
    MudancheeTension2_71 = 71,
    MudancheeEnderChase2_72 = 72,
    MudancheeEnderSafeTension_73 = 73,

    FeecoCalm3_74 = 74,
    FeecoCalm2_75 = 75,
    FeecoCalm1_76 = 76,
    FeecoTension1_77 = 77,
    FeecoPossess_78 = 78,
    FeecoChase1_79 = 79,
    FeecoTension2_80 = 80,
    FeecoChase2_81 = 81,
    FeecoSafeTension_82 = 82,

    FeecoEnderCalm_83 = 83,
    FeecoEnderTension_84 = 84,
    FeecoEnderPossess_85 = 85,
    FeecoEnderChase_86 = 86,

    BarracksCalm3_87 = 87,
    BarracksCalm2_88 = 88,
    BarracksCalm1_89 = 89,
    BarracksTension_90 = 90,
    BarracksPossess_91 = 91,
    BarracksChase_92 = 92,

    BarracksEnderCalm_93 = 93,
    BarracksEnderTension_94 = 94,
    BarracksEnderPossess_95 = 95,
    BarracksEnderChase_96 = 96,

    BonewerkzCalm3_97 = 97,
    BonewerkzCalm2_98 = 98,
    BonewerkzCalm1_99 = 99,
    BonewerkzTension1_100 = 100,
    BonewerkzPossess_101 = 101,
    BonewerkzChase1_102 = 102,
    BonewerkzTension2_103 = 103,
    BonewerkzChase2_104 = 104,
    BonewerkzSafeTension_105 = 105,

    BonewerkzEnderCalm_106 = 106,
    BonewerkzEnderTension1_107 = 107,
    BonewerkzEnderPossess_108 = 108,
    BonewerkzEnderChase1_109 = 109,
    BonewerkzEnderTension2_110 = 110,
    BonewerkzEnderChase2_111 = 111,
    BonewerkzEnderSafeTension_112 = 112,

    BreweryCalm3_113 = 113,
    BreweryCalm2_114 = 114,
    BreweryCalm1_115 = 115,
    BreweryTension1_116 = 116,
    BreweryPossess_117 = 117,
    BreweryChase1_118 = 118,
    BreweryTension2_119 = 119,
    BreweryChase2_120 = 120,
    BrewerySafeTension_121 = 121,

    BoilerCalm3_122 = 122, //Unused
    BoilerCalm2_123 = 123,
    BoilerCalm1_124 = 124,
    BoilerTension_125 = 125,
    BoilerPossess_126 = 126,
    BoilerChase_127 = 127,

    MinesCalm3_128 = 128,
    MinesCalm2_129 = 129,
    MinesCalm1_130 = 130,
    MinesTension1_131 = 131,
    MinesPossess_132 = 132,
    MinesChase1_133 = 133,

    MenuMainTheme1_134 = 134,
    MenuMainTheme2_135 = 135,
    MenuMainTheme3_136 = 136,
    MenuBeginTheme2_137 = 137, //Unused
    MenuSubTheme_138 = 138,
    MenuBeginTheme1_139 = 139,

    MinesTension2_140 = 140,
    MinesChase2_141 = 141,
    MinesSafeTension_142 = 142
};
