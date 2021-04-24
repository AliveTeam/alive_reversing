#include "config.h"
#include "logger.hpp"
#include "FunctionFwd.hpp"
#include "SDL_main.h"
#include "Io.hpp"
#include "Sys.hpp"
#include "../AliveLibAE/Sound/PsxSpuApi.hpp"
#include "../AliveLibAE/LvlArchive.hpp"
#include <string>

bool CC RunningAsInjectedDll()
{
    return false;
}

static std::vector<u8> ReadLvlFile(LvlArchive& archive, const s8* fileName)
{
    LvlFileRecord* pFileRec = archive.Find_File_Record_433160(fileName);
    if (!pFileRec)
    {
        ALIVE_FATAL("Failed find file in lvl");
    }
    std::vector<u8> fileContent;
    fileContent.resize(pFileRec->field_10_num_sectors * 2048);
    sLvlArchive_5BC520.Read_File_4330A0(pFileRec, fileContent.data());
    fileContent.resize(pFileRec->field_14_file_size);
    return fileContent;
}

static void WriteVec(const s8* fileName, const std::vector<u8>& vec)
{
    auto hFile = fopen(fileName, "wb");
    if (hFile)
    {
        fwrite(vec.data(), 1, vec.size(), hFile);
        fclose(hFile);
    }
}

s32 CC SND_SoundsDat_Get_Sample_Len_4FC400(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx);
BOOL CC sub_4FC470(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx);
DWORD* CC SND_SoundsDat_Get_Sample_Offset_4FC3D0(VabHeader* pVabHeader, VabBodyRecord* pBodyRecords, s32 idx);

s32 CC SND_SoundsDat_Read(FILE* file, VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx, void* pBuffer)
{
    const s32 sampleOffset = *SND_SoundsDat_Get_Sample_Offset_4FC3D0(pVabHeader, pVabBody, idx); // = field_8_fileOffset
    const s32 sampleLen = SND_SoundsDat_Get_Sample_Len_4FC400(pVabHeader, pVabBody, idx);
    if (sampleOffset == -1)
    {
        return 0;
    }

    fseek(file, sampleOffset, 0);
    fread(pBuffer, 1, sampleLen * 2, file);

    return sampleLen;
}


static void PCToPsxVab(bool isAe, const s8* lvlName, const s8* vhName, const s8* vbName)
{
    ResourceManager::Init_49BCE0();

    LvlArchive archive;

    if (!archive.Open_Archive_432E80(lvlName))
    {
        ALIVE_FATAL("Failed to open lvl");
    }

    auto vhData = ReadLvlFile(archive, vhName);
    auto pHeader = reinterpret_cast<VabHeader*>(vhData.data());
    WriteVec(vhName, vhData);

    auto vbData = ReadLvlFile(archive, vbName);
    auto pBody = reinterpret_cast<VabBodyRecord*>(vbData.data());

    s32 vagCount = pHeader->field_16_num_vags;
    if (vagCount > 255)
    {
        vagCount = 255;
    }

    if (isAe)
    {
        auto hDat = fopen("sounds.dat", "rb");
        if (!hDat)
        {
            ALIVE_FATAL("Failed to open sounds.dat");
        }

        for (s32 i = 0; i < vagCount; i++)
        {
            s32 sampleLen = SND_SoundsDat_Get_Sample_Len_4FC400(pHeader, pBody, i);
            if (sampleLen < 4000 && !sub_4FC470(pHeader, pBody, i))
            {
                sampleLen *= 2;
            }
            sampleLen = pBody->field_0_length_or_duration;

            std::vector<u8> tempBuffer;
            tempBuffer.resize(sampleLen);
           // SND_SoundsDat_Read(hDat, pHeader, pBody, i, tempBuffer.data());

            fseek(hDat, pBody->field_8_fileOffset, SEEK_SET);
            fread(tempBuffer.data(), 1, sampleLen , hDat);

            const std::string name = std::string(vbName) + "_" + std::to_string(i) + ".raw";
            WriteVec(name.c_str(), tempBuffer);
        }
        fclose(hDat);
    }

    archive.Free_433130();
}

s32 main(s32 /*argc*/, s8** /*argv*/)
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
#endif

    LOG_INFO("Test");

    PCToPsxVab(true, "ST.LVL", "MONK.VH", "MONK.VB");

    return 0;
}
