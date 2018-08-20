#ifndef TETRA_GL_BUFFER_HPP
#define TETRA_GL_BUFFER_HPP

#include <GL/glew.h>

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

    enum class BufferTarget : GLenum {
        Array = GL_ARRAY_BUFFER,
        AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
        CopyRead = GL_COPY_READ_BUFFER,
        CopyWrite = GL_COPY_WRITE_BUFFER,
        DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
        DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
        PixelPack = GL_PIXEL_PACK_BUFFER,
        PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
        Query = GL_QUERY_BUFFER,
        ShaderStorage = GL_SHADER_STORAGE_BUFFER,
        TextureBuffer = GL_TEXTURE_BUFFER,
        TransformFeedback = GL_TRANSFORM_FEEDBACK,
        Uniform = GL_UNIFORM_BUFFER,
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

    /**
     * Take some action (callable) while the buffer is bound to the specified
     * target.
     */
    template<typename Action>
    void while_bound(BufferTarget target, Action act)
    {
        glBindBuffer(static_cast<GLenum>(target), handle());
        act();
        glBindBuffer(static_cast<GLenum>(target), 0);
    }

  private:
    GLuint id;
};
} // namespace tetra

#endif
