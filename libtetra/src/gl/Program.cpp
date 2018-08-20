#include <tetra/gl/Program.hpp>

#include <memory>

using namespace std;
using tetra::Program;
using tetra::Shader;

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
    if (link_failed()) {
        throw_link_error();
    }
}

bool Program::link_failed()
{
    GLint status = GL_FALSE;
    glGetProgramiv(handle(), GL_LINK_STATUS, &status);
    return (status == GL_FALSE);
}

void Program::throw_link_error()
{
    GLint logLength = 0;
    glGetProgramiv(handle(), GL_INFO_LOG_LENGTH, &logLength);

    auto buffer = make_unique<char[]>(logLength);
    glGetProgramInfoLog(handle(), logLength, &logLength, buffer.get());

    throw LinkError(string(buffer.get()));
}

GLint Program::attrib_index(const string& attrib) const
{
    return glGetAttribLocation(handle(), attrib.c_str());
}

GLint Program::uniform_location(const string& uniform) const
{
    return glGetUniformLocation(handle(), uniform.c_str());
}

Program::LinkError::LinkError(const string& msg) : runtime_error(msg) {}
