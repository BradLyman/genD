#include <glad/glad.h>

#include "GlApp.hpp"

#include "gl/Shader.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

namespace tetra
{
class Program
{
  public:
    class LinkError : public std::runtime_error
    {
      public:
        LinkError(const std::string& msg);
    };

  public:
    Program();
    ~Program();
    Program(Program&& from);
    Program& operator=(Program&& from);
    Program(Program& from) = delete;

    GLuint handle() const;

    Program& attach(Shader&& shader);

    void link();

  private:
    bool linkFailed();
    void throwLinkError();

  private:
    GLuint id;
    std::vector<Shader> shaders;
};
} // namespace tetra

using namespace tetra;
using namespace std;

Program::Program() : id{glCreateProgram()} {}
Program::~Program() { glDeleteProgram(id); }
Program::Program(Program&& from) : id{0} { swap(id, from.id); }

Program& Program::operator=(Program&& from)
{
    swap(id, from.id);
    return *this;
}

GLuint Program::handle() const { return id; }

Program& Program::attach(Shader&& shader)
{
    shaders.emplace_back(move(shader));
    return *this;
}

void Program::link()
{
    for (auto& shader : shaders) {
        shader.compile();
        glAttachShader(handle(), shader.handle());
    }
    glLinkProgram(handle());
    shaders.clear();
    if (linkFailed()) {
        throwLinkError();
    }
}

bool Program::linkFailed()
{
    GLint status = GL_FALSE;
    glGetProgramiv(handle(), GL_LINK_STATUS, &status);
    return (status == GL_FALSE);
}

void Program::throwLinkError()
{
    GLint logLength = 0;
    glGetProgramiv(handle(), GL_INFO_LOG_LENGTH, &logLength);

    auto buffer = make_unique<char[]>(logLength);
    glGetProgramInfoLog(handle(), logLength, &logLength, buffer.get());

    throw LinkError(string(buffer.get()));
}

Program::LinkError::LinkError(const string& msg) : runtime_error(msg) {}

const char* vertexSource = R"src(
  #version 450

  in vec4 vVertex;

  void main() {
    gl_Position = vVertex;
  }
)src";

const char* fragSource = R"src(
  #version 450

  out vec4 fragColor;

  void main() {
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
)src";

GlApp::GlApp() { glClearColor(0.2f, 0.2f, 0.4f, 1.0f); }
GlApp::~GlApp() {}

void GlApp::render_frame()
{
    Program program{};
    program.attach(Shader(vertexSource, Shader::Type::Vertex))
        .attach(Shader(fragSource, Shader::Type::Fragment))
        .link();

    glClear(GL_COLOR_BUFFER_BIT);
}
