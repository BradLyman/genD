#include "Turtle.hpp"

#include <cmath>

using std::cos;
using std::sin;

using namespace tetra;

Turtle::Turtle(
    std::vector<ColoredLine::Vertex>& vertices, std::array<float, 2> start)
    : vertices{&vertices}, pos{start}
{
}

Turtle& Turtle::forward()
{
    static std::array<float, 4> white = {1.0f, 1.0f, 1.0f, 1.0f};
    std::array<float, 2> nextPos = {pos[0] + cos(angle) * step,
                                    pos[1] + sin(angle) * step};

    vertices->push_back(ColoredLine::Vertex{pos, white});
    vertices->push_back(ColoredLine::Vertex{nextPos, white});
    pos = nextPos;
    return *this;
}

Turtle& Turtle::turn(float deg)
{
    angle += (deg * 3.14159f / 180.0f);
    return *this;
}

Turtle& Turtle::set_step_size(float step)
{
    this->step = step;
    return *this;
}

float Turtle::step_size() const { return step; }
