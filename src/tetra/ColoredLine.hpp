#ifndef TETRA_COLORED_LINE_HPP
#define TETRA_COLORED_LINE_HPP

#include <GL/glew.h>

#include "gl/Buffer.hpp"
#include "gl/Program.hpp"
#include "gl/VAO.hpp"

#include <array>
#include <vector>

namespace tetra
{
/**
 * Objects of this class represent a colored line on screen.
 */
class ColoredLine
{
  public:
    struct Vertex {
        std::array<float, 2> pos;
        std::array<float, 4> color;
    };

  public:
    ColoredLine();

    void render(const float* viewMtx);

    void set_vertices(const std::vector<Vertex>& vertices);

  private:
    GLint view;
    Program program;
    Buffer vertices;
    VAO vao;

    int vertexCount;
};
} // namespace tetra

#endif
