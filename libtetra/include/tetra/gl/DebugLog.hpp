#ifndef TETRA_GL_DEBUG_LOG_HPP
#define TETRA_GL_DEBUG_LOG_HPP

namespace tetra
{
void report_gl_error(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userparam);
} // namespace tetra

#endif
