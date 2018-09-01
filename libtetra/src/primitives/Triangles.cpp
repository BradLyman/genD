#include <tetra/primitives/Triangles.hpp>

using namespace glm;
using namespace tetra;
using namespace tetra::primitives;

namespace
{
const char* vertex = R"src(
    #version 450

    layout (location=0) in vec2 pos;
    layout (location=1) in vec4 color;

    out vec4 varyColor;

    layout (location=0) uniform mat4 view;

    void main() {
        varyColor = color;
        gl_Position = view * vec4(pos, 0.0f, 1.0f);
    }
)src";

const char* fragment = R"src(
    #version 450

    in vec4 varyColor;

    out vec4 fragColor;

    void main() {
        fragColor = varyColor;
    }
)src";
}; // namespace

Triangles::Triangles() : vertex_count{0}
{
    flat_color.attach(Shader(vertex, Shader::Type::Vertex));
    flat_color.attach(Shader(fragment, Shader::Type::Fragment));
    flat_color.link();

    vao.while_bound([&]() {
        buffer.while_bound(Buffer::Target::Array, []() {
            set_attrib_pointer(0, &Triangles::Vertex::pos);
            set_attrib_pointer(1, &Triangles::Vertex::color);
        });
    });
}

void Triangles::set_view_transform(const glm::mat4& view)
{
    flat_color.while_bound(
        [&]() { glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]); });
}

void Triangles::set_vertices(const std::vector<Vertex>& vertices)
{
    buffer.write(vertices);
    vertex_count = vertices.size();
}

void Triangles::draw()
{
    flat_color.while_bound([&]() {
        vao.while_bound([&]() { glDrawArrays(GL_TRIANGLES, 0, vertex_count); });
    });
}
