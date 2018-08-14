#include <glad/glad.h>

#include "GlApp.hpp"

#include "gl/Buffer.hpp"
#include "gl/Program.hpp"
#include "gl/VAO.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

using namespace tetra;
using namespace std;

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

struct Vertex {
    array<float, 4> position;
    array<float, 4> color;
};

GlApp::GlApp()
{
    program.attach(Shader(vertexSource, Shader::Type::Vertex));
    program.attach(Shader(fragSource, Shader::Type::Fragment));
    program.link();

    vao.whileBound([&]() {
        glBindBuffer(GL_ARRAY_BUFFER, vertices.handle());
        attribPointer(program.attribIndex("vVertex"), &Vertex::position);
        attribPointer(program.attribIndex("vColor"), &Vertex::color);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    });

    glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
}
GlApp::~GlApp() {}

void GlApp::render_frame()
{
    vertices.write<Vertex>(
        {{{0.0f, 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
         {{-0.5f, 0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
         {{0.0f, 0.5f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}}});

    glClear(GL_COLOR_BUFFER_BIT);

    vao.whileBound([&]() {
        glUseProgram(program.handle());
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(0);
    });
}
