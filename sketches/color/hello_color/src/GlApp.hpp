#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include <tetra/Color.hpp>
#include <tetra/gl/Buffer.hpp>
#include <tetra/gl/Program.hpp>
#include <tetra/gl/VAO.hpp>
#include <tetra/sdl/SdlEventPump.hpp>
#include <tetra/transforms/AutoAspectOrtho.hpp>

#include <glm/mat4x4.hpp>

namespace tetra
{
class Triangles
{
  public:
    struct Vertex {
        std::array<float, 2> pos;
        std::array<float, 4> color;
    };

  public:
    Triangles();

    void set_view_transform(const glm::mat4& view);

    void set_vertices(const std::vector<Vertex>& vertices);

    void draw();

  private:
    VAO vao;
    Buffer buffer;
    int vertex_count;
    Program flat_color;
};

class GlApp final : public IWindowEvents
{
  public:
    GlApp();
    ~GlApp();

    virtual void on_frame_render() override;

    virtual void on_viewport_change(int width, int height) override;
    virtual void on_mouse_move(int x, int y) override;

  private:
    transforms::AutoAspectOrtho ortho;
    Triangles triangles;
};
} // namespace tetra

#endif
