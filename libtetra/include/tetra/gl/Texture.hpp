#ifndef TETRA_GL_TEXTURE_HPP
#define TETRA_GL_TEXTURE_HPP

#include <GL/glew.h>

namespace tetra
{
/**
 * Objects of this class represent an RAII OpenGL texture.
 */
class Texture
{
  public:
    enum class Target : GLenum {
        Tex1d = GL_TEXTURE_1D,
        Tex2d = GL_TEXTURE_2D,
        Tex3d = GL_TEXTURE_3D,
        Tex1dArray = GL_TEXTURE_1D_ARRAY,
        Tex2dArray = GL_TEXTURE_2D_ARRAY,
        Rect = GL_TEXTURE_RECTANGLE,
        CubeMap = GL_TEXTURE_CUBE_MAP,
        CubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
        Buffer = GL_TEXTURE_BUFFER,
        Tex2dMultisample = GL_TEXTURE_2D_MULTISAMPLE,
        Tex2dMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
    };

  public:
    Texture();
    ~Texture();

    Texture(Texture& from) = delete;
    Texture(Texture&& from);

    Texture& operator=(Texture&& from);

    GLuint handle() const;

    template<typename Fctn>
    void whileBound(Target target, Fctn action)
    {
        glBindTexture(static_cast<GLenum>(target), id);
        action();
        glBindTexture(static_cast<GLenum>(target), 0);
    }

  private:
    GLuint id;
};
} // namespace tetra

#endif
