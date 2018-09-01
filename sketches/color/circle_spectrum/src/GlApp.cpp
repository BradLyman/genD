#include "GlApp.hpp"

#include <tetra/Color.hpp>
#include <tetra/Countdown.hpp>
#include <tetra/gl/DebugLog.hpp>

#include <glm/vec2.hpp>
#include <iostream>

using namespace glm;
using namespace tetra;
using namespace std;
using tetra::primitives::Triangles;
using Vertex = Triangles::Vertex;

namespace
{
template<typename T, typename std::array<T, 1>::size_type size>
void append(std::vector<T>& target, const std::array<T, size>& to_add)
{
    using std::begin;
    using std::end;
    target.insert(end(target), begin(to_add), end(to_add));
}

constexpr float to_rad(float angle_degrees)
{
    return angle_degrees * 3.1415f / 180.0f;
}

vector<Vertex> spectrum_circle(int triangle_count)
{
    const float size = 4.0f;
    const float step = 360.0f / triangle_count;

    vector<Vertex> vertices;
    vertices.reserve(triangle_count * 3);
    for (int i = 0; i < triangle_count; i++) {
        const float angle = i * step;
        HSL interior_color{i * step, 1.0f, 0.6f};
        HSL color{i * step, 1.0f, 0.4f};
        vertices.push_back({{0.0f, 0.0f}, interior_color.as_rgba()});
        vertices.push_back({
            {
                size * sin(to_rad(angle)),
                size * cos(to_rad(angle)),
            },
            color.as_rgba(),
        });
        vertices.push_back({
            {
                size * sin(to_rad(angle + step)),
                size * cos(to_rad(angle + step)),
            },
            color.as_rgba(),
        });
    }
    return vertices;
}

} // namespace

GlApp::GlApp() : ortho{10.0f}
{
    auto rgba = HSL{180, 1.0f, 0.1f}.as_rgba();
    glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);

    // glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);

    triangles.set_vertices(spectrum_circle(6));
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    ortho.resize_viewport(width, height);
    triangles.set_view_transform(ortho.projection());
}

void GlApp::on_mouse_move(int x, int y) {}

void GlApp::on_frame_render()
{
    static Countdown update_tris;
    static const int max = 60;
    static int step = -1;
    static int current = 7;

    if (update_tris.time_up()) {
        update_tris.reset(chrono::milliseconds{100});

        current += step;
        if (current == 6 | current == max) {
            step *= -1;
        }
        triangles.set_vertices(spectrum_circle(current));
    }

    glClear(GL_COLOR_BUFFER_BIT);
    triangles.draw();
}
