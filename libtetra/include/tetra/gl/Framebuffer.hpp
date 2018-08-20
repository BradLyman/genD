#ifndef TETRA_GL_FRAMEBUFFER_HPP
#define TETRA_GL_FRAMEBUFFER_HPP

#include <GL/glew.h>

namespace tetra
{
/** Objects of this class represent an RAII Framebuffer object. */
class Framebuffer
{
  public:
    enum class Target : GLenum {
        READ = GL_READ_FRAMEBUFFER,
        DRAW = GL_DRAW_FRAMEBUFFER,
        BOTH = GL_FRAMEBUFFER
    };

  public:
    Framebuffer();
    ~Framebuffer();
    Framebuffer(Framebuffer& from) = delete;
    Framebuffer(Framebuffer&& from);

    Framebuffer& operator=(Framebuffer&& from);

    /**
     * A copy of the underlying OpenGL framebuffer id.
     */
    GLuint handle() const;

    /**
     * Take some action while the buffer is bound.
     */
    template<typename Fctn>
    void while_bound(Target target, Fctn action)
    {
        glBindFramebuffer(static_cast<GLenum>(target), id);
        action();
        glBindFramebuffer(static_cast<GLenum>(target), 0);
    }

  private:
    GLuint id;
};
} // namespace tetra

#endif
