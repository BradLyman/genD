#ifndef TETRA_GLAPP_HPP
#define TETRA_GLAPP_HPP

#include "gl/Buffer.hpp"
#include "gl/Program.hpp"
#include "gl/VAO.hpp"

namespace tetra
{
class GlApp
{
  public:
    GlApp();
    ~GlApp();

    void render_frame();

    void on_viewport_change(int width, int height);

  private:
    GLint viewSizeUniform;
    Program program;
    Buffer vertices;
    VAO vao;

    std::array<float, 2> view;
};
} // namespace tetra

#endif
