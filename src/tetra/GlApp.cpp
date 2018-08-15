#include <glad/glad.h>

#include "GlApp.hpp"

#include "Mat4x4.hpp"

#include <cmath>

using namespace tetra;
using namespace std;

class Turtle
{
  public:
    void reset();

    void turn(float deg);

    void forward();

    void set_step_size(float f);

    const std::vector<ColoredLine::Vertex>& primitive_geometry() const;

  private:
    std::vector<ColoredLine::Vertex> vertices;
    std::array<float, 2> pos = {0.0f, 0.0f};
    float angle = 90.0f * 3.14159f / 180.0f;
    float step = 1.0f;
};

void Turtle::reset() { vertices.clear(); }

void Turtle::forward()
{
    static std::array<float, 4> white = {1.0f, 1.0f, 1.0f, 1.0f};
    std::array<float, 2> nextPos = {pos[0] + cos(angle) * step,
                                    pos[1] + sin(angle) * step};

    vertices.push_back(ColoredLine::Vertex{pos, white});
    vertices.push_back(ColoredLine::Vertex{nextPos, white});
    pos = nextPos;
}

void Turtle::turn(float deg) { angle += (deg * 3.14159f / 180.0f); }

void Turtle::set_step_size(float step) { this->step = step; }

const vector<ColoredLine::Vertex>& Turtle::primitive_geometry() const
{
    return vertices;
}

GlApp::GlApp() : view{identity()}, line{}
{
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);

    Turtle turtle;
    turtle.set_step_size(0.5f);
    for (int i = 0; i < 100; i++) {
        turtle.turn(45.0f - i);
        turtle.forward();
    }

    line.set_vertices(turtle.primitive_geometry());
}
GlApp::~GlApp() {}

void GlApp::on_viewport_change(int width, int height)
{
    glViewport(0, 0, width, height);
    view = ortho(width, height, 20.0f);
}

void GlApp::render_frame()
{
    glClear(GL_COLOR_BUFFER_BIT);
    line.render(view.data());
}
