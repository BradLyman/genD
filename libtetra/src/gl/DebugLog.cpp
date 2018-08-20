#include <GL/glew.h>

#include <tetra/gl/DebugLog.hpp>

#include <iostream>

using namespace std;

const char* tetra::severity_to_string(GLenum severity)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        return "low";
    case GL_DEBUG_SEVERITY_MEDIUM:
        return "medium";
    case GL_DEBUG_SEVERITY_HIGH:
        return "high";
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        return "notification";
    }
    return "unknown";
}

void tetra::reportGlError(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userparam)
{
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
        cout << source << " " << type << " " << id << " "
             << severity_to_string(severity) << message << endl;
    }
}
