#ifndef TETRA_GL_BUFFER_HPP
#define TETRA_GL_BUFFER_HPP

#include <glad/glad.h>

#include <vector>

namespace tetra
{
/**
 * Objects of this class represent an OpenGL data buffer.
 */
class Buffer
{
  public:
    enum class Usage : GLenum {
        StreamDraw = GL_STREAM_DRAW,
        StreamRead = GL_STREAM_READ,
        StreamCopy = GL_STREAM_COPY,

        StaticDraw = GL_STATIC_DRAW,
        StaticRead = GL_STATIC_READ,
        StaticCopy = GL_STATIC_COPY,

        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicRead = GL_DYNAMIC_READ,
        DynamicCopy = GL_DYNAMIC_COPY
    };

  public:
    /**
     * Create a new buffer.
     */
    Buffer();
    ~Buffer();

    Buffer(Buffer& from) = delete;
    Buffer(Buffer&& from);

    Buffer& operator=(Buffer&& from);

    /**
     * The raw openGL buffer id.
     */
    GLuint handle() const;

    /**
     * Write the entire provided vector into the buffer.
     */
    template<typename T>
    void write(const std::vector<T>& data, Usage usage = Usage::StaticDraw)
    {
        static_assert(std::is_pod<T>::value, "buffers can only hold POD data");
        glNamedBufferData(
            handle(),
            data.size() * sizeof(T),
            data.data(),
            static_cast<GLenum>(usage));
    }

  private:
    GLuint id;
};
} // namespace tetra

#endif
