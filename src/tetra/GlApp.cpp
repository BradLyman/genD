#include <glad/glad.h>

#include "GlApp.hpp"

#include "gl/Program.hpp"

#include <memory>
#include <stdexcept>
#include <vector>

using namespace tetra;
using namespace std;

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
    program.attach(Shader(vertexSource, Shader::Type::Vertex));
    program.attach(Shader(fragSource, Shader::Type::Fragment));
    program.link();

    glClear(GL_COLOR_BUFFER_BIT);
}
