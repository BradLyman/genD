#include "GlApp.hpp"

#include <tetra/Color.hpp>
#include <tetra/gl/DebugLog.hpp>

#include <iostream>

using namespace tetra;
using namespace std;

struct CenteredQuad {
    float half_width, half_height;

    vector<Triangles::Vertex> triangulate(IColor& color) const
    {
        vector<Triangles::Vertex> vertices{};
        vertices.reserve(6);
        vertices.push_back({{-half_width, -half_height}, color.as_rgba()});
        vertices.push_back({{half_width, -half_height}, color.as_rgba()});
        vertices.push_back({{-half_width, half_height}, color.as_rgba()});

        vertices.push_back({{-half_width, half_height}, color.as_rgba()});
        vertices.push_back({{half_width, -half_height}, color.as_rgba()});
        vertices.push_back({{half_width, half_height}, color.as_rgba()});
        return vertices;
    }
};

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
    ortho.resize_viewport(width, height);
    triangles.set_view_transform(ortho.projection());
}

void GlApp::on_mouse_move(int x, int y)
{
    const auto norm = glm::abs(ortho.world_coords({x, y}));

    auto background = HSL{norm.y, 0.8f}.as_rgba();
    glClearColor(background[0], background[1], background[2], background[3]);

    auto foreground = HSL{fmod(norm.y + 90.0f, 360.0f), 0.8f};
    triangles.set_vertices(
        CenteredQuad{norm.x, norm.y}.triangulate(foreground));
}

void GlApp::on_frame_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    triangles.draw();
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
