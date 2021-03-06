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

constexpr float world_height = 1.0f;
constexpr float world_width = 360.0f;

namespace
{
struct Quad {
    float left, right, bottom, top;

    array<Triangles::Vertex, 6> triangulate(IColor& color) const
    {
        array<Triangles::Vertex, 6> vertices;
        vertices[0] = {{left, bottom}, color.as_rgba()};
        vertices[1] = {{right, bottom}, color.as_rgba()};
        vertices[2] = {{left, top}, color.as_rgba()};

        vertices[3] = {{left, top}, color.as_rgba()};
        vertices[4] = {{right, bottom}, color.as_rgba()};
        vertices[5] = {{right, top}, color.as_rgba()};
        return vertices;
    }
};

template<typename T, typename std::array<T, 1>::size_type size>
void append(std::vector<T>& target, const std::array<T, size>& to_add)
{
    using std::begin;
    using std::end;
    target.insert(end(target), begin(to_add), end(to_add));
}

vector<Triangles::Vertex> spectrum_quads(int h_subs, int v_subs)
{
    const float h_step = world_width / h_subs;
    const float v_step = world_height / v_subs;
    const vec2 origin{-world_width / 2.0f, world_height / 2.0f};

    vector<Triangles::Vertex> vertices;
    vertices.reserve(h_subs * v_subs * 6);

    for (int w = 0; w < h_subs; w++) {
        for (int v = 0; v < v_subs; v++) {
            const Quad quad{
                origin.x + (w * h_step),
                origin.x + ((w + 1) * h_step),
                origin.y - (v * v_step),
                origin.y - ((v + 1) * v_step),
            };
            const float hue = (h_step * w) * 360.0f / world_width;
            HSL color{hue, 1.0f, world_height - (v * v_step)};
            append(vertices, quad.triangulate(color));
        }
    }
    return vertices;
}
} // namespace

GlApp::GlApp() : ortho{world_width, world_height}
{
    auto rgba = HSL{0, 0.8f}.as_rgba();
    glClearColor(rgba[0], rgba[1], rgba[2], rgba[3]);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);

    triangles.set_view_transform(ortho.projection());
    triangles.set_vertices(spectrum_quads(7, 5));
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    ortho.resize_viewport(width, height);
}

void GlApp::on_mouse_move(int x, int y) {}

void GlApp::on_frame_render()
{
    static Countdown update_spectrum;
    static int step = -1;
    static int current = 3;
    static int max = 100;

    if (update_spectrum.time_up()) {
        update_spectrum.reset(chrono::milliseconds{100});

        if (current == 3 || current == max) {
            step = -step;
        }
        current += step;

        triangles.set_vertices(spectrum_quads(current, current));
    }

    glClear(GL_COLOR_BUFFER_BIT);
    triangles.draw();
}
