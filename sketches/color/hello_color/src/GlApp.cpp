#include "GlApp.hpp"

#include <tetra/Color.hpp>
#include <tetra/Countdown.hpp>
#include <tetra/Mat4x4.hpp>
#include <tetra/gl/DebugLog.hpp>

#include <cmath>
#include <iostream>
#include <stack>
#include <thread>

using namespace tetra;
using namespace std;

namespace
{
constexpr float screen_height = 720.0f;
};

GlApp::GlApp()
    : view{identity()}
    , clear_color{0, 0.5f}
    , rect_color{60, 0.5f}
    , dims{1.0f, 1.0f}
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    view = ortho(width, height, screen_height);
    dims = {(float)width, (float)height};
}

void GlApp::on_mouse_move(int x, int y)
{
    float norm_y = y * (screen_height / dims.height);
    float scaled_width = screen_height * (dims.width / dims.height);
    float norm_x = x * (scaled_width / dims.width);

    rect_size = norm_x / 2.0f;
    clear_color = HSL{norm_y / 2.0f};
    rect_color = HSL{fmod((norm_y / 2.0f) + 90.0f, 360.0f)};

    auto rgba = clear_color.as_rgba();
    glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);
}

void GlApp::on_frame_render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    colored_quads.set_quads({
        {
            -rect_size / 2,
            rect_size / 2,
            -rect_size / 2,
            rect_size / 2,
            rect_color.as_rgba(),
        },
    });
    colored_quads.draw(view);
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

void ColoredQuads::draw(const std::array<float, 16>& view)
{
    vao.while_bound([&]() {
        flat_color.while_bound([&]() {
            glUniformMatrix4fv(0, 1, GL_FALSE, view.data());
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        });
    });
}
