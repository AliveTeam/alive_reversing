#ifdef _WIN32
#include <windows.h>
#endif
#include "../../relive_lib/relive_config.h"
#include "logger.hpp"
#include "../../relive_lib/Function.hpp"
#include "SDL_main.h"
#include "../../AliveLibAE/Io.hpp"
#include "../../AliveLibAE/Sys.hpp"
#include "../../AliveLibAE/Sound/PsxSpuApi.hpp"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "GL/glew.h"
#include "../../AliveLibAE/GameAutoPlayer.hpp"

#include <string>

// TODO: This tool is broken till its updated to use the converted lvl data

BaseGameAutoPlayer& GetGameAutoPlayer()
{
    // Use the AE object, doesn't matter for this tool
    static GameAutoPlayer autoPlayer;
    return autoPlayer;
}

/*
static std::vector<u8> ReadLvlFile(LvlArchive& archive, const char_type* fileName)
{
    LvlFileRecord* pFileRec = archive.Find_File_Record_433160(fileName);
    if (!pFileRec)
    {
        ALIVE_FATAL("Failed find file in lvl");
    }
    std::vector<u8> fileContent;
    fileContent.resize(pFileRec->field_10_num_sectors * 2048);
    //sLvlArchive_5BC520.Read_File_4330A0(pFileRec, fileContent.data());
    fileContent.resize(pFileRec->field_14_file_size);
    return fileContent;
}


static void WriteVec(const char_type* fileName, const std::vector<u8>& vec)
{
    auto hFile = fopen(fileName, "wb");
    if (hFile)
    {
        fwrite(vec.data(), 1, vec.size(), hFile);
        fclose(hFile);
    }
}

s32 SND_SoundsDat_Get_Sample_Len_4FC400(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx);
bool sub_4FC470(VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx);
u32* SND_SoundsDat_Get_Sample_Offset_4FC3D0(VabHeader* pVabHeader, VabBodyRecord* pBodyRecords, s32 idx);

s32 SND_SoundsDat_Read(FILE* file, VabHeader* pVabHeader, VabBodyRecord* pVabBody, s32 idx, void* pBuffer)
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


void PCToPsxVab(bool isAe, const char_type* lvlName, const char_type* vhName, const char_type* vbName)
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
            fread(tempBuffer.data(), 1, sampleLen, hDat);

            const std::string name = std::string(vbName) + "_" + std::to_string(i) + ".raw";
            WriteVec(name.c_str(), tempBuffer);
        }
        fclose(hDat);
    }

    archive.Free_433130();
}
*/

class Sequencer final
{
public:
    Sequencer()
    {
       // settings = new_fluid_settings();

       // synth = new_fluid_synth(settings);

    }

    ~Sequencer()
    {
      //  for (fluid_player_t* player : mSeqPlayers)
      //  {
       //     delete_fluid_player(player);
       // }
       // delete_fluid_synth(synth);
       // delete_fluid_settings(settings);
    }

    bool LoadSoundFont(const char* )
    {
        /*
        sfont_id = fluid_synth_sfload(synth, fileName, 1);
        if (sfont_id == FLUID_FAILED)
        {
            LOG_ERROR("Failed to load sound font");
            return false;
        }*/
        return true;
    }

    void PlaySeq(const char* )
    {
        /*
        fluid_player_t* player = new_fluid_player(synth);
        if (fluid_player_add(player, fileName) != FLUID_OK)
        {
            LOG_ERROR("Failed to open midi");
        }
        fluid_player_play(player);
        mSeqPlayers.push_back(player);
        */
    }

    static void AudioCallBackStatic(void* userdata, Uint8* stream, int len)
    {
        reinterpret_cast<Sequencer*>(userdata)->RenderAudio(stream, len);
    }

    void RenderAudio(Uint8* , int )
    {
       // fluid_synth_write_s16(synth, lenBytes / (2 * sizeof(short)), stream, 0, 2, stream, 1, 2);
        
        //done = fluid_player_get_status(player1) == FLUID_PLAYER_DONE;
        //done |= fluid_player_get_status(player2) == FLUID_PLAYER_DONE;
    }

private:
    //fluid_settings_t* settings = nullptr;
    //fluid_synth_t* synth = nullptr;
    //int sfont_id = FLUID_FAILED;
    //std::vector<fluid_player_t*> mSeqPlayers;
};


void main_loop()
{
    SDL_Window* window = NULL;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow(
        "sound test",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
    );

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create context
    SDL_GLContext mContext = SDL_GL_CreateContext(window);
    glewInit();

    if (mContext == NULL)
    {
        LOG_ERROR("OpenGL context could not be created! SDL Error: %s", SDL_GetError());
        return;
    }
    else
    {
        // Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            LOG_ERROR("Error initializing GLEW! %s", glewGetErrorString(glewError));
        }

        // Use Vsync
        if (SDL_GL_SetSwapInterval(1) < 0)
        {
            LOG_ERROR("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
        }
    }

    ImGui::CreateContext();

    // Setup IMGUI for texture debugging
    ImGui_ImplSDL2_InitForOpenGL(window, mContext);
    ImGui_ImplOpenGL3_Init("#version 150");

    Sequencer seq;
    seq.LoadSoundFont("C:\\Users\\paul\\Downloads\\Abe2MidiPlayer\\oddworld.sf2");

    SDL_AudioSpec mAudioDeviceSpec = {};
    mAudioDeviceSpec.callback = Sequencer::AudioCallBackStatic;
    mAudioDeviceSpec.format = AUDIO_S16;
    mAudioDeviceSpec.channels = 2;
    mAudioDeviceSpec.freq = 44100;
    mAudioDeviceSpec.samples = 2048;
    mAudioDeviceSpec.userdata = &seq;

    if (SDL_OpenAudio(&mAudioDeviceSpec, NULL) < 0)
    {
        LOG_ERROR("Couldn't open SDL audio: %s", SDL_GetError());
        return;
    }
    SDL_PauseAudio(0);

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        ImGui::Begin("Test");

        for (int i = 0; i < 27; i++)
        {
            std::string name("PS1 SEQ.mid");
            if (i != 0)
            {
                name = std::to_string(i) + name;
            }

            if (ImGui::Button(name.c_str()))
            {
                seq.PlaySeq(("C:\\Users\\paul\\Downloads\\Abe2MidiPlayer\\midi\\" + name).c_str());
            }
        }

        ImGui::End();

        ImGui::EndFrame();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window);
        SDL_Delay(10);
    }
    ImGui_ImplSDL2_Shutdown();

    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(window);

    SDL_PauseAudio(1);
    SDL_QuitSubSystem(SDL_INIT_AUDIO);

    SDL_Quit();
    return;
}

s32 main(s32 /*argc*/, char_type** /*argv*/)
{
#if _WIN32
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::SetConsoleTitleA("Debug Console");
    ::SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    RedirectIoStream(true);
#endif

    //LOG_INFO("fluid synth version: " << fluid_version_str());

    main_loop();

    //PCToPsxVab(true, "ST.LVL", "MONK.VH", "MONK.VB");

    return 0;
}
