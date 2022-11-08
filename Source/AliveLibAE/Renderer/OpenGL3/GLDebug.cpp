#include <GL/glew.h>
#include <string>

#include "../AliveLibCommon/FatalError.hpp"
#include "../AliveLibCommon/Types.hpp"
#include "GLDebug.hpp"

bool gGlDebug = true;

void CheckGLError()
{
    GLenum lastGLError = glGetError();

    if (lastGLError != GL_NO_ERROR)
    {
        std::string buf;
        auto msg = reinterpret_cast<const char_type*>(glewGetString(lastGLError));

        buf.append("OpenGL error raised: ");

        if (msg != nullptr)
        {
            buf.append(msg);
        }
        else
        {
            buf.append(std::to_string(lastGLError));
        }

        ALIVE_FATAL(buf.c_str());
    }
}
