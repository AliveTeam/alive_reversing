#include <string>
#include <functional>
#include <filesystem>
#include <thread>

// pipe to ffmpeg
#include <windows.h>
#include <process.h>

#include "Masher.hpp"

namespace fs = std::filesystem;

struct AssetFMV_Video
{
    int width;
    int height;
    int fps;
};

struct AssetFMV_Audio
{
    int audioChannels;
    int audioSampleRate;
};

struct AssetFMVParams
{
    AssetFMV_Video video;
    AssetFMV_Audio audio;
    std::string outputPath;
};

static HANDLE videoPipeWrite;
static HANDLE audioPipeWrite;
static HANDLE videoPipeRead;
static HANDLE audioPipeRead;
static HANDLE videoProcess;
static HANDLE audioProcess;

static AssetFMVParams gParameters;

static std::vector<u8> frameBuffer;
static bool gFMVHasFrames = false;
static float gFrameTime = 0;
static int gFmvIndex = -1;

static Masher* gMasher = nullptr;
static GLuint gMasherTexture = 0;

#define GL_TO_IMGUI_TEX(v) *reinterpret_cast<ImTextureID*>(&v)

static bool                 g_bHasAudio_5CA234;
static s32                  g_fmv_audio_sample_offset_5CA238;
static s32                  g_fmv_num_read_frames_5CA23C;
static bool                 g_bNoAudioOrAudioError_5CA1F4;
static s32                  g_fmv_single_audio_frame_size_in_samples_5CA240;
static s32                  g_current_audio_offset_5CA1F0;
static s32                  g_fmv_num_played_audio_frames_5CA1FC;
static s32                  g_oldBufferPlayPos_5CA22C;

static std::thread gFMVExportThread;
static bool gFMVExporting = false;
static float gFMVExportProgress = 0;
static std::string gFMVExportMessage = "";
static bool gFMVShowExportPreDialog = false;

HANDLE CreateProcessWithPipe(std::string& command, HANDLE* pPipeWrite, HANDLE* pPipeRead)
{
    SECURITY_ATTRIBUTES sa = { sizeof(sa) };
    sa.lpSecurityDescriptor = nullptr;
    sa.bInheritHandle = TRUE;

    CreatePipe(pPipeRead, pPipeWrite, &sa, 0);

    SetHandleInformation(*pPipeWrite, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si = { sizeof(si) };
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = *pPipeRead;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    PROCESS_INFORMATION pi = {};

    CreateProcess(nullptr, (LPSTR)command.c_str(), nullptr, nullptr, TRUE, 0, nullptr, nullptr, &si, &pi);

    return pi.hProcess;
}

int ffmpeg_begin(AssetFMVParams paramters)
{
    gParameters = paramters;
    std::string pix_fmt = "rgba";
    std::string video_size = std::to_string(paramters.video.width) + "x" + std::to_string(paramters.video.height);
    std::string frame_rate = std::to_string(paramters.video.fps);
    
    std::string audio_channels = std::to_string(paramters.audio.audioChannels);
    std::string audio_sample_rate = std::to_string(paramters.audio.audioSampleRate);

    std::string args_video_input = " -f rawvideo -pix_fmt " + pix_fmt + " -thread_queue_size 64 -video_size " + video_size + " -r " + frame_rate + " -i pipe:0 -vf \"pad = ceil(iw / 2) * 2:ceil(ih / 2) * 2\" -preset medium -pix_fmt yuv420p -crf 10 ";
    std::string args_audio_input = " -f s16le -thread_queue_size 1024 -ar " + audio_sample_rate + " -ac " + audio_channels + " -i pipe:0 ";
    std::string ffmpeg_path = "ffmpeg.exe";
    std::string ffmpeg_args = args_audio_input + args_video_input + "-y " + paramters.outputPath;

    videoProcess = CreateProcessWithPipe(ffmpeg_path + args_video_input + "-y " + paramters.outputPath + ".video.mp4", &videoPipeWrite, &videoPipeRead);
    audioProcess = CreateProcessWithPipe(ffmpeg_path + args_audio_input + "-y " + paramters.outputPath + ".audio.m4a", &audioPipeWrite, &audioPipeRead);

    return 0;
}

// pushes a frame in rgba8888 format to ffmpeg
int ffmpeg_push_frame(u8* frameBuffer, int width, int height)
{
    DWORD bytesWritten;
    WriteFile(videoPipeWrite, frameBuffer, width * height * 4, &bytesWritten, nullptr);

    return 0;
}

int ffmpeg_push_audio(u8* audioBuffer, int lengthBytes)
{
    DWORD bytesWritten;
    WriteFile(audioPipeWrite, audioBuffer, lengthBytes, &bytesWritten, nullptr);

    return 0;
}

int ffmpeg_end()
{
    CloseHandle(videoPipeWrite);
    CloseHandle(audioPipeWrite);

    // wait for video process to finish
    WaitForSingleObject(videoProcess, INFINITE);

    // wait for audio process to finish
    WaitForSingleObject(audioProcess, INFINITE);

    // create a process to merge the video and audio
    std::string ffmpeg_path = "ffmpeg.exe";
    std::string ffmpeg_args = " -i " + gParameters.outputPath + ".video.mp4 -i " + gParameters.outputPath + ".audio.m4a -c:v copy -c:a aac -strict experimental -map 0:v:0 -map 1:a:0 -y " + gParameters.outputPath + ".mp4";

    videoProcess = CreateProcessWithPipe(ffmpeg_path + ffmpeg_args, &videoPipeWrite, &videoPipeRead);

    // wait for ffmpeg to finish
    WaitForSingleObject(videoProcess, INFINITE);

    // delete the temporary files
    std::filesystem::remove(gParameters.outputPath + ".video.mp4");
    std::filesystem::remove(gParameters.outputPath + ".audio.m4a");

    return 0;
}

static GLuint CreateVideoPlayerTexture()
{
    GLuint texId = 0;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return texId;
}

s8 CC Mash_DecompressAudio()
{
    if (!g_bHasAudio_5CA234)
    {
        return 1;
    }

    u32 audioBufferStartOffset = 0;
    g_fmv_audio_sample_offset_5CA238 = 0;

    // Keep reading frames till we have >= number of interleaved so that we have 1 full frame
    if (g_fmv_num_read_frames_5CA23C < gMasher->field_2C_audio_header.field_10_num_frames_interleave)
    {
        while (Masher::ReadNextFrameToMemory_4EAC30(gMasher))
        {
            const int bitsPerSample = (gMasher->field_2C_audio_header.field_0_audio_format & 2) ? 16 : 8;
            const int channels = (gMasher->field_2C_audio_header.field_0_audio_format & 1) ? 2 : 1;

            void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(gMasher);

            if (pDecompressedAudioFrame)
            {
                ffmpeg_push_audio(reinterpret_cast<u8*>(pDecompressedAudioFrame), (bitsPerSample / 8) * g_fmv_single_audio_frame_size_in_samples_5CA240 * channels);

                /*if (GetSoundAPI().SND_LoadSamples(
                    &g_fmv_sound_entry_5CA208,
                    g_fmv_audio_sample_offset_5CA238,
                    (u8*)pDecompressedAudioBuffer,
                    g_fmv_single_audio_frame_size_in_samples_5CA240))
                {
                    g_bNoAudioOrAudioError_5CA1F4 = 1;
                }*/
            }

            g_fmv_audio_sample_offset_5CA238 += g_fmv_single_audio_frame_size_in_samples_5CA240;
            audioBufferStartOffset = g_fmv_audio_sample_offset_5CA238;
            g_fmv_num_read_frames_5CA23C++;

            if (g_fmv_num_read_frames_5CA23C >= gMasher->field_2C_audio_header.field_10_num_frames_interleave)
            {
                break;
            }
        }
    }

    if (g_fmv_num_read_frames_5CA23C >= gMasher->field_2C_audio_header.field_10_num_frames_interleave)
    {
        // Update the offset to the size of the first demuxed frame
        g_current_audio_offset_5CA1F0 = audioBufferStartOffset;
        if (!g_bNoAudioOrAudioError_5CA1F4)
        {
            // Sound entry is created and populated with 1 frame, play it
            /*if (FAILED(GetSoundAPI().SND_PlayEx(&fmv_sound_entry_5CA208, 116, 116, 1.0, 0, 1, 100)))
            {
                g_bNoAudioOrAudioError_5CA1F4 = 1;
            }*/
        }
        g_fmv_num_played_audio_frames_5CA1FC = 0;
        g_oldBufferPlayPos_5CA22C = 0;
        return 1;
    }

    return 0;
}

static bool InitDDVPlayback(const std::string& filePath, bool ffmpegExport)
{
    g_bHasAudio_5CA234 = 0;
    g_fmv_audio_sample_offset_5CA238 = 0;
    g_fmv_num_read_frames_5CA23C = 0;
    g_bNoAudioOrAudioError_5CA1F4 = 0;
    g_fmv_single_audio_frame_size_in_samples_5CA240 = 0;
    g_current_audio_offset_5CA1F0 = 0;
    g_fmv_num_played_audio_frames_5CA1FC = 0;
    g_oldBufferPlayPos_5CA22C = 0;

    frameBuffer.resize(640 * 480 * 4);

    if (gMasher != nullptr)
    {
        delete gMasher;
        gMasher = nullptr;
    }

    gMasher = new Masher();

    gMasherTexture = CreateVideoPlayerTexture();

    gMasher->Init_4E6770(filePath.c_str());

    AssetFMVParams params;
    params.video.fps = 15;
    params.video.width = gMasher->field_14_video_header.field_4_width;
    params.video.height = gMasher->field_14_video_header.field_8_height;
    params.outputPath = filePath;

    params.audio.audioSampleRate = gMasher->field_2C_audio_header.field_4_samples_per_second;
    params.audio.audioChannels = (gMasher->field_2C_audio_header.field_0_audio_format & 1) ? 2 : 1;

    if (ffmpegExport)
    {
        ffmpeg_begin(params);
    }

    g_bHasAudio_5CA234 = ((u32)gMasher->field_4_ddv_header.field_4_contains >> 1) & 1;
    g_fmv_single_audio_frame_size_in_samples_5CA240 = gMasher->field_2C_audio_header.field_C_single_audio_frame_size;
    const auto fmv_sound_entry_size = g_fmv_single_audio_frame_size_in_samples_5CA240 * (gMasher->field_2C_audio_header.field_10_num_frames_interleave + 6);

    g_bNoAudioOrAudioError_5CA1F4 = 0;
    if (g_bHasAudio_5CA234 && gMasher->field_2C_audio_header.field_0_audio_format)
    {
        //if (GetSoundAPI().SND_New(
        //    &fmv_sound_entry_5CA208,
        //    fmv_sound_entry_size,
        //    pMasher_audio_header_5CA1E0->field_4_samples_per_second,
        //    (pMasher_audio_header_5CA1E0->field_0_audio_format & 2) != 0 ? 16 : 8,
        //    (pMasher_audio_header_5CA1E0->field_0_audio_format & 1) | 6)
        //    < 0)
        //{
        //    // SND_New failed
        //    fmv_sound_entry_5CA208.field_4_pDSoundBuffer = nullptr;
        //    g_bNoAudioOrAudioError_5CA1F4 = 1;
        //}
    }
    else
    {
        // Source DDV has no audio
        g_bNoAudioOrAudioError_5CA1F4 = 1;
    }

    if (Mash_DecompressAudio() && gMasher->ReadNextFrame_4E6B30() && gMasher->ReadNextFrame_4E6B30())
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool StepDDVPlayback(bool ffmpegExport)
{
    gMasher->VideoFrameDecode_Raw(frameBuffer.data());

    
    if (ffmpegExport)
    {
        ffmpeg_push_frame(frameBuffer.data(), gMasher->field_14_video_header.field_4_width, gMasher->field_14_video_header.field_8_height);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, gMasherTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gMasher->field_14_video_header.field_4_width, gMasher->field_14_video_header.field_8_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, frameBuffer.data());
    }
    
    g_fmv_num_read_frames_5CA23C++;

    if (!g_bNoAudioOrAudioError_5CA1F4)
    {
        const int bitsPerSample = (gMasher->field_2C_audio_header.field_0_audio_format & 2) ? 16 : 8;
        const int channels = (gMasher->field_2C_audio_header.field_0_audio_format & 1) ? 2 : 1;

        void* pDecompressedAudioFrame = Masher::GetDecompressedAudioFrame_4EAC60(gMasher);

        if (pDecompressedAudioFrame)
        {
            if (ffmpegExport)
            {
                ffmpeg_push_audio(reinterpret_cast<u8*>(pDecompressedAudioFrame), (bitsPerSample / 8) * g_fmv_single_audio_frame_size_in_samples_5CA240 * channels);
            }

            //// Push new samples into the buffer
            //if (GetSoundAPI().SND_LoadSamples(&fmv_sound_entry_5CA208, fmv_audio_sample_offset_5CA238, (u8*)pDecompressedAudioFrame, fmv_single_audio_frame_size_in_samples_5CA240) < 0)
            //{
            //    // Reload with data fail
            //    bNoAudioOrAudioError_5CA1F4 = 1;
            //}
        }
        else
        {
            //if (GetSoundAPI().SND_Clear(&fmv_sound_entry_5CA208, fmv_audio_sample_offset_5CA238, fmv_single_audio_frame_size_in_samples_5CA240) < 0)
            //{
            //    // Reload with silence on failure or no data
            //    bNoAudioOrAudioError_5CA1F4 = 1;
            //}
        }

        g_fmv_audio_sample_offset_5CA238 += g_fmv_single_audio_frame_size_in_samples_5CA240;
    }
    const s32 bMoreFrames = gMasher->ReadNextFrame_4E6B30();

    if (!bMoreFrames)
    {
        if (ffmpegExport)
        {
            ffmpeg_end();
        }

        return false;
    }

    return true;
}

static void StartPlayback(const std::string& filePath, bool ffmpegExport)
{
    gFrameTime = 0;
    gFMVHasFrames = InitDDVPlayback(filePath, ffmpegExport);
}

static std::string gFMVExportFilePath;
static std::vector<std::string> ddvFiles;

static void ExportDDVThreadFunc()
{
    int currentFrame = 0;
    StartPlayback(gFMVExportFilePath, true);
    while(StepDDVPlayback(true))
    {
        currentFrame++;
        gFMVExportProgress = (float)currentFrame / gMasher->field_4_ddv_header.field_C_number_of_frames;
        gFMVExportMessage = gFMVExportFilePath + "\nExporting frame " + std::to_string(currentFrame) + " of " + std::to_string(gMasher->field_4_ddv_header.field_C_number_of_frames);
    }

    gFMVExportProgress = 1;
    gFMVExportMessage = "Done!";
    gFMVExporting = false;
}

static void ExportAllDDVThreadFunc()
{
    int ddvCount = ddvFiles.size();

    for (int i = 0; i < ddvCount; i++)
    {
        const std::string& ddvFileName = ddvFiles[i];

        int currentFrame = 0;
        StartPlayback(ddvFileName, true);
        while (StepDDVPlayback(true))
        {
            currentFrame++;
            float p = ((float)currentFrame / gMasher->field_4_ddv_header.field_C_number_of_frames) / (float)ddvCount;
            gFMVExportProgress = (i * (1 / (float)ddvCount)) + p;
            gFMVExportMessage = ddvFileName + "\nExporting frame " + std::to_string(currentFrame) + " of " + std::to_string(gMasher->field_4_ddv_header.field_C_number_of_frames);
        }
    }

    gFMVExportProgress = 1;
    gFMVExportMessage = "Done!";
    gFMVExporting = false;
}

static void AppVideoViewer(ImGuiWindowFlags flags, float elapsedTime)
{
    IO_Init_494230();

    if (gFMVHasFrames && !gFMVExporting)
    {
        gFrameTime += elapsedTime;

        if (gFrameTime >= 1.0f / 15.0f)
        {
            gFrameTime = 0;
            gFMVHasFrames = StepDDVPlayback(false);
        }
    }

    ImVec2 windowPadding = ImGui::GetStyle().WindowPadding;

    std::string ddvDir = "movies/";

    if (ImGui::Begin("Video Viewer", 0, flags))
    {
        ImVec2 windowSize = ImGui::GetContentRegionAvail();
        float listBoxSize = 268.0f;
        if (ImGui::BeginChild("##fmv_list_frame", { listBoxSize , windowSize.y }, false))
        {
            static std::string searchQuery;
            ImGui::PushItemWidth(-1);
            ImGui::InputTextWithHint("##fmv_list_search", "Search...", &searchQuery);
            ImGui::PopItemWidth();

            // get list of .ddv files in movie directory
            
            
            if (ddvFiles.size() == 0)
            {
                for (const auto& entry : fs::directory_iterator(ddvDir))
                {
                    if (StringToLowerCase(entry.path().extension().string()) == ".ddv")
                    {
                        ddvFiles.push_back(entry.path().string());
                    }
                }
            }


            if (ImGui::BeginListBox("##fmv_list", ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y - windowPadding.y)))
            {
                for (auto i = 0; i < ddvFiles.size(); i++)
                {
                    const std::string& ddvFileName = ddvFiles[i];

                    if (!searchQuery.empty() && StringToLowerCase(ddvFileName).find(StringToLowerCase(searchQuery)) == std::string::npos)
                        continue;

                    if (ImGui::Selectable(ddvFileName.c_str(), i == gFmvIndex))
                    {
                        gFmvIndex = i;
                        StartPlayback(ddvFileName, false);
                    }
                }

                ImGui::EndListBox();
            }
        }
        ImGui::EndChild();

        ImGui::SameLine();

        if (ImGui::BeginChild("##fmv_preview", { windowSize.x - listBoxSize , windowSize.y }, false))
        {
            if (ImGui::Button("Export"))
            {
                if (gFmvIndex >= 0)
                {
                    gFMVExporting = true;
                    gFMVExportProgress = 0;
                    gFMVExportFilePath = ddvFiles[gFmvIndex];

                    // check if the export thread is running already and if so, detach it
                    if (gFMVExportThread.joinable())
                    {
                        gFMVExportThread.detach();
                    }

                    gFMVExportThread = std::thread(ExportDDVThreadFunc);
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Export All"))
            {
                gFMVExporting = true;
                gFMVExportProgress = 0;

                // check if the export thread is running already and if so, detach it
                if (gFMVExportThread.joinable())
                {
                    gFMVExportThread.detach();
                }

                gFMVExportThread = std::thread(ExportAllDDVThreadFunc);
                ImGui::CloseCurrentPopup();
            }
            ImVec2 previewSize = ImGui::GetContentRegionAvail();
            ImGui::Image(GL_TO_IMGUI_TEX(gMasherTexture), previewSize);
        }
        ImGui::EndChild();

    }
    ImGui::End();

    if (gFMVExporting)
    {
        ImGui::OpenPopup("FMV Export");
    }

    if (ImGui::BeginPopupModal("FMV Export", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::Text("Exporting FMV...");
        ImGui::ProgressBar(gFMVExportProgress, ImVec2(500.0f, 0.0f));

        ImGui::Text("%s", gFMVExportMessage.c_str());

        if (gFMVExportProgress >= 1.0f)
        {
            if (ImGui::Button("Close"))
            {
                ImGui::CloseCurrentPopup();
            }
        }
        else
        {
            if (ImGui::Button("Cancel"))
            {
                gFMVExporting = false;
                gFMVExportThread.detach();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
}