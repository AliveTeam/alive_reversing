#pragma once

#define GL_DEBUG 1
#define GL_INFOLOG_MAX_LENGTH 250

#if GL_DEBUG > 0
    #define GL_VERIFY(x)    \
        (x);                \
        if (gGlDebug)       \
        {                   \
            CheckGLError(); \
        }

    extern bool gGlDebug;
    void CheckGLError();
#else
    #define GL_VERIFY(x) (x);
#endif
