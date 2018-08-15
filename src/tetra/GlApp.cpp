#include <glad/glad.h>

#include "GlApp.hpp"

#include "Mat4x4.hpp"

using namespace tetra;
using namespace std;

GlApp::GlApp() : view{identity()}, line{}
{
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    line.set_vertices({{{0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
                       {{-0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}},
                       {{0.0f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}},
                       {{0.5f, 0.5f}, {1.0f, 1.0f, 1.0f, 1.0f}}});
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    view = ortho(width, height, 5.0f);
}

void GlApp::render_frame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    line.render(view.data());
}
