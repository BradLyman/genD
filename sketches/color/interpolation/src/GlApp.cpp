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

constexpr float world_height = 1.0f;
constexpr float world_width = 1.0f;

namespace
{
struct Quad {
    float left, right, bottom, top;
};

array<Triangles::Vertex, 6>
triangulate(const Quad& quad, const array<float, 4>& color)
{
    array<Triangles::Vertex, 6> vertices;
    vertices[0] = {{quad.left, quad.bottom}, color};
    vertices[1] = {{quad.right, quad.bottom}, color};
    vertices[2] = {{quad.left, quad.top}, color};

    vertices[3] = {{quad.left, quad.top}, color};
    vertices[4] = {{quad.right, quad.bottom}, color};
    vertices[5] = {{quad.right, quad.top}, color};
    return vertices;
}

/**
 * Generate and add all vertices for a single color gradient from the
 * left color to the right color.
 * The swatch is 1/10th the height of the screen and stretches from the left
 * to the right.
 */
template<typename ColorT>
void add_swatch_at(vector<Vertex>& vertices, float y, ColorT left, ColorT right)
{
    constexpr int steps = 50;
    constexpr float step_size = world_width / steps;
    constexpr float strip_height = 0.9f * (world_height / 10.0f);
    constexpr float start = -world_width / 2.0f;

    for (int i = 0; i < steps; i++) {
        const auto quad_verts = triangulate(
            Quad{
                start + i * step_size,
                start + (i + 1) * step_size,
                y - strip_height,
                y,
            },
            ColorT::mix(left, right, i * step_size).as_rgba());
        vertices.insert(end(vertices), begin(quad_verts), end(quad_verts));
    }
}

/**
 * Vertices for a set of color swatches showing varying colors from the left
 * to the right sides of the screen.
 */
vector<Vertex> all_swatch_vertices()
{
    vector<Vertex> verts;
    add_swatch_at(verts, 0.0f, RGBA{0.0f, 0.0f, 1.0f, 1.0f}, RGBA{0.8f, 1.0f});
    add_swatch_at(verts, 0.1f, HSL{240.0f}, HSL{240.0f, 0.0f, 0.8f});

    add_swatch_at(
        verts,
        0.3f,
        RGBA{1.0f, 0.0f, 0.0f, 1.0f},
        RGBA{0.0f, 0.0f, 0.5f, 1.0f});
    add_swatch_at(verts, 0.4f, HSL{0.0f}, HSL{240.0f, 1.0f, 0.25f});

    add_swatch_at(verts, -0.2f, HSL{120.0f}, HSL{0.0f});
    add_swatch_at(
        verts,
        -0.3f,
        RGBA{0.0f, 1.0f, 0.0f, 1.0f},
        RGBA{0.0f, 0.0f, 1.0f, 1.0f});

    return verts;
}

} // namespace

GlApp::GlApp() : ortho{world_width, world_height}
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(&tetra::report_gl_error, nullptr);

    triangles.set_vertices(all_swatch_vertices());
    triangles.set_view_transform(ortho.projection());
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
    glClear(GL_COLOR_BUFFER_BIT);
    triangles.draw();
}
