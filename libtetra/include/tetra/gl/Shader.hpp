#ifndef TETRA_GL_SHADER_HPP
#define TETRA_GL_SHADER_HPP

#include <GL/glew.h>

#include <stdexcept>
#include <string>

namespace tetra
{
/**
 * Objects of this class represent an OpenGL Shader Object.
 */
class Shader
{
  public:
    /** This exception is thrown when a compiler error is detected. */
    class CompileError : public std::runtime_error
    {
      public:
        CompileError(const std::string& msg);
    };

    enum class Type : GLenum {
        Compute = GL_COMPUTE_SHADER,
        Vertex = GL_VERTEX_SHADER,
        TessControl = GL_TESS_CONTROL_SHADER,
        TessEval = GL_TESS_EVALUATION_SHADER,
        Geometry = GL_GEOMETRY_SHADER,
        Fragment = GL_FRAGMENT_SHADER
    };

  public:
    /** create a new shader object with the given type */
    Shader(const std::string& source, Type type);
    ~Shader();
    Shader(Shader&& from);
    Shader& operator=(Shader&& from);
    Shader(Shader& from) = delete;

    /**
     * Compile the shader source, throws a CompileError if there is an error.
     */
    void compile();

    /**
     * The raw OpenGL Shader id.
     */
    GLuint handle() const;

  private:
    void set_source();
    bool failed_to_compile();
    void throw_compile_error();

  private:
    GLuint id;
    std::string source;
};
} // namespace tetra

#endif
