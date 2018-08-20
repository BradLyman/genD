#include <tetra/ColoredLine.hpp>

using namespace tetra;
using namespace std;

namespace
{
const char* vertexSource = R"src(
  #version 450

  uniform mat4 view;

  in vec2 vVertex;
  in vec4 vColor;

  out vec4 vVaryColor;

  void main() {
    vVaryColor = vColor;
    gl_Position = view * vec4(vVertex, 0.0f, 1.0f);
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
} // namespace

ColoredLine::ColoredLine()
{
    program.attach(Shader(vertexSource, Shader::Type::Vertex));
    program.attach(Shader(fragSource, Shader::Type::Fragment));
    program.link();

    view = glGetUniformLocation(program.handle(), "view");

    vao.whileBound([&]() {
        vertices.while_bound(Buffer::Target::Array, [&]() {
            attribPointer(program.attrib_index("vVertex"), &Vertex::pos);
            attribPointer(program.attrib_index("vColor"), &Vertex::color);
        });
    });
}

void ColoredLine::set_vertices(const vector<Vertex>& vertexData)
{
    vertices.write(vertexData, Buffer::Usage::StaticDraw);
    vertexCount = vertexData.size();
}

void ColoredLine::render(const float* viewMtx)
{
    vao.whileBound([&]() {
        program.while_bound([&]() {
            glUniformMatrix4fv(view, 1, GL_FALSE, viewMtx);
            glDrawArrays(GL_LINES, 0, vertexCount);
        });
    });
}
