#ifndef TETRA_TURTLE_HPP
#define TETRA_TURTLE_HPP

#include "ColoredLine.hpp"

#include <array>
#include <vector>

namespace tetra
{
/**
 * Objects of this class represent a stateful geometry builder for the
 * ColoredLine.
 */
class Turtle
{
  public:
    Turtle(
        std::vector<ColoredLine::Vertex>& vertices,
        std::array<float, 2> start = {0.0f, 0.0f});
    Turtle(const Turtle& from) = default;

    Turtle& turn(float deg);

    Turtle& forward();

    Turtle& set_step_size(float f);
    float step_size() const;

  private:
    std::vector<ColoredLine::Vertex>* vertices;
    std::array<float, 2> pos = {0.0f, 0.0f};
    float angle = 90.0f * 3.14159f / 180.0f;
    float step = 1.0f;
};
} // namespace tetra

#endif
