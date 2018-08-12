#include <glad/glad.h>

#include "GlApp.hpp"

#include "gl/Shader.hpp"

using namespace tetra;

const char* vertexSource = R"src(
  #version 450

  in vec4 vVertex;

  void main() {
    gl_Position = vVertex;
  }
)src";

const char* fragSource = R"src(
  #version 450

  void main() {
    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
  }
)src";

GlApp::GlApp() { glClearColor(0.2f, 0.2f, 0.4f, 1.0f); }
GlApp::~GlApp() {}

void GlApp::render_frame()
{
  Shader vertex(Shader::Type::Vertex);
  vertex.compile(vertexSource);

  Shader fragment(Shader::Type::Fragment);
  fragment.compile(fragSource);

  glClear(GL_COLOR_BUFFER_BIT);
}
