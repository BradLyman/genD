#include "GlApp.hpp"

#include <tetra/Color.hpp>
#include <tetra/gl/DebugLog.hpp>

using namespace tetra;
using namespace std;

GlApp::GlApp() : ortho{720.0f}
{
    auto rgba = HSL{0, 0.8f}.as_rgba();
    glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    ortho.resize_window(width, height);
}

void GlApp::on_mouse_move(int x, int y)
{
    const auto norm = glm::abs(ortho.world_coords({x, y}));

    auto clear = HSL{norm.y, 0.8f}.as_rgba();
    glClearColor(clear[0], clear[1], clear[2], clear[3]);

    colored_quads.set_quads({
        {
            -norm.x,
            norm.x,
            -norm.y,
            norm.y,
            HSL{fmod(norm.y + 90.0f, 360.0f), 0.8f}.as_rgba(),
        },
    });
}

void GlApp::on_frame_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    colored_quads.draw(ortho.projection());
}

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

ColoredQuads::ColoredQuads() : vertex_count{0}
{
    flat_color.attach(Shader(vertex, Shader::Type::Vertex));
    flat_color.attach(Shader(fragment, Shader::Type::Fragment));
    flat_color.link();

    vao.while_bound([&]() {
        buffer.while_bound(Buffer::Target::Array, []() {
            set_attrib_pointer(0, &Vertex::pos);
            set_attrib_pointer(1, &Vertex::color);
        });
    });
}

void ColoredQuads::set_quads(const std::vector<Quad>& quads)
{
    std::vector<Vertex> vertices;
    vertices.reserve(quads.size() * 6);
    for (const Quad& quad : quads) {
        vertices.push_back({{quad.left, quad.bottom}, quad.rgba_color});
        vertices.push_back({{quad.right, quad.bottom}, quad.rgba_color});
        vertices.push_back({{quad.left, quad.top}, quad.rgba_color});

        vertices.push_back({{quad.left, quad.top}, quad.rgba_color});
        vertices.push_back({{quad.right, quad.bottom}, quad.rgba_color});
        vertices.push_back({{quad.right, quad.top}, quad.rgba_color});
    }
    buffer.write(vertices);
    vertex_count = vertices.size();
}

void ColoredQuads::draw(const glm::mat4& view)
{
    vao.while_bound([&]() {
        flat_color.while_bound([&]() {
            glUniformMatrix4fv(0, 1, GL_FALSE, &view[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        });
    });
}
