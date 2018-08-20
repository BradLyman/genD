#ifndef TETRA_GL_PROGRAM_HPP
#define TETRA_GL_PROGRAM_HPP

#include <GL/glew.h>

#include "Shader.hpp"

#include <stdexcept>
#include <string>
#include <vector>

namespace tetra
{
/**
 * Objects of this class represent an OpenGL shader program.
 */
class Program
{
  public:
    /**
     * Objects of this class represent an error while linking a program.
     */
    class LinkError : public std::runtime_error
    {
      public:
        LinkError(const std::string& msg);
    };

  public:
    /** Create a new program */
    Program();
    ~Program();
    Program(Program&& from);
    Program& operator=(Program&& from);
    Program(Program& from) = delete;

    /** The raw OpenGL program handle. */
    GLuint handle() const;

    /** Attach the shader to this program for compiling/linking. */
    Program& attach(Shader&& shader);

    /** Link the shaders and throw any errors. */
    void link();

    GLint attribIndex(const std::string& attrib) const;

    GLint uniformLocation(const std::string& uniform) const;

    template<typename Fctn>
    void while_bound(Fctn fctn)
    {
        glUseProgram(handle());
        fctn();
        glUseProgram(0);
    }

  private:
    bool linkFailed();
    void throwLinkError();

  private:
    GLuint id;
    std::vector<Shader> shaders;
};
} // namespace tetra

#endif
