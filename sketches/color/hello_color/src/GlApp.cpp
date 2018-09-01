#include "GlApp.hpp"

#include <tetra/Color.hpp>
#include <tetra/gl/DebugLog.hpp>

#include <iostream>

using namespace tetra;
using namespace std;
using tetra::primitives::Triangles;

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
