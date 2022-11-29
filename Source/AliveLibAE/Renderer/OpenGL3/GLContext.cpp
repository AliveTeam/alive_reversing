#include <GL/glew.h>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "../AliveLibCommon/FatalError.hpp"
#include "../AliveLibCommon/logger.hpp"
#include "../AliveLibCommon/Types.hpp"
#include "GLContext.hpp"
#include "SDL.h"

GLContext::GLContext(TWindowHandleType window)
{
    // Find the opengl driver
    const s32 numDrivers = SDL_GetNumRenderDrivers();
    if (numDrivers < 0)
    {
        LOG_ERROR("Failed to get driver count %s", SDL_GetError());
    }

    LOG_INFO("Got %d drivers", numDrivers);

    s32 index = -1;
    for (s32 i = 0; i < numDrivers; i++)
    {
        SDL_RendererInfo info = {};
        if (SDL_GetRenderDriverInfo(i, &info) < 0)
        {
            LOG_WARNING("Failed to get render %d info %s", i, SDL_GetError());
        }
        else
        {
            LOG_INFO("%d name %s", i, info.name ? info.name : "(null)");
            if (info.name && strstr(info.name, "opengl"))
            {
                index = i;
                break;
            }
        }
    }

    if (index == -1)
    {
        // FIXME: Throw exception?
        ALIVE_FATAL("OpenGL SDL2 driver not found");
    }

    // We should attempt to load OpenGL 3.2 first, because this is the minimum
    // required version for RenderDoc captures so we can actually debug stuff
    const char_type* glslVer150 = "#version 150";
    const char_type* glslVer140 = "#version 140";

    bool glslVer150Supported = true;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Create context
    mContext = SDL_GL_CreateContext(window);

    if (mContext == NULL)
    {
        LOG_ERROR("OpenGL 3.2 context could not be created! SDL Error: %s", SDL_GetError());

        // Our ACTUAL minimum OpenGL requirement is 3.1, though we will check
        // supported extensions on the GPU in a moment
        glslVer150Supported = false;

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // Create context
        mContext = SDL_GL_CreateContext(window);
        if (mContext == NULL)
        {
            // FIXME: Throw exception?
            ALIVE_FATAL("OpenGL 3.1 context could not be created! SDL Error: %s", SDL_GetError());
        }
    }

    LOG_INFO("GL_VERSION = %s", glGetString(GL_VERSION));

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK)
    {
        LOG_ERROR("Error initializing GLEW! %s", glewGetErrorString(glewError));
    }

    // Use Vsync
    // FIXME: VSYNC disabled for now - remove before merge to master!
    if (SDL_GL_SetSwapInterval(0) < 0)
    {
        LOG_ERROR("Warning: Unable to set VSync! SDL Error: %s", SDL_GetError());
    }

    // Check supported extensions by the GPU
    if (!glewIsSupported("GL_ARB_vertex_array_object GL_ARB_framebuffer_object GL_ARB_explicit_attrib_location"))
    {
        ALIVE_FATAL("Your graphics device is not supported, sorry!");
    }

    ImGui::CreateContext();

    // Setup IMGUI for texture debugging
    ImGui_ImplSDL2_InitForOpenGL(window, mContext);
    ImGui_ImplOpenGL3_Init(glslVer150Supported ? glslVer150 : glslVer140);
}

GLContext::~GLContext()
{
    ImGui_ImplSDL2_Shutdown();

    if (mContext)
    {
        SDL_GL_DeleteContext(mContext);
        mContext = nullptr;
    }
}
