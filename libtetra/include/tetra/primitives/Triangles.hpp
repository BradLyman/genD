#ifndef TETRA_PRIMITIVES_TRIANGLES_HPP
#define TETRA_PRIMITIVES_TRIANGLES_HPP

#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/Program.hpp>
#include <tetra/gl/VAO.hpp>

#include <array>
#include <glm/mat4x4.hpp>

namespace tetra::primitives
{
/**
 * Objects of this class represent 2D colored triangles on screen.
 */
class Triangles
{
  public:
    /**
     * The vertex structure used for rendering.
     */
    struct Vertex {
        std::array<float, 2> pos;
        std::array<float, 4> color;
    };

  public:
    Triangles();

    void set_view_transform(const glm::mat4& view);

    /**
     * Replace the vertices for the triangles.
     * vertices.size() % 3 == 0
     */
    void set_vertices(const std::vector<Vertex>& vertices);

    /**
     * Render the vertices to the framebuffer as triangles.
     */
    void draw();

  private:
    VAO vao;
    Buffer buffer;
    int vertex_count;
    Program flat_color;
};
} // namespace tetra::primitives
#endif
