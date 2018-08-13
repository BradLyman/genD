#include <glad/glad.h>

#include "GlApp.hpp"

#include "gl/Program.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace tetra;
using namespace std;

namespace tetra
{
class VAO
{
  public:
    VAO();
    ~VAO();
    VAO(VAO& from) = delete;
    VAO(VAO&& from);

    VAO& operator=(VAO&& from);

    GLuint handle() const;

  private:
    GLuint id;
};

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
    Buffer();
    ~Buffer();

    Buffer(Buffer& from) = delete;
    Buffer(Buffer&& from);

    Buffer& operator=(Buffer&& from);

    GLuint handle() const;

    template<typename T>
    void write(const std::vector<T>& data, Usage usage = Usage::StaticDraw)
    {
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

Buffer::Buffer() : id{0} { glCreateBuffers(1, &id); }
Buffer::~Buffer() { glDeleteBuffers(1, &id); }

Buffer::Buffer(Buffer&& from) : id{0} { swap(id, from.id); }

Buffer& Buffer::operator=(Buffer&& from)
{
    swap(id, from.id);
    return *this;
}

GLuint Buffer::handle() const { return id; }

VAO::VAO() : id{0} { glCreateVertexArrays(1, &id); }
VAO::~VAO() { glDeleteVertexArrays(1, &id); }

VAO::VAO(VAO&& from) : id{0} { swap(id, from.id); }

VAO& VAO::operator=(VAO&& from)
{
    swap(id, from.id);
    return *this;
}

GLuint VAO::handle() const { return id; }

const char* vertexSource = R"src(
  #version 450

  in vec4 vVertex;
  in vec4 vColor;

  out vec4 vVaryColor;

  void main() {
    vVaryColor = vColor;
    gl_Position = vVertex;
  }
)src";

const char* fragSource = R"src(
  #version 450

  in vec4 vVaryColor;
  out vec4 fragColor;

  void main() {
    fragColor = vVaryColor;
  }
)src";

GlApp::GlApp() { glClearColor(0.2f, 0.2f, 0.4f, 1.0f); }
GlApp::~GlApp() {}

template<std::array<float, 1>::size_type length, typename T>
void attribPointer(GLint index, std::array<float, length> T::*memberPtr)
{
    static_assert(
        std::is_pod<T>::value,
        "only POD data types can be used for vertex attrib pointers");

    glEnableVertexAttribArray(index);

    // use this null pointer to get the offset of the member from 0
    // it's a hack that only works for POD, so notice the static_assert
    static T* nullObj = nullptr;
    glVertexAttribPointer(
        index,
        length,
        GL_FLOAT,
        GL_FALSE,
        sizeof(T),
        static_cast<const GLvoid*>(&(nullObj->*memberPtr)));
}

void GlApp::render_frame()
{
    Program program{};
    program.attach(Shader(vertexSource, Shader::Type::Vertex));
    program.attach(Shader(fragSource, Shader::Type::Fragment));
    program.link();

    struct Vertex {
        array<float, 4> position;
    };
    struct Color {
        array<float, 4> color;
    };

    VAO vao{};
    Buffer vertices{};
    Buffer colors{};

    glBindVertexArray(vao.handle());
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertices.handle());
        attribPointer(program.attribIndex("vVertex"), &Vertex::position);

        glBindBuffer(GL_ARRAY_BUFFER, colors.handle());
        attribPointer(program.attribIndex("vColor"), &Color::color);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    glBindVertexArray(0);

    vertices.write<Vertex>({{0.0f, 0.0f, 0.0f, 1.0f},
                            {-0.5f, 0.0f, 0.0f, 1.0f},
                            {0.0f, 0.5f, 0.0f, 1.0f}});

    colors.write<Color>({{0.0f, 1.0f, 0.0f, 1.0f},
                         {1.0f, 0.0f, 0.0f, 1.0f},
                         {0.0f, 0.0f, 1.0f, 1.0f}});

    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(vao.handle());
    glUseProgram(program.handle());
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(0);
    glBindVertexArray(0);
}
