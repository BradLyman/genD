#ifndef TETRA_GL_PROGRAM_HPP
#define TETRA_GL_PROGRAM_HPP

#include <GL/glew.h>

#include <tetra/gl/Shader.hpp>

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
    /**
     * Create a new program.
     * The program is not safe to use until shaders have been attached and it's
     * been linked.
     * */
    Program();
    ~Program();
    Program(Program&& from);
    Program& operator=(Program&& from);
    Program(Program& from) = delete;

    /**
     * Attach the shader to this program for compiling/linking.
     **/
    Program& attach(Shader&& shader);

    /**
     * Link the currently attached shaders and throw any errors.
     **/
    void link();

    /**
     * Get the attribute index for the attribute name.
     * Returns -1 if the attribute cannot be found.
     */
    GLint attrib_index(const std::string& attrib) const;

    /**
     * Get the uniform location for the provided uniform name.
     * Returns -1 if the location cannot be found.
     */
    GLint uniform_location(const std::string& uniform) const;

    /**
     * Take some action while the program is in use.
     */
    template<typename Fctn>
    void while_bound(Fctn fctn)
    {
        glUseProgram(handle());
        fctn();
        glUseProgram(0);
    }

    /**
     * The raw OpenGL program handle.
     **/
    GLuint handle() const;

  private:
    bool link_failed();
    void throw_link_error();

  private:
    GLuint id;
    std::vector<Shader> shaders;
};
} // namespace tetra

#endif
